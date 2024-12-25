#! /usr/bin/python3

import requests
import json

data = {
    "service_id":"get_self_info",
    "to":"config",
    "request":{}
}

response = requests.post('http://192.168.1.148:9006/', json=data)

str = json.dumps(response.json()).encode("utf-8")

print(str)





























