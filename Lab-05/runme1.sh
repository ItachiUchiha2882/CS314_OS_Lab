#!/bin/bash

cp system.c /usr/src/minix/kernel/system.c;
cd ../usr/src;
make build MKUPDATE=yes > log.txt;