
import os
import http.cookies
import cgi

form = cgi.FieldStorage()
name = form.getvalue('name', 'wonderful person')

cookie = http.cookies.SimpleCookie()
cookie['name'] = name
# cookie['name']['path'] = '/'
# cookie['name']['httponly'] = True

print("HTTP/1.1 302 Found")
print(cookie.output())
print("Content-type: text/html")
print("Location: say_hello.py")
print("Content-Length: 0")
print()
