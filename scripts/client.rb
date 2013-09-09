#!/usr/bin/env ruby
# encoding: UTF-8
# Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/07-

require 'socket'
host_port, clients = ARGV
host_port  ||= 'localhost:8000'
host, port   = host_port.split(':')
port       ||= 8000
clients    ||= 1

sockets = clients.to_i.times.map { TCPSocket.new(host, port.to_i) }

cnt  = 0
unit = 10000
st   = Time.now
while true
  sockets.sample.print "hello #{cnt += 1}!\r\n"
  if cnt % unit == 0
    print "\rputs/sec: #{unit / (Time.now - st)}"
    st = Time.now
  end
end
