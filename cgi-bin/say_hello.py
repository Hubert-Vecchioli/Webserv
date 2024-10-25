import cgi, os, datetime

form = cgi.FieldStorage

name = form.getvalue('name', 'wonderful person')

print ("Content-type:text/html")

response_body = f"""
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Hello you - WebSerVaBienOuBien Tester</title>
		<link href="../web/style.css" rel="stylesheet">
	</head>
	<body>
		<header>
			<nav class="navbar">
				<ul>
					<li><a href="../web/index.html">Home</a></li>
					<li><a href="../web/about.html">About</a></li>
					<li><a href="../web/contact.html">Contact</a></li>
					<li class="active"><a href="../web/say_hello.html">Say Hello</a></li>
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

print(response_body)