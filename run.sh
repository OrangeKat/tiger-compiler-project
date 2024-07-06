#! /bin/sh

./src/tc -XA "$1" > output.tig
./src/tc -XA output.tig
