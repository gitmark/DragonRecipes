#!/bin/bash 

ver=`cat Version.h | grep 'DRAGON_VERSION "' | sed -E 's/[^0-9]+([0-9]+[.][0-9]+)[.].*/\1/g'`
stage=`cat Version.h | grep 'DRAGON_DEV_STAGE' | sed -E 's/.*["](.+)["].*/\1/g'`

if [ $stage == "alpha" ]; then
	s=a
else 
if [ $stage == "beta" ]; then
	s=b
else 
	s=
fi
fi

echo $ver$s
 
