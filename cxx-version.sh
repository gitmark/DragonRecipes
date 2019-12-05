#/bin/bash

CXX="$1"

if [ `uname` == "Darwin" ]; then
osname=macos
osver=`sw_vers | awk '{if ($1 == "ProductVersion:") print $2}' | sed -E 's/[.][^.]+$//'`
else
osname=`hostnamectl | grep "Operating System:" | awk '{print tolower($3)}'`
osver=`hostnamectl | grep "Operating System:" |  sed -E 's/[^0-9]*([0-9]+)[^0-9].*/\1/'`
fi

if [ "$CXX" == "" ]; then
	cxxname=unknown
	cxxver=0
else

$CXX -v 2>&1 |grep -q "Apple clang version"
if [ $? -eq 0 ]; then
cxxname=appleclang
cxxver=`$CXX -v 2>&1| awk '{if ($1 == "Apple") print $4}' 2>&1| sed -E 's/[.].+//'`
else
$CXX -v 2>&1 |grep -q " version "
if [ $? -eq 0 ]; then
cxxname=`$CXX -v 2>&1| awk '{if ($2 == "version") print tolower($1)}'`
cxxver=`$CXX -v 2>&1| awk '{if ($2 == "version") print $3}' | sed -E 's/[.].+//'`
else
cxxname=unknown
cxxver=0
fi
fi
fi

echo $osname$osver-$cxxname$cxxver

