from tkinter import *
from tkinter.filedialog import askopenfilename
###from tkFileDialog import askopenfilename
root = Tk()

def NewConfigFile():
    name="RFQ_New.in"
def OpenFile():
    name=filedialog.askopenfilename(title="Choose Configuration file", filetypes=(("input file", ".in"),("all files", "*")))
def About():
    print("GUI for DEMIRCI")


menu=Menu(root)
root.config(menu=menu)

filemenu=Menu(menu)
filemenu.add_command(label="New",  command=NewConfigFile)
filemenu.add_command(label="Open", command=OpenFile)
filemenu.add_separator()
filemenu.add_command(label="Exit", command=root.quit)

helpmenu=Menu(menu)
helpmenu.add_command(label="About",command=About)

menu.add_cascade(label="File", menu=filemenu)
menu.add_cascade(label="Help", menu=helpmenu)

w= Label(root, text="DEMIRCI")
w.pack()
root.mainloop()
