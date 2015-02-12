#include <iostream>
#include <sstream>
#include <string>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"
#include "SDL_image.h"
#include "Texty.h"
#include "Windows.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	std::ostringstream errMsg;
	errMsg << "message: " << msg << " error: " << SDL_GetError() << std::endl;
	OutputDebugString(errMsg.str().c_str());
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}



int main(int argc, char **argv){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("TextyDemo", 800, 100, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}


	const std::string resPath = getResourcePath("TextyDemo");
	Texty* texty1 = new Texty(renderer, resPath + "sample.ttf", 14, true);

	SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
	if (image == nullptr){
		cleanup(image, renderer, window);
		logSDLError(std::cout, "loadTexture");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int BGx = SCREEN_WIDTH / 2 - iW / 2;
	int BGy = SCREEN_HEIGHT / 2 - iH / 2;

	SDL_Event e;
	bool quit = false;
	while (!quit){
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_RIGHT)
					BGx += 3;
			}
		}

		//Render the scene
		SDL_RenderClear(renderer);
		renderTexture(image, renderer, BGx, BGy);

		//texty1->write("This is a demo of Texty", 300, 300);
		//texty1->write("BGx: " + std::to_string(BGx), Texty::NEXT_LINE);
		//texty1->write("  BGy: " + std::to_string(BGy) + "/n/n");
		////texty1->write("  BGy: " + std::to_string(BGy));
		////texty1->write("", Texty::NEXT_LINE);
		////texty1->write("", Texty::NEXT_LINE);
		//texty1->write("End of information", Texty::NEXT_LINE);

		//Made Texty /n aware
		texty1->write("This is a demo of Texty/nBGx: " + std::to_string(BGx) + "  BGy: " + std::to_string(BGy) + "/n/n/nEnd of information", 300, 300);

		SDL_RenderPresent(renderer);
	}

	cleanup(image, renderer, window);
	delete texty1;

	IMG_Quit();
	SDL_Quit();

	return 0;
}
