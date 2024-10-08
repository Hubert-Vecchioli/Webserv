/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvecchio <hvecchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:54:18 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/08 07:55:52 by hvecchio         ###   ########.fr       */
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