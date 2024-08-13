Bug:

1)
We cannot connect to localhost:<port> through Firefox. The connection is established when we stop the webserver.


Crash:

To implement:
1)
GET => process GET commmand if valid GET request input


  - Return every response (GET, DELETE, POST and exceptions)to the main loop so we send it from there when POLLOUT available

  - Make receiv pass throught poll always and concatenate the buffer in each iteration
  - Replace errno after receiv for another error management
  - send can't be in a loop either, it has to go through poll

  - We shouldn't send the response before it is complete. For cgi, we need to read from the pipe several times. Checking if (clients[i].response.size()) will not be enough. Maybe we need a flag that says response complete.