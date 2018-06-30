#!/bin/sh

ps aux | grep -E "fork_server|client" | grep -v grep | grep -v java
