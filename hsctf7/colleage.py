#!/usr/bin/python

import socket
import os
import shlex
import subprocess
from pwn import *

def runner(command,text):
    proc = subprocess.run(shlex.split(command),
        universal_newlines=True,input=text, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return proc.stdout

host = "algo.hsctf.com"
port = 4002
conn = remote(host,port)

def get_str():
  return conn.recvline().decode('utf-8')

def get():
  return map(int,get_str().split(' '))

def main():
  for test in range(0,15):
    case = ''
    while(True):
      line = get_str()
      print("<",line)
      if 'Here' in line:
        break

    first = get_str()
    print("first input",first)
    case += first
    n,p = map(int,first.split(' '))
    for i in range(0,n):
      case += get_str()
    for i in range(0,n):
      case += get_str()
    for i in range(0,n):
      case += get_str()
    a=runner('./a.out', case)
    print(">",a)
    conn.send(a);

  while True:
    line = get_str()
    print("<",line)

main()
