#!/usr/bin/env ruby

require "serialport"
 
sp = SerialPort.new("/dev/ttyUSB0", 9600, 8, 1, SerialPort::NONE)
 
#just read forever
i = 0
minutes = 0
seconds = 0
velocity = 0
event = 0

while true do
  if (c = sp.getbyte) 
    minutes = c if i == 0
    seconds = c if i==1
    velocity = c if i==2
    event = c if i==3
    
    if i==4 
      checksum = c
      puts "DATA ARRIVED (#{minutes}:#{seconds}) Vel:#{velocity}  Ev: #{event} checksum:#{checksum}"
      i = 0
    else
      i = i +1
    end
    
  end
  
end
 
sp.close
