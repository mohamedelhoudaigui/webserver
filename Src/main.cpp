#include "../Include/Config.hpp"

void print_all_config(ConfigFile config) {
  const std::vector<ServerConf> &servers = config.GetServers();
  for (size_t i = 0; i < servers.size(); ++i) {
    std::cout << "Server: " << std::endl;
    std::cout << "	host = " << servers[i].GetHost() << std::endl;
    std::cout << "	port = " << servers[i].GetPort() << std::endl;
    std::cout << "	server name = " << servers[i].GetServerName()
              << std::endl;
    std::cout << "	root = " << servers[i].GetRoot() << std::endl;

    const std::vector<RouteConf> &Routes = servers[i].GetRoutes();
    for (size_t j = 0; j < Routes.size(); ++j) {
      std::cout << "	Route: " << std::endl;
      std::cout << "		Location = " << Routes[j].GetLocationPath()
                << std::endl;
      std::cout << "		Index = " << Routes[j].GetIndex() << std::endl;
      std::cout << "		AutoIndex = " << Routes[j].GetAutoIndex()
                << std::endl;
      std::cout << "		DirList = " << Routes[j].GetDirectoryListen()
                << std::endl;
      std::cout << "		IsCgi = " << Routes[j].GetIsCgi() << std::endl;
      std::cout << "		CgiName = " << Routes[j].GetCgiName()
                << std::endl;
      std::cout << "		Redir = " << Routes[j].GetRedirection()
                << std::endl;
      std::cout << "		UplaodDir = " << Routes[j].GetUploadDir()
                << std::endl;

      const std::vector<std::string> &Methods = Routes[j].GetMethods();
      std::vector<std::string>::const_iterator it;
      std::cout << "		Methods: " << std::endl;
      std::cout << "			";
      for (it = Methods.begin(); it != Methods.end(); ++it)
        std::cout << *it << "-";
      std::cout << std::endl;
    }
  }
}

int main(int ac, char **av) {
  if (ac != 2) {
    std::cout << "usage: ./webserv {ConfigFile}" << std::endl;
    return (1);
  }
  try {
    Config c(av[1]);
    c.Parse();
    print_all_config(c.GetResult());
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    exit(1);
  }
}
