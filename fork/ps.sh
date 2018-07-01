#!/bin/sh

ps aux | grep -E "fork_server|fork_client" | grep -v grep
