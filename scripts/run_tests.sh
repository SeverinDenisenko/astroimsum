#!/bin/bash
set -ex

./build/astroimsum/imsum --config ./scripts/config-test-embeded.cfg \
    --images "$PWD"/data/2015_HB10_H_alpha_005.fits \
    "$PWD"/data/2015_HB10_H_alpha_001.fits \
    "$PWD"/data/2015_HB10_H_alpha_002.fits \
    "$PWD"/data/2015_HB10_H_alpha_003.fits \
    "$PWD"/data/2015_HB10_H_alpha_004.fits \
    "$PWD"/data/2015_HB10_H_alpha_006.fits \
    "$PWD"/data/2015_HB10_H_alpha_007.fits \
    "$PWD"/data/2015_HB10_H_alpha_008.fits \
    "$PWD"/data/2015_HB10_H_alpha_009.fits \
    "$PWD"/data/2015_HB10_H_alpha_010.fits
ls -laht out.fits
rm out.fits

./build/astroimsum/imsum --config ./scripts/config-test-external.cfg \
    --images "$PWD"/data/2015_HB10_H_alpha_005.fits \
    "$PWD"/data/2015_HB10_H_alpha_001.fits \
    "$PWD"/data/2015_HB10_H_alpha_002.fits \
    "$PWD"/data/2015_HB10_H_alpha_003.fits \
    "$PWD"/data/2015_HB10_H_alpha_004.fits \
    "$PWD"/data/2015_HB10_H_alpha_006.fits \
    "$PWD"/data/2015_HB10_H_alpha_007.fits \
    "$PWD"/data/2015_HB10_H_alpha_008.fits \
    "$PWD"/data/2015_HB10_H_alpha_009.fits \
    "$PWD"/data/2015_HB10_H_alpha_010.fits
ls -laht out.fits
rm out.fits
