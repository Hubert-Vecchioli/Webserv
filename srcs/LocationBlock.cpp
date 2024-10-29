# include "webserv.hpp"

// Constructors and Destructos for the LocationBlock class

LocationBlock::LocationBlock(std::vector<std::string> block) : 
_location(""), _root(""), _index(), _dirlisting(false),
_methods(tokenize("GET", ' ')), _redirect(), cgi_extension(), _upload_path("")
{
	parseLocationBlock(block);
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
		this->cgi_extension = copy.cgi_extension;
		this->_upload_path = copy._upload_path;
	}
	return *this;
}

LocationBlock::~LocationBlock(void) {}

// Main Parsing Functions for the LocationBlock class

typedef void (LocationBlock::*parseFunction)(std::vector<std::string> &);

void LocationBlock::parseLocationBlock(std::vector<std::string> block) {
	std::map<std::string, parseFunction> parseFunctions;
	parseFunctions["root"] = &LocationBlock::parseRoot;
	parseFunctions["index"] = &LocationBlock::parseIndex;
	parseFunctions["dirlisting"] = &LocationBlock::parseDirlisting;
	parseFunctions["methods"] = &LocationBlock::parseMethods;
	parseFunctions["redirect"] = &LocationBlock::parseRedirect;
	parseFunctions["cgi_extension"] = &LocationBlock::parseCgiExtension;
	parseFunctions["upload_path"] = &LocationBlock::parseUploadPath;

	parseLocation(tokenize(block[0], ' '));
	for (size_t i = 1; i < block.size(); i++) {
		std::vector<std::string> tokens = tokenize(block[i], ' ');
		if (tokens.empty())
			continue;
		if (tokens[0][0] == '#')
			continue;
		if (parseFunctions.find(tokens[0]) != parseFunctions.end()) {
			std::vector<std::string> args(tokens.begin() + 1, tokens.end());
			(this->*parseFunctions[tokens[0]])(args);
		}
		else {
			std::string error =  "Error: invalid directive : " + tokens[0];
			throw std::runtime_error(error);
		}
	}
}

void LocationBlock::parseLocation(std::vector<std::string> args) {
	if (args.size() != 3)
		throw std::runtime_error("Error: invalid location directive");
	this->_location = args[1];
}

void LocationBlock::parseRoot(std::vector<std::string> &args) {
	if (args.size() != 1)
		throw std::runtime_error("Error: invalid root directive");
	if (args[0].find(";") != args[0].size() - 1)
		throw std::runtime_error("Error: invalid root directive");
	this->_root = args[0].substr(0, args[0].size() - 1);
}

void LocationBlock::parseIndex(std::vector<std::string> &args) {
	std::string line;
	for (size_t i = 0; i < args.size(); i++)
		line += args[i] + " ";
	if (line.find(";") != line.size() - 2)
		throw std::runtime_error("Error: invalid index directive");
	this->_index = tokenize(line.substr(0, line.size() - 2), ' ');
}

void LocationBlock::parseDirlisting(std::vector<std::string> &args) {
	if (args.size() != 1)
		throw std::runtime_error("Error: invalid dirlisting directive");
	if (args[0].find(";") != args[0].size() - 1)
		throw std::runtime_error("Error: invalid dirlisting directive");
	std::string dirlisting  = args[0].substr(0, args[0].size() - 1);
	if (dirlisting == "on")
		this->_dirlisting = true;
	else if (dirlisting == "off")
		this->_dirlisting = false;
	else
		throw std::runtime_error("Error: invalid dirlisting directive");
}

void LocationBlock::parseMethods(std::vector<std::string> &args) {
	std::string line;
	for (size_t i = 0; i < args.size(); i++)
		line += args[i] + " ";
	if (line.find(";") != line.size() - 2)
		throw std::runtime_error("Error: invalid methods directive");
	this->_methods = tokenize(line.substr(0, line.size() - 2), ' ');
	for (size_t i = 0; i < this->_methods.size(); i++) 
	{
		if (this->_methods[i] == "GET" || this->_methods[i] == "POST" || this->_methods[i] == "DELETE")
			continue;
		else
			throw std::runtime_error("Error: invalid methods directive");
	}
}

void LocationBlock::parseRedirect(std::vector<std::string> &args) {
	if (args.size() != 2)
		throw std::runtime_error("Error: invalid redirect directive");
	if (args[1].find(";") != args[1].size() - 1)
		throw std::runtime_error("Error: invalid dirlisting directive");
	this->_redirect = std::make_pair(std::atoi(args[0].c_str()), args[1].substr(0, args[1].size() - 1));
}

void LocationBlock::parseCgiExtension(std::vector<std::string> &args) {
	std::string line;
	for (size_t i = 0; i < args.size(); i++)
		line += args[i] + " ";
	if (line.find(";") != line.size() - 2 || args.size() != 2)
		throw std::runtime_error("Error: invalid cgi_extension directive");
	std::string key = args[0];
	std::string value = args[1].substr(0, args[1].size() - 1);
	this->cgi_extension[key] = value;
}

void LocationBlock::parseUploadPath(std::vector<std::string> &args) {
	if (args.size() != 1)
		throw std::runtime_error("Error: invalid upload_path directive");
	if (args[0].find(";") != args[0].size() - 1)
		throw std::runtime_error("Error: invalid upload_path directive");
	this->_upload_path = args[0].substr(0, args[0].size() - 1);
}
//  Getters for the LocationBlock class

std::string LocationBlock::getLocation(void) const {
	return this->_location;
}

std::string LocationBlock::getRoot(void) const {
	return this->_root;
}

std::vector<std::string> LocationBlock::getIndex(void) const {
	return this->_index;
}

bool LocationBlock::getDirlisting(void) const {
	return this->_dirlisting;
}

std::vector<std::string> LocationBlock::getMethods(void) const {
	return this->_methods;
}

std::pair<int, std::string> LocationBlock::getRedirect(void) const {
	return this->_redirect;
}

std::map<std::string, std::string> LocationBlock::getCgiExtension(void) const {
	return this->cgi_extension;
}

std::string LocationBlock::getUploadPath(void) const {
	return this->_upload_path;
}
