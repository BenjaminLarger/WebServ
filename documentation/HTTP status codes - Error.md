HTTP status codes you should consider handling, along with brief descriptions:

    400 Bad Request
        Occurs when the server cannot or will not process the request due to a client error (e.g., malformed request syntax).

    401 Unauthorized
        Indicates that the request requires user authentication.

    402 Payment Required
    
    403 Forbidden
        The server understood the request but refuses to authorize it.

    404 Not Found
        The server can't find the requested resource. This is the most commonly handled error page.

    405 Method Not Allowed
        The request method is known by the server but is not supported by the target resource.

    406 Not Acceptable

    407 Proxy Authentication Required

    408 Request Timeout
        The server would like to shut down this unused connection.

    409 Conflict
    410 Gone
    411 Length Required
    412 Precondition Failed
    413 Content Too Large
    414 URI Too Long
    415 Unsupported Media Type
    416 Range Not Satisfiable
    417 Expectation Failed
    418 I'm a teapot
    421 Misdirected Request
    422 Unprocessable Content
    423 Locked
    424 Failed Dependency
    425 Too Early
    426 Upgrade Required
    428 Precondition Required

    429 Too Many Requests
        The user has sent too many requests in a given amount of time ("rate limiting").

    431 Request Header Fields Too Large

    451 Unavailable For Legal Reasons

    500 Internal Server Error
        A generic error message when the server encounters an unexpected condition.

    501 Not Implemented
        The server does not support the functionality required to fulfill the request.

    502 Bad Gateway
        The server, while acting as a gateway or proxy, received an invalid response from the upstream server.

    503 Service Unavailable
        The server is currently unable to handle the request due to temporary overload or maintenance.

    504 Gateway Timeout
        The server, while acting as a gateway or proxy, did not receive a timely response from the upstream server.

    505 HTTP Version Not Supported
        The server does not support the HTTP protocol version used in the request.

    506 Variant Also Negotiates
    507 Insufficient Storage
    508 Loop Detected
    510 Not Extended
    511 Network Authentication Required

For Nginx, the configuration might look like this:

server {
    listen 80;
    server_name yourdomain.com;
    root /var/www/html;

    # Error Page configuration
    error_page 400 /errors/400.html;
    error_page 401 /errors/401.html;
    error_page 403 /errors/403.html;
    error_page 404 /errors/404.html;
    error_page 405 /errors/405.html;
    error_page 408 /errors/408.html;
    error_page 429 /errors/429.html;
    error_page 500 /errors/500.html;
    error_page 501 /errors/501.html;
    error_page 502 /errors/502.html;
    error_page 503 /errors/503.html;
    error_page 504 /errors/504.html;
    error_page 505 /errors/505.html;

}
