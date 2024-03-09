import server as s

host = "127.0.0.1"
port = 20000

serv = s.HTTPServer(host, port)
serv.start_server()
