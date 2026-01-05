#include <iostream>
#include <stdio.h>

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <cmath>//for sqrt
#include <sys/time.h>
#include <assert.h>

int n = 200 ;
int nr = 5;
int w = 400;//breite
int h = 400;//hohe

// num of particles
int eventhandler(){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
	//If user closes the window
	if (e.type == SDL_QUIT){
		return -1;
	}
	/*
	//If user presses any key
	if (e.type == SDL_KEYDOWN){
		switch( e.key.keysym.sym )
            {   //java codes
                case SDLK_UP: 
                    return 38;
                case SDLK_DOWN: 
                    return 40;
                case SDLK_LEFT: 
                    return 37;
                case SDLK_RIGHT: 
                    return 39; 
            }
	
	//If user clicks the mouse
	if (e.type == SDL_MOUSEBUTTONDOWN){
		quit = true;
	}
        }
    //else
    return 0;*/
    }
    
    return 0;
}
typedef struct _ball{ 
    int r; // radius
    float c[2]; //center of ball
    float v[2]; //velocity
    int m;  // mass
}ball;

typedef struct _line{
    // a line goes from point a,b to point c,d
    int a;
    int b;
    int c;
    int d;

    float normal[2];
    /*
     we will be using the wiki formula
    d = abs(d-b)x-(c-a)y+cb-da )/sqrt((d-b)**2+(c-a)**2)
    so we can precompute a lot of this
    distance = abs(f1x-f2y+f3)/divisor
     */
    int f1;
    int f2;
    int f3;
    float divisor;

}line;



float dot(float x[2], float y[2]){
    return (x[0]*y[0] + x[1]*y[1]);
}


float* sub(float x[2], float y[2], float out[2]){
    out[0] = x[0] - y[0];
    out[1] = x[1] - y[1];
    return out;
}
float* add(float x[2], float y[2], float out[2]){
    assert(out != x);
    assert(out != y);
    out[0] = x[0] + y[0];
    out[1] = x[1] + y[1];
    return out;
}
float norm(float x[2]){
    return std::sqrt(x[0]*x[0] + x[1]*x[1]);
}
float ranflo(float start, float stop){
    //int start=-2;
    //int stop=2;
    float range = stop - start;
    float res=100;
    float x= (float)( (rand()%(int)(res*range ) +res*start  ));
    x= x/res;
    return x;
}


void build_line(line * l){
    // i expect the points a to d to be set
    // so this function will only calculate f1 f2 f3 and divisor
    // well it will also enforce the downward line constrain
    //    so the ab cd points might be swapped

    if (l->a < l->c){
        int temp;
        temp = l->a;
        l->a = l->c;
        l->c = temp;
        temp = l->b;
        l->b = l->d;
        l->d = temp;
    }

    float n[2];
    n[0] = l->c - l->a;
    n[1] = l->d - l->b;
    float nor = norm(n);
    l->normal[0] = - (n[1] / nor);
    l->normal[1] = n[0]/nor;

    l->divisor = std::sqrt((l->d - l->b)*(l->d - l->b) + (l->c - l->a)*(l->c - l->a));
    l->f1 = l->d - l->b;
    l->f2 = l->c - l->a;
    l->f3 = l->c*l->b - l->d*l->a;
}

void clip_line(ball * balls, line* self){
    int distance;
    for (int i=0; i<n; i++){
        distance = std::abs(self->f1*balls[i].c[0] - self->f2*balls[i].c[1] +self->f3)/self->divisor;
        if (distance > balls[i].r){
            continue;
        }
        if (self->a < balls[i].c[0]-balls[i].r or balls[i].c[0]+balls[i].r < self->c){
            continue;
        }
        if (self->b > self->d and (self->b < balls[i].c[1]-balls[i].r or balls[i].c[1]+balls[i].r < self->d)){
            continue;
        }
        if (self->d > self->b and (self->d < balls[i].c[1]-balls[i].r or balls[i].c[1]+balls[i].r< self->b)){
            continue;
        }
        float temp[2];
        temp[0] = 2*dot(balls[i].v,self->normal) * self->normal[0];
        temp[1] = 2*dot(balls[i].v,self->normal) * self->normal[1];
        sub(balls[i].v , temp, balls[i].v);
    }

}

void newball(ball* b){
    //ball * b = (ball *) calloc(1,sizeof(ball));
    b->r=ranflo(1,nr);
    b->m=b->r;
    
    b->c[0]=ranflo(150, 200);
    b->c[1]=ranflo(150, 200);
    
    b->v[1]=ranflo(0,2);
    b->v[0]=ranflo(-.1,.1);
    
    //return b;
}


