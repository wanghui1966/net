#!/bin/sh

echo "\033[34m============thread_server==========\033[0m"
netstat -antp | grep thread_server
echo "\033[34m===================================\033[0m"

echo "\033[34m============thread_client==========\033[0m"
netstat -antp | grep thread_client
echo "\033[34m===================================\033[0m"
