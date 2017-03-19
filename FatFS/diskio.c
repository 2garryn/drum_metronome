/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "systick_manager.h"
#include "ff_spi.h"
#include "uart_manager.h"

/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */


/*-----------------------------------------------------------------------*/
/* Helper functions                                                      */
/*-----------------------------------------------------------------------*/
static uint8_t wait_ready (UINT wt) {/* 1:Ready, 0:Timeout */
	uint8_t d;
	systick_set_timer(2, wt);
	do {
		d = SPI_send_single(0xFF);
		/* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */
	} while (d != 0xFF && systick_get_timer(2));	/* Wait for card goes ready or timeout */

	return (d == 0xFF) ? 1 : 0;
}

static void chip_deselect(void) {
	CS_set();		/* Set CS# high */
	SPI_send_single(0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}

static uint8_t chip_select (void){	/* 1:OK, 0:Timeout */ 
	CS_reset();		/* Set CS# low */
	SPI_send_single(0xFF);	/* Dummy clock (force DO enabled) */
	if (wait_ready(500)) return 1;	/* Wait for card ready */

	chip_deselect();
	return 0;	/* Timeout */
}

static uint8_t send_cmd(uint8_t cmd, uint32_t arg) {
    uint8_t res, n;
    if (cmd & 0x80) {	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}   
    if (cmd != CMD12) {
		chip_deselect();
		if (!chip_select()) return 0xFF;
	} 
    /* Send command packet */
	SPI_send_single(0x40 | cmd);				/* Start + command index */
	SPI_send_single((uint8_t)(arg >> 24));		/* Argument[31..24] */
	SPI_send_single((uint8_t)(arg >> 16));		/* Argument[23..16] */
	SPI_send_single((uint8_t)(arg >> 8));			/* Argument[15..8] */
	SPI_send_single((uint8_t)arg);				/* Argument[7..0] */
    n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
    SPI_send_single(n);
	/* Receive command resp */
	if (cmd == CMD12) SPI_send_single(0xFF);	/* Diacard following one byte when CMD12 */
	n = 10;								/* Wait for response (10 bytes max) */
	do {
		res = SPI_send_single(0xFF);
    } while ((res & 0x80) && --n);

	return res;							/* Return received response */
}

static int rcvr_datablock (uint8_t *buff, uint16_t btr)		/* 1:OK, 0:Error */
{
	uint8_t token;


	systick_set_timer(1, 200);
	do {							/* Wait for DataStart token in timeout of 200ms */
		token = SPI_send_single(0xFF);
		/* This loop will take a time. Insert rot_rdq() here for multitask envilonment. */
	} while ((token == 0xFF) && systick_get_timer(1));
	if(token != 0xFE) return 0;		/* Function fails if invalid DataStart token or timeout */

	SPI_receive(buff, btr);		/* Store trailing data to the buffer */
	SPI_send_single(0xFF); SPI_send_single(0xFF);			/* Discard CRC */

	return 1;						/* Function succeeded */
}

static uint8_t CardType;

static volatile DSTATUS Stat = STA_NOINIT;	/* Physical drive status */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber to identify the drive */
)
{
	if (drv) return STA_NOINIT;		/* Supports only drive 0 */

	return Stat;	/* Return disk status */
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    uint8_t n, cmd, ty, ocr[4];;

	if (pdrv) return STA_NOINIT;	

    SPI_init(SPI_BaudRatePrescaler_256);
    CS_init();
    chip_deselect();
    for (uint8_t i = 0; i < 10; i++) SPI_send_single(0xFF); /* Send 80 dummy clocks */
    

    ty = 0;
    if (send_cmd(CMD0, 0) == 1) {			/* Put the card SPI/Idle state */
	    systick_set_timer(1, 1000);						/* Initialization timeout = 1 sec */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDv2? */
            LOGD("Card version: SDv2", 0);
            for (n = 0; n < 4; n++) ocr[n] = SPI_send_single(0xFF);	/* Get 32 bit return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* Is the card supports vcc of 2.7-3.6V? */
				while (systick_get_timer(1) && send_cmd(ACMD41, 1UL << 30)) ;	/* Wait for end of initialization with ACMD41(HCS) */
				if (systick_get_timer(1) && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = SPI_send_single(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Card id SDv2 */
				}
			}
        } else {
            LOGD("Card version: SDv1", 0);
			if (send_cmd(ACMD41, 0) <= 1) 	{	/* SDv1 or MMC? */
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
			}
			while (systick_get_timer(1) && send_cmd(cmd, 0)) ;		/* Wait for end of initialization */
			if (!systick_get_timer(1) || send_cmd(CMD16, 512) != 0)	/* Set block length: 512 */
				ty = 0;
		}
    }
	CardType = ty;	/* Card type */
	chip_deselect();

	if (ty) {			/* OK */
        SPI_init(SPI_BaudRatePrescaler_8);    
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	} else {			/* Failed */
		Stat = STA_NOINIT;
	}

	return Stat;


//	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector read */
		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512)) {
			count = 0;
		}
	}
	else {				/* Multiple sector read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	chip_deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */    
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

	return res;


//	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

    return res;
}

DWORD get_fattime (void)
{
	return 0;
}
