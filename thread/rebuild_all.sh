#!/bin/sh

g++ thread_server.cpp -o bin/thread_server -g -std=c++11 -pthread

g++ ../common/client.cpp -o bin/thread_client -g -std=c++11
