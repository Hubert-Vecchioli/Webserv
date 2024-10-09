/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblaye <jblaye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/09 18:47:03 by jblaye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


HttpRequest* HttpRequest::findInstanceWithFD(std::vector<HttpRequest>& vector, int fd) {
    for (std::vector<HttpRequest>::iterator it = vector.begin(); it != vector.end(); ++it) {
        if (it->_client->_fd == fd) {
            return &(*it);
        }
    }
    return (nullptr);
}

HttpRequest::HttpRequest(char *request) {
    parseRequestLine(request);
    parseHeader(request);
    parseBody(request);
}

HttpRequest::parseRequestLine(request) {
    std::string str_request = request;
    
    size_t pos1 = str_request.find(' ');
    if (pos1 != std::string::npos) {
        std::string method = std::string::substr(0, pos1);
        if (method == "GET")
            _method = GET;
        else if (method == "POST")
            _method == POST;
        else if (method == "DELETE")
            _method == DELETE;
        else
            _method == ERROR;

    size_t pos2 = str_request.find(' ', pos + 1);
    if (pos2 != std::string::npos) {
        _requestURI = std::string::substr()
    }
    }

}