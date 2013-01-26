#!/usr/bin/env python

import socket
import select

UDP_PORT=6666

sock = socket.socket( socket.AF_INET,socket.SOCK_DGRAM,socket.IPPROTO_UDP )

sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_BROADCAST,1)
sock.bind(('',UDP_PORT))
sock.setblocking(0)

fh = open("netCon.txt","a")
while True:
    result = select.select( [sock], [], [] )
    msg = result[0][0].recv(32768)
    print msg
    fh.write(msg)
    fh.flush()
