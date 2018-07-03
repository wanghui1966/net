#!/bin/sh

rm -rf log/select_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./bin/select_server > log/select_server.log 2>&1 &
