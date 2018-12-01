#!/bin/bash

CURDIR=`pwd`
DOCKERDIR=/home/timothy/Documents/personal/cc/halite3/halite3-gym/gym/
MNTDIR=$1
out=`cd $DOCKERDIR; docker run --rm -it -v $CURDIR/$MNTDIR:/halite halite gym-list; cd $CURDIR`
echo $out>temp.txt
python3 get_fitnesses.py
rm temp.txt
