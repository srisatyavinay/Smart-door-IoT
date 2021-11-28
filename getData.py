import requests
import json
import time

uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"
ae = "Permission_Details"
cnt = "Data"
uri_ae = uri_cse+"/"+ae
uri_cnt = uri_ae+"/"+cnt

headers = {
    'X-M2M-Origin': 'admin:admin',
    'Content-type': 'application/json'
}

uri = uri_cnt + "?rcn=4"
response = requests.get(uri, headers=headers)
_resp = json.loads(response.text)

details = []

for d in _resp["m2m:cnt"]["m2m:cin"]:
    details.append(d["con"])
