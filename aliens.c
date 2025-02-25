#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <string.h>
#include <c64.h>
#include <cbm_screen_charmap.h>

#include "common.h"
//#link "common.c"

#include "rasterirq.h"
//#link "rasterirq.s"

#include "sidmacros.h"

#include <cbm_petscii_charmap.h>
#include <6502.h>
// generated from https://8bitworkshop.com/bitmapfontgenerator/
// 16 x 32 MSB first y offset 6 Terminus

#include <cbm.h>
#include <conio.h>
#include <unistd.h>  // For sleep functions


#define LOCHAR 32
#define HICHAR 127
#define FONT_BWIDTH 2
#define FONT_HEIGHT 32
#define FONT_ROWS (FONT_HEIGHT/2)
const char FONT[HICHAR-LOCHAR+1][FONT_HEIGHT*FONT_BWIDTH] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x30,0x0C,0x30,0x0C,0x30,0x0C,0xFC,0x3F,0xFC,0x3F,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xF0,0x1F,0x80,0x39,0x80,0x31,0x80,0x31,0x80,0x31,0x8C,0x31,0x9C,0x39,0xF8,0x1F,0xF0,0x0F,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x00,0x30,0x0F,0xB0,0x1F,0x98,0x19,0x98,0x19,0x8C,0x1F,0x0C,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xF8,0x31,0x9C,0x3B,0x0E,0x1F,0x06,0x0E,0x06,0x0C,0x06,0x0C,0x06,0x0E,0x0E,0x1F,0xFC,0x3B,0xF8,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xE0,0x00,0xC0,0x00,0xC0,0x01,0x80,0x03,0x00,0x07,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x07,0x00,0x03,0x80,0x03,0xC0,0x01,0xE0,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFE,0x3F,0xC0,0x01,0xE0,0x03,0x70,0x07,0x38,0x0E,0x1C,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x3F,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0xC0,0x01,0xC0,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x00,0x30,0x00,0x30,0x00,0x18,0x00,0x18,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xCC,0x31,0xEC,0x30,0x7C,0x30,0x3C,0x30,0x1C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0xF0,0x0F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x0E,0x00,0x07,0x80,0x03,0xC0,0x01,0xE0,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xE0,0x1F,0x00,0x38,0x00,0x30,0x00,0x30,0x00,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x1C,0x30,0x0C,0x30,0x0C,0x30,0xFC,0x3F,0xFC,0x3F,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x38,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x38,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x06,0x00,0x03,0x00,0x03,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xF8,0x1F,0x1C,0x38,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xF8,0x3F,0xF0,0x3F,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x38,0xF8,0x1F,0xF8,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0xC0,0x01,0xC0,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x1C,0x00,0x38,0x00,0x70,0x00,0xE0,0x00,0xC0,0x01,0x80,0x03,0x00,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x38,0x00,0x1C,0x00,0x0E,0x00,0x07,0x80,0x03,0xC0,0x01,0xE0,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x07,0x80,0x03,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x66,0x30,0x66,0x30,0x66,0x30,0xE6,0x38,0xC6,0x3F,0x86,0x37,0x06,0x00,0x0E,0x00,0xFC,0x3F,0xF8,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x3F,0xFC,0x3F,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x1C,0x0C,0x38,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x38,0xFC,0x1F,0xFC,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x18,0x0C,0x1C,0xFC,0x0F,0xFC,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x07,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x07,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x3F,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x3F,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0xE0,0x07,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x0C,0x18,0x0C,0x18,0x0C,0x18,0x1C,0x1C,0xF8,0x0F,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x3C,0x00,0x7C,0x00,0xEC,0x00,0xCC,0x01,0x8C,0x03,0x0C,0x07,0x0C,0x0E,0x0C,0x1C,0x0C,0x38,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x06,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x8C,0x33,0x0C,0x37,0x0C,0x3E,0x0C,0x3C,0x0C,0x38,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x1F,0xFC,0x0F,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x8C,0x33,0x1C,0x3F,0xF8,0x1F,0xF0,0x1F,0x00,0x38,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xFC,0x1F,0xFC,0x0F,0xEC,0x00,0xCC,0x01,0x8C,0x03,0x0C,0x07,0x0C,0x0E,0x0C,0x1C,0x0C,0x38,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xF0,0x1F,0x00,0x38,0x00,0x30,0x00,0x30,0x00,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x60,0x06,0x60,0x06,0x60,0x06,0xC0,0x03,0xC0,0x03,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x86,0x30,0xC6,0x31,0xC6,0x31,0xE6,0x33,0x36,0x36,0x36,0x36,0x1E,0x3C,0x0E,0x38,0x06,0x30,0x06,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xC0,0x03,0xC0,0x03,0x60,0x06,0x60,0x06,0x30,0x0C,0x30,0x0C,0x18,0x18,0x18,0x18,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xC0,0x03,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xC0,0x01,0xE0,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0xF0,0x07,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x00,0x03,0x00,0x03,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0xF0,0x07,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x30,0xF0,0x3F,0xF8,0x3F,0x1C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x3F,0xF0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x38,0xFC,0x1F,0xFC,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x3F,0xF0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0xFC,0x3F,0xFC,0x3F,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x1C,0x30,0xF8,0x3F,0xF0,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x3F,0xF0,0x3F,0x00,0x30,0x00,0x30,0x00,0x30,0xF8,0x1F,0xF8,0x0F,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0xE0,0x07,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x38,0x1C,0xF0,0x0F,0xE0,0x07,0x00,0x00 },{ 0x98,0x03,0xD8,0x01,0xF8,0x00,0xF8,0x00,0xD8,0x01,0x98,0x03,0x18,0x07,0x18,0x0E,0x18,0x1C,0x18,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0xE0,0x07,0xE0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x38,0xFC,0x1F,0xFC,0x0F,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x3F,0xF0,0x3F,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x00 },{ 0x1C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x00,0x1C,0x00,0xF8,0x0F,0xF0,0x1F,0x00,0x38,0x00,0x30,0x00,0x30,0x1C,0x38,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x01,0x80,0x3F,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x3F,0xF0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x18,0x18,0x18,0x18,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x60,0x06,0x60,0x06,0xC0,0x03,0xC0,0x03,0xC0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x31,0x9C,0x39,0xF8,0x1F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x70,0x0E,0xE0,0x07,0xC0,0x03,0xC0,0x03,0xE0,0x07,0x70,0x0E,0x38,0x1C,0x1C,0x38,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x1C,0x30,0xF8,0x3F,0xF0,0x3F,0x00,0x30,0x00,0x30,0x00,0x38,0xF8,0x1F,0xF8,0x0F,0x00,0x00 },{ 0x00,0x0E,0x00,0x07,0x80,0x03,0xC0,0x01,0xE0,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0xFC,0x3F,0xFC,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x78,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x01,0x80,0x0F,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x0F,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0xC0,0x01,0xF8,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },};

