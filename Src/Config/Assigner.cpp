/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assigner.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houd <mel-houd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:32:18 by mel-houd          #+#    #+#             */
/*   Updated: 2025/02/17 00:58:01 by mel-houd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Include/Config.hpp"

void Config::AssignTokens() {
  std::vector<TokenLine>::iterator it;
  for (it = ConfLines.TokenLines.begin(); it != ConfLines.TokenLines.end();
       ++it) {
    std::vector<Token> &Tokens = it->Tokens;

    Token &Key = Tokens[0];
    AssignGlobalParams(Key, Tokens);
    AssignServer(Key, Tokens);
    AssignLocation(Key, Tokens);
  }
  Logger(DEBUG, "tokens assigning passed");
}

// assign global params from config file
void Config::AssignGlobalParams(Token &Key, std::vector<Token> &Tokens) {
  (void)Key;
  (void)Tokens;
}

// assign server scope from config file
void Config::AssignServer(Token &Key, std::vector<Token> &Tokens) {
  if (Key.Token == "Server") {
    this->Result.SetEmptyServer();
    return;
  }
  ServerConf &last_server = this->Result.GetLastServer();

  if (Key.Token == "Listen")
    last_server.SetPort(PairValueNum(Tokens, "Listen", MAX_PORT));
  else if (Key.Token == "ServerName")
    last_server.SetServerName(PairValueStr(Tokens, "ServerName"));
  else if (Key.Token == "Host")
    last_server.SetHost(PairValueStr(Tokens, "Host"));
  else if (Key.Token == "Root")
    last_server.SetRoot(PairValueStr(Tokens, "Root"));
}

// assign server scope from config file
void Config::AssignLocation(Token &Key, std::vector<Token> &Tokens) {
  if (Key.Token == "Location") {
    ServerConf &last_server = this->Result.GetLastServer();
    last_server.SetEmptyRoute();
    return;
  }

  ServerConf &last_server = this->Result.GetLastServer();
  RouteConf &last_route = last_server.GetLastRoute();

  if (Key.Token == "Index")
    last_route.SetIndex(PairValueStr(Tokens, "Index"));
  else if (Key.Token == "Redir")
    last_route.SetRedirection(PairValueStr(Tokens, "Redir"));
  else if (Key.Token == "UploadDir")
    last_route.SetUploadDir(PairValueStr(Tokens, "UploadDir"));
  else if (Key.Token == "AutoIndex")
    last_route.SetAutoIndex(PairValueBool(Tokens, "AutoIndex"));
  else if (Key.Token == "DirList")
    last_route.SetDirList(PairValueBool(Tokens, "DirList"));
  else if (Key.Token == "Methods")
    last_route.SetMethods(MultiValueStr(Tokens, "Methods"));
  else if (Key.Token == "IsCgi")
    last_route.SetIsCgi(PairValueBool(Tokens, "IsCgi"));
  else if (Key.Token == "CgiName")
    last_route.SetCgiName(PairValueStr(Tokens, "CgiName"));
}

void Config::CheckResult() {
  CheckServers();
  CheckLocations();
}

// check server data
void Config::CheckServers() {
  CheckHost();
  CheckServerName();
  CheckRoot();
  CheckPort();
}

void Config::CheckPort() {
  const std::vector<ServerConf> &Servers = Result.GetServers();
  std::vector<ServerConf>::const_iterator it;

  for (it = Servers.begin(); it != Servers.end(); ++it) {
    if (it->GetPort() == 0)
      throw std::runtime_error("Server wihtout port or value is 0");
  }
}

void Config::CheckHost() {
  const std::vector<ServerConf> &Servers = Result.GetServers();
  std::vector<ServerConf>::const_iterator it;

  for (it = Servers.begin(); it != Servers.end(); ++it) {
    if (it->GetHost().empty())
      throw std::runtime_error("Server without Host attribute");
  }
}

void Config::CheckServerName() {
  const std::vector<ServerConf> &Servers = Result.GetServers();
  std::vector<ServerConf>::const_iterator it;

  for (it = Servers.begin(); it != Servers.end(); ++it) {
    if (it->GetServerName().empty())
      throw std::runtime_error("Server without ServerName attribute");
  }
}
void Config::CheckRoot() {
  const std::vector<ServerConf> &Servers = Result.GetServers();
  std::vector<ServerConf>::const_iterator it;

  for (it = Servers.begin(); it != Servers.end(); ++it) {
    if (it->GetRoot().empty())
      throw std::runtime_error("Server without Root attribute");
  }
}

//------------------------------------------

void Config::CheckLocations() {
  const std::vector<ServerConf> &Servers = Result.GetServers();
  std::vector<ServerConf>::const_iterator server_it;

  for (server_it = Servers.begin(); server_it != Servers.end(); ++server_it) {
    const std::vector<RouteConf> &Routes = server_it->GetRoutes();
    std::vector<RouteConf>::const_iterator route_it;
    for (route_it = Routes.begin(); route_it != Routes.end(); ++route_it) {
      CheckIndex(*route_it);
      CheckCgi(*route_it);
    }
  }
}

void Config::CheckIndex(const RouteConf &Location) {
  if (!Location.GetIsCgi()) {
    if (Location.GetIndex().empty()) {
      if (Location.GetAutoIndex() == false)
        throw std::runtime_error("no index in location");
    }
  }
}

void Config::CheckCgi(const RouteConf &Location) {
  if (Location.GetIsCgi()) {
    if (Location.GetCgiName().empty())
      throw std::runtime_error("no CgiName in CGI location");
  }
}

void Config::CheckMethods(const RouteConf &Location) {
  std::vector<std::string>::const_iterator it;
  for (it = Location.GetMethods().begin(); it != Location.GetMethods().end();
       ++it) {
    if (*it != "GET" && *it != "POST" && *it != "DELETE" && *it != "PUT")
      throw std::runtime_error("Location params error invalid HTPP method: " +
                               *it);
  }
}
