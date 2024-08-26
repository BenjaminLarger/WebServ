#!/usr/bin/env python3

import cgi
import cgitb
import os
import sys

# Enable debugging
cgitb.enable()

# Print the HTTP headers to indicate HTML content
print("HTTP/1.1 200 OK")
print("Content-type:text/html\r\n\r\n")

# Create an instance of FieldStorage to retrieve form data
form = cgi.FieldStorage()

# Retrieve the name and age fields from the form
name = form.getvalue("name", "None")

# Generate and print the HTML response
print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Form Response</title>
    <style>
        body {{
            background-color: black;
						font-family: 'Arial, sans-serif', monospace;
            color: #00FF00;
						text-align: center;
						font-size: 2rem; 
        }}
        h1 {{
						font-family: 'Arial, sans-serif', monospace;
            text-align: center;
						font-size: 60px;
						font-size: 3rem;
        }}
    </style>
</head>
<body>
    <h1 style="color: white;">Hello {name}!</h1>
		<a href='https://hackertyper.net/#'><img src='https://raw.githubusercontent.com/BenjaminLarger/BenjaminLarger/main/walk.gif' alt='walking' border='0'></a>
</body>
</html>
""")