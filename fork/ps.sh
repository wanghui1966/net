#!/bin/sh

echo "\033[34m============fork_server============\033[0m"
netstat -antp | grep "fork_server"
echo "\033[34m===================================\033[0m"

echo "\033[34m============fork_client============\033[0m"
netstat -antp | grep "fork_client"
echo "\033[34m===================================\033[0m"
