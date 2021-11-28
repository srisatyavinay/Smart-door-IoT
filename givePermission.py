from onem2m import *

uri_cse = "http://127.0.0.1:8080/~/in-cse/in-name"
ae = "Permission_Details"
cnt = "Data"

inputData = input("Enter Name,Password: ")  # if form of "Name,Password"

if(inputData.find(',') > 0):
    uri = uri_cse+"/"+ae+"/"+cnt
    create_data_cin(uri, inputData)
else:
    print("Incorrect Data Format")
