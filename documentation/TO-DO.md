Bug:


Crash:

Doruk:
- Finish clean up when a pipe or a connection is closed. In case one connection is executing scripts in the browser, but then quits.
clientScriptMap, pidMap and terminatedPidMap need to be purged on closeConnection().
- getPostData() udring POST CGI should only read up to Content-Length

Benjamin:
- Each send request must call getCookieRequestLine


Bugs:
