/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:19 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/16 19:19:15 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void HttpResponse::_generateResponseContent(HttpRequest  & request)
{
	//Check redirect and prepare the response
	// TODO: add a pointer to HTTPrequest
	
	// if(/*request is CGI*/)
	// 	// trigger the CGI

	//will need to add the try & catch
	switch(request.getMethod())
	{
		case GET:
			this->_generateGETResponse();
			break;
		case POST:
			this->_generatePOSTResponse();
			break;
		case DELETE:
			this->_generateDELResponse();
			break;
		default:
			throw this->_generateErrorResponse(405, ClientError(405).what());
	}
	
}

ServerBlock	& HttpResponse::_fetchServerBlock(HttpRequest & request) {
	std::string host = request.getHost();
	std::vector<ServerBlock> server_blocks = _server.getConfigurationFile().getServerBlocks();
	ServerBlock server_block;

	for (size_t i = 0; i < server_blocks.size(); i++) {
		std::vector<std::string> server_names = server_blocks[i].getServerName();
		for (size_t j = 0; j < server_names.size(); j++) {
			if (host == server_names[j])
				return server_blocks[i];
		}
	}
	if !(server_block)
		throw ClientError(400);
}

LocationBlock &	HttpResponse::_fetchLocationBlock(HttpRequest & request, ServerBlock & server_block) {
	std::string uri = request.getRequestURI();
	if (uri.size() > MAX_URI_SIZE)
		throw ClientError(414);

	std::vector<LocationBlock> location_blocks = server_block.getLocationBlocks();
	
	while (uri.size() > 0) {
		for (size_t i = 0; i < location_blocks.size(); i++) {
			if (location_blocks[i].getLocation() == uri)
				return location_blocks[i];
		}
		size_t pos = uri.find_last_of('/')
		if (pos != std::string::npos && pos != 0)
			uri = uri.substr(pos);
		else if (pos == 0 && uri.size() > 1)
			uri = "/";
		else
			throw ClientError(404);
	}
	throw ClientError(404);
}

// CURRENT WORK => GET CONTENT TYPE
// std::string getContentType(HttpRequest & request) {
// 	std::string uri = request.getRequestURI();
// 	if ((size_t pos = uri.find('?')) != std::string::npos) {
// 		uri = uri.substr(pos);
// 	}
// }

void HttpResponse::_generateGETResponse(HttpRequest & request)
{
	std::stringstream ss;
	if (request.getHTTP == false)
		return _generateErrorResponse(505, ServerError(505).what());
	try {
		ServerBlock server_block = _fetchServerBlock(request);
		try {
			LocationBlock location_block = _fetchLocationBlock(request, server_block);
			std::vector<std::string> methods = location_block.getMethods();
			if (methods.find(request.getMethod) == methods.end())
				return _generateErrorResponse(405, ClientError(405).what());
			
		}
	}
	catch (HttpResponse::ClientError & e) {
		return _generateErrorResponse(e.getErrorCode(), e.what());
	}
}

//Assuming DEL is only to del files
void HttpResponse::_generateDELResponse(void)
{
	// TODO: Review if the file to delete exists, if not error 404
	// TODO: Review if the file can be deleted, if not error 403

	this->_reponseContent = "HTTP/1.1 204 No Content\r\n";
}

//Assuming POST is only to upload files
void HttpResponse::_generatePOSTResponse(void)
{
	std::ostringstream oss;
	oss << /*TODO: Get the file size*/;
    std::string sizeStr = oss.str();
	
	std::string reponseBody = "{\n";
	reponseBody += "\"message\": \"File uploaded successfully.\",\n";
	reponseBody += "\"filename\": \"" + /*Path file*/ + "\",\n";
	reponseBody += "\"uploadedAt\": \""+ displayTimestampResponseFormat()+ "\",\n";
	reponseBody += "\"size\": " + sizeStr + "\n";
	reponseBody += "}\n";

	std::ostringstream oss2;
	oss2 << reponseBody.size();
    std::string sizeStr = oss2.str();

	this->_reponseContent = "HTTP/1.1 201 Created\r\n";
	this->_reponseContent += "Content-Type: application/json\r\n";
	this->_reponseContent += "Content-Length: " + sizeStr + "\r\n";
	this->_reponseContent += "\r\n";
	this->_reponseContent += reponseBody
}

void HttpResponse::_generateErrorResponse(int errorCode, char *errorMessage, ServerBlock & server_block) {
	std::stringstream ss;
	
	ss << "HTTP/1.1 " << errorCode << " " << errorMessage << "\r\n";
	ss << _displayTimeStamp() << "\r\n";
	ss << "Content-Type: text/html\r\n";
	if (!server_block)
		return _generateGenericErrorResponse(errorCode, errorMessage);
	std::string error_page = server_block.getErrorPages()[errorCode];
	if (!error_page)
		return _generateGenericErrorResponse(errorCode, errorMessage);
	struct stat stats;
	if (stat(&stats) == -1)
		return _generateGenericErrorResponse(errorCode, errorMessage);
	size_t size = stats.st_size;
	ss << "Content-Size: " << size << "\r\n";
	ss << "\r\n";
	
	std::ifstream file(error_page.c_str());
	if (!file.is_open())
		return _generateGenericErrorResponse(errorCode, errorMessage);
	std::string line;
	while (getline(file, line))
		ss << line << "\r\n";
	file.close();
	_responseContent = ss.str();		
}

void HttpResponse::_generateGenericErrorResponse(int errorCode, char *errorMessage) {
	std::stringstream ss;
	std::stringstream body;

	body << "<html><head><title>";
	body << errorCode << " " << errorMessage;
	body << "</title></head><body><h1>";
	body << errorCode << " " << errorMessage;
	body << "</h1></body></html>";
	
	ss << "HTTP/1.1 " << errorCode << " " << errorMessage;
	ss << _displayTimeStamp()<< "\r\n";
	ss << "Content-Type: text/html\r\n";
	ss << "Content-Lenght: " << body.gcount() << "\r\n";
	ss << "\r\n";
	ss << body;

	_responseContent = ss.str();
}

std::stringstream	HttpResponse::_displayTimeStamp(void) {
	std::stringstream ss;
	struct tm *gmt = gmtime(&_lastActionTimeStamp);
	char buffer[100];
	
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);

	ss << "Date: " << buffer;
    return ss;
}

std::string HttpResponse::getResponseContent(void)
{
	if (this->_reponseContent.empty())
		this->_generateResponseContent();
	return (this->_reponseContent());
}

const char *HttpResponse::ServerError::what() const throw() {
	if (code < 500 || code > 505)
		return "Unknown error\n";
	else {
		std::string errorMessage = _errorMap[_errorCode];
		return errorMessage.c_str();
	}
}

const char *HttpResponse::ClientError::what() const throw() {
	if (code < 400 || (code > 415 && code != 429))
		return "Unknown error\n";
	else {
		std::string errorMessage = _errorCode[_errorCode];
		return errorMessage.c_str();
	}
}