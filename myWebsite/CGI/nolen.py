import os
import sys

try:
    cwd = os.getcwd()

    content_length = os.environ.get('CONTENT_LENGTH', '')

    # Print the HTTP response headers
    print("HTTP/1.1 200 OK")
    print("Content-Type: text/html; charset=utf-8")
    print("")

    # Read the input data from stdin if CONTENT_LENGTH is set
    input_data = ""
    if content_length:
        input_data = sys.stdin.read(int(content_length))

    # HTML content to be displayed in the browser
    print("""
    <!DOCTYPE html>
    <html>
    <head>
    <title>CGI Test</title>
    </head>
    <body>
    """)

    # Print the current working directory and QUERY_STRING
    print(f"<p>Current Working Directory: {cwd}</p>")

    # Print input data if available
    if input_data:
        print(f"<p>This is input: {input_data}</p>")

    # Print all environment variables
    print("<h2>Environment Variables</h2>")
    print("<ul>")
    for key, value in os.environ.items():
        print(f"<li><strong>{key}</strong>: {value}</li>")
    print("</ul>")

    # Close the HTML content
    print("""
    <h1>CGI Test Page</h1>
    <p>This is a simple test.✊</p>
    </body>
    </html>
    """)
except Exception as e:
    pass