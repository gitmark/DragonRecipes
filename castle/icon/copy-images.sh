#!/bin/bash

base=$1

if [ "$base" == "" ]; then
	echo "must provide a base filename"
	exit 1
fi

src=$2

if [ "$src" == "" ]; then
	echo "must provide a source dir"
	exit 1
fi

dest=$3

if [ "$dest" == "" ]; then
	echo "must provide a dest dir"
	exit 1
fi

rm $dest/*.png

for i in 1024 512 256 128 64 32 16;
do
	cp $src/$base$i.png $dest/
done

for i in 512 256 32;
do
	cp $dest/$base$i.png $dest/$base$i-1.png
done




