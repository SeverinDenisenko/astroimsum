#!/usr/bin/python3

import sys
import astroimsum.astroimsum as astroimsum


def main():
    lib = astroimsum.astroimsum()

    frames = []
    for f in sys.argv[1:]:
        frames.append(lib.load_frame(f))

    base_frame_index = len(frames) // 2 - 1
    base_frame = frames[base_frame_index]
    other_frames = frames[:base_frame_index] + frames[base_frame_index + 1 :]

    lib.set_base_frame(base_frame)

    for f in other_frames:
        lib.add_frame(f)

    base_frame.write("result.fits")


if __name__ == "__main__":
    main()
