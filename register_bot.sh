#!/bin/bash

CURDIR=`pwd`
DOCKERDIR=/home/timothy/Documents/personal/cc/halite3/halite3-gym/gym/

args=($@)

MNTDIR=$1

i=0
path=""
for a in $@; do
    if [[ $i < 2 ]]; then
        i=$((i+1));
        continue;
    fi;
    if [[ $i == 2 ]]; then        
        path=$path/halite/bots/${args[$i]};
    else
        path=$path" "/halite/bots/${args[$i]};
    fi;
    i=$((i+1))
done

echo $path

cd $DOCKERDIR
docker run --rm -it -v $CURDIR/$MNTDIR:/halite halite gym-register "$2" "$path"
cd $CURDIR
