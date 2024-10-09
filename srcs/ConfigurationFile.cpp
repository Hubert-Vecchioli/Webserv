#include "webserv.hpp"

ConfigurationFile::ConfigurationFile(void) : _filename("") {
	this->_user = "www www";
	this->_error_log = "/var/log/webserv/error.log";
	this->_body_size = 1000000;
}

ConfigurationFile::ConfigurationFile(int numberArgs, char** args) {
	if (numberArgs != 2)
		throw std::runtime_error("Error: invalid number of arguments");
	this->_filename = args[1];
	this->_user = "www www";
	this->_error_log = "/var/log/webserv/error.log";
	this->_body_size = 1000000;
	read();
	parseBase();
	parseServerBlock();
}

ConfigurationFile::ConfigurationFile(ConfigurationFile const & copy) {
	*this = copy;
}

ConfigurationFile &ConfigurationFile::operator=(ConfigurationFile const & copy) {
	if (this != &copy) {
		this->_filename = copy._filename;
		this->_content = copy._content;
		this->_user = copy._user;
		this->_error_log = copy._error_log;
		this->_body_size = copy._body_size;
		this->_serverBlocks = copy._serverBlocks;
	}
	return *this;
}

ConfigurationFile::~ConfigurationFile(void) {}

void ConfigurationFile::read(void) {
	if (this->_filename.empty())
		throw std::runtime_error("Error: no filename provided");
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file");
	std::string line;
	while (std::getline(file, line))
		 this->_content.append(line);
	file.close();
}

void ConfigurationFile::read(std::string filename) {
	this->_filename = filename;
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file");
	std::string line;
	while (std::getline(file, line))
		 this->_content.append(line);
	file.close();
}

void ConfigurationFile::parseBase(void) {
	parseUser();
	parseErrorLog();
	parseBodySize();
}

void ConfigurationFile::parseServerBlock(void) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	std::string::size_type checkloc = 0;
	std::string::size_type end = this->_content.size();
	while (pos < end) {
		pos = this->_content.find("server {", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = this->_content.find("}", pos);
		if (pos == std::string::npos)
			break;
		checkloc = prev;
		if (this->_content.substr(checkloc, pos - checkloc).find("location") != std::string::npos) {
			checkloc = pos;
			pos = this->_content.find("}", checkloc + 1);
		}
		std::string serverBlock = this->_content.substr(prev, pos - prev);
		ServerBlock block(serverBlock);
		this->_serverBlocks.push_back(block);
	}
}

void ConfigurationFile::parseUser(void) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = this->_content.find("user", pos);
	if (pos == std::string::npos)
		return ;
	prev = pos;
	pos = this->_content.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid user directive");
	this->_user = this->_content.substr(prev + 5, pos - prev - 5);
}

void ConfigurationFile::parseErrorLog(void) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = this->_content.find("error_log", pos);
	if (pos == std::string::npos)
		return ;
	prev = pos;
	pos = this->_content.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid error_log directive");
	this->_error_log = this->_content.substr(prev + 10, pos - prev - 10);
}

void ConfigurationFile::parseBodySize(void) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = this->_content.find("body_size", pos);
	if (pos == std::string::npos) {
		this->_body_size = 1000000;
		return ;
	}
	prev = pos;
	pos = this->_content.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid body_size directive");
	std::vector<std::string> size = tokenize(this->_content.substr(prev + 9, pos - prev - 9));
	if (size.size() != 2)
		throw std::runtime_error("Error: invalid body_size directive");
	unsigned long multiplier = 1;
	if (size[1] == "KB")
		multiplier = 1024;
	else if (size[1] == "MB")
		multiplier = 1024 * 1024;
	else if (size[1] == "GB")
		multiplier = 1024 * 1024 * 1024;
	this->_body_size = std::atoi(size[0].c_str()) * multiplier;
}

std::string ConfigurationFile::getUser(void) const {
	return this->_user;
}

std::string ConfigurationFile::getError_log(void) const {
	return this->_error_log;
}

unsigned long ConfigurationFile::getBody_size(void) const {
	return this->_body_size;
}

std::vector<ServerBlock> ConfigurationFile::getServerBlocks(void) const {
	return this->_serverBlocks;
}

std::vector<std::pair<std::string, int> > ConfigurationFile::getserverIPandPorts(void) const {
	std::vector<std::pair<std::string, int> > ip_ports;
	for (std::vector<ServerBlock>::const_iterator it = this->_serverBlocks.begin(); it != this->_serverBlocks.end(); ++it)
		ip_ports.push_back(it->getIPandPort());
	return ip_ports;
}
