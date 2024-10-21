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

		void executeCgi(HttpResponse const &response);
		void execChild(char **argv, char **envp, int fd[2]);
		void execParent(pid_t pid, int fd[2]);
		void timeout(pid_t pid);
		char **convertEnv();
		


};

#endif //CGIHANDLER_HPP
