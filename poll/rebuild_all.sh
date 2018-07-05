#!/bin/sh

g++ poll_server.cpp -o bin/poll_server -g -std=c++11

g++ ../common/client.cpp -o bin/poll_client -g -std=c++11
