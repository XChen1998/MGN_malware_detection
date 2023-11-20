# USBdrop
A friendly way to do a social engineering test without actually opening a backdoor onto a client's computer

If you want to change any of the source code, you will need to recompile to get the icon back. I use pyinstaller

pip install pyinstaller

pyinstaller -F -i icon.ico script.py

USB Drop Documentation The point of this USB drop is not to compromise a machine or network, it is a test to see if a company is susceptible to social engineering attacks. Server side: Not much needs to be done here unless you want to edit how the server logs connections. Requirements: Python

Requests Folder:
This is the most current source code for the USB Drop
It looks like web traffic and uses a web server to communicate

TCP Folder:
This is the older source code which used the python library socket to create a TCP Connection and send ASCII 

Recommended Setup Folder:
This is what you would copy over to the USB Drive.

TODO:
This may or may not be caught by the antivirus, looking into ways to avoid that because the computer might see it as an unrecognized executable. 
