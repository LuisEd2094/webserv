import time
import sys

import os
cwd = os.getcwd()
env = os.environ['HOME']
len = ""

try:
    len = os.environ['CONTENT_LENGTH']    
except:
    len = ""

#input_data = sys.stdin.read()
if len != "":
    input_data = sys.stdin.read()
else:
    input_data = ""

print("HTTP/1.1 200 OK")
print("Content-Type: text/html; charset=utf-8")


# HTML content to be displayed in the browser
print(
len + """
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test</title>
</head>
<body>
      """ + cwd + "ENV: " + env+ """
    <h1>CGI Test Page</h1>
    <p>This is a simple CGI test.✊</p>
</body>
</html>
""")

if input_data != "":
    print("this is input: " + input_data)

