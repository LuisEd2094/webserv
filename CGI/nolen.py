print("HTTP/1.1 200 OK")
print("Content-Type: text/html")
print("")

# HTML content to be displayed in the browser
print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test</title>
</head>
<body>
    <h1>CGI Test Page</h1>
    <p>This is a simple CGI test.✊</p>
</body>
</html>
""")