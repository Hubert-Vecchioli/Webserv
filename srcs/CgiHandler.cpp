#include "CgiHandler.hpp"

CgiHandler::CgiHandler(HttpResponse const &response) {
	_env["AUTH_TYPE"] = "";
	_env["CONTENT_LENGTH"] = std::to_string(response.getRequest.getContentLength);
	_env["CONTENT_TYPE"] = response.getRequest.getContentType();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = "";
	_env["PATH_TRANSLATED"] = "";
	_env["QUERY_STRING"] = request.getQueryString();
	_env["REMOTE_ADDR"] = "";
	_env["REMOTE_HOST"] = "";
	_env["REMOTE_IDENT"] = "";
	_env["REMOTE_USER"] = "";
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["SCRIPT_NAME"] = "";
	_env["SERVER_NAME"] = "";
	_env["SERVER_PORT"] = "";
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "webserv";
}
