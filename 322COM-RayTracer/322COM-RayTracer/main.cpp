#include "sphere.h"
#include <iostream>
#include <algorithm>
#include <fstream>     
#include <math.h>
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;


//SDL window and renderer
SDL_Window* sdlWindow;
SDL_Renderer* sdlRenderer;


SDL_Event event;

//Rendering width/height
int width = 640;
int height = 480;

int pitch;

bool quit = false;

int main(int argc, char* argv[])
{
	cout << "Initialzing SDL...";

	//Init SDL with video mode
	if((SDL_Init(SDL_INIT_VIDEO) < 0))
	{
		cout << ("Error initializing SDL! \n", SDL_GetError());
		exit(-1);
	}
	else
	{

		cout << "\nSDL init succesful";
		sdlWindow = SDL_CreateWindow("322Com Ray Tracer",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

		sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);


		//Create new texture to modify with 32 bit colour and streaming access (On GPU mem)
		SDL_Texture* drawTex = SDL_CreateTexture(sdlRenderer, 
			SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

		//Pixel array we will write to
		Uint32* pixels = new Uint32[width * height];

		//Set background colour to white
		memset(pixels, 255, width * height * sizeof(Uint32));

		//Instance of sphere
		Sphere redSphere = Sphere(vec3(1, 10, 0), 100, vec3(0, 0, 255));


		while(!quit)
		{
			
			SDL_UpdateTexture(drawTex, NULL, pixels, 640 * sizeof(uint32));
			SDL_WaitEvent(&event);

			double i, angle, x1, y1;

			
			//Rendering loop for each pixel
			for(int y = 0; y <height; y++)
			{
				for(int x =0; x<width; x++)
				{

					
					
					if((int)sqrt((y - redSphere.currRad) * (y - redSphere.currRad) + (x - redSphere.currRad) * (x - redSphere.currRad)) < redSphere.currRad)
					{
						pixels[y * width + x] = 900;
					}
					

				}
			}

			

			switch(event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
			}
			//Clear the renderer
			SDL_RenderClear(sdlRenderer);
			//Copy texture to renderer
			SDL_RenderCopy(sdlRenderer, drawTex, NULL, NULL);
			//Render it
			SDL_RenderPresent(sdlRenderer);

		}
		

		delete[] pixels;

		//Destroy window
		SDL_DestroyWindow(sdlWindow);
		//Destroy texture
		SDL_DestroyTexture(drawTex);

		SDL_Quit();

		return 0;
	}
}


