#!/bin/sh -eux

cmake -DCOVERAGE=true .
make 
make test

pip install --user cpp-coveralls 'requests[security]'
coveralls --exclude third-party --gcov-options '\-lp'; fi
