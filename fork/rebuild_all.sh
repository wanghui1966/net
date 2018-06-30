#!/bin/sh

g++ fork_server.cpp -o fork_server -g -std=c++11

g++ client.cpp -o client -g -std=c++11
