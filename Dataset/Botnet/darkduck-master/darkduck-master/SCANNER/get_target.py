#!/usr/bin/python3
import json
import requests
##### TARGETS #####
API_URL = "https://censys.io/api/v1"
UID = "9541ad2f-b698-43b3-a1fe-3c5efa49bf82"
SECRET = "9U7Zcclxk82QNztg5JhibRjbsRRhyXK0"
payload = {'query': 'protocols: "22/ssh"'}
res = requests.post(API_URL + "/search/ipv4", json=payload, auth=(UID, SECRET))
with open('ips', 'a') as targets_file:
    for result in res.json()['results']:
        targets_file.write(result['ip'] + "\n")
with open('ips_local', 'a') as targetslocal_file:
    targetslocal_file.write("10.3.107.98\n10.3.107.96")
##### TARGETS #####


