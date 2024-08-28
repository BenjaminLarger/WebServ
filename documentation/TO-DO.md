Bug:


Crash:

To implement:
1)
  - Return every POST response to the main loop so we send it from there when POLLOUT available

  - Make receiv pass throught poll always and concatenate the buffer in each iteration
  - Replace errno after receiv for another error management

2)
	-Delete recvAll, use recv chunk by chunk

Doruk:
- I think if a CGI returns an output, it should return a Content-type? text/html or text/plain, image... that way we can process it accordingly by wrapping the output in html if there isn't any for instance.

- It is currently possible for the CGI to not return any output, see http://localhost:8080/wait.py

- I need to finish the clean up when a pipe or a connection is closed. In case one connection is executing scripts in the browser, but then quits.
clientScriptMap, pidMap and terminatedPidMap need to be purged on closeConnection().

- I added a function to check if the child processes terminated properly.

- all responses are sent after a poll() in handleClientResponse()

- the ouput of the CGI is sent to a pipe added to fds and read after a poll() in readAndHandleScriptOutput()

https://www.jmarshall.com/easy/cgi/
https://www.jmarshall.com/easy/cgi/cgi_footnotes.html#othertypes


Bugs:

1)
	-redirection return 405
	->		./webserv config/server.conf
	->		http://localhost:8081/old-page
	=>		serverName = 'localhost:8081'
				Current URI : /old-page
				URIpath: /old-page, pathFolder: , pathOnServer: , isDir 0, isFile 0, pathFolderOnServer: , isDir 0, isFile 0
				Clearing request buffers
				Error: 405 Method is not allowed on that path

				Clearing request buffers. Method well proceeded