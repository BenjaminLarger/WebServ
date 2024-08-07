301 example with content-body:
GET / HTTP/1.1
host: google.com

HTTP/1.1 301 Moved Permanently
Location: http://www.google.com/
Content-Type: text/html; charset=UTF-8
Content-Length: 219

<HTML><HEAD><meta http-equiv="content-type" content="text/html;charset=utf-8">
<TITLE>301 Moved</TITLE></HEAD><BODY>
<H1>301 Moved</H1>
The document has moved
<A HREF="http://www.google.com/">here</A>.
</BODY></HTML>


## There are several types of HTTP status codes used for redirection, each serving different purposes. Here are the common ones:

1. **301 Moved Permanently**:
   - Indicates that the resource has been permanently moved to a new URL.
   - Search engines will update their links to the new URL.
   - Example:
     ```nginx
     location /old {
         return 301 http://example.com/new;
     }
     ```

2. **302 Found (Temporary Redirect)**:
   - Indicates that the resource has temporarily moved to a different URL.
   - Search engines do not update their links to the new URL.
   - Example:
     ```nginx
     location /old {
         return 302 http://example.com/new;
     }
     ```

3. **303 See Other**:
   - Typically used for redirection after a PUT or POST request to redirect the client to a different resource, usually using the GET method.
   - Example:
     ```nginx
     location /submit {
         return 303 http://example.com/confirmation;
     }
     ```

4. **307 Temporary Redirect**:
   - Similar to 302 but the method and the body of the original request are reused to perform the redirect.
   - Example:
     ```nginx
     location /old {
         return 307 http://example.com/new;
     }
     ```

5. **308 Permanent Redirect**:
   - Similar to 301 but ensures the method and the body of the original request are reused to perform the redirect.
   - Example:
     ```nginx
     location /old {
         return 308 http://example.com/new;
     }
     ```

### Nginx Configuration Examples

#### Permanent Redirect (301)

```nginx
server {
    listen 80;
    server_name example.com;

    location /oldpage {
        return 301 http://example.com/newpage;
    }
}
```

#### Temporary Redirect (302)

```nginx
server {
    listen 80;
    server_name example.com;

    location /oldpage {
        return 302 http://example.com/newpage;
    }
}
```

#### See Other Redirect (303)

```nginx
server {
    listen 80;
    server_name example.com;

    location /submit {
        return 303 http://example.com/thankyou;
    }
}
```

#### Temporary Redirect with Method Preservation (307)

```nginx
server {
    listen 80;
    server_name example.com;

    location /oldpage {
        return 307 http://example.com/newpage;
    }
}
```

#### Permanent Redirect with Method Preservation (308)

```nginx
server {
    listen 80;
    server_name example.com;

    location /oldpage {
        return 308 http://example.com/newpage;
    }
}
```

By using the appropriate status code, you can control how clients and search engines handle the redirection.