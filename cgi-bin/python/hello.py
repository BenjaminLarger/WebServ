#!/usr/bin/env python3

import cgi
import cgitb
import os
import sys

# Enable debugging
cgitb.enable()

# Print the HTTP headers to indicate HTML content
print("Content-Type: text/html\n")

# Print debugging information
print("<html><body>")
print("<h1>Debugging Information</h1>")
print("<pre>")

# Print environment variables
print("Environment Variables:\n")
for key, value in os.environ.items():
    print(f"{key}: {value}")

# Print the request method
print("\nRequest Method: " + os.environ.get("REQUEST_METHOD", "Unknown"))

# Print raw input data
print("\nRaw Input Data:\n")
try:
    input_length = int(os.environ.get("CONTENT_LENGTH", 0))
    raw_input = sys.stdin.read(input_length)
    print(raw_input)
except Exception as e:
    print(f"Error reading input data: {e}")

# Create an instance of FieldStorage to retrieve form data
form = cgi.FieldStorage()

# Check if the form has any keys
if not form.keys():
    print("\nNo form data received.")
else:
    for key in form.keys():
        value = form.getvalue(key)
        value_size = len(value) if value else 0
        print(f"{key}: {value} (Size: {value_size})")

print("</pre>")

# Retrieve the name and age fields from the form
name = form.getvalue("name")
age = form.getvalue("age")

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