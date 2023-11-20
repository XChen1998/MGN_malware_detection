import telnetlib
from .Shell import shell
from .Looper import format
from .Colors import Colors


class Utils:

    def dbg(self, something, debug=True):
        if debug:
            print("{}debug{} {} {}".format(Colors.WARNING, Colors.FAIL, something, Colors.END))

    @staticmethod
    def test_telnet(host, user, pwd):
        """
        test telnet connction return True if success // not used for now
        :param host:
        :param user:l
        :param pwd:
        :return:
        """
        thost = host
        tuser = user
        tpass = pwd

        tn = telnetlib.Telnet(thost, 23)
        tn.read_until("ogin: ")
        tn.write(tuser + "\n")
        tn.read_until("assword: ")
        tn.write(tpass + "\n")
        n, match, previous_text = tn.expect([r'incorrect', r'\$'], 4)
        if n == 0:
            return True
        else:
            return False

    def test_ssh(self, ip, u_p, protocol):
        """
        test ssh connection , return True if success
        :param ip:
        :param u_p:
        :param protocol:
        :return:
        """
        cmd = "sshpass -p {} ssh -q -tt -o ConnectTimeout=1 -o StrictHostKeyChecking=no {}@{} -p {} 'exit'".format(
            u_p[1].rstrip(), u_p[0].rstrip(), ip.rstrip(), protocol[0].rstrip())
        r = shell().execom(cmd)
        if r["retcode"] == 255 or r["retcode"] == 4:
            self.dbg("{}".format(r))
            return False
        elif r["retcode"] == 0:
            self.dbg(format(ip, protocol[0], u_p[0], u_p[1]).format())
            return True
