#!/bin/bash

rm gpssim.bin
./gnss_sim -e brdc3540.14n \
    -s 10000000 \
    -l 35.681298,139.766247,10.0 \
    -d 30
