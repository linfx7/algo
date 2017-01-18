# coding=utf-8
import os
import random
import socket
import struct
from IPy import IP

#将str_ip转为 int_ip
def ip_str_to_int(str_ip):
    int_ip = socket.ntohl(struct.unpack("I",socket.inet_aton(str_ip))[0])
    return int_ip

# 创建文件
file_size = input("input file size(K item):")
file_name = "rules_not_repeat_" + str(file_size) + "K.txt"
flow_file = open(file_name,"w")

# 生成规则数据
i = 0
while True:
    all_items = ""
    #生成ip
    src_ip_seg = "10.1." + str(random.randint(0,255)) + "." + str(random.randint(0,255))
    dst_ip_seg = "10.2." + str(random.randint(0,255)) + "." + str(random.randint(0,255))
    #生成掩码
    src_mask = random.randint(16,32)
    dst_mask = random.randint(16,32)
    # 获取网段：10.1.0.0/16  并转化为字符串 10.1.0.0
    src_ip_seg = IP(src_ip_seg).make_net(src_mask).strNormal(0)
    dst_ip_seg = IP(dst_ip_seg).make_net(dst_mask).strNormal(0)
    #转换成int
    src_ip_int = ip_str_to_int(src_ip_seg)
    dst_ip_int = ip_str_to_int(dst_ip_seg)
    #组装字符串
    item = str(src_ip_int) + " " + str(src_mask) + " " + str(dst_ip_int) + " " + str(dst_mask)
    #防止重复
    if item in all_items:
        continue
    all_items += (item+";")
    #计数
    i += 1
    #写入文件
    if i == file_size*1024:
        flow_file.write(item)
        break
    flow_file.write(item + "\n")


print file_name + " generated success!"
flow_file.close