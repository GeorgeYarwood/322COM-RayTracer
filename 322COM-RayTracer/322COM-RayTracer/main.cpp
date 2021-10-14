#include "sphere.h"
#include <iostream>
#include <algorithm>
#include <fstream>     
#include <math.h>
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>
#include "light.h"
using namespace std;
using namespace glm;


//SDL window and renderer
SDL_Window* sdlWindow;
SDL_Renderer* sdlRenderer;


SDL_Event event;

//Rendering width/height/FOV
int width = 640;
int height = 480;
float fov = 60;


int pitch;

bool quit = false;




bool sphereIntersection(vec3 centre, vec3& orig, vec3& dir, float radius, float& t)
{
	float t0, t1;
	// geometric solution  // vector dir has to be normalize, length is 1.0
	vec3 L = centre - orig;
	float tca = dot(L, dir);
	if (tca < 0) return false;
	float rayDir = dot(L, L) - tca * tca;
	if (rayDir > (radius * radius)) return false;

	float thc = sqrt(radius * radius - rayDir);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	t = t0;
	return true;
}


Uint32 convertColour(vec3 colour)
{
	int tt;
	Uint8 r, g, b;

	tt = (int)(colour.r * 255);
	if (tt <= 255) r = tt; else r = 255;
	tt = (int)(colour.g * 255);
	if (tt <= 255) g = tt; else g = 255;
	tt = (int)(colour.b * 255);
	if (tt <= 255) b = tt; else b = 255;

	Uint32 rgb;

	//check which order SDL is storing bytes
	rgb = (r << 16) + (g << 8) + b;

	return rgb;
};


void drawPixel(Uint32* &pixels, int x, int y, Uint32 colour)
{

	pixels[y * width + x] = colour;

}

int main(int argc, char* argv[])
{
	cout << "Initialzing SDL...";

	//Init SDL with video mode
	if ((SDL_Init(SDL_INIT_VIDEO) < 0))
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
		Sphere redSphere = Sphere(vec3(0, 0, -10), 1, vec3(255, 0, 0));
		Sphere greenSphere = Sphere(vec3(-2, 0, -20), 1, vec3(0, 255, 0));
		Sphere blueSphere = Sphere(vec3(0, 0, -5), 0.2, vec3(0, 0, 255));

		//Calculate image aspect ratio
		float imageAspect = width / (float)height;

		vec3** framebuffer = new vec3 * [width];
		for (int i = 0; i < width; i++) framebuffer[i] = new vec3[height];

		//Calculate ray origin
		vec3 rayOrigin = vec3(0, 0, 0);

		//Used to transform the camera
		vec3 camVec;

		///light setting
		vec3 lightSrc;
		lightSrc.x = 0.0; lightSrc.y = 20.0; lightSrc.z = 0.0;

		vector<float> obj_arr;
		vector<vec3> color_arr;

		


		


		
		while (!quit)
		{
			//Update texture with the pixel array
			SDL_UpdateTexture(drawTex, NULL, pixels, width * sizeof(uint32));
			SDL_WaitEvent(&event);
			//Rendering loop for each pixel
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{


					obj_arr.clear();
					color_arr.clear();
					//Convert to NDC space
					float NDCx = (x + 0.5) / width;
					float NDCy = (y + 0.5) / height;
					//Convert to screen space

					float ScreenX = 2 * NDCx - 1;


					ScreenX = ScreenX * width/(float)height;
					float ScreenY = 1 - 2 * NDCy;

					//Convert to camera space
					float camSpaceX = ScreenX * tan(fov / M_PI / 180);
					float camSpaceY = ScreenY * tan(fov / M_PI / 180);

					//Ray direction
					vec3 rayDir;
					camVec.x = camSpaceX;
					camVec.y = camSpaceY;
					camVec.z = -1.0; //Depth


					rayDir = normalize(camVec);

					float rayDist;

					if (sphereIntersection(redSphere.getPos(), rayOrigin, rayDir, redSphere.getRad(), rayDist))
					{
						//Check distance of ray from camera and save colour for later
						obj_arr.push_back(rayDist);
						color_arr.push_back(redSphere.getColour());
					}

					if (sphereIntersection(blueSphere.getPos(), rayOrigin, rayDir, blueSphere.getRad(), rayDist))
					{
						obj_arr.push_back(rayDist);
						color_arr.push_back(blueSphere.getColour());
					}
					if (sphereIntersection(greenSphere.getPos(), rayOrigin, rayDir, greenSphere.getRad(), rayDist))
					{
						obj_arr.push_back(rayDist);
						color_arr.push_back(greenSphere.getColour());
					}


					if (obj_arr.size() == 0)
					{
						framebuffer[x][y].x = 1.0;
						framebuffer[x][y].y = 1.0;
						framebuffer[x][y].z = 1.0;
						
						drawPixel(pixels, x, y, convertColour(framebuffer[x][y]));

					}
					else
					{
						float minRayDist = 1000;

						int whichone = 0;
						for (int i = 0; i < obj_arr.size(); i++)
						{
							if (obj_arr[i] < minRayDist) { whichone = i; minRayDist = obj_arr[i]; }
						}
						framebuffer[x][y].x = color_arr[whichone].x;
						framebuffer[x][y].y = color_arr[whichone].y;
						framebuffer[x][y].z = color_arr[whichone].z;

						drawPixel(pixels, x, y, convertColour(framebuffer[x][y]));

					}
					//Test raster method

					/*if ((int)sqrt((y - redSphere.currRad) * (y - redSphere.currRad) + (x - redSphere.currRad) * (x - redSphere.currRad)) < redSphere.currRad)
					{
						 =2988;
					}*/
					

				}


				
				

			}
			//Poll exit button
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Clear the renderer
			SDL_RenderClear(sdlRenderer);
			//Copy texture to renderer
			SDL_RenderCopy(sdlRenderer, drawTex, NULL, NULL);
			//Render it
			SDL_RenderPresent(sdlRenderer);
		}
		
		delete[] pixels;

		delete[] framebuffer;

		

		//Destroy window
		SDL_DestroyWindow(sdlWindow);
		//Destroy texture
		SDL_DestroyTexture(drawTex);

		SDL_Quit();



		return 0;
	}

	
}






