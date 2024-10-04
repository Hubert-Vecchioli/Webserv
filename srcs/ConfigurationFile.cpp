#include "webserv.hpp"

ConfigurationFile::ConfigurationFile(void) _filename("") {}

ConfigurationFile::ConfigurationFile(int numberArgs, char** args) {
	if (numberArgs != 2)
		throw std::runtime_error("Error: invalid number of arguments");
	this->_filename = args[1];
}

ConfigurationFile::ConfigurationFile(ConfigurationFile const & copy) {
	*this = copy;
}

ConfigurationFile &ConfigurationFile::operator=(ConfigurationFile const & copy) {
	if (this != &copy) {
		this->_filename = copy._filename;
		this->_content = copy._content;
		this->_user = copy._user;
		this->_workers = copy._workers;
		this->_error_log = copy._error_log;
		this->_ip_port = copy._ip_port;
	}
	return *this;
}

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
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file");
	std::string line;
	while (std::getline(file, line))
		 this->_content.append(line);
	file.close();
}

void ConfigurationFile::parseServerBlock(void) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	std::string::size_type end = this->_content.size();
	while (pos < end) {
		pos = this->_content.find("server {", pos);
		if (pos == std::string::npos)
			break;
		prev = pos;
		pos = this->_content.find("}", pos);
		if (pos == std::string::npos)
			break;
		std::string serverBlock = this->_content.substr(prev, pos - prev);
		ServerBlock block(serverBlock);
		this->_serverBlocks.push_back(block);
	}
}
