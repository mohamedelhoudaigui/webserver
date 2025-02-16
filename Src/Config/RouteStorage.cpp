#include "../../Include/Storage.hpp"

RouteConf::RouteConf() {
  this->DirList = false;
  this->AutoIndex = false;
  this->IsCgi = false;
}

RouteConf::RouteConf(const std::string Location) {
  this->Location = Location;
  this->DirList = false;
  this->AutoIndex = false;
  this->IsCgi = false;
}

RouteConf::RouteConf(const RouteConf &other) { *this = other; }

RouteConf &RouteConf::operator=(const RouteConf &other) {
  if (this != &other) {
    this->DirList = other.GetDirectoryListen();
    this->AutoIndex = other.GetAutoIndex();
    this->IsCgi = other.GetIsCgi();
    this->CgiName = other.GetCgiName();
    this->Location = other.GetLocationPath();
    this->Redir = other.GetRedirection();
    this->Index = other.GetIndex();
    this->UploadDir = other.GetUploadDir();
    this->Methods = other.GetMethods();
  }
  return (*this);
}

RouteConf::~RouteConf() {}

//------------------------

bool RouteConf::GetDirectoryListen() const { return (this->DirList); }

bool RouteConf::GetAutoIndex() const { return (this->AutoIndex); }

bool RouteConf::GetIsCgi() const { return (this->IsCgi); }

const std::string &RouteConf::GetCgiName() const { return (this->CgiName); }

const std::string &RouteConf::GetLocationPath() const {
  return (this->Location);
}

const std::string &RouteConf::GetRedirection() const { return (this->Redir); }

const std::string &RouteConf::GetIndex() const { return (this->Index); }

const std::string &RouteConf::GetUploadDir() const { return (UploadDir); }

const std::vector<std::string> &RouteConf::GetMethods() const {
  return (this->Methods);
}

bool  RouteConf::CheckMethod(const std::string &Method) const 
{
  std::vector<std::string>::const_iterator it;
  it = find(this->Methods.begin(), this->Methods.end(), Method);
  if (it != this->Methods.end())
    return (true);
  return (false);
}


//---------------------
void  RouteConf::SetDirList(bool DirList) {
  this->DirList = DirList;
}

void  RouteConf::SetAutoIndex(bool AutoIndex) {
  this->AutoIndex = AutoIndex;
}

void  RouteConf::SetIsCgi(bool IsCgi) {
  this->IsCgi = IsCgi;
}

void  RouteConf::SetCgiName(const std::string& CgiName) {
  this->CgiName = CgiName;
}

void  RouteConf::SetLocationPath(const std::string& LocationPath) {
  this->Location = LocationPath;
}

void  RouteConf::SetRedirection(const std::string& Redirection) {
  this->Redir = Redirection;
}

void  RouteConf::SetIndex(const std::string& Index) {
  this->Index = Index;
}

void  RouteConf::SetUploadDir(const std::string& UploadDir) {
  this->UploadDir = UploadDir;
}

void  RouteConf::SetMethod(const std::string& Method) {
  this->Methods.push_back(Method);
}

void  RouteConf::SetMethods(std::vector<std::string> Methods) {
  this->Methods = Methods;
}
