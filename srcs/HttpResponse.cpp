/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:19 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/15 16:38:31 by jblaye           ###   ########.fr       */
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
			throw this->_generateInvalidMethod();
	}
	
}

void HttpResponse::_generateInvalidMethod(void)
{
/*
Example
HTTP/1.1 405 Method Not Allowed
Content-Type: application/json
Content-Length: 88
Allow: GET, POST

{
  "error": "Method Not Allowed",
  "message": "The DELETE method is not allowed for the requested resource."
}*/
	
	std::string reponseBody = "{\n";
	reponseBody += "\"error\": \"Method Not Allowed\",\n";
	reponseBody += "\"message\": \"The " + /*Request Method*/ + " method is not allowed for the requested resource.\",\n";
	reponseBody += "}\n";

	std::ostringstream oss;
	oss << reponseBody.size();
    std::string sizeStr = oss.str();

	this->_reponseContent = "HTTP/1.1 405 Method Not Allowed\r\n";
	this->_reponseContent += "Content-Type: application/json\r\n";
	this->_reponseContent += "Content-Length: " + sizeStr + "\r\n";
	this->_reponseContent += "Allow: " + /*Allowed Methods*/ + "\r\n";
	this->_reponseContent += "\r\n";
	this->_reponseContent += reponseBody
}

void HttpResponse::_generateGETResponse(HttpRequest & request)
{
	if (request.getHTTP == false)
		throw ServerError(505);
	if ()
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

LocationBlock &	HttpResponse::_fetchLocationBlock(HttpRequest & request) {
	std::string host = request.getHost();
	std::vector<ServerBlock> server_blocks = _server.getConfigurationFile().getServerBlocks();
	ServerBlock server_block;
	
	// Fecthing the appropriate server block;
	for (size_t i = 0; i < server_blocks.size(); i++) {
		std::vector<std::string> server_name = server_blocks[i].getServerName();
		for (size_t j = 0; j < server_name.size(); j++) {
			if (host == server_name[j])
				server_block = server_blocks[i];
				break;
		}
	}
	if !(server_block)
		throw ClientError(400);

	std::string uri = request.getRequestURI();
	if (uri.size() > 65536)
		throw ClientError(414);

	std::vector<LocationBlock> location_blocks = server_block.getLocationBlocks();
	LocationBlock location_block;

	while (!location_block  && uri.size() > 0) {
		for (size_t i = 0; i < location_blocks.size(); i++) {
			if (location_blocks[i].getLocation() == uri) {
				location_block = location_blocks[i];
				break;
			}
		}
		size_t pos = uri.find_last_of('/')
		if (pos != std::string::npos && pos != 0)
			uri = uri.substr(pos);
		else if (pos == 0)
			uri = "/";
		else
			throw ClientError(404);
	}

	if (!location_block)
		throw ClientError(404);
	return location_block;
}

//Assuming DEL is only to del files
void HttpResponse::_generateDELResponse(void)
{
	// TODO: Review if the file to delete exists, if not error 404
	// TODO: Review if the file can be deleted, if not error 403

	this->_reponseContent = "HTTP/1.1 204 No Content\r\n";
}


std::string HttpResponse::getResponseContent(void)
{
	if (this->_reponseContent.empty())
		this->_generateResponseContent();
	return (this->_reponseContent);
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