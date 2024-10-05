/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:08:49 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/05 15:29:59 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <arpa/inet.h>

ServerBlock::ServerBlock(std::string block) {
	parseListen(block);
	parseServerName(block);
	//parseLocationBlock(block);
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

void ServerBlock::parseLocationBlock(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	std::string::size_type end = block.size();
	while (pos < end) {
		pos = block.find("location ", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = block.find("}", pos);
		if (pos == std::string::npos)
			break;
		std::string locationBlock = block.substr(prev, pos - prev);
		LocationBlock block(locationBlock);
		this->_locationBlocks.push_back(block);
	}
}

void ServerBlock::parseIPandPort(std::string block) {
	std::regex ip_regex(R"(listen (\d+\.\d+\.\d+\.\d+) (\d+);)");
	std::smatch match;
	if (std::regex_search(block, match, ip_regex)) {
		this->_listen = std::make_pair(match[1].str(), std::atoi(match[2].str().c_str()));
		return ;
	}
	throw std::runtime_error("Error: invalid listen directive");
	
}

std::pair<int, int> ServerBlock::getIPandPort(void) const {
	return this->_listen;
}
