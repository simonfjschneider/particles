import numpy as np
from pygame.locals import *
import random,pygame,sys

w=400
h=400
GRAVITY = True
disp=pygame.display.set_mode((w,h))


class line:
    """
    this just defines one line that balls cann collide with
    itself does nothing else and cant move

    """
    def __init__(self, a,b,c,d):

        if a < c: # only allow lines to go one direction
            # this might be silly but whatever
            a,c = c,a
            b,d = d,b

        self.a = a
        self.b = b
        self.c = c
        self.d = d

        n = np.array([c,d]) - np.array([a,b])
        n = n / np.linalg.norm(n)
        self.normal = np.array([-n[1],n[0]])

        """
        we will be using the wiki formula
        d = abs(d-b)x-(c-a)y+cb-da )/sqrt((d-b)**2+(c-a)**2)
        so we can precompute a lot of this
        distance = abs(f1x-f2y+f3)/divisor
        """
        self.divisor = np.sqrt((d-b)**2+(c-a)**2)
        self.f1 = d-b
        self.f2 = c-a
        self.f3 = c*b - d*a

        if a==c or b == d:
            # this is the "trivial" case of a axis aligned line
            # we can do fancy stuff here, ie have the calcualtions ge way easier
            pass


    def clip(self, balls):
        for i in balls:
            distance = abs(self.f1*i.c[0] - self.f2*i.c[1] +self.f3)/self.divisor

            if distance > i.r:
                continue
            if self.a < i.c[0]-i.r or i.c[0]+i.r < self.c:
                continue
            if self.b > self.d and (self.b < i.c[1]-i.r or i.c[1]+i.r < self.d):
                continue
            if self.d > self.b and (self.d < i.c[1]-i.r or i.c[1]+i.r< self.b):
                continue
            i.v = i.v - 2*np.dot(i.v,self.normal) * self.normal

            # prolly also have to unstick from here, might be even more important

            if distance < i.r:
                signed_distance = (self.f1*i.c[0] - self.f2*i.c[1] +self.f3)/self.divisor
                distance_sign = signed_distance / abs(signed_distance)
                i.c[0] += self.normal[0]*(distance-i.r)*distance_sign
                i.c[1] += self.normal[1]*(distance-i.r)*distance_sign



    def draw(self, disp):
        pygame.draw.line(disp, (255,0,0), (self.a, self.b), (self.c,self.d))

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
            
        
    def clip(self, balls):#gets list of balls and checks himself against them
        for i in balls:
            if np.linalg.norm(i.c - self.c) < (self.r+i.r):

                vtemp=self.v.copy()
                massfacktor=i.m*2 / (self.m+i.m)
                d = np.dot(self.v-i.v , self.c-i.c)
                d = d / (np.linalg.norm(self.c - i.c)**2)
                d = d * (self.c - i.c)
                d=d*massfacktor
                self.v=self.v - d

                #stupid second time for other ball
                massfacktor=self.m*2 / (self.m+i.m)
                xdiff=i.c-self.c
                d = np.dot(i.v-vtemp , xdiff)
                d = d / (np.linalg.norm(xdiff)**2)
                d = d * (xdiff)
                d=d*massfacktor
                i.v=i.v - d

        


    def move(self):
        self.c = self.c + self.v

        self.c[0] = self.c[0]%w

        if GRAVITY:
            self.v = self.v/1.005
            self.v[1] += 0.01
            if self.c[1] > h: self.v[1] = 0.1

        self.c[1] = self.c[1]%h

    def draw(self,disp):
        pygame.draw.circle(disp,(0,255,self.blue),(int(self.c[0]),int(self.c[1])),self.r)
        #print("hey")
    def __float__(self):
        return np.linalg.norm(self.v)


balls=[]
s=2#maxspeed
for i in range(20):
    a=random.uniform(150,200)
    b=random.uniform(150,200)

    
    c=random.uniform(0,s)
    d=random.uniform(-.1,.1)
    #newball=ball(1,[a,b],[c,d],1)
    balls.append(ball(10,[a,b],[c,d],3))


#balls.append(ball(30,[400.0,100.0],[0,0],200))
#balls.append(ball(30,[100.0,100.0],[-1,0],20))
lines = []
"""
lines.append(line(100,100,100,300))
lines.append(line(100,100,300,100))
lines.append(line(300,300,100,300))
lines.append(line(300,300,300,100))

lines.append(line(200, 100, 300, 200))  # top → right
lines.append(line(300, 200, 200, 300))  # right → bottom
lines.append(line(200, 300, 100, 200))  # bottom → left
lines.append(line(100, 200, 200, 100))  # left → top
"""
#lines.append(line(222.5, 102.6, 297.4, 222.5))
#lines.append(line(297.4, 222.5, 177.5, 297.4))
#lines.append(line(177.5, 297.4, 102.6, 177.5))
#lines.append(line(102.6, 177.5, 222.5, 102.6))

lines.append(line(0,100,100,200))
lines.append(line(400,100,300,200))
lines.append(line(200,200,100,300))
lines.append(line(200,200,300,300))


lines.append(line(0,350,50,400))
lines.append(line(400,350,350,400))

lines.append(line(1,0,1,h))
lines.append(line(w-2,0,w-2,h))


while True:
    #print(sum(map(float,balls)))
    pygame.display.update()
    pygame.time.delay(10)
    pygame.draw.rect(disp,(255,255,187),(0,0,w,h))  # oh no, im really just drawing a big rect over every frame
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
    for i, k in enumerate(balls):
        k.clip(balls[i+1:])#check collision with all higher balls
    #for i, k in enumerate(balls):
        k.untangle(balls[i+1:])

    for i in lines:
        i.clip(balls)
        i.draw(disp)

    energy = 0
    for i in balls:
        i.move()
        i.draw(disp)
        energy += np.sqrt(i.v[0]**2 + i.v[1]**2)
    print(energy)














