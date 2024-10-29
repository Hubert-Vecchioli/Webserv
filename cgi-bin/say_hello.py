
import os, cgi, http.cookies, datetime

print(f"HTTP/1.1 200 OK")
print(f"Content-type: text/html")

cookie_string = os.environ.get('HTTP_COOKIE', '')
cookie = http.cookies.SimpleCookie(cookie_string)

cookie_name = cookie.get('name')
name = cookie_name.value if cookie_name else None 

form_html = f"""
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Hello you - WebSerVaBienOuBien Tester</title>
		<link href="/style.css" rel="stylesheet">
		<link rel="icon" type="image/png" sizes="96x96" href="favicon-96x96.png">
	</head>
	<body>
		<header>
			<nav class="navbar">
				<ul>
					<li><a href="/index.html">Home</a></li>
					<li><a href="/about.html">About</a></li>
					<li><a href="/contact.html">Contact</a></li>
					<li><a href="/destroyer.html">Destroyer</a></li>
					<li><a href="/set_color.html">Set Color</a></li>
					<li class="active"><a href="/cgi-bin/say_hello.py">Say Hello</a></li>
				</ul>
			</nav>
		</header>
	<main>
		<h1>Hello... But what's your name?</h1>
			<form class="styled-form" method="get" action="/cgi-bin/set_name.py">
				<input type="text" name="name" placeholder="name" autofocus>
				<button type="submit">Submit</button>
			</form>
	</main>
</html>
"""

say_hello_html = f"""
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
			<title>Hello you - WebSerVaBienOuBien Tester</title>
			<link href="/style.css" rel="stylesheet">
			<link rel="icon" type="image/png" sizes="96x96" href="/favicon-96x96.png">
		</head>
		<body>
			<header>
				<nav class="navbar">
					<ul>
						<li><a href="/index.html">Home</a></li>
						<li><a href="/about.html">About</a></li>
						<li><a href="/contact.html">Contact</a></li>
						<li><a href="/destroyer.html">Destroyer</a></li>
						<li><a href="/set_color.html">Set Color</a></li>
						<li class="active"><a href="/cgi-bin/say_hello.py">Say Hello</a></li>
					</ul>
				</nav>
			</header>
			<main>
				<h1>Hello {name}!</h1>
				<p>You look great today!</p>
			</main>
		</body>
	</html>
"""
	
if "SERVER_NAME" in os.environ:
	server_name = os.environ.get('SERVER_NAME')
	print(f"Server: {server_name}")
date = datetime.datetime.now().astimezone().strftime("%a, %d %b %Y %T %Z")
print(f"Date: {date}")

if name:
	content_length = len(say_hello_html)
	print(f"Content-Length: {content_length}")
	print()
	print(say_hello_html)
else:
	content_length = len(form_html)
	print(f"Content-Length: {content_length}")
	print()
	print(form_html)
