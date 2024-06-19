import time
import sys

import os
cwd = os.getcwd()
query =""
try:
    query = os.environ['QUERY_STRING']
except:
    pass
len = ""

try:
    len = os.environ['CONTENT_LENGTH']    
except:
    len = ""


log = open("log.log", "w" )

print("HTTP/1.1 200 OK")
print("Content-Type: text/html; charset=utf-8")
print("")

input_data = ""
#input_data = sys.stdin.read()
if len != "":
    for line in sys.stdin:
        print(line, file=log)
        input_data += line
else:
    input_data = ""

# HTML content to be displayed in the browser
print(
len + """
<!DOCTYPE html>
<html>
<head>
    <title>CGI Test</title>
</head>
<body>
      """ + cwd + "ENV: " + query + """
    <h1>CGI Test Page</h1>
    <p>This iadsadasdsadsadasdasdass a simple  test.✊</p>
</body>
</html>
""")

if input_data != "":
    print("this is input: " + input_data)

