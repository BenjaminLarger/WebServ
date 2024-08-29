#!/usr/bin/env python3

import cgi
import cgitb
import os
import sys

# Enable debugging
cgitb.enable()

# Create an instance of FieldStorage to retrieve form data
form = cgi.FieldStorage()

# Retrieve the name and age fields from the form
name = form.getvalue("name", "None")

html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Form Response</title>
    <style>
        body {{
            background-color: black;
            color: white;
						text-align: center;
						font-size: 2rem;
						display: flex; 
            flex-direction: column; 
        }}
        h1 {{
            text-align: center;
						font-family: 'Arial, sans-serif', monospace;
						text-align: center;
						font-size: 60px;
						font-size: 3rem;
        }}
    </style>
</head>
<body>
    <h1>Hello {name}!</h1>
		<a href='https://hackertyper.net/#'><img src='https://raw.githubusercontent.com/BenjaminLarger/BenjaminLarger/main/walk.gif' alt='walking' border='0'></a>
</body>
</html>
"""

# Calculate the content length
content_length = len(html_content)

# Print the HTTP headers to indicate HTML content
print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print(f"Content-Length: {content_length}")
print("\r\n")  # End of headers section

# Print the HTML content
print(html_content)