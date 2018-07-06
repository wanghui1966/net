#!/bin/sh

rm -rf log/thread_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./bin/thread_server > log/thread_server.log 2>&1 &
