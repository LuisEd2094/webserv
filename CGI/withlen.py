html_content = """
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test</title>
</head>
<body>
    <h1>CGI Test Page</h1>
    <p>This is a simple CGI test.</p>
</body>
</html>
"""

content_length = len(html_content)

print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("Content-Length:", content_length)
print("")

# HTML content to be displayed in the browser
print(html_content)