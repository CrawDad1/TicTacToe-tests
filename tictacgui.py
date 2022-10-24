from os import makedirs
import string
import tkinter as tk
from tkinter import Tk, font, mainloop
from tkinter.constants import DISABLED, N, S
from typing import Text

#set global variable Turn state
Turn_State=['X',9]

def BoardString(btn_List)-> str:
    sBoard:str=""
    
    for i in range (9):
        if str(btn_list[i]["text"]).isdigit():
            sBoard+="e"
        else:
            sBoard+=str(btn_list[i]["text"]).lower()

        
    return sBoard


def UpdateState():
    global Turn_State
    Turn_State[1]-=1

    if Turn_State[0] == 'X':
        Turn_State[0]='O'
    elif Turn_State[0] == 'O':
        Turn_State[0]='X'

def CheckBoard(btn_list):
    
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
    

    return False
        
def BoardClick(btn : tk.Button):
    global Turn_State
    global lbl1
    global btn_list
    btn.configure(text=Turn_State[0], state=DISABLED)
    UpdateState()    
    print(Turn_State)
    print(BoardString(btn_list))
    if CheckBoard(btn_list) is True:
        for i in btn_list:
            i['state']=DISABLED
            lbl1['text']=(f"The Winner is: {btn['text']}")
    elif Turn_State[1] <=0:
            lbl1['text']=("DRAW!")
    
def GenerateButtons(_master):
    btn_list=list()

    for i in range(9):
        button=tk.Button(master=_master, foreground="white",text=f'{i+1}',background="black", disabledforeground="white", activeforeground='black',font=("arial", 25),height=2,width=4)
        button.configure(command=lambda arg1=button : BoardClick(arg1))
        button.grid(column=int(i%3),row=int(i//3), ipadx=2,ipady=2)
        btn_list.append(button)
        


    return btn_list

def GenerateButtons(_master, rows : int, cols : int):
    btn_list=list()

    for i in range(rows*cols):
        button=tk.Button(master=_master, foreground="white",text=f'{i+1}',background="black", disabledforeground="white", activeforeground='black',font=("arial", 25),height=2,width=4)
        button.configure(command=lambda arg1=button : BoardClick(arg1))
        button.grid(column=int(i%cols),row=int(i//cols), ipadx=2,ipady=2)
        btn_list.append(button)
        


    return btn_list


def fClose(num):
    #workaround for me not understanding tkinter
    if num == -1:
        exit()


def fReset(Game:tk.Tk):
    global rst_flag; rst_flag=True
    print("Game reset\n")
    Game.destroy()
    Game.quit()
    



def PlayGame(Main_Window):
    global Turn_State
    rows=3
    cols=3
    Turn_State=['X',(rows*cols)]
    background = tk.Frame(master=Main_Window,background="black")
    Frame1=tk.Frame(master=background,background="black",relief="raised", padx=5,pady=2,border=10)
    Frame2=tk.Frame(master=background,background="black",relief="sunken", padx=5,pady=2,border=10)
    global lbl1; lbl1=tk.Label(master = background, foreground="white",wraplength=400,background= "black", text="Let's play some Tic-Tac-Toe!", font=("arial", 20),pady=10)
    btn_close=tk.Button(master = Frame2 ,height=3,foreground="white",  border=5 ,background= "black",text="CLOSE!", font=("arial", 25), command= (lambda arg1 = -1: fClose(arg1)))
    btn_retry=tk.Button(master = Frame2 ,height=3,foreground="white" , border=5 ,background= "black",text="RESET", font=("arial", 25), command= (lambda arg1=background: fReset(arg1)))

    
    ##pack order
    background.pack(fill='both')
    #Frame1.pack()
    Frame1.grid(column=0,row=0, rowspan=3, columnspan=3)
    global btn_list; btn_list = GenerateButtons(Frame1,rows,cols)
    #Frame2.pack(after=Frame1,side="right", fill="both", )
    Frame2.grid(column=3,row=0, rowspan=3,sticky=N)
    btn_close.pack(fill='both',pady=5.2)
    btn_retry.pack(fill='both',pady=5.2)
    #lbl1.pack(after = Frame2, side="bottom")
    lbl1.grid(column=0,row=4,columnspan=4,sticky=S)

    

    Main_Window.mainloop()
    flag=rst_flag 
    if not flag:
        exit()
    else:
        flag=False

    



#main code
rst_flag = False

Main_Window = tk.Tk()
Main_Window.title("Tic-Tac-Toe")    
while(True):{
    PlayGame(Main_Window)
}
