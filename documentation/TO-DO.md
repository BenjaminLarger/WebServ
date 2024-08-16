Bug:

1)
We cannot connect to localhost:<port> through Firefox. The connection is established when we stop the webserver.


Crash:

To implement:
1)
GET => process GET commmand if valid GET request input


  - Return every POST response to the main loop so we send it from there when POLLOUT available

  - Make receiv pass throught poll always and concatenate the buffer in each iteration
  - Replace errno after receiv for another error management

  - Rewrite sendRGeneric so that it sends by chunk through poll() each time

  Bug:
  Buffer not cleared when /wait15.py after /wait.py in same browser

  New POLLIN event detected 4
New client request detected 4
fds[i].fd: 4
bytes received = 562
 temp buffer = GET /wait15.py HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
DNT: 1
Sec-GPC: 1
Connection: keep-alive
Cookie: sessionId=; theme=light; rememberMe=yes
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Priority: u=0, i


clientInput[lastNewlinePos - 2] 10clientInput[lastNewlinePos - 1] = 13
The last newline character is preceded by another newline character.
Client request buffer: 
GET /wait.py HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
DNT: 1
Sec-GPC: 1
Connection: keep-alive
Cookie: sessionId=j4Ru9xiJYhX7et8SUyE6KCyAW470zdch; theme=light; rememberMe=yes
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Priority: u=0, i

GET /wait15.py HTTP/1.1
Host: localhost:8080
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br, zstd
DNT: 1
Sec-GPC: 1
Connection: keep-alive
Cookie: sessionId=; theme=light; rememberMe=yes
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Priority: u=0, i
