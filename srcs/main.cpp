#include "webserv.hpp"

int main(int ac, char **av)
{
	try {
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
		}
		server.cleanup();
		delete &Server::getInstance();
		print(1, "[Info] - Webserv is now stopped");
		return (0);
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return (1);
	}
}