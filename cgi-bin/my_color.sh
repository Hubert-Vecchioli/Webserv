#!/bin/bash
read -r input

# Get the color + decode the url formatting (' ' instead of '+' & %xx by ascii)
color=$(echo "$input" | sed -n 's/.*color=\([^&]*\).*/\1/p'| sed 's/+/ /g;s/%/\\x/g')

echo "HTTP/1.1 200 OK"
echo "Content-Type: text/html; charset=utf-8"


cat << PP > ${color}.css
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>About - WebSerVaBienOuBien Tester</title>
		<link rel="icon" type="image/png" sizes="96x96" href="/favicon-96x96.png">
		<style>
			body {
				font-family: Helvetica, sans-serif;
				text-align: center;
				background: linear-gradient(90deg, $color, #faae5a);
				color: white;
			}

			main {
				justify-content: center;
				width: 50%;
				padding: 20px;
				margin: 20px auto;
				display: flex;
				flex-direction: column;
			}

			img {
				justify-items: center;
				margin: 20px auto;
				padding: 20px;
			}

			li {
				list-style-type: none;
			}

			.pointer-list li {
				padding: 5px 0;
				position: relative;
			}

			.pointer-list li:hover::before {
				content: "👉";
				position: absolute;
				left: 50px;
			}

			h1 {
				color: $color;
				margin-bottom: 20px;
				/* text-align: center; */
				width: 100%;
			}

			p {
				margin-bottom: 20px;
				/* text-align: center; */
				width: 100%;
			}

			strong {
				font-weight: 700;
			}

			header {
				padding: 10px 0;
			}

			.navbar {
				/*display: flex;*/
				justify-content: center;
			}

			.navbar ul {
				list-style-type: none;
				display: flex;
			}

			.navbar li {
				margin: 0 20px;
				color: rgba(255, 255, 255, 0.7); /* Couleur claire */
				transition: color 0.3s, transform 0.3s; /* Animation douce pour le changement */
			}

			.navbar li.active {
				color: rgba(255, 255, 255, 1); /* Titre de la page active plus foncé */
			}

			.navbar li:hover {
				color: rgba(255, 255, 255, 1); /* Couleur au survol */
				transform: scale(1.2); /* Augmenter la taille au survol */
			}

			a, a:hover, a:visited, a:active {
				color: inherit;
				text-decoration: none;
			}

			.styled-form {
				max-width: 500px;          /* Limit form width */
				margin: 0 auto;            /* Center the form */
				padding: 20px;             /* Add padding inside the form */
				border-radius: 10px;       /* Rounded corners */
				box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1); /* Add shadow */
			}

			.styled-form label {
				display: block;            /* Make label a block element */
				margin-bottom: 8px;        /* Space between label and input */
				font-weight: bold;         /* Make label text bold */
			}

			.styled-form input[type="text"],
			.styled-form input[type="email"],
			.styled-form textarea {
				width: 90%;               /* Full width of input fields */
				padding: 10px;             /* Padding inside the input */
				margin-bottom: 20px;       /* Space below each input field */
				border: 1px solid #ccc;    /* Light gray border */
				border-radius: 5px;        /* Rounded corners */
				font-size: 16px;           /* Increase font size for readability */
				background-color: rgba(255, 255, 255, 0.123);

			}

			input::placeholder, textarea::placeholder {
				color: rgba(255, 255, 255, 0.329);
			}

			.styled-form textarea {
				height: 100px;
				resize: vertical;
				font-family: Arial, Helvetica, sans-serif;
			}

			.styled-form button {
				width: 100%;               /* Full width button */
				padding: 10px;             /* Padding inside the button */
				background-color: transparent;
				color: white;              /* Button text color */
				border: none;              /* Remove border */
				border-radius: 5px;        /* Rounded corners */
				font-size: 16px;           /* Increase font size */
				cursor: pointer;           /* Pointer cursor on hover */
				transition: background-color 0.3s ease; /* Smooth background color transition */
			}

			.styled-form button:hover {
				background-color: #ffffff10;
			}

			.error-container {
				text-align: center;
			}

			.error-code {
				font-size: 10em;
				font-weight: bold;
				color: #55228f;
			}

			.error-text {
				font-size: 1em;
				color: #ffffff;
			}

			.container {
				display: grid;
				grid-template-columns: repeat(2, 1fr); /* 2 columns */
				gap: 20px;
				width: 60%; /* Smaller width to fit your design */
				max-width: 800px; /* Maximum width for the container */
				margin: 20px auto; /* Center the grid on the page */
			}

			.page-box {
				display: flex;
				flex-direction: column;
				justify-content: flex-start;
				align-items: center;
				width: 100%;
				padding: 10px;
				box-sizing: border-box;
				border: 2px solid #55228f70; /* Dark purple border */
				border-radius: 10px;
				background-color: transparent;
				transition: background-color 0.3s, box-shadow 0.3s;
				text-align: center;
				word-wrap: break-word;
				/* min-height: 150px; Reduced height */
			}

			.page-content {
				flex: 1; /* Allows the content to take up remaining space */
				display: flex;
				flex-direction: column;
				justify-content: center; /* Center the paragraph text vertically */
				align-items: center; /* Center the paragraph text horizontally */
				width: 100%; /* Full width of the box */
			}

			.page-box h2 {
				text-decoration: none;
				color: #55228f; /* Dark purple text */
				font-size: 18px; /* Slightly smaller font */
				margin: 0;
			}

			.page-box p {
				/* margin-top: 10px; */
				color: white; /* White text for better contrast with background */
				font-size: 14px; /* Slightly smaller font */
			}

			.page-box:hover {
				background-color: rgba(255, 255, 255, 0.1); /* Very transparent white */
				box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); /* Light shadow effect */
			}

			.page-box:hover h2 {
				color: #faae5a; /* Color change on hover */
			}

			/* Responsive adjustments */
			@media screen and (max-width: 800px) {
				.container {
					width: 90%; /* Increase container width to fit screen */
					grid-template-columns: 1fr; /* Change to 1 column layout */
				}
			}

			@media screen and (max-width: 500px) {
				.page-box {
					height: auto; /* Allow height to be flexible */
				}
				
				.page-box h2 {
					font-size: 16px; /* Adjust font size for smaller screens */
				}

				.page-box p {
					font-size: 12px; /* Adjust font size for smaller screens */
				}
			}
		</style>
	</head>
	<body>
		<header>
			<nav class="navbar">
				<ul>
					<li><a href="/index.html">Home</a></li>
					<li><a href="/about.html">About</a></li>
					<li><a href="/contact.html">Contact</a></li>
					<li><a href="/destroyer.html">Destroyer</a></li>
					<li class="active"><a href="say_hello.html">Say Hello</a></li>
				</ul>
			</nav>
		</header>
		<main>
			<h1>Hello... But what's your name?</h1>
			<form class="styled-form" method="get" action="../cgi-bin/say_hello.py">
				<input type="text" name="name" placeholder="name" autofocus>
				<button type="submit">Submit</button>
			</form>
			<!-- Test Hubert -->
			<h2>How about changing the color ?</h2>
			<form action="../cgi-bin/my_color.sh" method="post">
				<label for="color">Choose a color:</label>
				<select id="color" name="color">
					<option value="red">Red</option>
					<option value="blue">Blue</option>
					<option value="green">Green</option>
					<option value="yellow">Yellow</option>
					<option value="purple">Purple</option>
				</select>
				<br><br>
				<input class="button" type="submit" value="Submit">
			</form>
		</main>
	</body>
</html>
PP

$len = wc -c ${color}.css
echo -n "Content-Length: $(stat -c%s "${color}.css")" 
echo -e "\r\n\r\n"
cat ${color}.css
rm ${color}.css
