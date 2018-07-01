#!/bin/sh

rm -rf log/fork_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./bin/fork_server > log/fork_server.log 2>&1 &