#define SCREEN 0x400
#define BITMAP 0x3000
#define SCROLL_TOP 5
#define SCROLL_ROWS FONT_ROWS

#define TOP_Y (SCROLL_TOP*4+50)
#define BOTTOM_Y (TOP_Y+SCROLL_ROWS*32)

byte scroll_x = 0;
byte msg_x = 0;
byte msg_index = 0;
byte fonttile_index = 1;
byte color = 0;

const byte* get_font_ptr(char ch) {
  return FONT[ch - LOCHAR];
}
  
word get_charbitmap_addr(char code) {
  return BITMAP + code*8;
}

const char *MSG = 
" HELLO WORLD! THIS IS SOME *BIG*"
" TEXT SCROLLING! THE BITMAP FONT IS"
" 16 X 32 PIXELS."
" EACH TIME WE SCROLL 8 PIXELS,"
" WE DRAW INTO 4 CHARACTER TILES,"
" DISPLAYING 160 AT ANY GIVEN TIME."
" THE RAINBOW EFFECT IS JUST FROM"
" SETTING CHARACTER COLORS IN COLOR RAM."
" MESSAGE REPEATS .........."
;

char get_next_char() {
  char ch = MSG[msg_index];
  if (ch == 0 || ch == 128) {
    msg_index = 0;
    ch = MSG[0];
  }
  return ch;
}

