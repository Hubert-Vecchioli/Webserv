/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:08:49 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/08 13:22:59 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <arpa/inet.h>

ServerBlock::ServerBlock(std::string block) {
	parseListen(block);
	parseServerName(block);
	parseErrorPages(block);
	parseLocationBlock(block);
}

ServerBlock::ServerBlock(ServerBlock const & copy) {
	*this = copy;
}

ServerBlock &ServerBlock::operator=(ServerBlock const & copy) {
	if (this != &copy) {
		this->_listen = copy._listen;
		this->_server_name = copy._server_name;
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
		std::string servers = block.substr(prev + 12, pos - prev - 12);
		this->_server_name =  tokenize(servers);
	}
}

void ServerBlock::parseErrorPages(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	std::string::size_type end = block.size();
	while (pos < end) {
		pos = block.find("error_page", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = block.find(";", pos);
		if (pos == std::string::npos)
			break;
		std::string errorPage = block.substr(prev + 10, pos - prev - 10);
		std::vector<std::string> tokens = tokenize(errorPage);
		for (int i= 0; i < tokens.size() - 1; i++) {
			this->_error_pages[std::atoi(tokens[i].c_str())] = tokens[tokens.size() - 1];
		}
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

std::vector<std::string> ServerBlock::getServerName(void) const {
	return this->_server_name;
}

std::map<int, std::string> ServerBlock::getErrorPages(void) const {
	return this->_error_pages;
}
