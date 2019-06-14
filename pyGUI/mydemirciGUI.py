""" Python to simulate RFQ as frontend
"""

from __future__ import print_function
from __future__ import absolute_import

import os
import sys


from builtins import zip
from builtins import str
from builtins import range
from builtins import object

from tkinter import *
from tkinter import filedialog
from tkinter.filedialog import askopenfilename


class Input(Frame):
    def __init__(self, parent):
        Frame.__init__(self, parent)
        self.parent = parent

        self.name_label = Label(root, text = 'Simulator for RFQ')
        self.name_entry = Entry(root)

    def close_window(self):
        self.name = self.name_entry.get()
        self.quit()


def NewConfigFile():
    print("Test")
    
def OpenFile():
    # winOpen = Toplevel(root)
    # menuNewConfigFile = Menu(winOpen)
    # winOpen.config(menu = menuNewConfigFile)
    # menuNewConfigFile.add_command(label = "Opening New RFQ Configuration File")

    name = filedialog.askopenfilename(
        title = "Choose Configuration file", filetypes=(("input file", ".in"),("all files", "*"))
    )
    return name
    
    
def About():
    print("GUI for DEMIRCI")
    

root = Tk()
root.geometry("700x400+300+300")


menu=Menu(root)
root.config(menu=menu)
app = Input(root)

filemenu=Menu(menu)
filemenu.add_command(label="New",  command=NewConfigFile)
filemenu.add_command(label="Open", command=OpenFile)
filemenu.add_separator()
filemenu.add_command(label="Exit", command=root.quit)

helpmenu=Menu(menu)
helpmenu.add_command(label="About",command=About)

menu.add_cascade(label="File", menu=filemenu)
menu.add_cascade(label="Help", menu=helpmenu)

win1= Label(root, text="DEMIRCI")
win1.pack()
root.mainloop()
