Bug:

1)
We cannot connect to localhost:<port> through Firefox. The connection is established when we stop the webserver.


Crash:

To implement:
1)
  - Return every POST response to the main loop so we send it from there when POLLOUT available

  - Make receiv pass throught poll always and concatenate the buffer in each iteration
  - Replace errno after receiv for another error management

