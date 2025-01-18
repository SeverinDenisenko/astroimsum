from astropy.io import fits
import numpy as np
import matplotlib.pyplot as plt
import sys

image = fits.open(sys.argv[1])[0].data

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
