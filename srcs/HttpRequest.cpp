/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/10 15:39:39 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


// HttpRequest* HttpRequest::findInstanceWithFD(std::vector<HttpRequest>& vector, int fd) {
//     for (std::vector<HttpRequest>::iterator it = vector.begin(); it != vector.end(); ++it) {
//         if (it->_client->_fd == fd) {
//             return &(*it);
//         }
//     }
//     return (nullptr);
// }

HttpRequest::HttpRequest(char *request) {
    parseRequestLine(request);
    parseRequestHeader(request);
    parseRequestBody(request);
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
        // delete _client;
        // delete _HttpResponse;
        // _client = Client(rhs._client);
        // _response = HttpResponse(rhs._response);
    }
    return *this;
}
HttpRequest::~HttpRequest(void) {};

void HttpRequest::parseRequestLine(char *request) {
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

void HttpRequest::parseRequestHeader(char *request) {
        std::string str_request = request;

        _host = getValue(str_request, "Host: ");
        _accept = getValue(str_request, "Accept: ");
        if (getValue(str_request, "Connection: ") == "keep-alive")
            _connection = KEEP_ALIVE;
        else
            _connection = CLOSE;
        _content_type = getValue(str_request, "Content-Type: ");
        std::string len = getValue(str_request, "Content-Length: ");
        _content_len = atol(len.c_str());
}

void HttpRequest::displayRequestHeader(std::ostream & o) {
    o << "Host: " << _host << std::endl;
    o << "Accept: " << _accept << std::endl;
    o << "Content-Type: " << _content_type << std::endl;
    o << "Content-Length: " << _content_len << std::endl;
    if (_connection == KEEP_ALIVE)
        o << "Connection: keep-alive" << std::endl;
    else
        o << "Connection: close" << std::endl;
}

void HttpRequest::parseRequestBody(char *request) {
    std::string str_request = request;

    size_t pos = str_request.find("\r\n");
    if (pos != std::string::npos) {
        _content_body = str_request.substr(pos + 2);
    }
}

void HttpRequest::displayRequestBody(std::ostream & o) {
    o << _content_body << std::endl;
}