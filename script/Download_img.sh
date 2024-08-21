#!/bin/sh

mkdir -p ../rec/img/1                 #本地不存在就创建文件夹

rm -rf ../rec/img/1/*                   #先把本地的清空了

scp -r sen@192.168.8.126:/home/sen/io/img/* ../rec/img/1
