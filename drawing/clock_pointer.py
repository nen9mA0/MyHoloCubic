import turtle as tu
from tkinter import *

r = 100
x = 0
y = 0

width = 240
height = 240


def draw():
    screen = tu.TurtleScreen(canva)
    path = tu.RawTurtle(canva)
    path.setup(width,height,x,y)
    path.pu()
    path.goto(x+r, height//2)
    path.pd()
    path.circle(r)

    screen.mainloop()


root = Tk()
root.title("test")
canva = Canvas(root, width=width, height=height)
canva.pack()

aa = Button(root, text="Draw", command=draw)
aa.pack()

root.mainloop()
