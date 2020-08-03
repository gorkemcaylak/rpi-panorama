import os
import time

FRAMES = 2
TIMEBETWEEN = 6

os.system("rm -rf cam/image*")
os.system("rm -rf cam/out/image*");

frameCount = 0
while frameCount < FRAMES:
    imageNumber = str(frameCount)#.zfill(2)
    os.system("raspistill -o cam/image%s.jpg"%(imageNumber))
    frameCount += 1
    time.sleep(TIMEBETWEEN - 6) #Takes roughly 6 seconds to take a picture
