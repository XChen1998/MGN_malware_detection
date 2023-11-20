import os
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument(dest='testcase',help = 'Set a test case')
args = parser.parse_args()
case =  args.testcase;

if case == "test":
	os.system('interpreter/build/interpreter test/testbyte')

elif case == "login":
	os.system('interpreter/build/interpreter login/loginbyte')

elif case == "backdoor":
	os.system('backdoor/build/interpreter login/loginbyte')

else:
	print("Wrong case. type 'test' or 'login' or 'backdoor'")