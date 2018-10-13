#!/bin/sh

rm -rf log/unix_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./bin/unix_server > log/unix_server.log 2>&1 &
