#!/bin/sh

rm -rf log/poll_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./bin/poll_server > log/poll_server.log 2>&1 &
