#!/bin/sh

rm fork_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./fork_server > fork_server.log 2>&1 &
