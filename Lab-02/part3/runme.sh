#!/bin/bash

cp forkexit.c /usr/src/minix/servers/pm/forkexit.c;
cd ../usr/src;
make build MKUPDATE=yes > log.txt;