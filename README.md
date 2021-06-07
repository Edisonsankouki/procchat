# procchat
asm4 for usyd comp2017
a record.

## How your solution communicates between the client-handler and client?
The the client handler talks to the client by writing into their rd pipe and reading their message from the wr pipe. And you can do this with popen() to get their pipes by their dirtories.

## How are you handling erroneous messages?
This client handler will ignore message type out of the range given. And if the size is not reached to required it will still sends the message.(what is the meaning of this shit?)

## How is your solution handling client failure?
If the client did not send pong in the last 15 minutes the main process will know that it failed and clean its body.




