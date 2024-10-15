/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebesnoin <ebesnoin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:08:49 by hvecchio          #+#    #+#             */
/*   Updated: 2024/10/15 12:12:28 by ebesnoin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// Constructors and Destructos for the ServerBlock class

ServerBlock::ServerBlock(std::vector<std::string> block) :
_listen(), _server_name(), _error_pages(), _locationBlocks() {
	parseServer(block);
}

ServerBlock::ServerBlock(ServerBlock const & copy) {
	*this = copy;
}

ServerBlock &ServerBlock::operator=(ServerBlock const & copy) {
	if (this != &copy) {
		this->_listen = copy._listen;
		this->_server_name = copy._server_name;
		this->_error_pages = copy._error_pages;
		this->_locationBlocks = copy._locationBlocks;
	}
	return *this;
}

ServerBlock::~ServerBlock(void) {}

// Main Parsing Functions for the ServerBlock class

typedef void (*parseFunction)(const std::vector<std::string> &);

void ServerBlock::parseServer(std::vector<std::string> block) {
	std::map<std::string, parseFunction> parseFunctions;
	parseFunctions["listen"] = &parseListen;
	parseFunctions["server_name"] = &parseServerName;
	parseFunctions["error_page"] = &parseErrorPages;

	for (size_t i = 0; i < block.size(); i++) {
		std::vector<std::string> tokens = tokenize(block[i]);
		if (tokens.empty())
			continue;
		if (tokens[0].size >= 2 && tokens[0].substr(0, 2) == "//")
			continue;
		if (tokens[0] == "location")
			i = parseLocationBlock(block, i);
		else if (parseFunctions.find(tokens[0]) != parseFunctions.end()) {
			std::vector<std::string> args(tokens.begin() + 1, tokens.end());
			parseFunctions[tokens[0]](args);
		}
		else {
			std::string error =  "Error: invalid directive : " + tokens[0];
			throw std::runtime_error(error);
		}
	}
}

void ServerBlock::parseListen (std::vector<std::string> args) {
	std::string line;
	for (int i = 0; i < args.size(); i++)
		line += args[i] + " ";
	if (line.find(";") != line.size() - 2)
		throw std::runtime_error("Error: invalid listen directive");
	if (args.size() == 2)
		this->_listen = std::make_pair(args[0], std::atoi(args[1].c_str()));
	else
		throw std::runtime_error("Error: invalid listen directive");
}

void ServerBlock::parseServerName(std::vector<std::string> args) {
	std::string line;
	for (int i = 0; i < args.size(); i++)
		line += args[i] + " ";
	if (line.find(";") != line.size() - 2) {
		throw std::runtime_error("Error: invalid server name directive");
	args[args.size() -1] = args[args.size() - 1].substr(0, args[args.size() - 1].find(";"));
		this->_server_name =  args;
	}
}

void ServerBlock::parseErrorPages(std::vector<std::string> args) {
	std::string line;
	for (int i = 0; i < args.size(); i++)
		line += args[i] + " ";
	if (line.find(";") != line.size() - 2)
		throw std::runtime_error("Error: invalid error pages directive");
	for (unsigned long i = 0; i < args.size() - 1; i++)
		this->_error_pages[std::atoi(args[i].c_str())] = args[args.size() - 1];
}

void ServerBlock::parseLocationBlock(std::vector<std::string> block, int i) {
	std::vector<std::string> lBlock;
	size_t openBrackets = 0;
	size_t start = i;
	while (openBrackets != 0 || start == i) {
		i++;
		if (i >= block.size())
			throw std::runtime_error("Error: invalid location block");
		lBlock.push_back(block[i]);
		for (size_t j = 0; j < block[i].size(); j++) {
			if (block[i][j] == '{')
				openBrackets++;
			else if (block[i][j] == '}')
				openBrackets--;
		}
	}
	LocationBlock location(lBlock);
	this->_locationBlocks.push_back(location);
	return i;
}

// Getters for the ServerBlock class

std::pair<std::string, int> ServerBlock::getIPandPort(void) const {
	return this->_listen;
}

std::vector<std::string> ServerBlock::getServerName(void) const {
	return this->_server_name;
}

std::map<int, std::string> ServerBlock::getErrorPages(void) const {
	return this->_error_pages;
}

std::vector<LocationBlock> ServerBlock::getLocationBlocks(void) const {
	return this->_locationBlocks;
}

std::map<std::string, std::vector<std::string> > ServerBlock::getLocMethods(void) const {
	std::map<std::string, std::vector<std::string> > locMethods;
	for (size_t i = 0; i < this->_locationBlocks.size(); i++)
		locMethods[this->_locationBlocks[i].getLocation()] = this->_locationBlocks[i].getMethods();
	return locMethods;
}
