raspybot
========

##Server

Run the test server with:
```
sudo python2 server.py
```
This will listen in on port 5000 to incoming clients.

##Client

Compile the test client with:
```
gcc -lncurses -o client.o client.c
```
Unless you want to use clang, obviously. That might be a good idea.

`ncurses` is used to avoid having to press ENTER when issuing a command to the robot. The `ncurses` library might only be temporary, and could be removed once the client sends 'real' commands to the robot, not just instantaneous key presses to test the LEDs and motors.

Invoke the client program with the following command:
```
./client.o <IP of Robot>
```

You can send the following the commands (probably woefully out of date):
```
a - both ('all') LEDs on
o - both LEDs 'off'
1 - toggle LED1 on/off
2 - toggle LED2 on/off
q - quit the client.
```
