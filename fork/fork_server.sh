#!/bin/sh

rm fork_server.log
ulimit -c unlimited
nohup ./fork_server > fork_server.log 2>&1 &
