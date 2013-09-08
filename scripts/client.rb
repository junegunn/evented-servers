#!/usr/bin/env ruby
# encoding: UTF-8
# Junegunn Choi (junegunn.c@gmail.com)
# 2013/09/07-

require 'socket'
port = ARGV.length > 0 ? ARGV.first.to_i : 8000
socket = TCPSocket.new('localhost', port)

cnt  = 0
unit = 10000
st   = Time.now
while true
  socket.puts "hello #{cnt += 1}!"
  if cnt % unit == 0
    print "\rputs/sec: #{unit / (Time.now - st)}"
    st = Time.now
  end
end
