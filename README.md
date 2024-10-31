# Webserv Documentation

## Compilation
- **Standard Compilation**: Run `make` to compile the web server

## Test Site
Access a test site locally after stating the executable `./webserv config.yaml` at : [http://127.0.0.1:8080](http://127.0.0.1:8080).

## Configuration File
The configuration file defines the server settings, routes, and directives to run the web server. Below is a brief overview of the available directives.

### General Directives
- **user** `"username"`: Specifies the user under which the server runs.
- **error_log** `"path"`: Path to the error log file.
- **body_size** `"size"`: Limits the request body size. Units supported: KB, MB, GB.

### Server Directives
- **listen** `"ip"` `"port"`: IP and port to bind the server.
- **server_name** `"domain"`: Defines the server name or domain.
- **error_page** `"code"` `"path"`: Custom error page for specific HTTP status codes.

### Location Block
The `location` block defines the behavior for specific routes on the server:
- **location** `"path"`: Path to match.
- **root** `"path"`: Root directory for files.
- **index** `"file"`: Default file served at the root.
- **dirlisting** `on|off`: Enables or disables directory listing.
- **methods** `[GET | POST | DELETE]`: Supported HTTP methods.
- **redirect** `"code"` `"url"`: Redirect requests with a specified HTTP status code.
- **cgi_extension** `".ext" "path"`: Defines CGI scripts and the interpreter path (e.g., Python, Bash).
- **upload_path** `"path"`: Path for file uploads.

### Example Configuration File
```plaintext
user www;
error_log /var/log/nginx/error.log;
body_size 1 MB;

server {
    listen 127.0.0.1 8080;
    server_name 127.0.0.1:8080 localhost:8080;
    error_page 404 ./web/error404.html;

    location / {
        root ./web;
        index index.html;
        cgi_extension .py /usr/bin/python3;
        cgi_extension .sh /usr/bin/bash;
        dirlisting off;
        upload_path ./web/upload;
        methods GET POST DELETE;
    }

    location /farfadet {
        redirect 301 https://www.rickroll.com;
    }
}
```

## Features
- **HTTP Methods**: Supports `GET`, `POST`, and `DELETE` requests.
- **CGI Management**: Executes CGI scripts in Python and Bash.
- **Cookie Handling**: Manages cookies within HTTP requests.
