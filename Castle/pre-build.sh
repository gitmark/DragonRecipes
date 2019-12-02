#!/bin/bash

src=$1

if [ "$src" == "" ]; then
	echo "must provide a source dir"
	exit 1
fi

dest=$2

if [ "$dest" == "" ]; then
	echo "must provide a dest dir"
	exit 1
fi

cd $src
srcBundle=$src/bundle
destBundle=$dest/Castle.app

echo "src = $src"
echo "dest = $dest"
cp -a $srcBundle $destBundle

cd $src/icon
./compile-icon.sh $dest $destBundle


