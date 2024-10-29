import cgi, os, datetime

form = cgi.FieldStorage()

name = form.getvalue("name", "wonderful person")
print(f"HTTP/1.1 200 OK")
print ("Content-type:text/html")

response_body = f"""
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
					<li class="active"><a href="/say_hello.html">Say Hello</a></li>
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

content_length = len(response_body)

print(f"Content-Length: {content_length}")
if "SERVER_NAME" in os.environ:
	server_name = os.environ.get('SERVER_NAME')
	print(f"Server: {server_name}")
date = datetime.datetime.now().astimezone().strftime("%a, %d %b %Y %T %Z")
print(f"Date: {date}\n")
print()
print(response_body)