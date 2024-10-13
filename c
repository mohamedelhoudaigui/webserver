ClientMaxBodySize   16000
MaxClients          100
ErrorPage           ./www/Error.html


Server {
    Listen 80 default_server
    ServerName example.com www.example.com
    Root /var/www/example.com

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

Server {
    Listen 8080
    ServerName test.example.com
    Root /var/www/test

    Location / {
        Index index.html index.htm
    }

    Location /kapouet {
        Redir /tmp/www
        AutoIndex off
    }
}
