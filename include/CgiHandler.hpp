#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include "webserv.hpp"

class HttpResponse;

class CgiHandler
{
	public:
		CgiHandler(HttpResponse const &response);
		CgiHandler(CgiHandler const & rhs);
		CgiHandler &operator=(CgiHandler const & rhs);
		~CgiHandler(void);

		int getStatus(void) const {return _status;}
		std::string getOutput(void) const {return _output;}
	
	private:
		std::map<std::string, std::string> _env;
		int _status;
		std::string _output;

		char **convertEnv(void);
		void timeout(pid_t pid);
		void execChild(char **argv, char **envp, int fd[2]);
		void execParent(int fd[2]);
		char **convertEnv();
		char **convertArgs(std::string cgi_fullpath, std::string exec_cgi);
		


};

#endif //CGIHANDLER_HPP
