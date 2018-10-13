#!/bin/sh

rm -rf log/epoll_server.log
ulimit -c unlimited
ulimit -n 4096
nohup ./bin/epoll_server > log/epoll_server.log 2>&1 &
