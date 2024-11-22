#!/bin/bash

echo $PWD
ls $PWD
ls ./data
./build/astroimsum/imsum "$PWD"/data/2015_HB10_H_alpha_005.fits 9 \
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
