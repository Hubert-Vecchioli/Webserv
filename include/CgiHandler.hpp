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

		int getStatus(void) const {return _status;}
		std::string getOutput(void) const {return _output;}
	
	private:
		std::map<std::string, std::string> _env;
		int _status;
		std::string _output;

		char **convertEnv(void);
		void timeout(int pid);
		void execChild(char **argv, char **envp, int fd[2]);
		void execParent(pid_t pid, int fd[2]);
		void executeCgi(HttpResponse const &response);


};

#endif //CGIHANDLER_HPP
