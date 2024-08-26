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
age = form.getvalue("age", "None")

# Generate and print the HTML response
print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Form Response</title>
</head>
<body>
    <h1>Form Submission Result</h1>
    <p>Name: {name}</p>
    <p>Age: {age}</p>
</body>
</html>
""")