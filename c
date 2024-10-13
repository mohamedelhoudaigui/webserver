ClientMaxBodySize   16000
MaxClients          100
ErrorPage           ./www/Error.html


Server {
    Listen 80
    ServerName www.example.com
    Root /var/www/


    Location / {
        Index index.html
        AutoIndex on
        Methods GET POST
        Redir /new-page
    }

}
