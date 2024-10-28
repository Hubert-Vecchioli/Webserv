#include "CgiHandler.hpp"

pid_t pid = -1;

CgiHandler::CgiHandler(HttpResponse const &response) {
	std::string uri = response.getRequest().getRequestURI();
	std::cout<<"uri: "<< uri << std::endl;

	std::string pathinfo = uri.substr(uri.find('.', 8));
	pathinfo = pathinfo.substr(0, pathinfo.find_last_of('/') + 1);
	_env["CONTENT_LENGTH"] = ft_toString(response.getRequest().getContentLength());
	_env["CONTENT_TYPE"] = response.getRequest().getContentType();
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = pathinfo;
	_env["PATH_TRANSLATED"] = response.getLocationBlock().getRoot() + pathinfo;
	_env["QUERY_STRING"] = response.getRequest().getQueryString();
		std::cout<<"QUERY_STRING: "<< response.getRequest().getQueryString() << std::endl;
	_env["REMOTE_ADDR"] = ""; //client IP :: TODO
	_env["REQUEST_METHOD"] = response.getRequest().getStringMethod();
	_env["SCRIPT_NAME"] = uri.substr(uri.find('/') + 1);;//.substr(uri.find_last_of('/') + 1);
	_env["SERVER_NAME"] = uri.substr(8, uri.find('.', 8) - 8);
	_env["SERVER_PORT"] = ft_toString(response.getServerBlock().getIPandPort().second);
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "webserv";
	_env["HTTP_COOKIE"] = response.getRequest().getCookieString();
	//_env["REMOTE_HOST"] = nullptr; // client domain name if available
	//_env["REMOTE_IDENT"] = nullptr; // not implemented
	//_env["REMOTE_USER"] = ""; // linked to auth type - not implemented
	//_env["AUTH_TYPE"] = ""; // not implemented
	executeCgi(response);
}

CgiHandler::CgiHandler(CgiHandler const & rhs) {
	*this = rhs;
}

CgiHandler & CgiHandler::operator=(CgiHandler const & rhs) {
	if (this != &rhs) {
		_env = rhs._env;
		_status = rhs._status;
		_output = rhs._output;
	}
	return *this;
}

CgiHandler::~CgiHandler(void) {}

// main functions

void CgiHandler::executeCgi(HttpResponse const &response) {
	std::string cgi_fullpath = _env["SCRIPT_NAME"] ; //_env["PATH_TRANSLATED"] +
    size_t query_pos = cgi_fullpath.find_last_of('?');
	if (query_pos != std::string::npos)
    	cgi_fullpath = cgi_fullpath.substr(0, query_pos);
	std::string cgi_ext = cgi_fullpath.substr(cgi_fullpath.find_last_of('.'), cgi_fullpath.find_last_of('?') - (cgi_fullpath.find_last_of('.')));
	std::string exec_cgi = response.getLocationBlock().getCgiExtension()[cgi_ext];
	std::map<std::string, std::string> cgiExtension = response.getLocationBlock().getCgiExtension();
	std::cout<<"Location block address : "<< &response.getLocationBlock() << std::endl;
	if (exec_cgi.empty()) {
		_status = 501;
		throw std::runtime_error("Unrecognized CGI extension");
		return;
	}
	int fd[2];
	if (pipe(fd) == -1) {
		_status = 500;
		throw std::runtime_error("Pipe failed");
	}
	int fdpost[2];
	if (pipe(fdpost) == -1) {
		_status = 500;
		throw std::runtime_error("Pipe failed");
	}
	char **argv = convertArgs(cgi_fullpath, exec_cgi);
	char **envp = convertEnv();
	pid = fork();
	if (pid < 0) {
		_status = 500;
		delete[] envp;
		delete[] argv;
		throw std::runtime_error("Fork failed");
	}
	else if (pid == 0)
		execChild(argv, envp, fd, fdpost);
	else
		try {
			execParent(fd, fdpost, response.getRequest().getBody());
		}
		catch (std::exception &e) {
			delete[] envp;
			delete[] argv;
			throw e;
		}
	_status = 200;
	delete[] envp;
	delete[] argv;
}

static void timeoutHandler(int sig) {
	(void)sig;
	if (pid > 0)
		kill(pid, SIGKILL);
}

char **CgiHandler::convertEnv() {
	char **env = new char*[_env.size() + 1];
	int i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++) {
		std::string keyvalue = it->first + "=" + it->second;
		env[i] = strdup(keyvalue.c_str());
		std::cout<<" env [" << i << "]"<< env[i] << std::endl;
		i++;
	}
	env[i] = NULL;
	return env;
}

char **CgiHandler::convertArgs(std::string cgi_fullpath, std::string exec_cgi) {
	char **argv = new char*[3];
	argv[0] = strdup(exec_cgi.c_str());
	argv[1] = strdup(cgi_fullpath.c_str());
	argv[2] = NULL;
	return argv;
}

void CgiHandler::execChild(char **argv, char **envp, int fd[2], int fdpost[2]) {
	close(fdpost[1]);
	close(fd[0]);
	dup2(fdpost[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fdpost[0]);
	close(fd[1]);
	execve(argv[0], argv, envp);
	exit(1);
}

void CgiHandler::execParent(int fd[2], int fdpost[2], std::string body) {
	close(fdpost[0]);
	close(fd[1]);
	signal(SIGALRM, timeoutHandler);
	alarm(5);

	int status;
	if (-1 == write(fdpost[1], body.c_str(), body.size())) {
		close(fdpost[1]);
		close(fd[0]);
		_status = 500;
		kill(pid, SIGKILL);
		throw std::runtime_error("Write failed");
	}
	close(fdpost[1]);
	pid_t result = waitpid(pid, &status, 0);
	alarm(0);

	if (result == -1) {
		close(fd[0]);
		_status = 500;
		throw std::runtime_error("Waitpid failed");
	}	
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		close(fd[0]);
		_status = 500;
		throw std::runtime_error("CGI Failed to execute properly");
	}
	else if (WIFSIGNALED(status)) {
		close(fd[0]);
		_status = 504;
		throw std::runtime_error("CGI Timeout");
	}
	
	int i = 0;
	const int bufferSize = 1024;
	char buffer[bufferSize];
	while ((i = read(fd[0], buffer, bufferSize - 1)) > 0) {
		buffer[i] = '\0';
		this->_output += buffer;
	}
	if (i == -1) {
		close(fd[0]);
		_status = 500;
		throw std::runtime_error("Read failed");
	}
	close(fd[0]);
	
}
