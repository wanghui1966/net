#!/bin/sh

g++ unix_server.cpp -o bin/unix_server -g -std=c++11

g++ unix_client.cpp -o bin/unix_client -g -std=c++11
