/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:08:49 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/04 18:35:49 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <arpa/inet.h>

ServerBlock::ServerBlock(std::string block) {
	parseListen(block);
	parseServerName(block);
	while (pos < end) {
		pos = block.find("root", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = block.find(";", pos);
		if (pos == std::string::npos)
			break;
		this->_root = block.substr(prev + 5, pos - prev - 5);
	}
}

ServerBlock::ServerBlock(ServerBlock const & copy) {
	*this = copy;
}

ServerBlock &ServerBlock::operator=(ServerBlock const & copy) {
	if (this != &copy) {
		this->_listen = copy._listen;
		this->_server_name = copy._server_name;
		this->_root = copy._root;
	}
	return *this;
}

ServerBlock::~ServerBlock(void) {}


void ServerBlock::parseListen (std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	std::string::size_type end = block.size();
	while (pos < end) {
		pos = block.find("listen", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = block.find(";", pos);
		if (pos == std::string::npos)
			break;
		parseIPandPort(block.substr(prev, pos - prev));
	}
}

void ServerBlock::parseServerName(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	std::string::size_type end = block.size();
	while (pos < end) {
		pos = block.find("server_name", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = block.find(";", pos);
		if (pos == std::string::npos)
			break;
		this->_server_name = block.substr(prev + 12, pos - prev - 12);
	}
}

//need to add the parsing to Location blocks

void ServerBlock::parseIPandPort(std::string block) {
	std::regex ip_regex(R"(listen (\d+\.\d+\.\d+\.\d+) (\d+);)");
	std::smatch match;
	if (std::regex_search(block, match, ip_regex)) {
		this->_listen = std::make_pair(inet_addr(match[1].str().c_str()), //using inet_addr to convert string to int <arpa/inet.h> needed
						std::atoi(match[2].str().c_str()));
		return ;
	}
	throw std::runtime_error("Error: invalid listen directive");
	
}

std::pair<int, int> ServerBlock::getIPandPort(void) const {
	return this->_listen;
}