byte get_char_for_row(byte row) {
  byte x = msg_x;
  byte x1 = x % FONT_BWIDTH;
  char code = fonttile_index + row;
  word bmp = get_charbitmap_addr(code);
  char nextchar = get_next_char();
  const char* fptr = get_font_ptr(nextchar) 
    + row*32*FONT_BWIDTH 
    + x1;
  byte i;
  for (i=0; i<8; i++) {
    POKE(bmp+i, fptr[i*FONT_BWIDTH]);
  }
  return code;
}
  
void next_msg_column() {
  // increment variables
  fonttile_index += FONT_ROWS;
  if (fonttile_index >= 1+2*FONT_ROWS) {
    fonttile_index = 1;
  }
  if ((++msg_x % FONT_BWIDTH) == 0) {
    msg_index++;
  }
}

void draw_right_column() {
  // get the top-right corner address of scroll area
  word addr = SCRNADR(SCREEN, 39, SCROLL_TOP);
  byte row;
  // draw one character per row
  for (row=0; row<SCROLL_ROWS; row++) {
    POKE(addr, get_char_for_row(row));
    addr += 40;
  }
}

void scroll_one_column_left() {
  // copy several rows of screen memory
  // backwards one byte
  const word start = SCRNADR(SCREEN, 0, SCROLL_TOP);
  const word nbytes = SCROLL_ROWS*40-1;
  memcpy((byte*)start, (byte*)start+1, nbytes);
  // draw the right column of characters
  draw_right_column();
  // increment variables
  next_msg_column();
}

void scroll_one_pixel_left() {
  // scroll left one pixel
  scroll_x--;
  // set scroll register with lower three bits
  VIC.ctrl2 = (VIC.ctrl2 & ~7) | (scroll_x & 7);
  // move screen memory if the scroll register
  // has just gone past 0 and wrapped to 7
  if ((scroll_x & 7) == 7) {
    scroll_one_column_left();
  }
  // double-speed! (can comment this out)
  scroll_x++;
}

void display_list() {
  VIC.bgcolor[0] = COLOR_BLACK;
  DLIST_NEXT(BOTTOM_Y);  
  VIC.bgcolor[0] = COLOR_GRAY1;
  DLIST_RESTART(TOP_Y);
}

void set_row_color(byte line, byte color) {
  memset((byte*)COLOR_RAM+line*40, color, 40);
}
  
int get_random(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
  
void set_row_color_rand() {
  memset((byte*)COLOR_RAM+(0)*40, color, 40);
  color++;
  if ((color & 7) == 7) {
    color = 1;
  }
  
}

void main(void) {
  
  // clear the screen to 0 (not ' ' space)
  memset((byte*)SCREEN, 0, 0x400);
  
  
  // set color RAM for each row of chars
  set_row_color(SCROLL_TOP+0, 02);
  set_row_color(SCROLL_TOP+1, 02);
  set_row_color(SCROLL_TOP+2, COLOR_ORANGE);
  set_row_color(SCROLL_TOP+3, COLOR_ORANGE);
  set_row_color(SCROLL_TOP+4, COLOR_YELLOW);
  set_row_color(SCROLL_TOP+5, COLOR_YELLOW);
  set_row_color(SCROLL_TOP+6, COLOR_LIGHTGREEN);
  set_row_color(SCROLL_TOP+7, COLOR_LIGHTGREEN);
  set_row_color(SCROLL_TOP+8, COLOR_GREEN);
  set_row_color(SCROLL_TOP+9, COLOR_GREEN);
  set_row_color(SCROLL_TOP+10, COLOR_CYAN);
  set_row_color(SCROLL_TOP+11, COLOR_CYAN);
  set_row_color(SCROLL_TOP+12, COLOR_LIGHTBLUE);
  set_row_color(SCROLL_TOP+13, COLOR_LIGHTBLUE);
  set_row_color(SCROLL_TOP+14, COLOR_PURPLE);
  set_row_color(SCROLL_TOP+15, COLOR_PURPLE);
  
  
  
  // set new address for character bitmaps
  SET_VIC_BITMAP(BITMAP);
  // set 38 column mode (for X scrolling)
  VIC.ctrl2 = 0b00000000;
  // setup IRQs to change colors
  DLIST_SETUP(display_list);
  // infinite loop
  while (1) {
    waitvsync();
    scroll_one_pixel_left();
    set_row_color_rand();

  }
  
}