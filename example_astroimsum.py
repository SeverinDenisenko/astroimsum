from astropy.io import fits
import numpy as np
import matplotlib.pyplot as plt
from glob import glob

from pyastroimsumlib import astroimsum

frames = sorted(glob("./data/2015_HB10_H_alpha_*.fits"))

base_frame_index = len(frames) // 2 + 1
base_frame = frames[base_frame_index]
other_frames = frames[:base_frame_index] + frames[base_frame_index + 1 :]

lib = astroimsum.astroimsum()
lib.set_config("config.cfg")

base_frame_loaded = lib.load_frame(base_frame)
lib.set_base_frame(base_frame_loaded)

for frame in other_frames:
    loaded_frame = lib.load_frame(frame)
    lib.add_frame(loaded_frame)
    del loaded_frame

base_frame_loaded.write("result.fits")

image = fits.open("result.fits")[0].data

plt.imshow(
    image,
    vmin=np.median(image) - np.std(image),
    vmax=np.median(image) + np.std(image),
    origin="lower",
    cmap="gray",
)
plt.axis("off")
plt.tight_layout()
plt.show()
