#include <iostream>
#include <stdio.h>

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <cmath>//for sqrt
#include <sys/time.h>

int n = 2000 ; 
int nr = 1; 
int w = 640;//breite
int h = 480;//hohe

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
    int r; 
    float c[2]; 
    float v[2]; 
    int m; 
}ball; 

float dot(float x[], float y[]){
    return (x[0]*y[0] + x[1]*y[1]);
}

float* sub(float x[], float y[], float z[]){
    z[0] = x[0] - y[0];
    z[1] = x[1] - y[1];
    return z;
}
float* add(float x[], float y[], float z[]){
    z[0] = x[0] + y[0];
    z[1] = x[1] + y[1];
    return z;
}
float norm(float x[]){
    return std::sqrt(x[0]*x[0] + x[1]*x[1]);
}
float ranflo(int start, int stop){
    //int start=-2;
    //int stop=2;
    int range = stop - start;
    int res=100;
    float x= (float)( (rand()%(res*range ) +res*start  ));
    x= x/res;
    return x;
}


void newball(ball* b){
    //ball * b = (ball *) calloc(1,sizeof(ball));
    b->r=nr;
    b->m=1;
    
    b->c[0]=ranflo(0, w);
    b->c[1]=ranflo(0, h);
    
    b->v[0]=ranflo(-2,2);
    b->v[1]=ranflo(-2,2);
    
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
    //the fuck?
    //wieso kann eine variable den namen seines typen haben?
    
    SDL_RenderDrawLine(ren, ball.c[0] + ball.r, ball.c[1], ball.c[0] - ball.r, ball.c[1]);
    SDL_RenderDrawLine(ren, ball.c[0], ball.c[1] + ball.r, ball.c[0], ball.c[1] - ball.r);
    //printf("draw ball\n");
    
    }


void clip(ball * balls, int i){
    ball b1 = balls[i];
    int ii=i;
    float z[2];
    i++;
    
    float temp1[2];
    float temp2[2];
    float vemp1[2];
    float k;
    ball b2;
    for(; i<n; i++){
        b2=balls[i];
        sub(b1.c, b2.c, z);
        if( norm(z) < b1.r + b2.r){
            vemp1[0]=b1.v[0];
            vemp1[1]=b1.v[1];
            //v1
            sub(b1.v, b2.v , temp1);
            
            sub(b1.c, b2.c , temp2);

            k = dot(temp1, temp2);
            
            k = k / (norm(temp2) * norm(temp2));
            
            k = k * ( (2 * b2.m) / (b1.m + b2.m) );
            
            temp2[0] = temp2[0] * k;
            temp2[1] = temp2[1] * k;
            
            sub(b1.v, temp2, vemp1);
            
            //v1
            
            
            //v2
            sub(b2.v, b1.v, temp1);
            
            sub(b2.c, b1.c, temp2);
            
            k = dot(temp2, temp1);
            k = k / (norm(temp2) * norm(temp2));
            
            k = k * ( (2 * b1.m) / (b2.m + b1.m) );
            
            temp2[0] = temp2[0] * k;
            temp2[1] = temp2[1] * k;
            
            b1.v[0]= vemp1[0];
            b1.v[1]= vemp1[1];
            sub(b2.v, temp2, vemp1);
            b2.v[0]= vemp1[0];
            b2.v[1]= vemp1[1];
            //v2
            balls[i]=b2;
            
            
            //untangle:
            
            /*
            
            
            
            
            
            */
            
            //intersect = (self.r+i.r) - np.linalg.norm(i.c - self.c)
            float intersect;
            sub(b2.c,b1.c,temp1);
            intersect = b1.r+b2.r - norm(temp1) ;
            
            //touch=self.c-i.c
            float touch[2];
            sub(b1.c, b2.c, touch);
            
            //touch = touch/np.linalg.norm(touch)
            k=norm(touch);
            touch[0] = touch[0]/k;
            touch[1] = touch[1]/k;
            
            //touch=touch*intersect/2
            touch[0] = ( touch[0] * intersect ) / 2;
            touch[1] = ( touch[1] * intersect ) / 2;
            
            //self.c+=touch
            //i.c-=touch
            b1.c[0]+=touch[0];
            b1.c[1]+=touch[1];
            b2.c[0]-=touch[0];
            b2.c[1]-=touch[1];
            
            
            
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
        
    //setup
    
    
    if (1==2){
        balls[0].r=30;
        balls[0].m=1000000000;
        balls[0].v[0]=0.0;
        balls[0].v[1]=0.0;
        
        balls[0].c[0]=300.0;
        balls[0].c[1]=200.0;
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
            draw_ball(ren, balls[i]);
        }
        
        for(int i=0; i<n ; i++){
            clip(balls, i);
        }
        
        
        SDL_RenderPresent(ren);
        
        gettimeofday(&T1,NULL);
        printf("Time spent  (%ld,%ld)\n",T1.tv_sec-T0.tv_sec, T1.tv_usec-T0.tv_usec);
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
    
