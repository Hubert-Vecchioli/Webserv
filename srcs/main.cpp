#include "webserv.hpp"

int main(int ac, char **av)
{
	Server &server  = Server::getInstance();
	signal(SIGINT, ft_stop_server);
	try
	{
		ConfigurationFile config = ConfigurationFile(ac, av);
		server.startServer(&config);
		server.runServer();
	}
	catch (std::exception & e)
	{
		print(2, e.what());
		return (1);
	}
	server.cleanup();
	delete &Server::getInstance();
	print(1, "[Info] - Webserv is now stopped");
	return (0);
}