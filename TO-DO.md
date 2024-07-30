Bug:
1)
- start server
- start one connection with telnet localhost 8082
- make a request:
  GET / HTTP/1.1
  host: localhost
- try starting second connection with telnet localhost 8082
error message:
Trying 127.0.0.1...
telnet: Unable to connect to remote host: Connection refused


Crash:
