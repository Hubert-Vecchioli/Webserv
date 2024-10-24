#include "webserv.hpp"

// Constructors and Destructos for the ConfigurationFile class

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
	parseFile();
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

// Main Parsing Functions for the ConfigurationFile class

typedef void (ConfigurationFile::*parseFunction)(std::vector<std::string> &);

void ConfigurationFile::read(void) {
	if (this->_filename.empty())
		throw std::runtime_error("Error: no filename provided");
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file");
	std::string line;
	while (std::getline(file, line))
		 this->_content.append(line + "\n" + " ");
	file.close();
}

void ConfigurationFile::read(std::string filename) {
	this->_filename = filename;
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file");
	std::string line;
	while (std::getline(file, line))
		 this->_content.append(line + "\n" + " ");
	file.close();
}

void ConfigurationFile::parseFile(void) {
	std::map<std::string, parseFunction> parseFunctions;
	parseFunctions["user"] = &ConfigurationFile::parseUser;
	parseFunctions["error_log"] = &ConfigurationFile::parseErrorLog;
	parseFunctions["body_size"] = &ConfigurationFile::parseBodySize;

	std::istringstream iss(this->_content);
	std::string line;

	while (std::getline(iss, line)) {
		if (line.empty())
			continue;
		std::vector<std::string> tokens = tokenize(line, ' ');
		if (tokens.empty())
			continue;
		if (tokens[0][0] == '#')
			continue;
		if (tokens[0] == "server" && tokens.size() > 1 && tokens[1] == "{")
		{
			parseServerBlock(iss);
		}
		else if (parseFunctions.find(tokens[0]) != parseFunctions.end()) {
			std::vector<std::string> args(tokens.begin() + 1, tokens.end());
			(this->*parseFunctions[tokens[0]])(args);
		}
		else {
			std::string error =  "Error: invalid directive : " + tokens[0];
			throw std::runtime_error(error);
		}	
	}
	if (this->_serverBlocks.empty())
		throw std::runtime_error("Error: no server block found");
}

void ConfigurationFile::parseUser(std::vector<std::string> &args) {
	if (args.size() != 1)
		throw std::runtime_error("Error: invalid user directive");
	this->_user = args[0].substr(0, args[0].find(";"));
}

void ConfigurationFile::parseErrorLog(std::vector<std::string> &args) {
	if (args.size() != 1)
		throw std::runtime_error("Error: invalid error_log directive");
	this->_error_log = args[0].substr(0, args[0].find(";"));
}

void ConfigurationFile::parseBodySize(std::vector<std::string> &args) {
	if (args.size() != 2)
		throw std::runtime_error("Error: invalid body_size directive");
	unsigned long multiplier = 1;
	std::string size = args[1].substr(0, args[1].find(";"));
	if (size == "KB")
		multiplier = 1024;
	else if (size == "MB")
		multiplier = 1024 * 1024;
	else if (size == "GB")
		multiplier = 1024 * 1024 * 1024;
	else
		throw std::runtime_error("Error: invalid body_size directive");
	this->_body_size = std::atoi(args[0].c_str()) * multiplier;
}

void ConfigurationFile::parseServerBlock(std::istringstream &iss) {
	std::string line;
	std::vector<std::string> blockLines;
	int openBrackets = 1;

	while (std::getline(iss, line)) {
		for (size_t i = 0; i < line.size(); i++) {
			//for (size_t j = 0; j < line.size(); j++) {
			if (line[i]/*[j]*/ == '{')
				openBrackets++;
			else if (line[i]/*[j]*/ == '}')
				openBrackets--;
			//}
		}
		if (openBrackets == 0) {
			ServerBlock block(blockLines);
			this->_serverBlocks.push_back(block);
			return ;
		}
		blockLines.push_back(line);
	}
}

// Getter Functions for the ConfigurationFile class

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

std::vector<ServerBlock*> ConfigurationFile::getServerBlocksPointers(void) {
	std::vector<ServerBlock*> pointers;
    for (std::vector<ServerBlock>::iterator it = _serverBlocks.begin(); it != _serverBlocks.end(); ++it) {
		pointers.push_back(&(*it));
	}
	return pointers;
}


// IP and Port Getter Function for the ConfigurationFile class

std::vector<std::pair<std::string, int> > ConfigurationFile::getserverIPandPorts(void) const {
	std::vector<std::pair<std::string, int> > ip_ports;
	for (std::vector<ServerBlock>::const_iterator it = this->_serverBlocks.begin(); it != this->_serverBlocks.end(); ++it)
		ip_ports.push_back(it->getIPandPort());
	return ip_ports;
}
