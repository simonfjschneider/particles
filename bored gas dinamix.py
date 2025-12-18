import numpy as np
from pygame.locals import *
import random,pygame,sys

w=640
h=640
disp=pygame.display.set_mode((w,h))




class ball:
    def __init__(self,r,c,v,m):
        self.r=r#radius
        self.c=np.array(c)#center
        self.v=np.array(v)#vector
        self.m=m#mass
        self.blue=random.randint(0,255)

    def untangle(self,balls):
        for i in balls:
            if np.linalg.norm(i.c - self.c) < (self.r+i.r):

                #calc intersect
                intersect= (self.r+i.r) - np.linalg.norm(i.c - self.c)

                
                touch=self.c-i.c
                touch = touch/np.linalg.norm(touch)
                touch=touch*intersect/2
                self.c+=touch
                i.c-=touch
            
        
    def clip(self,balls):#gets list of balls and checks himself against them
        for i in balls:
            if np.linalg.norm(i.c - self.c) < (self.r+i.r):
                vtemp=self.v.copy()
                #massfacktor=i.m*2 / (self.m+i.m)
                d = np.dot(self.v-i.v , self.c-i.c)
                d = d / (np.linalg.norm(self.c - i.c)**2)
                d = d * (self.c - i.c)
                d=d#*massfacktor
                self.v=self.v - d

                #stupid second time for other ball
                #massfacktor=self.m*2 / (self.m+i.m)
                xdiff=i.c-self.c
                d = np.dot(i.v-vtemp , xdiff)
                d = d / (np.linalg.norm(i.c - self.c)**2)
                d = d * (xdiff)
                d=d#*massfacktor
                i.v=i.v - d

        


    def move(self):
        self.c = self.c + self.v
        self.c[0] = self.c[0]%w
        self.c[1] = self.c[1]%h

    def draw(self,disp):
        pygame.draw.circle(disp,(0,255,self.blue),(int(self.c[0]),int(self.c[1])),self.r)
        #print("hey")
    def __float__(self):
        return np.linalg.norm(self.v)


balls=[]
s=2#maxspeed
for i in range(500):
    a=random.uniform(0,w)
    b=random.uniform(0,h)

    
    c=random.uniform(-s,s)
    d=random.uniform(-s,s)
    #newball=ball(1,[a,b],[c,d],1)
    balls.append(ball(1,[a,b],[c,d],1))



while True:
    #print(sum(map(float,balls)))
    pygame.display.update()
    pygame.time.delay(10)
    pygame.draw.rect(disp,(255,255,187),(0,0,w,h))
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
    for i, k in enumerate(balls):
        k.clip(balls[i+1:])#check collision with all bigger balls
    for i, k in enumerate(balls):
        k.untangle(balls[i+1:])

    for i in balls:
        i.move()
        i.draw(disp)
    















