#!/bin/bash

CURDIR=`pwd`
DOCKERDIR=/home/timothy/Documents/personal/cc/halite3/halite3-gym/gym/
MNTDIR=$1
cd $DOCKERDIR
docker run --rm --env LD_LIBRARY_PATH=/halite -it -v $CURDIR/$MNTDIR:/halite halite gym-evaluate --output-dir /halite/replays/ ${@:2}
cd $CURDIR
