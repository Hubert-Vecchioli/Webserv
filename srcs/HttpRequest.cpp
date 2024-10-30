
#include "webserv.hpp"

// Constructors and Destructors for the HttpRequest class

HttpRequest::HttpRequest(Client *client, unsigned char *request, int requestSize) : _client(client) {
    std::string str_request((char *) request, requestSize);
    if (str_request == "413")
    {
        _requestURI = "413";
        return;
    }
	parseRequestLine(str_request);
    parseRequestHeader(str_request);
    getCGIExtension();
	if (this->_method == POST)
    {
        parseRequestBody(str_request);   
    }
}

HttpRequest::HttpRequest(HttpRequest const & rhs) {
    *this = rhs;
}

HttpRequest & HttpRequest::operator=(HttpRequest const & rhs) {
    if (this != &rhs) {
        _method = rhs._method;
        _http1 = rhs._http1;
        _queryString = rhs._queryString;
        _requestURI = rhs._requestURI;
        _host = rhs._host;
        _connection = rhs._connection;
        _content_type = rhs._content_type;
        _content_len = rhs._content_len;
        _content_body = rhs._content_body;
        _accept = rhs._accept;
		_cookie = rhs._cookie;
        _cookieString = rhs._cookieString;
        // delete _client;
        // delete _HttpResponse;
        // _client = Client(rhs._client);
        // _response = HttpResponse(rhs._response);
    }
    return *this;
}

HttpRequest::~HttpRequest(void) {};

// GETTERS

void HttpRequest::parseRequestLine(std::string request) {
    std::string str_request = request;
    
    size_t pos1 = str_request.find(' ');
    if (pos1 != std::string::npos) {
        std::string method = str_request.substr(0, pos1);
        if (method == "GET")
            _method = GET;
        else if (method == "POST")
            _method = POST;
        else if (method == "DELETE")
            _method = DELETE;
        else
            _method = ERROR;

        size_t pos2 = str_request.find(' ', pos1 + 1);
        if (pos2 != std::string::npos) {
            _requestURI = str_request.substr(pos1 + 1, pos2 - pos1 - 1);
            if (str_request.substr(pos2 + 1, 8) != "HTTP/1.1")
                _http1 = false;
            else
                _http1 = true;
            
            size_t pos_query = _requestURI.find('?');
            if (pos_query != std::string::npos) {
                _queryString = _requestURI.substr(pos_query + 1);
            }
            else
                _queryString = "";
        }
    }
}

void HttpRequest::parseRequestHeader(std::string request) {
        _host = getValue(request, "Host: ");
        std::string accept = getValue(request, "Accept: ");
        for (size_t i = 0; i < accept.size(); i++) {
            if (accept[i] == ';') accept[i] = ',';
        }
        _accept = tokenize(accept, ',');
        _content_type = getValue(request, "Content-Type: ");
        std::string len = getValue(request, "Content-Length: ");
        _content_len = atol(len.c_str());
		parseConnection(request);
		parseCookie(request);
}

void HttpRequest::parseRequestBody(std::string request) {
    if (_content_len != 0) {
        _content_body = request.substr(request.size() - _content_len);
    }
    else {
        _content_body = "";
    }

}

void HttpRequest::parseConnection(std::string request) {
	if (getValue(request, "Connection: ") == "keep-alive")
		this->_connection = KEEP_ALIVE;
	else
		this->_connection = CLOSE;
}

void HttpRequest::parseCookie(std::string request) {
    _cookieString = getValue(request, "Cookie: ");
}

// Helper functions

void HttpRequest::getCGIExtension() {
    _CGItype = "";
    if (!_requestURI.empty()) {
        size_t pos = _requestURI.find_last_of('.');
        size_t pos2 = _requestURI.find_last_of('?');
        if (pos != std::string::npos && pos != _requestURI.size() && (pos2 - pos) > 0) {
            std::string extension = _requestURI.substr(pos, pos2 - (pos));
            if (extension == ".py")
                _CGItype = "python";
            if (extension == ".sh")
                _CGItype = "bash";
        }
    }
}

std::string HttpRequest::getStringMethod() {
    std::string method;
    switch (_method)
    {
    case GET:
        method = "GET";
        break;
    case POST:
        method = "POST";
        break;
    case DELETE:
        method = "DELETE";
        break;
    default:
        method = "ERROR";
        break;
    }
    return method;
}

std::string HttpRequest::getValue(std::string request, std::string key_with_sep) {
    std::string value;
    
    size_t pos_key = request.find(key_with_sep);
    if (pos_key != std::string::npos) {
        size_t endline = request.find('\n', pos_key);
            if (endline != std::string::npos)
                value = request.substr(pos_key + key_with_sep.size(), endline - pos_key - key_with_sep.size());
            else 
                value = request.substr(pos_key + key_with_sep.size());
    }
    else
        value = "";
    return value;
}

HttpRequest* HttpRequest::findInstanceWithFD(std::vector<HttpRequest*>& vector, int fd) {
    for (std::vector<HttpRequest*>::iterator it = vector.begin(); it != vector.end(); ++it) {
        if ((*it)->_client->getFD() == fd) {
            return (*it);
        }
    }
    return (0);
}

// DEBUG

void HttpRequest::displayRequestLine(std::ostream &o) {
    switch (_method)
    {
        case GET:
            o << "GET ";
            break;
        case POST:
            o << "POST ";
            break;
        
        case DELETE:
            o << "DELETE ";
            break;
        
        default:
            o << "ERROR ";
            break;
    }
    o << _requestURI << " ";
    if (_http1 == true)
        o << "HTTP/1.1" << std::endl;
    else
        o << "unknown HTTP" << std::endl;
    // if (_queryString != "")
    //     o << _queryString << std::endl; // DEBUG
}

void HttpRequest::displayRequestHeader(std::ostream & o) {
    o << "Host: " << _host << std::endl;
    o << "Accept: ";
    for (size_t i = 0; i < _accept.size(); i++) {
        o << _accept[i];
        if (i < _accept.size() - 1)
            o << " ";
    }
    o << std::endl;
    o << "Content-Type: " << _content_type << std::endl;
    o << "Content-Length: " << _content_len << std::endl;
    if (_connection == KEEP_ALIVE)
        o << "Connection: keep-alive" << std::endl;
    else
        o << "Connection: close" << std::endl;
}
