#!/bin/bash

export PATH=$PATH:/usr/local/bin

for i in 1024 512 256 152 128 120 96 80 76 64 58 60 48 40 32 30 29 24 20 16;
do
    convert  -depth 32 -units pixelsperinch ${1} -resample 192 -resize ${i}x${i} ${2}${i}.png
done

for i in 512 256 32;
do
    cp $2$i.png $2$i-1.png
done
