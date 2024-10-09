/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:19 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/08 19:13:28 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void HttpResponse::_generateResponseContent(void)
{
	//Check redirect and prepare the response
	
	if(/*request is CGI*/)
		// trigger the CGI
	switch(/*Request Method*/)
	{
		case "GET":
			this->_generateGETResponse();
			break;
		case "POST":
			this->_generatePOSTResponse();
			break;
		case "PUT":
			this->_generatePUTResponse();
			break;
		case "DEL":
			this->_generateDELResponse();
			break;
		default:
			throw this->_generateInvalidResponse(405);
	}
	
}

void HttpResponse::_generateInvalidResponse(int code)
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

	this->_reponseContent = "HTTP/1.1 "/*code and its message*/"\r\n";
	this->_reponseContent += "Content-Type: application/json\r\n";
	this->_reponseContent += "Content-Length: " + sizeStr + "\r\n";
	this->_reponseContent += "Allow: " + /*Allowed Methods*/ + "\r\n";
	this->_reponseContent += "\r\n";
	this->_reponseContent += reponseBody
}

void HttpResponse::_generateGETResponse(void)
{
	/*TODO*/
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

//Assuming PUT is only to upload files
void HttpResponse::_generatePUTResponse(void)
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