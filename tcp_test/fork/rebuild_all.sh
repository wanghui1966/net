#!/bin/sh

g++ fork_server.cpp -o bin/fork_server -g -std=c++11

g++ ../common/client.cpp -o bin/fork_client -g -std=c++11
