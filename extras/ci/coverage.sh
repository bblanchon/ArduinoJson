#!/bin/sh -eux

cmake -DCOVERAGE=true .
make 
make test

pip install --user cpp-coveralls 'requests[security]'
pwd
coveralls --include 'src' --gcov-options '\-lp'
