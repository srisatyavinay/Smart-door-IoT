from tkinter import *
from tkinter import font
from getData import details  # list details from file getData.py
from onem2m import *
import webbrowser as wb
import time

notif_cnt = "http://127.0.0.1:8080/~/in-cse/in-name/Permission_Details/Notification"

chances = 3
match_flag = 0
root = Tk()

root.geometry("800x400")

nameval = StringVar()
passval = StringVar()
var = StringVar()
E = StringVar()
Name = ""
Password = ""
Label(root, text="Get Permission", font="arial 25 bold").grid(row=1, column=0)
Label(root, text="Number of chances: 3", font="arial 15").grid(row=1, column=2)


def submit():
    global chances, match_flag
    Name = nameval.get()
    Password = passval.get()
    match_flag = 0
    for x in details:
        y = x.split(",")
        if Name == y[0] and Password == y[1]:
            print("Matched")
            create_data_cin(notif_cnt, "M")  # M for matched
            match_flag = 1
            chances = 3

    if match_flag == 0:
        chances -= 1

    print("The name is : " + Name)

    nameval.set("")
    passval.set("")
    var.set("Left="+str(chances))

    if chances == 0:
        print("Ring buzzer")
        create_data_cin(notif_cnt, "N")  # N for not matched
        var.set("")
        chances = 3


Label(root, text="").grid(row=2, column=0)
name = Label(root, text='Name', font=('calibre', 20, 'bold'))
nameEntry = Entry(root, textvariable=nameval, font=('calibre', 20, 'normal'))
name.grid(row=3, column=0)
nameEntry.grid(row=3, column=1)

password = Label(root, text='Password', font=('calibre', 20, 'bold'))
passEntry = Entry(root, textvariable=passval, font=(
    'calibre', 20, 'normal'), show='*')
password.grid(row=4, column=0)
passEntry.grid(row=4, column=1)

sub_btn = Button(root, text='Submit', command=submit)
sub_btn.grid(row=5, column=1)


def askPermission():
    wb.open("https://docs.google.com/forms/d/e/1FAIpQLScLxTYVFGg_Rq3evjDE82Mpgh-DpnJ_KVVSvNli7TiiCodEEA/viewform?usp=sf_link")


permission_btn = Button(root, text='Ask for Permission', command=askPermission)
permission_btn.grid(row=7, column=2)
Label(root, textvariable=var, font="arial 15 bold").grid(row=2, column=2)

if match_flag == 1:
    E.set("")
    Label(root, textvariable=E, font="Arial 20 bold").grid(row=8, column=1)

root.mainloop()
