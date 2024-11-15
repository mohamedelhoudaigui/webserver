/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboutuil <mboutuil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:46:26 by mboutuil          #+#    #+#             */
/*   Updated: 2024/11/15 17:17:21 by mboutuil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP

#include<iostream>
#include<string>
// #include<ofstream>
#include<fstream>
#include<map>

// enum _status
// {
//     REQUEST_LINE,
//     HEADERS,
//     BODY,
//     INVALID
// }_status;

typedef enum _methods
{
    GET,
    POST,
    DELETE,
}_methods;

class request_line
{
    private :
        std::string uri;
        std::string http_version;
        enum _methods method;
    public :
        void    reqline_info(std::string _reqline);
        enum _methods get_method();
        std::string get_uri();
        std::string get_httpversion();
};

class core_request{
    private:
        std::string _requ_line;
        std::string _headers;
        std::string _body;
        bool        _has_body;
    public:
        void    read_req(std::string request);
        std::string get_reqline(){return this->_requ_line;}
        std::string get_headers(){return this->_headers;}
        std::string get_body(){return this->_body;}
};

class _http_req
{
    private:
        request_line reqline;
        std::map<std::string,std::string> headers;
        std::string body;
    public :
        virtual void parse_headers(std::string headers);
        virtual void parse_body(std::string body);
};

class   Postparser : public _http_req
{
    public :
        void    parse_headers(std::string headers);
        void    parse_body(std::string body);
        void    process_headers();
        // Postparser();
        // ~Postparser();
    private:
};
class   Getparser : public _http_req
{
    public :
        // Getparser();
        // ~Getparser();
        void    parse_headers(std::string headers);
        void    parse_body(std::string body);
        void    process_headers();
    private:

};
class   Deleteparser : public _http_req
{
    public :
        // Deleteparser();
        // ~Deleteparser();
        void    parse_headers(std::string headers);
        void    parse_body(std::string body);
        void    process_headers();
    private:

};

class   factory
{
    private:
        core_request c_req;
        request_line reqline;
    public :
    _http_req *parser()
    {
        if (reqline.get_method() == POST)
            return new Postparser();
        else if (reqline.get_method() == GET)
            return new Getparser();
        else if (reqline.get_method() == DELETE)
            return new Deleteparser();
        else
            return nullptr;
    }
};
#endif