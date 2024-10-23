/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:56:19 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/23 16:12:34 by hvecchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void HttpResponse::_generateResponseContent(void)
{
	//Check redirect and prepare the response
	// TODO: add a pointer to HTTPrequest
	
	

	if (_request.getHTTP() == false) {
		ServerError error(505);
		return _generateErrorResponse(505, error.what());
	}

	try {
		_fetchServerBlock();
		_fetchLocationBlock();
			std::cout<< "ROROORORORO" << std::endl;
		_checkAllowedMethod();
	}
	catch (HttpResponse::ClientError & e) {
		return _generateErrorResponse(e.getErrorCode(), e.what());
	}
	// if(/*_request is CGI*/)
	// 	// trigger the CGI
	
	/*
	CgiHandler cgi(this);
	if (cgi.getStatus() != 200)
		gen the error response
	std::string out = cgi.getOutput();
	*/

	//will need to add the try & catch
	switch(_request.getMethod())
	{
		case GET:
			this->_fetchGETResource();
			break;
		case POST:
			this->_generatePOSTResponse();
			break;
		case DELETE:
			this->_generateDELResponse();
			break;
		default:
			_generateErrorResponse(405, ClientError(405).what());
	}
	
}

void	HttpResponse::_fetchServerBlock(void) {
	std::string host = _request.getHost();
	std::vector<ServerBlock> server_blocks = _server.getConfigurationFile().getServerBlocks();
	_server_block = 0;

	for (size_t i = 0; i < server_blocks.size(); i++) {
		std::vector<std::string> server_names = server_blocks[i].getServerName();
		for (size_t j = 0; j < server_names.size(); j++) {
			server_names[j] += "\r";
			if (host == server_names[j]) {
				_server_block = &server_blocks[i];
				return ;
			}
		}
	}
	if (!_server_block)
		throw ClientError(400);
}

void	HttpResponse::_fetchLocationBlock(void) {
	std::string uri = _request.getRequestURI();
	_location_block = 0;
	if (uri.size() > MAX_URI_SIZE)
		throw ClientError(414);

	std::vector<LocationBlock> location_blocks = _server_block->getLocationBlocks();
	
	while (uri.size() > 0) {
		for (size_t i = 0; i < location_blocks.size(); i++) {
			std::cout<< location_blocks[i].getLocation() << std::endl;
			std::cout<< uri << std::endl;
			if (location_blocks[i].getLocation() == uri) {
				_location_block = &location_blocks[i];
				return ;
			}
		}
		size_t pos = uri.find_last_of('/');
		if (pos != std::string::npos && pos != 0)
			uri = uri.substr(pos + 1);
		else if (pos == 0 && uri.size() > 1)
			uri = "/";
		else
			throw ClientError(404);
	}
	_location_block = 0;
	throw ClientError(404);
}

void HttpResponse::_checkAllowedMethod(void) {
	std::vector<std::string> methods = _location_block->getMethods();
	if (std::find(methods.begin(), methods.end(), _request.getStringMethod()) == methods.end())
		throw ClientError(405);
}

void HttpResponse::_fetchGETResource(void) {
	std::string uri_no_query = _request.getRequestURI();
	
	size_t pos = uri_no_query.find('?');
	if(pos != std::string::npos)
		uri_no_query = uri_no_query.substr(pos);
	
	std::string path = _location_block->getRoot() + uri_no_query;
	struct stat st;
	if (stat(path.c_str(), &st) == -1)
		throw ClientError(404);
	int fd;
	if (S_ISDIR(st.st_mode)) {
		try {
			fd = _fetchDirectoryRessource(path);
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
			_checkAcceptedFormat(path);
			close(fd);
			// if (_isFileAboveThreshold(path))
			// 	_generateChunkedGETResponseContent(path); //_isResponseSent en fcontion de la ou on en est
			// else
			_generateGETResponseContent(path); //this->_isResponseSent = true && to be added in the error page, POST & DEL
			}
		catch (ClientError &e) {
			throw e;
		}
		return ;
	}
}

