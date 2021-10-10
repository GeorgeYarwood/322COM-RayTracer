// raycastingTest.cpp : Defines the entry point for the console application.
//

#include "sphere.h"
#include <iostream>
#include <algorithm>
#include <fstream>      // std::ofstream
#include <math.h>
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;

#define PI 3.14159265
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Event event;

bool initSDL(SDL_Window*& window, SDL_Surface*& screenSurface)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		//create the window
		window = SDL_CreateWindow("SDL Version", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			cout << "SDL Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			return false;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			return true;
		}
	}
};

void closeSDL(SDL_Window*& window)
{
	SDL_DestroyWindow(window);
	SDL_Quit();
};

Uint32 convertColour(vec3 colour)
{
	int tt;
	Uint8 r,g,b;
	
	tt = (int)(colour.r * 255);
	if (tt <= 255) r = tt; else r = 255;
	tt = (int) (colour.g * 255);
	if (tt <= 255) g = tt; else g = 255;
	tt = (int)(colour.b * 255);
	if (tt <= 255) b = tt; else b = 255;

	Uint32 rgb;

	//check which order SDL is storing bytes
	rgb = (r << 16) + (g << 8) + b;

	return rgb;
};

void PutPixel32_nolock(SDL_Surface*& surface, int x, int y, Uint32 colour)
{
	Uint8* pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint32));
	*((Uint32*)pixel) = colour;
};


void ComputeColorSphere(const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, float &ColValue)
{
	vec3 lightToPt, surNorm,rVec, ttvec;
	float Ca, Cd, Cs,tt; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float vecdot;
	lightToPt = normalize(sourcePt - IntPt);
	surNorm = normalize(IntPt - CenPt);
	Cd = std::max(0.0, (double)dot(lightToPt, surNorm));
	Ca = 0.2;

	//compute specular value
	vecdot = dot(surNorm, lightToPt);
	ttvec.x = surNorm.x*2.0 * vecdot;
	ttvec.y = surNorm.y*2.0 * vecdot;
	ttvec.z = surNorm.z*2.0 * vecdot;

	rVec = ttvec - lightToPt;
	tt = std::max(0.0, (double)dot(rVec, -dir));
	Cs = pow(tt,20)*0.7;

	//ColValue = Cs;
	ColValue = Ca + Cd + Cs;
}

//center is the sphere center position
//orig is the origin of the ray. You set it up as (0,0,0)
//dir is the direction of the ray
//t is the return parameter value of intersection. Range from [0,1]
bool intersectSphere(vec3 center, vec3 orig, vec3 dir, float radius, float& t)
{
	float t0, t1; // solutions for t if the ray intersects 

	// geometric solution  // vector dir has to be normalize, length is 1.0
	vec3 L = center - orig;
	float tca = dot(L, dir);
	if (tca < 0) return false;
	float d = dot(L, L) - tca * tca;
	if (d > (radius * radius)) return false;

	float thc = sqrt(radius * radius - d);
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

int main(int argc, char* args[])
{
	const int WIDTH = 640;
	const int HEIGHT = 480;
	float PixelNdx, PixelNdy, PixelRdx, PixelRdy, Iar, tanvalue, PCameraX,PCameraY;

	///SECTION - SDL Setup
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if (!initSDL(window, screenSurface)) return -1;

	//create two dimensional pxiel array for the image
	vec3 **image = new vec3*[WIDTH];
	for (int i=0; i < WIDTH; i++) image[i] = new vec3[HEIGHT];

	Iar = WIDTH / (float)HEIGHT;
	tanvalue = tanf(15.0 * PI / 180.0);  //40 degree for big field of view //15 for zoom in

	bool Intersection;
	float t, min_t,ColorVal;
	int i,whichone;
	vec3 ttvec, dir,org, mat_color, final_Color,IntPt;
	vector<float> t_arr;
	vector<vec3> color_arr;

	Sphere redsphere(4, vec3(0, 0, -20), vec3(1.00, 0.32, 0.36));
	color_arr.push_back(redsphere.getMyColor());

	Sphere yellowsphere(2, vec3(5, -1, -15), vec3(0.90, 0.76, 0.46));
	color_arr.push_back(yellowsphere.getMyColor());

	Sphere bluesphere(3, vec3(5, 0, -25), vec3(0.65, 0.77, 0.97));
	color_arr.push_back(bluesphere.getMyColor());

	Sphere greyphere(3, vec3(-5.5, 0, -15), vec3(0.90, 0.90, 0.90));
	color_arr.push_back(greyphere.getMyColor());


	///light setting
	vec3 sourcePt;
	sourcePt.x = 0.0; sourcePt.y = 20.0; sourcePt.z = 0.0;
	
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			t_arr.clear();
			color_arr.clear();

			PixelNdx = (x + 0.5) / (float)WIDTH;
			PixelNdy = (y + 0.5) / (float)HEIGHT;
			PixelRdx = 2 * PixelNdx - 1.0;
			PixelRdy = 1.0 - 2 * PixelNdy;
			PixelRdx = PixelRdx * Iar;

			PCameraX = PixelRdx * tanvalue;
			PCameraY = PixelRdy * tanvalue;

			ttvec.x = PCameraX;
			ttvec.y = PCameraY;
			ttvec.z = -1.0;
			//dir need to be normalized
			dir = normalize(ttvec);

			org.x = 0.0; org.y = 0.0; org.z = 0.0;

			Intersection = intersectSphere(redsphere.getCenter(), org, dir, redsphere.getRadius(), t);
			if (Intersection)
			{
				t_arr.push_back(t);
				color_arr.push_back(redsphere.getMyColor());
			}

			Intersection = intersectSphere(yellowsphere.getCenter(), org, dir, yellowsphere.getRadius(), t);
			if (Intersection)
			{
				t_arr.push_back(t);
				color_arr.push_back(yellowsphere.getMyColor());
			}

			Intersection = intersectSphere(bluesphere.getCenter(), org, dir, bluesphere.getRadius(), t);
			if (Intersection)
			{
				t_arr.push_back(t);
				color_arr.push_back(bluesphere.getMyColor());
			}

			Intersection = intersectSphere(greyphere.getCenter(), org, dir, greyphere.getRadius(), t);
			if (Intersection)
			{
				t_arr.push_back(t);
				color_arr.push_back(greyphere.getMyColor());
			}

			if (t_arr.size() == 0)
			{
				image[x][y].x = 1.0;
				image[x][y].y = 1.0;
				image[x][y].z = 1.0;

				PutPixel32_nolock(screenSurface, x, y, convertColour(image[x][y]));
			}
			else
			{
				min_t = 1000.0;
				whichone = 0;
				for (i = 0; i < t_arr.size(); i++)
				{
					if (t_arr[i] < min_t) { whichone = i; min_t = t_arr[i]; }
				}
				image[x][y].x = color_arr[whichone].x;
				image[x][y].y = color_arr[whichone].y;
				image[x][y].z = color_arr[whichone].z;

				PutPixel32_nolock(screenSurface, x, y, convertColour(image[x][y]));
			}
		}
	}
	

	SDL_UpdateWindowSurface(window);
	bool quit = false;
	while (!quit)
	{
		//Keyboard Input
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}


	closeSDL(window);
    return 0;
}

