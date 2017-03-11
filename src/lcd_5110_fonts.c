#include "lcd_5110_fonts.h"
// FONT: x(width): 12
//       y(height): 24
static uint8_t bpm_main_font[][36] = {
    // 0 - zero
    {
        0xF8, 0xFC, 0x0E, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0E, 0xFC, 0xF8,
        0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
        0x1F, 0x3F, 0x70, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0x70, 0x3F, 0x1F
    },
    // 1 - one
    {
        0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0xFE, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
    },
    // 2 - two
    {
        0x38, 0x3C, 0x0E, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0E, 0xFC, 0xF8,
        0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0F, 0x07,
        0xF8, 0xFC, 0xFE, 0xC7, 0xC3, 0xC1, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0
    },
    // 3 - three
    {
        0x38, 0x3C, 0x0E, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0E, 0xFC, 0xF8,
        0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x66, 0x66, 0xC3, 0x81, 0x00,
        0x1C, 0x3C, 0x70, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0x70, 0x3F, 0x1F
    },
    // 4 - four
    {
        0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0xFE, 0xFF,
        0x08, 0x0C, 0x0E, 0x0F, 0x0F, 0x0D, 0x0C, 0x0C, 0x0C, 0x0C, 0xFF, 0xFF, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
    },
    // 5 - five 
    {
        0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x1F, 0x1F, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0xF8, 0xF0,
        0x1C, 0x3C, 0x70, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0x70, 0x3F, 0x1F
    },
    // 6 - six
    {
        0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x03, 0x01,
        0xFE, 0xFF, 0x1F, 0x0D, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1C, 0xF8, 0xF0,
        0x1F, 0x3F, 0x70, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0x70, 0x3F, 0x1F
    },
    // 7 - seven
    {
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xC3, 0xF3, 0x3F, 0x0F,
        0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00,
        0xC0, 0xF0, 0x3C, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    // 8 - eight
    {
        0xFE, 0xFF, 0x87, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x87, 0xFF, 0xFE,
        0x00, 0x81, 0xC3, 0xE7, 0x7E, 0x3C, 0x3C, 0x7E, 0xE7, 0xC3, 0x81, 0x00, 
        0x7F, 0xFF, 0xC1, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC1, 0xFF, 0x7F
    },
    // 9 - nine
    {
        0xF8, 0xFC, 0x0E, 0x07, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0E, 0xFC, 0xF8,
        0x07, 0x0F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x98, 0xD8, 0xFF, 0x7F,
        0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x00, 0x00  
    },
    // nothing
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
    
};

static uint8_t rhytm_note_font[][48] = {
    {
        0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x60, 0x70, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x60, 0x70, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x70, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
    {
        0x00, 0x00, 0x00, 0x00, 0xFE, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0xFE, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0xFE, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0xFE, 0x00,
        0x00, 0x00, 0x60, 0x70, 0x3F, 0x00, 0x00, 0x00, 0x60, 0x70, 0x3F, 0x00, 0x00, 0x00, 0x60, 0x70, 0x3F, 0x00, 0x00, 0x00, 0x60, 0x70, 0x3F, 0x00

    }

};

uint8_t get_bpm_main_font(uint8_t num, uint8_t i) {
    return bpm_main_font[num][i];
}

uint8_t get_rhytm_note_font(uint8_t rhytm_note, uint8_t i) {
    return rhytm_note_font[rhytm_note][i];
}
