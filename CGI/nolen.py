import time

import os
cwd = os.getcwd()
time.sleep(0)


print("HTTP/1.1 200 OK")
print("Content-Type: text/html; charset=utf-8")

# HTML content to be displayed in the browser
print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test</title>
</head>
<body>
      """ + cwd + """
    <h1>CGI Test Page</h1>
    <p>This is a simple CGI test.✊</p>
</body>
</html>
""")

