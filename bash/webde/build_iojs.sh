#!/bin/bash
echo
echo -----------------------------------
echo Build io.js
echo
set -e
# This script mustn't be run with root rights.
if [ $UID -eq 0 ]; then
    echo Error! You should not run this shell with root rights.
    exit 1
fi
WORKDIR=$(cd ~; pwd)
cd $WORKDIR
if [ ! -d iojs ]; then
  mkdir iojs
fi

if [ ! -d io.js ]; then
  git clone git@124.16.141.145:ibp/io.js.git
fi

cd $WORKDIR/io.js
git checkout v1.2.0
./configure --prefix=${WORKDIR}/iojs
make
make install

$WORKDIR/iojs/bin/npm install nw-gyp -g
$WORKDIR/iojs/bin/npm install node-pre-gyp -g
$WORKDIR/iojs/bin/npm install grunt-cli -g

echo
echo Successed building io.js.
echo ===================================
echo ***add $WORKDIR/iojs/bin/ to PATH***
