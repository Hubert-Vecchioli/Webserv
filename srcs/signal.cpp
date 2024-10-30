#include "webserv.hpp"

void ft_stop_server(int signum)
{
	(void) signum;
	print(1, "[Info] - Signal request to stop the server received");
	Server::stopServer();
}