#!/bin/sh

mkdir -p ../rec/video/1                  #本地不存在就创建视频文件夹

rm -rf ../rec/video/1/*                    #先把本地视频清空了

scp -r sen@192.168.8.126:/home/sen/io/video/* ../rec/video/1
