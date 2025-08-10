# WebServ

A custom HTTP web server implementation written in C++98, compliant with HTTP/1.1 standards.

## Overview

WebServ is a high-performance HTTP web server that supports multiple virtual hosts, CGI script execution, file uploads, and various HTTP methods. Built as part of the 42 School curriculum, this project demonstrates advanced systems programming concepts including socket programming, process management, and HTTP protocol implementation.

## Features

- **HTTP/1.1 Compliance**: Full support for HTTP/1.1 protocol
- **Multiple Virtual Hosts**: Configure multiple servers with different ports and domains
- **HTTP Methods**: Support for GET, POST, and DELETE requests
- **CGI Support**: Execute CGI scripts (Python, shell scripts, etc.)
- **File Operations**: File uploads, downloads, and deletion
- **Directory Listing**: Automatic directory index generation
- **Error Handling**: Custom error pages with proper HTTP status codes
- **Cookie Support**: Session management with cookie handling
- **Non-blocking I/O**: Efficient handling of multiple concurrent connections
- **Configuration Files**: Nginx-like configuration syntax

## Building

### Requirements

- C++98 compatible compiler (clang++ recommended)
- Unix-like operating system (Linux, macOS)
- Make

### Compilation

```bash
make
```

This will create the `webserv` executable in the project directory.

### Clean Up

```bash
make clean      # Remove object files
make fclean     # Remove object files and executable
make re         # Clean and rebuild
```

## Usage

### Basic Usage

```bash
# Use default configuration
./webserv

# Use custom configuration file
./webserv config/custom.conf
```

### Configuration

The server uses configuration files similar to nginx syntax. Example configuration:

```nginx
server {
    listen 8080;
    server_names localhost;
    
    error_page 404 /errors/404.html;
    
    location / {
        root /var/www/;
        index index.html;
    }
    
    location /cgi-bin/ {
        root /var/www/;
        cgi_pass on;
        allowed_methods GET POST;
    }
}
```

### Configuration Options

- `listen`: Port number to listen on
- `server_names`: Server names/domains
- `error_page`: Custom error page paths
- `root`: Document root directory
- `index`: Default index files
- `allowed_methods`: HTTP methods allowed for the location
- `cgi_pass`: Enable CGI script execution
- `client_max_body_size`: Maximum request body size
- `autoindex`: Enable/disable directory listing

## Project Structure

```
webserv/
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Webserv.cpp        # Main server class
│   ├── ServerConfig.cpp   # Configuration parsing
│   ├── GET.cpp            # GET method implementation
│   ├── POST.cpp           # POST method implementation
│   ├── DELETE.cpp         # DELETE method implementation
│   └── CGI.cpp            # CGI script handling
├── inc/                   # Header files
├── config/                # Configuration files
├── var/www/              # Default web root
├── cgi-bin/              # CGI scripts
└── documentation/        # Project documentation
```

## Testing

The project includes various test configurations and examples:

- **Configuration Files**: Located in `config/` directory
- **Test Endpoints**: Various HTML pages and forms in `var/www/`
- **CGI Scripts**: Example scripts in `cgi-bin/`
- **Request Examples**: Sample HTTP requests in `request&response_examples/`

### Testing with cURL

```bash
# GET request
curl -X GET http://localhost:8080/

# POST request with data
curl -X POST -d "name=value" http://localhost:8080/form/

# File upload
curl -X POST -F "file=@example.txt" http://localhost:8080/upload/

# DELETE request
curl -X DELETE http://localhost:8080/delete/file.txt
```

## Architecture

The server uses an event-driven architecture with:

- **Non-blocking I/O**: Using poll() for efficient connection handling
- **Process Management**: Fork/exec for CGI script execution
- **Memory Management**: RAII principles and careful resource cleanup
- **Modular Design**: Separate classes for different HTTP methods and functionality

## HTTP Methods Supported

- **GET**: Retrieve resources, directory listings, CGI execution
- **POST**: Submit data, file uploads, form processing
- **DELETE**: Remove files from the server

## Error Handling

The server provides comprehensive error handling with:

- HTTP status code compliance
- Custom error pages
- Proper connection management
- Resource cleanup on errors

## Security Features

- Request size limits
- Path traversal protection
- CGI execution sandboxing
- Proper file permissions handling

## Authors

- **blarger** (Benjamin Larger)
- **demre** (Demir)

## License

This project is part of the 42 School curriculum.