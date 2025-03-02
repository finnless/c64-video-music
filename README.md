# C64 Music Visualizer

A Commodore 64 demo that combines smooth scrolling text visualization with SID music playback. The project includes both the C64 program and a Python MIDI converter utility.

## Features

- Smooth scrolling 16x32 pixel text display with rainbow color effects
- SID music playback using the C64's sound chip
- Custom bitmap font rendering
- Python utility to convert MIDI files into the project's music format
- Glitch-art style visualization that responds to the music

## Building the C64 Program

TODO

## MIDI Converter Tool

The project includes `midi_convert.py`, a Python script that converts MIDI files into the custom SID music format used by the C64 program.

### Installation

1. Make sure you have Python 3.6+ installed
2. Install the required dependency:
```bash
pip install mido
```

### Usage

```bash
python midi_convert.py input.mid [--note-offset 36] [--fps 50]
```

Options:
- `--note-offset`: MIDI note offset for conversion (default: 36)
- `--fps`: Frames per second for conversion (default: 50)

The script will output C array data that can be copied into the C64 program's music data section.

## Music Format

The music format is a simple sequence of bytes where:
- Notes are represented as 6-bit values (0-63)
- Timing information is encoded in bytes with the high bit set
- Multiple notes can be played simultaneously
- The sequence ends with 0xFF marker