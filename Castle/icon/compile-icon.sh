#!/bin/bash

image="$PWD/castle1024.png"

destBuild=$1

if [ "$destBuild" == "" ]; then
	echo "must provide a destination build dir"
	exit 1
fi

destBundle=$2

if [ "$destBundle" == "" ]; then
	echo "must provide a destination bundle dir"
	exit 1
fi

srcIcon=$destBuild/srcIcon
destIcon=$destBuild/destIcon

if [ -e $srcIcon ]; then
   rm -rf $srcIcon
fi

if [ -e $destIcon ]; then
   rm -rf $destIcon
fi

echo "image = $image"
echo "srcIcon = $srcIcon"
echo "destIcon = $destIcon"
echo "destBundle = $destBundle"

mkdir -p $srcIcon
mkdir -p $destIcon

cp -a Assets.xcassets $srcIcon

srcIconSetDir=$srcIcon/Assets.xcassets/AppIcon.appiconset
export dir=`pwd`

pushd $srcIconSetDir
$dir/resize-squares.sh $image castle
$dir/create-icon.sh castle
popd

pushd $srcIcon
$dir/make-assets-car.sh $destIcon
cp $destIcon/Assets.car $destBundle/Contents/Resources/
cp $destIcon/AppIcon.icns $destBundle/Contents/Resources/
popd


