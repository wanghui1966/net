#!/bin/sh

g++ select_server.cpp -o bin/select_server -g -std=c++11

g++ ../common/client.cpp -o bin/select_client -g -std=c++11
