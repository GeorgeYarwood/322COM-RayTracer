#include "sphere.h"
#include <iostream>
#include <algorithm>
#include <fstream>     
#include <math.h>
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>
#include "light.h"
#include "rayHit.h"
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




bool sphereIntersection(vec3 centre, vec3& orig, vec3& dir, float radius, rayHit& hit)
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


	hit.intersectPoint = orig + rayDir * t0;
	
	//ray origin 
	// 	   +
	//ray direction
	//*
		//t2
	hit.rayDist = t0;
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

vec3 convertColourToVec(Uint32 colour)
{
	
	vec3 rgb;
	
	Uint8 r;
	Uint8 g;
	Uint8 b;

	//Get pixel format from SDL wimdow
	Uint32 format = SDL_GetWindowPixelFormat(sdlWindow);
	SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

	SDL_GetRGB(colour, mappingFormat, &r, &g, &b);

	rgb.r = (int)r;
	rgb.g = (int)g;
	rgb.b = (int)b;

	return rgb;
};





void drawPixel(Uint32* &pixels, int x, int y, Uint32 colour)
{

	pixels[y * width + x] = colour;

}

void ComputeColorSphere(float Ca, float Cd, const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, float& ColValue)
{
	vec3 lightToPt, surNorm, rVec, ttvec;
	float Cs, tt; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float vecdot;
	lightToPt = normalize(sourcePt - IntPt);
	surNorm = normalize(IntPt - CenPt);
	Cd = std::max(0.0, (double)dot(lightToPt, surNorm));
	//Ca = 0.2;

	//compute specular value
	vecdot = dot(surNorm, lightToPt);
	ttvec.x = surNorm.x * 2.0 * vecdot;
	ttvec.y = surNorm.y * 2.0 * vecdot;
	ttvec.z = surNorm.z * 2.0 * vecdot;

	rVec = ttvec - lightToPt;
	tt = std::max(0.0, (double)dot(rVec, -dir));
	Cs = pow(tt, 20) * 0.7;

	//ColValue = Cs;
	ColValue = Ca + Cd + Cs;
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
		memset(pixels, 155, width * height * sizeof(Uint32));

		//Instance of sphere
		Sphere redSphere = Sphere(vec3(0, 0, -10), 0.5, vec3(255, 0, 0));
		Sphere greenSphere = Sphere(vec3(2, -1, -20), 1, vec3(0, 255, 0));
		Sphere blueSphere = Sphere(vec3(-2, 0, -35), 3, vec3(0, 0, 255));
		Sphere yellowSphere = Sphere(vec3(0, -37, -100), 40, vec3(255, 69, 0));

		//Vector 
		vec3** framebuffer = new vec3 * [width];
		for (int i = 0; i < width; i++) framebuffer[i] = new vec3[height];

		//Calculate ray origin
		vec3 rayOrigin = vec3(0, 0, 0);

		//Used to transform the camera
		vec3 camVec;

		///light setting
		vec3 lightSrc;
		lightSrc.x = 1.0; lightSrc.y = 3.0; lightSrc.z = -1.0;

		vector<float> saved_rayDists;
		vector<vec3> saved_colour;

		
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
					saved_rayDists.clear();
					saved_colour.clear();

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

					//Normalize camera vector
					rayDir = normalize(camVec);

					//Written to and saved with each intersection check
					rayHit hit;
					float colVal;

					if (sphereIntersection(redSphere.getPos(), rayOrigin, rayDir, redSphere.getRad(), hit))
					{
						//Check distance of ray from camera and save colour for later
						saved_rayDists.push_back(hit.rayDist);
						hit.ambientCol = convertColour(redSphere.getColour());
						hit.diffuseCol = 0.7;
						ComputeColorSphere(hit.ambientCol, hit.diffuseCol,lightSrc, redSphere.getPos(), hit.intersectPoint, rayDir, colVal);
						saved_colour.push_back(convertColourToVec(colVal));
					}
					if (sphereIntersection(blueSphere.getPos(), rayOrigin, rayDir, blueSphere.getRad(), hit))
					{
						//Check distance of ray from camera and save colour for later
						saved_rayDists.push_back(hit.rayDist);
						hit.ambientCol = convertColour(blueSphere.getColour());
						hit.diffuseCol = 0.7;
						ComputeColorSphere(hit.ambientCol, hit.diffuseCol, lightSrc, blueSphere.getPos(), hit.intersectPoint, rayDir, colVal);
						saved_colour.push_back(convertColourToVec(colVal));
					}
					if (sphereIntersection(greenSphere.getPos(), rayOrigin, rayDir, greenSphere.getRad(), hit))
					{
						//Check distance of ray from camera and save colour for later
						saved_rayDists.push_back(hit.rayDist);
						hit.ambientCol = convertColour(greenSphere.getColour());
						hit.diffuseCol = 0.7;
						ComputeColorSphere(hit.ambientCol, hit.diffuseCol, lightSrc, greenSphere.getPos(), hit.intersectPoint, rayDir, colVal);
						saved_colour.push_back(convertColourToVec(colVal));
					}

					

					//If we don't hit anything, draw default colours
					if (saved_rayDists.size() == 0)
					{
						framebuffer[x][y].x = 0.0;
						framebuffer[x][y].y = 0.0;
						framebuffer[x][y].z = 0.0;
						

						drawPixel(pixels, x, y, convertColour(framebuffer[x][y]));

					}
					else
					{
						float minRayDist = 1000;

						//Sort through hit objects and decide which ones to draw first
						int whichone = 0;
						for (int i = 0; i < saved_rayDists.size(); i++)
						{
							if (saved_rayDists[i] < minRayDist) { whichone = i; minRayDist = saved_rayDists[i]; }
						}
						framebuffer[x][y].x = saved_colour[whichone].x;
						framebuffer[x][y].y = saved_colour[whichone].y;
						framebuffer[x][y].z = saved_colour[whichone].z;

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