void move_ball(ball * b){
    b->c[0] = b->c[0] + b->v[0];
    b->c[1] = b->c[1] + b->v[1];
    
    if (b->c[0] < 0)b->c[0]=w-1;
    if (b->c[0] > w)b->c[0]=1;
    if (b->c[1] < 0)b->c[1]=h-1;
    if (b->c[1] > h)b->c[1]=1;
}



void draw_ball(SDL_Renderer* ren, ball ball){

    
    SDL_RenderDrawLine(ren, ball.c[0] + ball.r, ball.c[1], ball.c[0] - ball.r, ball.c[1]);
    SDL_RenderDrawLine(ren, ball.c[0], ball.c[1] + ball.r, ball.c[0], ball.c[1] - ball.r);
    //printf("draw ball\n");
    
    }
// Source - https://stackoverflow.com/a
// Posted by Scotty Stephens, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-18, License - CC BY-SA 4.0

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}


void draw_ball_circle(SDL_Renderer* ren, ball ball){
    DrawCircle(ren, ball.c[0], ball.c[1], ball.r);
}

void clip(ball * balls, int ballindex){
    ball b1 = balls[ballindex];
    int ii=ballindex;
    float z[2];

    
    float v_diff[2];
    float pos_diff[2];
    float vemp1[2];
    float k;
    ball b2;
    for(int i = ballindex +1; i<n; i++){
        b2=balls[i];
        sub(b1.c, b2.c, z);
        if( norm(z) < b1.r + b2.r){

            //v1
            sub(b1.v, b2.v , v_diff);
            
            sub(b1.c, b2.c , pos_diff);

            k = dot(v_diff, pos_diff);
            k = k / pow(norm(pos_diff), 2);
            k = k * (float)(2 * b2.m) / (float)(b1.m + b2.m);

            pos_diff[0] = pos_diff[0] * k;
            pos_diff[1] = pos_diff[1] * k;

            
            sub(b1.v, pos_diff, vemp1);
            
            // end v1

            //v2
            sub(b2.v, b1.v , v_diff);
            
            sub(b2.c, b1.c , pos_diff);
            
            k = dot(v_diff, pos_diff);
            k = k / pow(norm(pos_diff), 2);
            k = k * ( (float)(2 * b1.m) / (float)(b2.m + b1.m) );

            pos_diff[0] = pos_diff[0] * k;
            pos_diff[1] = pos_diff[1] * k;
            
            b1.v[0]= vemp1[0];
            b1.v[1]= vemp1[1];
            sub(b2.v, pos_diff, vemp1);
            b2.v[0]= vemp1[0];
            b2.v[1]= vemp1[1];
            // end v2
            balls[i]=b2;
            /*printf("b1.v = (%f, %f)   b2.v = (%f, %f)\n",
                    b1.v[0], b1.v[1],
                    b2.v[0], b2.v[1]);
            printf("b1.c = (%f, %f)   b2.c = (%f, %f)\n",
                    b1.c[0], b1.c[1],
                    b2.c[0], b2.c[1]);
            */
            //untangle:
            
            /*
            
            
            
            
            
            */
            
            //intersect = (self->r+i.r) - np.linalg.norm(i.c - self->c)
            float intersect;
            sub(b2.c,b1.c,pos_diff);
            intersect = b1.r+b2.r - norm(pos_diff) ;
            intersect = intersect + .01;
            //touch=self->c-i.c
            float touch[2];
            sub(b1.c, b2.c, touch);
            
            //touch = touch/np.linalg.norm(touch)
            k=norm(touch);
            touch[0] = touch[0]/k;
            touch[1] = touch[1]/k;
            
            //touch=touch*intersect/2
            touch[0] = ( touch[0] * intersect ) / 2;
            touch[1] = ( touch[1] * intersect ) / 2;
            
            //self->c+=touch
            //i.c-=touch
            b1.c[0]+=touch[0];
            b1.c[1]+=touch[1];
            b2.c[0]-=touch[0];
            b2.c[1]-=touch[1];
            balls[i]=b2;
            
        }
    }
    balls[ii]=b1;
}

    
    

