#
# hello.reflect.45.py
#
# receive and display synchronous light reflection 
# hello.light.45.py serial_port
#
# Neil Gershenfeld
# CBA MIT 10/25/12
#
# (c) Massachusetts Institute of Technology 2012
# Permission granted for experimental and personal use;
# license for commercial sale available from MIT
#

from Tkinter import *
import serial

WINDOW = 600 # window size
eps = 0.9 # filter time constant
filter = 0.0 # filtered value
nloop = 100.0 # number of loops accumulated
amp = 25.0 # difference amplitude

def idle(parent,canvas):
   global filter, eps
   #
   # idle routine
   #
   byte2 = 0
   byte3 = 0
   byte4 = 0
   ser.flush()
   while 1:
      #
      # find framing 
      #
      byte1 = byte2
      byte2 = byte3
      byte3 = byte4
      byte4 = ord(ser.read())
      if ((byte1 == 1) & (byte2 == 2) & (byte3 == 3) & (byte4 == 4)):
         break
   on_low = ord(ser.read())
   on_high = ord(ser.read())
   on_value = (256*on_high + on_low)/nloop
   x = int(.25*WINDOW + (.9-.25)*WINDOW*on_value/1024.0)
   canvas.itemconfigure("text_on",text="on %.1f"%on_value)
   canvas.coords('rect1_on',.25*WINDOW,.05*WINDOW,x,.2*WINDOW)
   canvas.coords('rect2_on',x,.05*WINDOW,.9*WINDOW,.2*WINDOW)
   off_low = ord(ser.read())    
   off_high = ord(ser.read())
   off_value = (256*off_high + off_low)/nloop
   x = int(.25*WINDOW + (.9-.25)*WINDOW*off_value/1024.0)
   canvas.itemconfigure("text_off",text="off %.1f"%off_value)
   canvas.coords('rect1_off',.25*WINDOW,.25*WINDOW,x,.4*WINDOW)
   canvas.coords('rect2_off',x,.25*WINDOW,.9*WINDOW,.4*WINDOW)
   filter = (1-eps)*filter + eps*amp*(on_value-off_value)
   x = int(.25*WINDOW + (.9-.25)*WINDOW*filter/1024.0)
   canvas.itemconfigure("text_diff",text="diff %.1f"%filter)
   canvas.coords('rect1_diff',.25*WINDOW,.45*WINDOW,x,.6*WINDOW)
   canvas.coords('rect2_diff',x,.45*WINDOW,.9*WINDOW,.6*WINDOW)
   canvas.update()
   parent.after_idle(idle,parent,canvas)

#
#  check command line arguments
#
if (len(sys.argv) != 2):
   print "command line: hello.light.45.py serial_port"
   sys.exit()
port = sys.argv[1]
#
# open serial port
#
ser = serial.Serial(port,9600)
ser.setDTR()
#
# set up GUI
#
root = Tk()
root.title('hello.reflect.45.py (q to exit)')
root.bind('q','exit')
canvas = Canvas(root, width=WINDOW, height=.65*WINDOW, background='white')
#
canvas.create_text(.125*WINDOW,.125*WINDOW,text=".33",font=("Helvetica", 24),tags="text_on",fill="#0000b0")
canvas.create_rectangle(.25*WINDOW,.05*WINDOW,.3*WINDOW,.2*WINDOW, tags='rect1_on', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.05*WINDOW,.9*WINDOW,.2*WINDOW, tags='rect2_on', fill='#0000b0')
#
canvas.create_text(.125*WINDOW,.325*WINDOW,text=".33",font=("Helvetica", 24),tags="text_off",fill="#0000b0")
canvas.create_rectangle(.25*WINDOW,.25*WINDOW,.3*WINDOW,.4*WINDOW, tags='rect1_off', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.25*WINDOW,.9*WINDOW,.4*WINDOW, tags='rect2_off', fill='#0000b0')
#
canvas.create_text(.125*WINDOW,.525*WINDOW,text=".33",font=("Helvetica", 24),tags="text_diff",fill="#0000b0")
canvas.create_rectangle(.25*WINDOW,.45*WINDOW,.3*WINDOW,.6*WINDOW, tags='rect1_diff', fill='#b00000')
canvas.create_rectangle(.3*WINDOW,.45*WINDOW,.9*WINDOW,.6*WINDOW, tags='rect2_diff', fill='#0000b0')
canvas.pack()
#
# start idle loop
#
root.after(100,idle,root,canvas)
root.mainloop()
