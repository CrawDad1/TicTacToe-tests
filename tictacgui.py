import tkinter as tk
from tkinter import font
from tkinter.constants import DISABLED
from typing import Text

#set global variable Turn state

Turn_State=['X',9]

def UpdateState():
    global Turn_State

    if Turn_State[0] == 'X':
        Turn_State[0]='O'
    elif Turn_State[0] == 'O':
        Turn_State[0]='X'

def CheckBoard(btn_list):
    global Turn_State

    #horizontal
    if btn_list[0]['text'] == btn_list[1]['text'] == btn_list[2]['text']:
        return True
    elif btn_list[3]['text'] == btn_list[4]['text'] == btn_list[5]['text']:
        return True
    elif btn_list[6]['text']==btn_list[7]['text']==btn_list[8]['text']:
        return True

    #vertical
    if btn_list[0]['text'] == btn_list[3]['text'] == btn_list[6]['text']:
        return True
    elif btn_list[1]['text'] == btn_list[4]['text'] == btn_list[7]['text']:
        return True
    elif btn_list[2]['text']==btn_list[5]['text']==btn_list[8]['text']:
        return True

    #diagonal
    if btn_list[0]['text'] == btn_list[4]['text'] == btn_list[8]['text']:
        return True
    elif btn_list[2]['text'] == btn_list[4]['text'] == btn_list[6]['text']:
        return True
    
    if Turn_State[1]<=0:
        return True

    return False
        
def BoardClick(btn : tk.Button):
    global Turn_State
    global lbl1
    global btn_list
    btn.configure(text=Turn_State[0], state=DISABLED)
    UpdateState()
    Turn_State[1]-=1    
    print(Turn_State)
    if CheckBoard(btn_list) is True:
        if Turn_State[1] > 0:
            lbl1['text']=("Sorry, this game's a draw")
        elif Turn_State[1] <=0:
            for i in btn_list:
                i['state']=DISABLED
            lbl1['text']=(f"The Winner is: {btn['text']}")
    
def GenerateButtons(_master,):
    btn_list=list()

    for i in range(9):
        button=tk.Button(master=_master,text=f'{i+1}',background='gray', disabledforeground='black', activeforeground='black',font=("arial", 25),height=2,width=4)
        button.configure(command=lambda arg1=button : BoardClick(arg1))
        button.grid(column=int(i%3),row=int(i//3), ipadx=2,ipady=2)
        btn_list.append(button)
        


    return btn_list

def fClose(num):
    if num == -1:
        exit()

Main_Window = tk.Tk()
background = tk.Frame(master=Main_Window,background='gray')
Frame1=tk.Frame(master=background,background='gray',relief="raised", padx=5,pady=2,border=10)
#Frame1.tk_bisque()
lbl1=tk.Label(master = background,wraplength=400,background= 'gray', text="Let's play some\nTic-Tac-Toe!", font=("arial", 25),padx=10,pady=10)
btn_close=tk.Button(master = background,background= 'gray',text="CLOSE!", font=("arial", 25), command= (lambda arg1 = -1: fClose(arg1)))


##pack order
background.pack(fill='both')
Frame1.pack()
btn_list = GenerateButtons(Frame1)

lbl1.pack()
btn_close.pack()



Main_Window.mainloop()
