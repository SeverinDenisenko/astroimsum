#!/bin/bash
set -ex

./build/astroimsum/imsum --config ./scripts/config-test-embeded-2015-HB10.cfg \
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

./build/astroimsum/imsum --config ./scripts/config-test-external-2015-HB10.cfg \
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

./build/astroimsum/imsum --config ./scripts/config-test-embeded-Neptune.cfg \
    --images "$PWD"/data/Neptune_R_010.fits \
    "$PWD"/data/Neptune_R_001.fits \
    "$PWD"/data/Neptune_R_002.fits \
    "$PWD"/data/Neptune_R_003.fits \
    "$PWD"/data/Neptune_R_004.fits \
    "$PWD"/data/Neptune_R_005.fits \
    "$PWD"/data/Neptune_R_006.fits \
    "$PWD"/data/Neptune_R_008.fits \
    "$PWD"/data/Neptune_R_009.fits \
    "$PWD"/data/Neptune_R_011.fits \
    "$PWD"/data/Neptune_R_012.fits \
    "$PWD"/data/Neptune_R_013.fits \
    "$PWD"/data/Neptune_R_014.fits \
    "$PWD"/data/Neptune_R_016.fits \
    "$PWD"/data/Neptune_R_017.fits \
    "$PWD"/data/Neptune_R_018.fits \
    "$PWD"/data/Neptune_R_019.fits \
    "$PWD"/data/Neptune_R_020.fits
ls -laht out.fits
rm out.fits

./build/astroimsum/imsum --config ./scripts/config-test-external-Neptune.cfg \
--images "$PWD"/data/Neptune_R_010.fits \
    "$PWD"/data/Neptune_R_002.fits \
    "$PWD"/data/Neptune_R_003.fits \
    "$PWD"/data/Neptune_R_004.fits \
    "$PWD"/data/Neptune_R_006.fits \
    "$PWD"/data/Neptune_R_007.fits \
    "$PWD"/data/Neptune_R_008.fits \
    "$PWD"/data/Neptune_R_009.fits \
    "$PWD"/data/Neptune_R_011.fits \
    "$PWD"/data/Neptune_R_012.fits \
    "$PWD"/data/Neptune_R_013.fits \
    "$PWD"/data/Neptune_R_014.fits \
    "$PWD"/data/Neptune_R_016.fits \
    "$PWD"/data/Neptune_R_017.fits \
    "$PWD"/data/Neptune_R_018.fits \
    "$PWD"/data/Neptune_R_020.fits
ls -laht out.fits
rm out.fits
