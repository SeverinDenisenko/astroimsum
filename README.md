# Astronomy imagery summation tool

[![Builds](https://github.com/SeverinDenisenko/astroimsum/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/SeverinDenisenko/astroimsum/actions/workflows/cmake-multi-platform.yml)

## About

![example](data/example.png)
![example](data/example4.png)

This tool can add images (in .fits format) with stars on them. Tool computes proper transform frame to frame, to make all sourses line up together.

## Usage

```bash
imsum --config <config.cfg path> --images <images list>
```

This command adds other images to base image and results in out.fits.

## Pyhton interface

Example (jupiter notebook):

```Python
from pyastroimsumlib import astroimsum

frames = glob("path_to_frames/*.fits")

base_frame_index = len(frames) // 2 - 1
base_frame = frames[base_frame_index]
other_frames = frames[:base_frame_index] + frames[base_frame_index + 1 :]

lib = astroimsum.astroimsum()

!{"rm result.fits"} # Without that kernel will die!

base_frame_loaded = lib.load_frame(base_frame)
lib.set_base_frame(base_frame_loaded)

for frame in other_frames:
    loaded_frame = lib.load_frame(frame)
    lib.add_frame(loaded_frame)
    del loaded_frame

base_frame_loaded.write("result.fits")
```

## Dependencies

- Source extractor (optional)
- Boost
- C++17 or higher

## Known issues

- Sometimes transformation is just total grbage, I can't fix that. If so folow astrometry.net example.
