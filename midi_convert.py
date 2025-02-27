import mido
import argparse

def midi_to_sid(midi_filename, note_offset=36, fps=50):
    """
    Converts a MIDI file into the SID music format.
    
    The conversion assumes:
      - Note events are taken from 'note_on' messages with velocity > 0.
      - Each note is converted to a 6‑bit value by subtracting note_offset and clamping
        the result to the 0–63 range.
      - Events occurring at the same time are grouped together.
      - The time between groups is converted to a duration (in frames) which is encoded
        as a byte with its high bit set (the lower 6 bits hold the frame count).
      - If the duration exceeds 63, multiple duration bytes are output.
      - The song ends with the special marker 0xff.
    """
    mid = mido.MidiFile(midi_filename)
    output = []
    # accumulated delay in frame ticks (1 tick = 1 frame)
    delay = 0  
    # list to hold simultaneous note events
    current_group = []
    
    # Iterate over all messages in order.
    for msg in mid:
        # Convert the message delta time (in seconds) to frame ticks.
        # (Adjust using the fps parameter.)
        delay += int(round(msg.time * fps))
        
        if msg.type == 'note_on' and msg.velocity > 0:
            # When a note-on event occurs after a delay and we already have some notes,
            # output the previous group along with its delay.
            if delay > 0 and current_group:
                # Output the group notes first.
                for note in current_group:
                    output.append(note)
                # Emit the delay as one or more duration events.
                d = delay
                while d > 63:
                    output.append(0x80 | 63)  # 0x80 sets the high bit.
                    d -= 63
                output.append(0x80 | d)
                delay = 0
                current_group = []
            # Convert the MIDI note to our format.
            note_val = msg.note - note_offset
            if note_val < 0:
                note_val = 0
            if note_val > 63:
                note_val = 63
            current_group.append(note_val)
        # Ignore note_off and note_on with velocity 0.

    # Flush any remaining notes/delay.
    if current_group or delay > 0:
        for note in current_group:
            output.append(note)
        d = delay
        while d > 63:
            output.append(0x80 | 63)
            d -= 63
        output.append(0x80 | d)

    # Append the end-of-song marker.
    output.append(0xff)
    return output

def main():
    parser = argparse.ArgumentParser(description="Convert a MIDI file to SID music format")
    parser.add_argument("midi_file", help="Path to the input MIDI file")
    parser.add_argument("--note-offset", type=int, default=36,
                        help="MIDI note offset for conversion (default: 36)")
    parser.add_argument("--fps", type=float, default=50,
                        help="Frames per second for conversion (default: 50)")
    args = parser.parse_args()

    sid_data = midi_to_sid(args.midi_file, note_offset=args.note_offset, fps=args.fps)

    # Output the converted data as a C array.
    print("const byte music_new[] = {")
    for i, b in enumerate(sid_data):
        print("0x{:02x},".format(b), end=' ')
        if (i + 1) % 16 == 0:
            print()
    print("\n};")

if __name__ == '__main__':
    main()