// void HttpResponse::_generateChunkedGETResponseContent(std::string path)
// {
//     print(1, "[Info] - Opening file to answer the request from Client FD : ", this->_request.getClient()->getFD());
//     //Check si presence de l extension du fichier
// 	//TODO HV: Factoriser la partie concernant l'extension
//     size_t pos = path.find_last_of('.');
//     if (pos == std::string::npos)
//         throw ClientError(400);
//     std::string extension = path.substr(pos);
//     if (_mimeMap[extension].empty())
//         throw ClientError(400);
    
//     this->_responseContent = "HTTP/1.1 200 OK\r\n";
//     this->_responseContent += "Content-Type: " + _mimeMap[extension] + "\r\n";
//     this->_responseContent += "Transfer-Encoding: chunked\r\n";
//     this->_responseContent += "\r\n";

//     // int fileFd = open(path.c_str(), O_RDONLY);
//     // if (fileFd == -1)
//     // {
//     //     print(2, "[Error] - Failure to open the requested file from Client FD : ", this->_request.getClient()->getFD());
//     //     struct stat stats;
//     //     if (stat(path.c_str(), &stats) != 0)
//     //         throw ClientError(404); 
//     //     if ((stats.st_mode & S_IFDIR) != 0)
//     //         throw ClientError(403);
//     //     else
//     //         throw ClientError(404);
//     // }
// 	//TODO HV: A tester avec un tres long texte si ça passe
//     char buffer[RESPONSE_BUFFER];
//     ssize_t readSize = read(fileFd, buffer, RESPONSE_BUFFER);
//     if (readSize == -1)
//         throw ClientError(403);
//     else if (readSize == 0)
//     {
//         close(fileFd);
//         this->_responseContent += "0\r\n\r\n";
// 		// TODO HV: si timeout, est ce que je leak d'un fd non fermé??
//         // ajouter que la reponse est done // TODO avoir un bool: response is ready!
//     }
//     else
//     {
//         std::stringstream readSizeHex;
//         readSizeHex << std::hex << readSize;
//         this->_responseContent += readSizeHex.str() + "\r\n" + buffer + "\r\n";
//     }
// }

void HttpResponse::_generateGETResponseContent(std::string path)
{
    print(1, "[Info] - Opening file to answer the request from Client FD : ", this->_request.getClient()->getFD());
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos)
        throw ClientError(400);
    std::string extension = path.substr(pos);
    if (_mimeMap[extension].empty())
        throw ClientError(400);
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        print(2, "[Error] - Failure to open the requested file from Client FD : ", this->_request.getClient()->getFD());
        struct stat stats;
        if (stat(path.c_str(), &stats) != 0)
            throw ClientError(404); 
        if ((stats.st_mode & S_IFDIR) != 0)
            throw ClientError(403);
        else
            throw ClientError(404);
    }
    std::ostringstream content;
    std::string line;
    while (std::getline(file, line))
    {
        content << line << "\n";
    }
    file.close();
    std::string fileContent = content.str();
    std::ostringstream oss2;
    oss2 << fileContent.size();
    std::string sizeStr = oss2.str();

    this->_responseContent = "HTTP/1.1 200 OK\r\n";
    this->_responseContent += "Content-Type: " + _mimeMap[extension] + "\r\n";
    this->_responseContent += "Content-Length: " + sizeStr + "\r\n";
    this->_responseContent += "\r\n";
    this->_responseContent += fileContent;
}

// bool HttpResponse::_isFileAboveThreshold(std::string &path)
// {
// 	struct stat stats;
// 	if (stat(path.c_str(), &stats) != 0)
// 		return false;
// 	return stats.st_size > FILE_CHUNK_THRESHOLD;
// }

int	HttpResponse::_fetchDirectoryRessource(std::string path) {
	std::vector<std::string> index = _location_block->getIndex();

	for (size_t i = 0; i < index.size(); i++) {
		std::string new_path = path + index[i];
		int fd = open(new_path.c_str(), O_RDONLY);
		if (fd > 0){
			if (_checkAcceptedFormat(new_path) == true)
				return fd;
			else
				close(fd);
		}
	}
	if (_location_block->getDirlisting() == true) {
		_generateDirlistingResponse(path);
		return 0;
	}
	//close(fd); Propsotion to add this (from HV)
	throw ClientError(404);
}

