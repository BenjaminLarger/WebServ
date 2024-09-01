from datetime import datetime

current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

html_content = f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Python HTML Page</title>
</head>
<body>
    <h1>This was built in Python for Webserv</h1>
    <p>Current date and time: {current_time}</p>
</body>
</html>
"""

# Calculate the content length
content_length = len(html_content)

# Print the HTTP headers to indicate HTML content
print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print(f"Content-Length: {content_length + 2}")
print("\r\n")  # End of headers section

print(html_content)
