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
- Finish clean up when a pipe or a connection is closed. In case one connection is executing scripts in the browser, but then quits.
clientScriptMap, pidMap and terminatedPidMap need to be purged on closeConnection().
- getPostData() udring POST CGI should only read up to Content-Length


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