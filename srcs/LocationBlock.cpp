# include "webserv.hpp"

LocationBlock::LocationBlock(std::string block) {
	parseLocation(block);
	parseRoot(block);
	parseIndex(block);
	parseDirlisting(block);
	parseMethods(block);
	parseRedirect(block);
}

LocationBlock::LocationBlock(LocationBlock const & copy) {
	*this = copy;
}

LocationBlock &LocationBlock::operator=(LocationBlock const & copy) {
	if (this != &copy) {
		this->_location = copy._location;
		this->_root = copy._root;
		this->_index = copy._index;
		this->_dirlisting = copy._dirlisting;
		this->_methods = copy._methods;
		this->_redirect = copy._redirect;
	}
	return *this;
}

LocationBlock::~LocationBlock(void) {}

void LocationBlock::parseLocation(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = block.find("location", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid location directive");
	prev = pos;
	pos = block.find("{", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid location directive");
	this->_location = block.substr(prev + 9, pos - prev - 9);
	this->_location.erase(this->_location.find_last_not_of(" \n\r\t") + 1);
}

void LocationBlock::parseRoot(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = block.find("root", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid root directive");
	prev = pos;
	pos = block.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid root directive");
	this->_root = block.substr(prev + 5, pos - prev - 5);
	this->_root.erase(this->_root.find_last_not_of(" \n\r\t") + 1);
}

void LocationBlock::parseIndex(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = block.find("index", pos);
	if (pos == std::string::npos)
		return ;
	prev = pos;
	pos = block.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid index directive");
	this->_index = block.substr(prev + 6, pos - prev - 6);
	this->_index.erase(this->_index.find_last_not_of(" \n\r\t") + 1);
}

void LocationBlock::parseDirlisting(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = block.find("dirlisting", pos);
	if (pos == std::string::npos) {
		this->_dirlisting = false;
		return ;
	}
	prev = pos;
	pos = block.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid dirlisting directive");
	std::string dirlisting = block.substr(prev + 11, pos - prev - 11);
	dirlisting.erase(dirlisting.find_last_not_of(" \n\r\t") + 1);
	if (dirlisting == "on")
		this->_dirlisting = true;
	else if (dirlisting == "off")
		this->_dirlisting = false;
	else
		throw std::runtime_error("Error: invalid dirlisting directive");
}

void LocationBlock::parseMethods(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = block.find("methods", pos);
	if (pos == std::string::npos) {
		this->_methods = tokenize("GET POST DELETE");
		return ;
	}
	prev = pos;
	pos = block.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid methods directive");
	std::string methods = block.substr(prev + 7, pos - prev - 7);
	methods.erase(methods.find_last_not_of(" \n\r\t") + 1);
	this->_methods = tokenize(methods);
}

void LocationBlock::parseRedirect(std::string block) {
	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	pos = block.find("redirect", pos);
	if (pos == std::string::npos)
		return ;
	prev = pos;
	pos = block.find(";", pos);
	if (pos == std::string::npos)
		throw std::runtime_error("Error: invalid redirect directive");
	this->_redirect = block.substr(prev + 9, pos - prev - 9);
	this->_redirect.erase(this->_redirect.find_last_not_of(" \n\r\t") + 1);
}
