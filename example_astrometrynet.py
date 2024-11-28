from astropy.io import fits
from astropy.coordinates import SkyCoord
from astropy import units as u
from reproject.mosaicking import find_optimal_celestial_wcs
from reproject.mosaicking import reproject_and_coadd
from reproject import reproject_to_healpix, reproject_from_healpix, reproject_interp
import numpy as np
import matplotlib.pyplot as plt
from glob import glob

from astrometrynet.net.client import client

frames = glob("./data/2015_HB10_H_alpha_*.fits")

# Add WCS to frames
"""
for frame in frames:
    image = fits.open(frame)
    ra = image[0].header['RA']
    dec = image[0].header['DEC']
    c = SkyCoord(ra, dec, unit=(u.hourangle, u.deg))

    options = client.ClientRunnerOptions(
        upload=frame,
        apikey="", # Add api key
        scale_est=0.39,
        scale_err=10,
        center_ra=c.ra.degree,
        center_dec=c.dec.degree,
        radius=0.5,
        scale_units="degwidth",
        newfits=frame,
    )

    client.run_client(options)

"""
hdus = []
for frame in frames:
    hdus.append(fits.open(frame))

wcs_out, shape_out = find_optimal_celestial_wcs(hdus)

image, footprint = reproject_and_coadd(
    hdus,
    wcs_out,
    shape_out=shape_out,
    reproject_function=reproject_interp,
    combine_function="sum",
)

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
