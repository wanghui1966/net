#!/bin/sh

echo "\033[34m============epoll_server=============\033[0m"
netstat -antp | grep epoll_server
echo "\033[34m=====================================\033[0m"

echo "\033[34m============epoll_client=============\033[0m"
netstat -antp | grep epoll_client
echo "\033[34m=====================================\033[0m"
