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
		void execChild(char **argv, char **envp, int fd[2], int fdpost[2]);
		void execParent(int fd[2], int fdpost[2], std::string body);
		char **convertEnv();
		char **convertArgs(std::string cgi_fullpath, std::string exec_cgi);
		void _freeTab(char **tab);

		template <typename T>
		std::string ft_toString(const T &value) {
			std::stringstream ss;
			ss << value;
			return ss.str();
		}
		


};

#endif //CGIHANDLER_HPP
