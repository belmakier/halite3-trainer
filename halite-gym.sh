#!/bin/bash

MNTDIR=$1
docker run --rm -it -v $(pwd)/$MNTDIR:/halite halite "${@:2}"
