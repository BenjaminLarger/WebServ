from datetime import datetime

def generate_html():
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

  return html_content

if __name__ == "__main__":
  print(generate_html())
