ClientMaxBodySize   16000
MaxClients          100
ErrorPage           ./www/Error.html


Server {
    Listen 80
    ServerName www.example.com
    Root /var/www/

    Location /api {
        Redir /new-page
    }

    Location / {
        Index index.html index.htm
        AutoIndex on
        Methods GET POST
    }

    Location /files {
        Redir /var/www/
    }
}

Server {
    Listen 69
    ServerName www.zbi.com
    Root /var/wwww

    Location /api {
        Redir /new-page
    }

    Location / {
        Index index.html index.htm
        AutoIndex on
        Methods GET POST
    }

    Location /files {
        Redir /var/www/files
    }
}

