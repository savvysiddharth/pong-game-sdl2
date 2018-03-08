#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Dot
{
public:
	static const int width=20, height=20;
	static const int dotVel=5;

	Dot();

	void handleEvent(SDL_Event& e); //JUST FOR TESTING!
	void move(SDL_Rect& p1,SDL_Rect& p2);
	void render();
	int posX,posY;
	int velX,velY;

	SDL_Rect dotCollider;
};

void Dot::handleEvent(SDL_Event& e)
{
	if(e.type == SDL_KEYDOWN && e.key.repeat==0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP: velY-=dotVel; break;
			case SDLK_DOWN: velY+=dotVel; break;
			case SDLK_RIGHT: velX+=dotVel; break;
			case SDLK_LEFT: velX-=dotVel; break;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat==0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP: velY+=dotVel; break;
			case SDLK_DOWN: velY-=dotVel; break;
			case SDLK_RIGHT: velX-=dotVel; break;
			case SDLK_LEFT: velX+=dotVel; break;
		}
	}
}

class Plank
{
public:
	static const int width=20, height=70;
	static const int plankVel=10;

	Plank(int,int);

	void handleEvent(SDL_Event& e, int p);
	void render();
	void move();
	int posX,posY;
	int velX,velY;

	SDL_Rect plankCollider;
};

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);
	void free();
	void render(int x,int y);
	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

bool init();
bool loadMedia();
void close();
bool collisonDetector(SDL_Rect a, SDL_Rect b);
bool collisonLeftRight(SDL_Rect a, SDL_Rect b);
bool collisonTopBottom(SDL_Rect a, SDL_Rect b);

SDL_Window* gWindow =NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gDotTexture;
LTexture gPlankTexture;

bool collisonLeftRight(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
    int rightA, rightB;

    leftA = a.x;
    rightA = a.x + a.w;

    leftB = b.x;
    rightB = b.x + b.w;

    if( rightA >= leftB || rightB >= leftA)
        return true;
    else
    	return false;
}

bool collisonTopBottom(SDL_Rect a, SDL_Rect b)
{
	int topA, topB;
    int bottomA, bottomB;

    topA = a.y;
    bottomA = a.y + a.h;
	
	topB = b.y;
    bottomB = b.y + b.h;

    if( bottomA >= topB || topA >= bottomB )
        return true;
	else
		return false;
}

bool collisonDetector(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if( bottomA <= topB )
        return false;

    if( topA >= bottomB )
        return false;

    if( rightA <= leftB )
        return false;

    if( leftA >= rightB )
        return false;

    return true;
}

Plank::Plank(int x,int y)
{
	posX=x;
	posY=y;
	velX=0;
	velY=0;

	plankCollider.x=x;
	plankCollider.y=y;
	plankCollider.w=width;
	plankCollider.h=height;
}

void Plank::handleEvent(SDL_Event& e, int p)
{
	if(e.type == SDL_KEYDOWN && e.key.repeat==0 && p==0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_w: velY=velY-plankVel; break;
			case SDLK_s: velY=velY+plankVel; break;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat==0 && p==0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_w: velY=velY+plankVel; break;
			case SDLK_s: velY=velY-plankVel; break;
		}
	}
	else if(e.type == SDL_KEYDOWN && e.key.repeat==0 && p==1)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_i: velY=velY-plankVel; break;
			case SDLK_k: velY=velY+plankVel; break;
		}
	}
	else if(e.type == SDL_KEYUP && e.key.repeat==0 && p==1)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_i: velY=velY+plankVel; break;
			case SDLK_k: velY=velY-plankVel; break;
		}
	}
}

void Plank::move()
{
	posY=posY+velY;
	plankCollider.y=posY;
	if(posY<=0 || posY+height >= SCREEN_HEIGHT)
	{
		posY=posY-velY;
	}
}

void Plank::render()
{
	gPlankTexture.render(posX,posY);
}

Dot::Dot()
{
	posX=50; posY=50;

	dotCollider.w=width;
	dotCollider.h=height;

	velX=-4; velY=2; //uncomment it if u wanna ball to move automatically
	//velY=0; velX=0; //developer mode
}

