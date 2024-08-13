#!/usr/bin/env python3

import cgi
import cgitb

# Enable error logging to the browser
cgitb.enable()

# Get form data
form = cgi.FieldStorage()

# Extract data from the form fields
name = form.getvalue("name")
age = form.getvalue("age")

# Print the HTTP headers
print("Content-Type: text/html\n")

# Print the HTML response
print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Hello CGI</title>
</head>
<body>
    <h1>Hello, {name}!</h1>
    <p>You are {age} years old.</p>
</body>
</html>
""")
