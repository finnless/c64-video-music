
/*
A simple music player.
*/

#include "common.h"
//#link "common.c"

#include "sidmacros.h"

#include <cbm_petscii_charmap.h>
#include <6502.h>

// SID frequency table (PAL version)
const int note_table_pal[96] = {
278,295,313,331,351,372,394,417,442,468,496,526,557,590,625,662,702,743,788,834,884,937,992,1051,1114,1180,1250,1325,1403,1487,1575,1669,1768,1873,1985,2103,2228,2360,2500,2649,2807,2973,3150,3338,3536,3746,3969,4205,4455,4720,5001,5298,5613,5947,6300,6675,7072,7493,7938,8410,8910,9440,10001,10596,11226,11894,12601,13350,14144,14985,15876,16820,17820,18880,20003,21192,22452,23787,25202,26700,28288,29970,31752,33640L,35641L,37760L,40005L,42384L,44904L,47574L,50403L,53401L,56576L,59940L,63504L,67280L
};

#define NOTE_TABLE note_table_pal

void sid_init() {
  SID.amp = 0x0f; // volume 15, no filters
}

// music player logic

byte music_index = 0;
byte cur_duration = 0;
byte music_wavebits = SID_SQUARE;

const byte* music_ptr = 0;

byte next_music_byte() {
  return *music_ptr++;
}

void play_music() {
  static byte chs = 0;
  if (music_ptr) {
    // run out duration timer yet?
    while (cur_duration == 0) {
      // fetch next byte in score
      byte note = next_music_byte();
      // is this a note?
      if ((note & 0x80) == 0) {
        int period = NOTE_TABLE[note & 63];
        // see which pulse generator is free
        if (!(chs & 1)) {
          SID_STOP(v1, music_wavebits);
	  SID_FREQ(v1, period);
          SID_START(v1, music_wavebits);
          chs |= 1;
        } else if (!(chs & 2)) {
          SID_STOP(v2, music_wavebits);
	  SID_FREQ(v2, period);
          SID_START(v2, music_wavebits);
          chs |= 2;
        } else if (!(chs & 4)) {
          SID_STOP(v3, music_wavebits);
	  SID_FREQ(v3, period);
          SID_START(v3, music_wavebits);
          chs |= 4;
        }
      } else {
        // end of score marker
        if (note == 0xff)
          music_ptr = NULL;
        // set duration until next note
        cur_duration = note & 63;
        // reset channel used mask
        chs = 0;
      }
    }
    cur_duration--;
  }
}

void start_music(const byte* music) {
  music_ptr = music;
  cur_duration = 0;
}

#ifdef __MAIN__

typedef struct Param {
  const char* const name;
  word index;
  word mask;
  byte lshift;
  word low;
  word high;
} Param;

#define NPARAMS 21

const Param SID_PARAMS[NPARAMS] = {
  { "Main Volume", 0x18, 0x0f, 0, 0, 15 },
  { "Pulse Width", 0x03, 0x0f, 0, 0, 15 },
  { "Env Attack", 0x05, 0xf0, 4, 0, 15 },
  { "Env Decay", 0x05, 0x0f, 0, 0, 15 },
  { "Env Sustain", 0x06, 0xf0, 4, 0, 15 },
  { "Env Release", 0x06, 0x0f, 0, 0, 15 },
  { "Noise", 0x04, 0x80, 7, 0, 1 },
  { "Pulse", 0x04, 0x40, 6, 0, 1 },
  { "Sawtooth", 0x04, 0x20, 5, 0, 1 },
  { "Triangle", 0x04, 0x10, 4, 0, 1 },
  { "Ring Mod", 0x04, 0x04, 2, 0, 1 },
  { "Sync", 0x04, 0x02, 1, 0, 1 },
  { "Voice 3 Off", 0x18, 0x80, 7, 0, 1 },
  //{ "Filter Freq", 0x15, 0xffff, 5, 0, 0x7ff },
  { "Filter Frequency", 0x16, 0xff, 0, 0, 0xff },
  { "Filter Resonance", 0x17, 0xf0, 4, 0, 15 },
  { "Filter Voice 1", 0x17, 0x1, 0, 0, 1 },
  { "Filter Voice 2", 0x17, 0x2, 1, 0, 1 },
  { "Filter Voice 3", 0x17, 0x4, 2, 0, 1 },
  { "Filter Lowpass", 0x18, 0x10, 4, 0, 1 },
  { "Filter Bandpass", 0x18, 0x20, 5, 0, 1 },
  { "Filter Highpass", 0x18, 0x40, 6, 0, 1 },
};

int paramValues[NPARAMS];
int currentParam;

void setSIDRegisters() {
  char i;
  word val;
  char buf[30];
  memset(buf, 0, sizeof(buf));
  for (i = 0; i < NPARAMS; ++i) {
    const Param* param = &SID_PARAMS[i];
    val = PEEKW(buf + param->index);
    val &= ~param->mask;
    val |= (paramValues[i] << param->lshift) & param->mask;
    if (param->mask & 0xff00) {
      POKEW(buf + param->index, val);
    } else {
      POKE(buf + param->index, val);
    }
  }
  //memcpy((void*)0xd400, buf, sizeof(buf));
  memcpy((void*)0xd415, buf+0x15, 8);
  POKE(0xd403, buf[0x03]);
  POKE(0xd405, buf[0x05]);
  POKE(0xd406, buf[0x06]);
  POKE(0xd40a, buf[0x03]);
  POKE(0xd40c, buf[0x05]);
  POKE(0xd40d, buf[0x06]);
  POKE(0xd411, buf[0x03]);
  POKE(0xd413, buf[0x05]);
  POKE(0xd414, buf[0x06]);
  music_wavebits = buf[0x04];
}

const byte music1[]; // music data -- see end of file

char music_update() {
  if (!music_ptr) start_music(music1);
  play_music();
  return IRQ_NOT_HANDLED;
}


void main(void)
{
  joy_install (joy_static_stddrv);
  
  // set initial SID parameters
  paramValues[0] = 15;
  paramValues[1] = 8;
  paramValues[2] = 8;
  paramValues[3] = 8;
  paramValues[4] = 4;
  paramValues[5] = 4;
  paramValues[7] = 1; // pulse
  setSIDRegisters();
  
  // set IRQ routine called every frame
  set_irq(music_update, (void*)0x9f00, 0x100);
  
  // main loop
  music_ptr = 0;
  while (1) {
  }
}

//
// MUSIC DATA
//
const byte music1[] = {
0x0f, 0x03, 0x8a, 0x0f, 0x03, 0x85, 0x1b, 0x85, 0x03, 0x85, 0x1b, 0x85, 0x19, 0x03, 0x85, 0x1b,
0x85, 0x0f, 0x03, 0x8a, 0x0f, 0x03, 0x85, 0x1b, 0x85, 0x03, 0x85, 0x1b, 0x85, 0x19, 0x03, 0x85,
0x1b, 0x85, 0x0f, 0x03, 0x8a, 0x0f, 0x03, 0x85, 0x1b, 0x85, 0x03, 0x85, 0x1b, 0x85, 0x19, 0x03, 0xff,
};

#endif
