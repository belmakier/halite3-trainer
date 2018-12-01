#!/bin/bash

CURDIR=`pwd`
DOCKERDIR=/home/timothy/Documents/personal/cc/halite3/halite3-gym/gym/
MNTDIR=$1
cd $DOCKERDIR
docker run --rm -it -v $CURDIR/$MNTDIR:/halite halite gym-deregister $2
cd $CURDIR
