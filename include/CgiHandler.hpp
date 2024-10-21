#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

#include "webserv.hpp"

class HttpResponse;

class CgiHandler
{
	public:
		CgiHandler(HtppResponse &response);
		CgiHandler(CgiHandler const & rhs);
		CgiHandler &operator=(CgiHandler const & rhs);
		~CgiHandler(void);
	
	private:
		std::map<std::string, std::string> _env;
		int _status;
		std::string _output;

		void executeCgi(HttpResponse const &response);
		void execChild(char **argv, char **envp, int fd[2]);
		void execParent(pid_t pid);
		void timeout(pid_t pid);
		char **convertEnv();
		


};

#endif //CGIHANDLER_HPP
