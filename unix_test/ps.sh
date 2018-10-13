#!/bin/sh

echo "\033[34m============unix_server============\033[0m"
ps aux | grep unix_server | grep -v grep
echo "\033[34m===================================\033[0m"

echo "\033[34m============unix_client============\033[0m"
ps aux | grep unix_client | grep -v grep
echo "\033[34m===================================\033[0m"
