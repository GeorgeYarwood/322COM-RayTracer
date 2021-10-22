#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include <iostream>
#include <algorithm>
#include <fstream>     
#include <math.h>
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>
#include "light.h"
#include "rayHit.h"
#include "OBJloader.h"
#include "AABB.h";
using namespace std;
using namespace glm;


//SDL window and renderer
SDL_Window* sdlWindow;
SDL_Renderer* sdlRenderer;

//Event manager
SDL_Event event;

//Rendering width/height/FOV
int width = 1280;
int height = 720;
float fov = 110;


int pitch;
bool quit = false;


//Converts colours from RGB vec3 to a Uint for SDL
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

//Converts SDL Uint colour to RGB vec3
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


void drawPixel(Uint32*& pixels, int x, int y, Uint32 colour)
{
	//Set the specified pixel to a certain colour
	pixels[y * width + x] = colour;
}

bool shadowCalc(vec3 lightSrc,vec3 dir, vec3 IntPoint, shape *currShape, rayHit &shadowHit)
{
	if(!currShape->inShadow)
	{
		return false;
	}
	//Calculate ray direction
	vec3 l =  lightSrc - IntPoint;
	vec3 normal = (normalize(IntPoint - currShape->currPos));
	l = normalize(l);
	vec3 orig = IntPoint + normal * 0.0001f;
	//If we hit an object that should be in shadow
	if(currShape->intersection(orig, l, shadowHit))
	{
		//Make it so
		return true;
	}
	return false;
}

bool reflectCalc(vec3 lightSrc,vec3 dir, vec3 IntPoint, shape* currShape, rayHit& reflectHit)
{

	vec3 normal = (normalize(IntPoint - currShape->currPos));

	//Calculate ray direction
	vec3 l = dir - 2 * dot(normalize(dir), normalize(normal)) * normal;
	vec3 orig = IntPoint + normal * 0.0001f;
	if (currShape->intersection(orig, l, reflectHit))
	{
		
		//Make it so
		return true;
	}

	return false;
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
		//memset(pixels, 155, width * height * sizeof(Uint32));


		vector<light*>lights;
		//Vector of shapes we iterate through
		vector<shape*>shapes;


		//Instance of sphere		//Pos		Radius		//Colour   //D  S  intensity
		Sphere redSphere = Sphere(vec3(-2, 1, -10), 0.5, vec3(255, 0, 0), 0, 1.2);
		Sphere greenSphere = Sphere(vec3(2, 0, -10), 0.5, vec3(0, 255, 0), 0, 1.2);
		Sphere blueSphere = Sphere(vec3(-3, -0.1, -10), 0.5, vec3(0, 0, 255), 0, 1.2);
		Sphere yellowSphere = Sphere(vec3(6, 3, -25), 2, vec3(255, 255, 0), 0, 1.2);

		//Instance of plane			//Col		//Point on plane	//Normal
		plane testPlane = plane(vec3(10, 100, 0), vec3(0, -1, 0), vec3(0, 1, 0));
		
		//Triangle
		triangle testTriangle = triangle(vec3(1, 0.2, -20), vec3(0, 1, -2), vec3(-1.9, -1, -2), vec3(1.6, -0.5, -2), vec3(255, 0, 0), vec3(0, 255, 0), vec3(0, 0, 255), vec3(0.0, 0.6, 1.0), vec3(-0.4, -0.4, 1.0), vec3(0.4, -0.4, 1), 0, 1);

		vector<VertexWithAll> mesh = loadOBJ("Cube.obj");

		vector<triangle> trisToLoad;

		for(int m = 0; m< mesh.size(); m+=3)
		{
			
			triangle tr = triangle(vec3(-0.5, -0.1, -5), mesh[m].position, mesh[m+1].position, mesh[m+2].position, vec3(255, 0, 0), vec3(0, 255, 0), vec3(0, 0, 255), mesh[m].normal, mesh[m+1].normal, mesh[m+2].normal, 1, 1);
			//Disable shadows cause they break on loaded models
			tr.inShadow = false;
			trisToLoad.push_back(tr);	
			
		}
		
		for(int t = 0; t<trisToLoad.size(); t++)
		{
			shapes.push_back(&trisToLoad[t]);
		}

		//Add them into our vector
		shapes.push_back(&redSphere);
		shapes.push_back(&greenSphere);
		shapes.push_back(&yellowSphere);
		shapes.push_back(&blueSphere);
		shapes.push_back(&testPlane);
		shapes.push_back(&testTriangle);
		

		cout << shapes.size();

		//Where we store our image
		vec3** framebuffer = new vec3 * [width];
		for (int i = 0; i < width; i++) framebuffer[i] = new vec3[height];

		//Calculate ray origin
		vec3 rayOrigin = vec3(0, 0, 0);
			 
		//Used to transform the camera
		vec3 camVec;

		///light setting

		light light1(vec3(0.1, 0.1, 0.1), vec3(0, 20, 0));
		light light2(vec3(0.1, 0.1, 0.1), vec3(-2, 15, 0));


		lights.push_back(&light1);
		//lights.push_back(&light2);

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
					ScreenX = ScreenX * width / (float)height;
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
					rayHit shadowHit;
					rayHit reflectHit;
					vec3 colVal;
					

					bool inShadow = false;

					//For every shape in the vector
					for(int currShape = 0; currShape < shapes.size(); currShape++)
					{
						//Run intersection check
						if (shapes[currShape]->intersection(rayOrigin, rayDir, hit))
						{
							for (int j = 0; j < shapes.size(); j++)
							{
								if (currShape == j) continue;

								//For each light in the scene
								for(int l =0; l<lights.size(); l++)
								{
									//Shadow calculation
									if (shadowCalc(lights[l]->currPos, rayDir, hit.intersectPoint, shapes[j], shadowHit))
									{
										//saved_rayDists.push_back(shadowHit.rayDist);
										//saved_colour.push_back(vec3(0, 0, 0));
										inShadow = true;
										break;
									}
								}
							}

							if (!inShadow)
							{
								//For each light in the scene
								for (int l = 0; l < lights.size(); l++)
								{
									//If we hit, save our ray
									saved_rayDists.push_back(hit.rayDist);
									hit.ambientCol = shapes[currShape]->currColour;
									//Calculate colour and push onto vector for later
									//Base colour
									colVal = shapes[currShape]->ComputeColour(lights[l]->currIntensity, lights[l]->currPos, hit.intersectPoint, rayDir);
									saved_colour.push_back(colVal);
								}
							}
						}
					}

					if (inShadow)
					{
					
						framebuffer[x][y].x = 0.0;
						framebuffer[x][y].y = 0.0;
						framebuffer[x][y].z = 0.0;


						drawPixel(pixels, x, y, convertColour(framebuffer[x][y]));

					}
					//If we don't hit anything, draw default colours
					else if (saved_rayDists.size() == 0)
					{
						framebuffer[x][y].x = 0.1;
						framebuffer[x][y].y = 0.1;
						framebuffer[x][y].z = 1;


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

					//if ((int)sqrt((y - redSphere.currRad) * (y - redSphere.currRad) + (x - redSphere.currRad) * (x - redSphere.currRad)) < redSphere.currRad)
					//{
					//	pixels[y * width + x] =2988;
					//}

				}
			}
			//Poll exit button
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					quit = true;
				}
				else if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_RIGHT:
							rayOrigin.x += 1;
							break;
						case SDLK_LEFT:
							rayOrigin.x -= 1;
							break;
						case SDLK_UP:
							rayOrigin.z -= 1;
							break;
						case SDLK_DOWN:
							rayOrigin.z += 1;
							break;
					}
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






