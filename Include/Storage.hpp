#pragma once
#include "Tools.hpp"
#include "Types.hpp"


class RouteConf {
  private:
    bool DirList;
    bool AutoIndex;
    bool IsCgi;
    std::string CgiName;
    std::string Location;
    std::string Redir;
    std::string Index;
    std::string UploadDir;
    std::vector<std::string> Methods;

  public:
    RouteConf();
    RouteConf(const std::string Location);
    RouteConf(const RouteConf &other);
    RouteConf &operator=(const RouteConf &other);
    ~RouteConf();

    bool GetDirectoryListen() const;
    bool GetAutoIndex() const;
    bool GetIsCgi() const;
    const std::string &GetCgiName() const;
    const std::string &GetLocationPath() const;
    const std::string &GetRedirection() const;
    const std::string &GetIndex() const;
    const std::string &GetUploadDir() const;
    const std::vector<std::string> &GetMethods() const;
    bool CheckMethod(const std::string &Method) const;

    void  SetDirList(bool DirList);
    void  SetAutoIndex(bool AutoIndex);
    void  SetIsCgi(bool IsCgi);
    void  SetCgiName(const std::string& CgiName);
    void  SetLocationPath(const std::string& LocationPath);
    void  SetRedirection(const std::string& Redirection);
    void  SetIndex(const std::string& Index);
    void  SetUploadDir(const std::string& UploadDir);
    void  SetMethod(const std::string& Method);
    void  SetMethods(std::vector<std::string> Methods);
};

class ServerConf {
  private:
    std::string Host;
    std::string ServerName;
    std::string Root;
    unsigned int Port;
    std::vector<RouteConf> Routes;
    std::map<unsigned int, std::string> ErrorPages;

  public:
    ServerConf();
    ServerConf(std::string Host, std::string ServerName, std::string Root,
              unsigned int Port);
    ServerConf(const ServerConf &other);
    ServerConf &operator=(const ServerConf &other);
    ~ServerConf();

    const std::string &GetHost() const;
    const std::string &GetServerName() const;
    const std::string &GetRoot() const;
    unsigned int GetPort() const;
    const std::vector<RouteConf> &GetRoutes() const;
    const std::map<unsigned int, std::string> &GetErrorPages() const;
    const RouteConf &GetLocation(const std::string &LocationPath) const;
    bool CheckLocation(const std::string &LocationPath) const;
    RouteConf&  GetLastRoute();

    void  SetHost(const std::string& Host);
    void  SetServerName(const std::string& ServerName);
    void  SetRoot(const std::string& Root);
    void  SetPort(unsigned int Port);
    void  SetErrorPage(const std::pair<unsigned int, std::string>& ErrorPage);
    void  SetEmptyRoute();
};

class ConfigFile {
  public:
    ConfigFile();
    ConfigFile(const ConfigFile &other);
    ConfigFile &operator=(const ConfigFile &other);
    ~ConfigFile();

    bool  CheckServer(const std::string &Host, unsigned int Port) const;
    const ServerConf &GetServer(const std::string &Host, unsigned int Port) const;

    const std::vector<ServerConf> &GetServers() const;
    ServerConf&  GetLastServer();

    void  SetEmptyServer();

  private:
    std::vector<ServerConf> servers;
};
