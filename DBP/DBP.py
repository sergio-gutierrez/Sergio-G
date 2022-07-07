from inspect import EndOfBlock
from tkinter import *

class GUI:
    def __init__(self, master):
        self.master = master
        master.title("DBP")

        self.frame_tasks = LabelFrame(master, text =  "Tasks To Do")
        self.frame_tasks.grid(row =0, column = 0)
        
        self.test_button = Button(self.frame_tasks, text = "test")
        self.test_button.grid(row = 3, column = 3)

        self.greet_button = Button(master, text="Greet", command=self.greet).grid(row = 3, column = 2, pady = 2)

        self.close_button = Button(master, text="Close", command=master.quit).grid(row = 4, column = 2, pady = 2)

        self.task_label = Label(text = "Enter Task Here").grid(row = 5, column = 0, pady = 2)
        self.input_text = Text(root, height = 1, width = 25).grid(row = 5, column = 1, pady = 2)
        
        self.enter_button = Button(master, text = "Enter", command = master.quit).grid(row = 5, column = 2, padx = 5, pady = 5)

    def greet(self):
        print("Greetings!")

    #this is test commit
    
    

root = Tk()
root.geometry("400x600")
my_gui = GUI(root)
root.mainloop()