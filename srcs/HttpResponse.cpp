/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:19 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/17 18:00:24 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void HttpResponse::_generateResponseContent(void)
{
	//Check redirect and prepare the response
	// TODO: add a pointer to HTTPrequest
	
	

	if (_request.getHTTP == false)
		return _generateErrorResponse(505, ServerError(505).what());

	try {
		_fetchServerBlock(_request);
		_fetchLocationBlock(_request);
		_checkAllowedMethod(_request);
	}
	catch (HttpResponse::ClientError & e) {
		return _generateErrorResponse(e.getErrorCode(), e.what());
	}
	
	// if(/*_request is CGI*/)
	// 	// trigger the CGI
	//will need to add the try & catch
	switch(_request.getMethod())
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

void	HttpResponse::_fetchServerBlock(void) {
	std::string host = _request.getHost();
	std::vector<ServerBlock> server_blocks = _server.getConfigurationFile().getServerBlocks();
	ServerBlock server_block;

	for (size_t i = 0; i < server_blocks.size(); i++) {
		std::vector<std::string> server_names = server_blocks[i].getServerName();
		for (size_t j = 0; j < server_names.size(); j++) {
			if (host == server_names[j]) {
				_server_block = server_blocks[i];
				return ;
			}
		}
	}
	if !(server_block)
		throw ClientError(400);
}

void	HttpResponse::_fetchLocationBlock(void) {
	std::string uri = _request.getRequestURI();
	if (uri.size() > MAX_URI_SIZE)
		throw ClientError(414);

	std::vector<LocationBlock> location_blocks = _server_block.getLocationBlocks();
	
	while (uri.size() > 0) {
		for (size_t i = 0; i < location_blocks.size(); i++) {
			if (location_blocks[i].getLocation() == uri) {
				_location_block = location_blocks[i];
				return ;
			}
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

void HttpResponse::_checkAllowedMethod(void) {
	std::vector<std::string> methods = _location_block.getMethods();
	if (methods.find(_request.getMethod) == methods.end())
		throw ClientError(405);
}

void HttpResponse::_fetchGETResource(void) {
	std::string uri_no_query = _request.getRequestURI();
	
	if((size_t pos = uri_no_query.find('?')) != std::string::npos)
		uri_no_query = uri_no_query.substr(pos);
	
	std::string path = _location_block.getRoot() + uri_no_query;
	struct stat st;
	if (stat(path.c_str(), &st) == -1)
		throw ClientError(404);
	int fd;
	if (S_ISDIR(st.st_mode)) {
		try {
			fd = _fetchDirectoryRessource(_request, path);
		}
		catch (ClientError &e) {
			throw e;
		}
	}
	if (S_ISREG(st.st_mode)) {
		int fd = open(path.c_str(), O_RDONLY);
		if (fd == -1)
			throw ClientError(403);
		try {
			_checkAcceptedFormat(_request, path);}
		catch (ClientError &e) {
			throw e;
		}
		return
	}
}


int	HttpResponse::_fetchDirectoryRessource(std::string path) {
	std::vector<std::string> index = _location_block.getIndex();

	for (size_t i; i < index.size(); i++) {
		std::string new_path = path + index[i];
		int fd = open(new_path.c_str(), O_RDONLY);
		if (fd > 0){
			if (_checkAcceptedFormat(_request, new_path) == true)
				return int;
			else
				close(fd);
		}
	}
	if (_location_block.getDirlisting() == true) {
		_generateDirlistingResponse(_request);
		return 0;
	}
	throw ClientError(404);
}

void	HttpResponse::_generateDirlistingResponse(void) {
	/* To do parce que tbh j en ai marre*/
}

bool HttpResponse::_checkAcceptedFormat(std::string path) {
	std::vector<std::string> accepted_formats = _request.getAccept();
	
	if (!accepted_formats)
		return true;
	if (accepted_formats.find("*/*") != accepted_formats.end())
		return true;
	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return false; // TBC HERE => i refuse any file without an extension because there is no way to know if it s binary or text
	std::string extension = path.substr(pos);
	if (_mimeMap[extension]) {
		std::string mime_type = _mimeMap[extension];
		if (accepted_formats.find(mime_type) != accepted_formats.end())
			return true;
		if ((size_t pos = mime_type.find("/")) != std::string::npos) {
			mime_type = mime_type.substr(pos) + "/*";
			if (accepted_formats.find(mime_type) != accepted_formats.end())
				return true;
		}
	}
	return false;
}

// CURRENT WORK => GET CONTENT TYPE
// std::string getContentType(void) {
// 	std::string uri = request.getRequestURI();
// 	if ((size_t pos = uri.find('?')) != std::string::npos) {
// 		uri = uri.substr(pos);
// 	}
// }

void HttpResponse::_generateGETResponse(void)
{
	std::stringstream ss;
	
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

void HttpResponse::_generateErrorResponse(int errorCode, char *errorMessage) {
	std::stringstream ss;
	
	ss << "HTTP/1.1 " << errorCode << " " << errorMessage << "\r\n";
	ss << _displayTimeStamp() << "\r\n";
	ss << "Content-Type: text/html\r\n";
	if (!_server_block)
		return _generateGenericErrorResponse(errorCode, errorMessage);
	std::string error_page = _server_block.getErrorPages()[errorCode];
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

void	HttpResponse::_generateMimeMap(void) {
	_mimeMap[".html"] = "text/html";
    _mimeMap[".htm"] = "text/html";
    _mimeMap[".css"] = "text/css";
    _mimeMap[".js"] = "application/javascript";
    _mimeMap[".json"] = "application/json";
    _mimeMap[".xml"] = "application/xml";
    _mimeMap[".txt"] = "text/plain";

    _mimeMap[".jpeg"] = "image/jpeg";
    _mimeMap[".jpg"] = "image/jpeg";
    _mimeMap[".png"] = "image/png";
    _mimeMap[".gif"] = "image/gif";
    _mimeMap[".bmp"] = "image/bmp";
    _mimeMap[".svg"] = "image/svg+xml";
    _mimeMap[".ico"] = "image/x-icon";

    _mimeMap[".pdf"] = "application/pdf";
    _mimeMap[".zip"] = "application/zip";
    _mimeMap[".rar"] = "application/x-rar-compressed";
    _mimeMap[".tar"] = "application/x-tar";
    _mimeMap[".gz"] = "application/gzip";

    _mimeMap[".mp4"] = "video/mp4";
    _mimeMap[".avi"] = "video/x-msvideo";
    _mimeMap[".mov"] = "video/quicktime";
    _mimeMap[".mp3"] = "audio/mpeg";
    _mimeMap[".wav"] = "audio/wav";
    _mimeMap[".ogg"] = "audio/ogg";

    _mimeMap[".doc"] = "application/msword";
    _mimeMap[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    _mimeMap[".xls"] = "application/vnd.ms-excel";
    _mimeMap[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    _mimeMap[".ppt"] = "application/vnd.ms-powerpoint";
    _mimeMap[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
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