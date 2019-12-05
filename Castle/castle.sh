#!/bin/sh

B_REALPATH=`readlink "$0"`

if [ "$B_REALPATH" = "" ]; then
B_REALPATH="$0"
fi

cd $(dirname "$B_REALPATH")
cd ../../..
open -a $PWD/Castle.app



