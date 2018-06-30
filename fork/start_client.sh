#!/bin/sh

rm client_*.log
ulimit -c unlimited
ulimit -n 4096

# 客户端数量
CLIENT_COUNT=10

for((i=1;i<=$CLIENT_COUNT;++i));
do

	nohup ./client > client_$i.log 2>&1 &
	echo 客户端进程$i启动完毕
	sleep 1

done
