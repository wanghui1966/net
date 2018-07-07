#!/bin/sh

g++ epoll_server.cpp -o bin/epoll_server -g -std=c++11

g++ ../common/client.cpp -o bin/epoll_client -g -std=c++11
