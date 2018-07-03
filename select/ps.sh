#!/bin/sh

echo "\033[34m============select_server============\033[0m"
netstat -antp | grep select_server
echo "\033[34m=====================================\033[0m"

echo "\033[34m============select_client============\033[0m"
netstat -antp | grep select_client
echo "\033[34m=====================================\033[0m"
