#!/bin/sh

echo "\033[34m============poll_server==============\033[0m"
netstat -antp | grep poll_server
echo "\033[34m=====================================\033[0m"

echo "\033[34m============poll_client==============\033[0m"
netstat -antp | grep poll_client
echo "\033[34m=====================================\033[0m"
