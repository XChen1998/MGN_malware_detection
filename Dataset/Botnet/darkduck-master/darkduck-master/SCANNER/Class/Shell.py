import socket
from subprocess import Popen, PIPE


class shell:
    def __init__(self):
        pass

    @staticmethod
    def get_ip_address():
        """
        Returns actual public address
        :return:
        """
        ip, port = "8.8.8.8", 80
        execution = {}
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect((ip, port))
            execution["ping"] = "ping to {}:{} success".format(ip, port)
            execution["local_ip"] = s.getsockname()[0]
            s.close()
            execution["socket"] = "closed"
            execution["error"] = False

        except Exception as e:
            execution["local_ip"] = "0.0.0.0"
            execution["ping"] = "ping to {}:{} fail".format(ip, port)
            execution["exception"] = str("{}".format(str(e)))
            s.close()
            execution["socket"] = "closed"
            execution["error"] = True

        return execution

    @staticmethod
    def execom(command: str):
        __exec = {}
        try:
            __exc = Popen(command.split(" "), stdout=PIPE)
            __exec['subprocess'] = str("".join(map(str, __exc.communicate()[0].decode()))),
            __exec['retcode'] = __exc.returncode
            __exec['exec'] = str("true")
        except Exception as e:
            __exec['subprocess'] = str("something went wrong with the executed command")
            __exec['exception'] = str("{}".format(str(e.args[1])))
            __exec['retcode'] = 1
            __exec['uniresp'] = str("false")

        return __exec