void Dot::move(SDL_Rect& p1,SDL_Rect& p2)
{
	posY=posY+velY;
	dotCollider.y=posY;
	posX=posX+velX;
	dotCollider.x=posX;
	

	//leftRight
	if(posX<0 || (posX+width)>SCREEN_WIDTH)
	{
		velX=-1*velX;
		//dotCollider.x=posX;
	}
	//topBottom
	else if(posY<0 || (posY+height)>SCREEN_HEIGHT)
	{
		velY=-1*velY;
		//dotCollider.y=posY;

		printf("celing floor collison!\n");
		std::cout<<"----------------------\n";	
	}
	
	if((collisonDetector(dotCollider,p1) && collisonLeftRight(dotCollider,p1)) || (collisonDetector(dotCollider,p2) && collisonLeftRight(dotCollider,p2)))
	{
		std::cout<<velX<<","<<velY<<"\n";
		velX=-1*velX;
		//dotCollider.x=posX;
		std::cout<<velX<<","<<velY<<"\n";
		printf("leftRight collison:\n");
		std::cout<<"----------------------\n";
	}
	else if((collisonDetector(dotCollider,p1) && collisonTopBottom(dotCollider,p1)) || (collisonDetector(dotCollider,p2)&&collisonTopBottom(dotCollider,p2)))
	{
		std::cout<<velX<<","<<velY<<"\n";
		velY=-1*velY;
		//dotCollider.y=posY;
		std::cout<<velX<<","<<velY<<"\n";

		printf("topBottom collison:\n");
		std::cout<<"----------------------\n";
	}

	if(posX<15 || posX>SCREEN_WIDTH-35)
	{
		posX=SCREEN_WIDTH/2;
		posY=rand()%480;
		int z = rand()%4;
		switch(z)
		{
			case 0: velX=-5;velY=-3; break;
			case 1: velX=6;velY=-4; break;
			case 2: velX=-7;velY=3; break;
			default: velX=4;velY=7; break;
		}
		printf("Out of boundry!\n");
		std::cout<<"----------------------\n";		
	}
}



void Dot::render()
{
	gDotTexture.render(posX,posY);
}


LTexture::LTexture()
{
	mTexture=NULL;
	mWidth=0;
	mHeight=0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* newTexture =NULL;

	SDL_Surface* loadedSurface=IMG_Load(path.c_str());
	if(loadedSurface==NULL)
	{
		printf("FAILED TO LOAD SURFACE\n");
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
		if(newTexture==NULL)
		{
			printf("FAILED TO CREATE texture\n");
		}
		else
		{
			mWidth=loadedSurface->w;
			mHeight=loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}

	mTexture=newTexture;
	return mTexture!=NULL;
}

void LTexture::free()
{
	if(mTexture!=NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture=NULL;
		mWidth=0;
		mHeight=0;
	}
}

void LTexture::render(int x,int y)
{
	SDL_Rect renderQuad = {x,y,mWidth,mHeight};

	SDL_RenderCopy(gRenderer,mTexture,NULL,&renderQuad);
}

int LTexture::getHeight()
{
	return mHeight;
}

int LTexture::getWidth()
{
	return mWidth;
}

//prototyping

bool init()
{
	bool success=true;

	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("failed to INITIALIZE!!\n");
		success=false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow=SDL_CreateWindow("ProjectPingPong! - DEVELOPER VERSION",50,50,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
		if(gWindow==NULL)
		{
			printf("failed to create window\n");
			success=false;
		}
		else
		{
			gRenderer=SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer==NULL)
			{
				printf("failed to create renderer\n");
				success=false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				//good
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success=true;

	if(!gDotTexture.loadFromFile("dot.bmp"))
	{
		printf("failed to load dot.bmp\n");
		success=false;
	}

	if(!gPlankTexture.loadFromFile("plank.bmp"))
	{
		printf("failed to load plank.bmp\n");
	}
	return success;
}

void close()
{
	gPlankTexture.free();
	gDotTexture.free();
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	SDL_Quit();
}

int main(int argc, char const *argv[])
{
	omp_set_dynamic(0);
	omp_set_num_threads(2);
	#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		if(tid==0)
		{
			if(!init())
			{
				printf("FAILED TO INITIALIZE\n");
			}
			else
			{
				if(!loadMedia())
				{
					printf("MEDIA LOAD FAILED\n");
				}
				else
				{
					bool quit=false;
					SDL_Event e;
					Dot dot;
					Plank plank1(20,SCREEN_HEIGHT/2-35);
					Plank plank2(SCREEN_WIDTH-40, SCREEN_HEIGHT/2-35);

					while(!quit)
					{
						while(SDL_PollEvent(&e))
						{
							if(e.type == SDL_QUIT)
							{
								quit=true;
							}
							if(e.type == SDL_KEYDOWN) //just randomizing ball origin
							{
								switch(e.key.keysym.sym)
								{
									srand(time(0)%100);
									case SDLK_r: 
										dot.posX=SCREEN_WIDTH/2;
										dot.posY=rand()%480;
										int z = rand()%4;
										if(z==0){
											dot.velX=-5;
											dot.velY=-5;}
										else if(z==1){
											dot.velX=5;
											dot.velY=-5;}
										else if(z==2){
											dot.velX=-5;
											dot.velY=5;	}
										else{
											dot.velX=5;
											dot.velY=5;}
										break;
								}
							}

							//dot.handleEvent(e); //developer mode
							plank1.handleEvent(e,0);
							plank2.handleEvent(e,1);
						}


						plank1.move();
						plank2.move();
						dot.move(plank1.plankCollider,plank2.plankCollider);

						SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
						SDL_RenderClear( gRenderer );

						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
						for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
						{
							SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
						}

						plank1.render();
						plank2.render();
						dot.render();
						SDL_RenderPresent( gRenderer );
					}

				}
			}

			close();
		}
		else if(tid==1)
		{
			printf("HELLO WORLD!!!\n");
			int x=0;
			//scanf("%d",&x);
			printf("SUPER DUPER %d \n",x);
		}
	}
	return 0;
}