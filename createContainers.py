from onem2m import *
uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"
ae = "Permission_Details"
cnt = "Data"

create_ae(uri_cse, ae)  # application entity

uri_ae = uri_cse+"/"+ae
create_cnt(uri_ae, cnt)  # data container

create_cnt(uri_ae, "Notification")

uri_cnt = uri_ae+"/"+cnt
notif_cnt = uri_ae+"/Notification"
create_data_cin(notif_cnt, "X")