bool HttpResponse::_isPathWithinRoot(std::string path)
{
		std::string root = _location_block->getRoot();
		if (path.size() < root.size())
			return (false);
		for(size_t i = 0; i < root.size(); ++i)
		{
			if (root[i] != path[i])
				return (false);
		}
		return (true);
}

void HttpResponse::_generateDirlistingResponse(std::string path)
{
	if (!this->_isPathWithinRoot(path)) //
		throw ClientError(403);

	DIR *dp = opendir(path.c_str());
	if (!dp)
		throw ClientError(404);

	std::vector<std::string> files;
	struct dirent *dirContent;
	while ((dirContent = readdir(dp)) != NULL)
	{
		files.push_back(dirContent->d_name);
	}
	closedir(dp);

	std::string responseBody = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Directory listing</title><style>body {font-family: 'Arial', sans-serif;background: linear-gradient(90deg, #8360c3, #faae5a);color: #fff;margin: 0;padding: 0;}h1 {text-align: center;color: #5d5fef;margin-top: 50px;}.container {max-width: 800px;margin: 20px auto;padding: 20px;box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.1);border-radius: 8px;}ul {list-style-type: none;padding: 0;}ul li {padding: 10px;border-bottom: 1px solid #ddd;}ul li a {text-decoration: none;color: #fff;}ul li:hover {background-color: #f0f0f5;}footer {text-align: center;margin-top: 20px;padding: 10px 0;background-color: #5d5fef;color: white;}</style></head><body><h1>Liste des fichiers</h1><div class=\"container\"><ul>";
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		responseBody += "<li><a href=\"";
		responseBody += *it;
		responseBody += "\">";
		responseBody += *it;
		responseBody += "</a></li>";
	}
	responseBody +="</ul></div><footer><p>Brought to you by JB, EB & HV</p></footer></body></html>";
	
	std::ostringstream oss2;
	oss2 << responseBody.size();
    std::string sizeStr = oss2.str();

	this->_responseContent = "HTTP/1.1 200 OK\r\n";
	this->_responseContent += "Content-Type: text/html\r\n";
	this->_responseContent += "Content-Length: " + sizeStr + "\r\n";
	this->_responseContent += "\r\n";
	this->_responseContent += responseBody;
}

bool HttpResponse::_checkAcceptedFormat(std::string path) {
	std::vector<std::string> accepted_formats = _request.getAccept();
	
	if (accepted_formats.size() == 0)
		return true;
	
	std::vector<std::string>::iterator beg = accepted_formats.begin();
	std::vector<std::string>::iterator end = accepted_formats.end(); 
	if (std::find(beg, end, "*/*") != end)
		return true;
	size_t pos = path.find_last_of('.');
	if (pos == std::string::npos)
		return false; // TBC HERE => i refuse any file without an extension because there is no way to know if it s binary or text
	std::string extension = path.substr(pos);
	if (_mimeMap[extension].empty()) {
		std::string mime_type = _mimeMap[extension];
		if (std::find(beg, end, mime_type) != end)
			return true;
		//size_t pos = std::find(mime_type.begin(), mime_type.end(), "/");
		if (size_t pos = mime_type.find("/") != std::string::npos) {
			mime_type = mime_type.substr(pos) + "/*";
			if (std::find(beg, end, mime_type) != end)
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
	// can we find the exact path?
		// Yes then 

}

void HttpResponse::_generateDELResponse(void)
{
	std::string uri_no_query = _request.getRequestURI();
	
	size_t pos = uri_no_query.find('?');
	if(pos != std::string::npos)
		uri_no_query = uri_no_query.substr(pos);
	std::string path = _location_block->getRoot() + uri_no_query;
	// Review if the file to delete exists, if not error 404
	struct stat stats;
	if (stat(path.c_str(), &stats) != 0 || !S_ISREG(stats.st_mode))
		throw ClientError(404);

	// Review if the file can be deleted, if not error 403
	if ((stats.st_mode & S_IFDIR) != 0 || remove(path.c_str()) != 0)
		throw ClientError(403);
	this->_responseContent = "HTTP/1.1 204 No Content\r\n";
}

//Assuming POST is only to upload files
void HttpResponse::_generatePOSTResponse(void)
{
	std::ostringstream oss;
	/*oss << TODO: Get the file size;*/
    std::string sizeStr = oss.str();
	
	std::string responseBody = "{\n";
	responseBody += "\"message\": \"File uploaded successfully.\",\n";
	responseBody += "\"filename\": \"";
	/*responseBody += Path file;*/ 
	responseBody += "\",\n";
	responseBody += "\"uploadedAt\": \"";
	responseBody += displayTimestampResponseFormat();
	responseBody += "\",\n";
	responseBody += "\"size\": " + sizeStr + "\n";
	responseBody += "}\n";

	std::ostringstream oss2;
	oss2 << responseBody.size();
    std::string sizeStr2 = oss2.str();

	this->_responseContent = "HTTP/1.1 201 Created\r\n";
	this->_responseContent += "Content-Type: application/json\r\n";
	this->_responseContent += "Content-Length: " + sizeStr2 + "\r\n";
	this->_responseContent += "\r\n";
	this->_responseContent += responseBody;
}

void HttpResponse::_generateErrorResponse(int errorCode, const char *errorMessage) {
	std::stringstream ss;
	
	ss << "HTTP/1.1 " << errorCode << " " << errorMessage << "\r\n";
	ss << _displayTimeStamp() << "\r\n";
	ss << "Content-Type: text/html\r\n";
	if (!_server_block)
		return _generateGenericErrorResponse(errorCode, errorMessage);
	std::string error_page = _server_block->getErrorPages()[errorCode];
	if (error_page.empty())
		return _generateGenericErrorResponse(errorCode, errorMessage);
	std::string path = _location_block->getRoot() + error_page;
	struct stat stats;
	if (stat(path.c_str(), &stats) == -1)
		return _generateGenericErrorResponse(errorCode, errorMessage);
	size_t size = stats.st_size;
	ss << "Content-Size: " << size << "\r\n";
	ss << "\r\n";
	
	std::ifstream file(path.c_str());
	if (!file.is_open())
		return _generateGenericErrorResponse(errorCode, errorMessage);
	std::string line;
	while (getline(file, line))
		ss << line << "\r\n";
	file.close();
	_responseContent = ss.str();		
}

void HttpResponse::_generateGenericErrorResponse(int errorCode, const char *errorMessage) {
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

std::string	HttpResponse::_displayTimeStamp(void) {
	std::stringstream ss;
	struct tm *gmt = gmtime(&_lastActionTimeStamp);
	char buffer[100];
	
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);

	ss << "Date: " << buffer;
    return ss.str();
}

std::string HttpResponse::getResponseContent(void)
{
	if (this->_responseContent.empty())
		this->_generateResponseContent();
	return (this->_responseContent);
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
	if (_errorCode < 500 || _errorCode > 505)
		return "Unknown server error of type 1\n";
	else {
		std::map<int, std::string>::const_iterator it = _errorMap.find(_errorCode);
		if (it != _errorMap.end()) {
			std::string errorMessage = it->second;
			const char *error = errorMessage.c_str();
			return error;
		}
		else
			return "Unknown server error of type 2\n";
	}
}

const char *HttpResponse::ClientError::what() const throw() {
	if (_errorCode < 400 || (_errorCode > 415 && _errorCode != 429))
		return "Unknown client error of type 1\n";
	else {
		std::map<int, std::string>::const_iterator it = _errorMap.find(_errorCode);
		if (it != _errorMap.end()) {
			std::string errorMessage = it->second;
			const char *error = errorMessage.c_str();
			return error;
		}
		else
			return "Unkown client error of type 2\n";
	}
}

HttpResponse & HttpResponse::operator=(HttpResponse const & rhs) {
	if (this != &rhs) {
		_isResponseSent = rhs._isResponseSent;
		_responseContent = rhs._responseContent;
		_lastActionTimeStamp = rhs._lastActionTimeStamp;
		_mimeMap = rhs._mimeMap;
	}
	return *this;
}