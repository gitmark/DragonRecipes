#!/bin/bash

export PATH=$PATH:/usr/local/bin

convert ${1}16.png ${1}20.png ${1}24.png ${1}30.png ${1}32.png ${1}40.png ${1}48.png ${1}64.png ${1}96.png ${1}128.png ${1}256.png ${1}.ico

