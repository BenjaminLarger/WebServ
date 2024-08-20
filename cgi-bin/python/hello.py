#!/usr/bin/env python3

import cgi
import cgitb

# Enable debugging
cgitb.enable()

# Create an instance of FieldStorage to retrieve form data
form = cgi.FieldStorage()

# Retrieve the name and age fields from the form
name = form.getvalue("name")
age = form.getvalue("age")

# Print the HTTP headers to indicate HTML content
print("Content-Type: text/html\n")

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
