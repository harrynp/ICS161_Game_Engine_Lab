// Texty.h

#pragma once

#include <iostream>
#include <SDL.h>
#include "SDL_ttf.h"

class Texty
{

public:
	Texty(SDL_Renderer* renderer, std::string fontName, int fontSize = 10, bool visible = true);
	~Texty();

	enum writeOption_t { CONTINUE, NEXT_LINE };

	void write(std::string text, writeOption_t option = Texty::CONTINUE);

	void write(std::string text, int x, int y);


private:
	int nextX, nextY;
	int lineStartX;
	std::string fontName;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Texture* texture;
	SDL_Color color;  // always black for now
	bool error;
	bool visible;

	void writeText(std::string text);
	void yikes(std::string message); // for error conditions
};