void mainloop(SDL_Renderer* ren){
    
    //setup
    ball* balls = (ball*) calloc(n, sizeof(ball));
    //ball balls[10];
    for (int i=0; i<n; i++){
        newball(&(balls[i]));
    }
    int linenum = 4;
    line* lines = (line*) calloc(linenum, sizeof(line));
    lines[0].a = 222.5;
    lines[0].b = 102.6;
    lines[0].c = 297.4;
    lines[0].d = 222.5;
    build_line(&lines[0]);

    lines[1].a = 297.4;
    lines[1].b = 222.5;
    lines[1].c = 177.5;
    lines[1].d = 297.4;
    build_line(&lines[1]);

    lines[2].a = 177.5;
    lines[2].b = 297.4;
    lines[2].c = 102.6;
    lines[2].d = 177.5;
    build_line(&lines[2]);

    lines[3].a = 102.6;
    lines[3].b = 177.5;
    lines[3].c = 222.5;
    lines[3].d = 102.6;
    build_line(&lines[3]);
    //setup
    
    
    if (2==1){
        balls[n-1].r=30;
        balls[n-1].m=200;
        balls[n-1].v[0]=0.0;
        balls[n-1].v[1]=0.0;

        balls[n-1].c[0]=400.0;
        balls[n-1].c[1]=100.0;

        balls[n-2].r=30;
        balls[n-2].m=20;
        balls[n-2].v[0]=1.0;
        balls[n-2].v[1]=0.0;

        balls[n-2].c[0]=100.0;
        balls[n-2].c[1]=100.0;
    }
    
    
    
    
    // more setup
    //int frames =0;
    
    struct timeval T0;
    T0.tv_sec=0;
    T0.tv_usec=0;
    struct timeval T1;
    T1.tv_sec=0;
    T1.tv_usec=0;
    while(true){
        gettimeofday(&T0,NULL);
        //printf("%d\n",frames++);
        if (eventhandler() == -1)return;
        SDL_FlushEvents(SDL_KEYDOWN,SDL_LASTEVENT);
        SDL_SetRenderDrawColor(ren,255,255,187,255);
        SDL_RenderClear( ren );
        SDL_SetRenderDrawColor(ren,0,0,0,255);
        for(int i=0; i<n; i++){
            move_ball(&(balls[i]));
            draw_ball_circle(ren, balls[i]);
        }
        for(int i=0; i<linenum; i++){
            SDL_RenderDrawLine(ren, lines[i].a, lines[i].b, lines[i].c, lines[i].d);
        }
        
        for(int i=0; i<n ; i++){
            clip(balls, i);
        }
        for (int i=0; i<linenum; i++){
            clip_line(balls, &lines[i]);
        }
        
        
        SDL_RenderPresent(ren);
        
        gettimeofday(&T1,NULL);
        //printf("Time spent  (%ld,%ld)\n",T1.tv_sec-T0.tv_sec, T1.tv_usec-T0.tv_usec);
    }
    
}




int main(int argc,char** argv){

    if (argc==2){
        n= atoi(argv[1]);
        printf("atoi%d\n",n);
        if (n==0)n=100;
    }
    
    if (argc==3){
        n= atoi(argv[1]);
        printf("atoi[1] number %d\n",n);
        if (n==0)n=100;
        
        nr = atoi(argv[2]);
        printf("atoi[2] radius %d\n",nr);
        if (nr==0)nr=5;
    }
    
    /*
    ball * b1= (ball*) calloc(1,sizeof(ball));
    ball * b2= NULL;
    newball(b1);
    printf("teststart \n");
    printf("b1==NULL: ");if (b1==NULL)printf("True\n");else printf("False\n");
    printf("b2==NULL: ");if (b2==NULL)printf("True\n");else printf("False\n");
    printf("b1 && b2 == NULL: ");if (b1 && b2 == NULL)printf("True\n");else printf("False\n");
    printf("b2 && b1 == NULL: ");if (b2 && b1 == NULL)printf("True\n");else printf("False\n");
    printf("b1 == NULL && b2 == NULL: ");if (b1 == NULL && b2 == NULL)printf("True\n");else printf("False\n");
    printf("(b2 && b1) == NULL: ");if ((b2 && b1) == NULL)printf("True\n");else printf("False\n");
    printf("testend \n");
    
    
    return 1;
    */
    
        if (SDL_Init(SDL_INIT_VIDEO) != 0){
	std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	return 1;
    }
    
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, w, h, SDL_WINDOW_SHOWN);
    if (win == NULL){
            std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
    }
    
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if (ren == NULL){
            SDL_DestroyWindow(win);
            std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
    
    }
    
    
    mainloop(ren);
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
        
 
    printf("done\n");
 return 0;   
}
    
