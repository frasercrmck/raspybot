#! /usr/bin/python

import sys
import socket
import select
from raspirobotboard import *

led1 = False
led2 = False

################################################################################
### Helper functions
################################################################################
def handle_command(s, inputs, rr, command):
  global led1
  global led2
  print 'Received command from', s.getpeername(), ':', command
  if command == '1':
    led1 = not led1
    rr.set_led1(led1)
  elif command == '2':
    led2 = not led2
    rr.set_led2(led2)
  elif command == 'a':
    led1 = True
    led2 = True
    rr.set_led1(led1)
    rr.set_led2(led2)
  elif command == 'o':
    led1 = False
    led2 = False
    rr.set_led1(led1)
    rr.set_led2(led2)
  elif command == 'q':
    print 'Client disconnected'
    s.close()
    inputs.remove(s)
  else:
    print 'Server error: unrecognised command:', command

################################################################################
### Main Function
################################################################################

rr = RaspiRobot()

# Init state to off
rr.set_led1(False)
rr.set_led2(False)

server = socket.socket()

port = 5000
host = socket.gethostname()

server.bind((host, port))

server.listen(5)

inputs = [server, sys.stdin]

print '########################################'
print 'Server listening on', server.getsockname()
print '########################################'

running = True
while running:
  inputready, outputready, exceptready = select.select(inputs, [], [])

  for s in inputready:
    if s == server:
      # Handle the server socket
      client, address = s.accept()
      inputs.append(client)
      print 'Got connection from', address
    elif s == sys.stdin:
      # Handle standard input
      # TODO: Handle server commands
      junk = sys.stdin.readline()
      running = False
    else:
      # Handle client command
      command = s.recv(4)
      if command:
        handle_command(s, inputs, rr, command)
      else:
        print 'Client force-quit'
        s.close()
        inputs.remove(s)

# Tidy up
server.close()
