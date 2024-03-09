import socket

MAX_BUF_SIZE = 1024


class HTTPServer:

    def __init__(self, host_, port_):
        self.host = host_
        self.port = port_

    def start_server(self):
        serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serv.bind((self.host, self.port))
        serv.listen()
        while True:
            cli, cli_addr = serv.accept()
            try:
                self.serve_client(cli)
            except Exception as e:
                print('Client serving failed: ', e)

    def serve_client(self, cli):
        try:
            req = self.parse_request(cli)
            self.handle_request(req, cli)
        except Exception as e:
            print(e)
        if cli:
            cli.close()

    def parse_request(self, cli):
        data = cli.recv(MAX_BUF_SIZE)
        if not data:
            raise Exception('Not recv data from client')
        str_data = str(data, 'iso-8859-1')
        split_data = str_data.split(' ')
        req = Request(split_data[0], split_data[1], split_data[2])
        return req

    def handle_request(self, req, cli):
        if req.method != 'GET' and req.version != 'HTTP/1.1':
            raise Exception('Wrong request format!')
        try:
            if '.txt' in req.uri:
                self.send_file(cli, 'example.txt')
            if '/index.html' in req.uri:
                self.send_file(cli, 'example.html')
        except Exception as e:
            print(e)

    def send_file(self, cli, file_name):
        file = open(file_name, 'rb')
        data = file.read(MAX_BUF_SIZE)
        if not data:
            raise Exception('Data from file is empty')
        while data:
            size = cli.send(data)
            if not size:
                raise Exception('Data was not send!')
            data = file.read(MAX_BUF_SIZE)
        print('File was successfully send!')
        file.close()


class Request:
    def __init__(self, method_, uri_, version_, ):
        self.version = version_
        self.method = method_
        self.uri = uri_
