#include "Texty.h"
#include <string>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

void renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture2(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture2(tex, ren, dst, clip);
}


Texty::Texty(SDL_Renderer* renderer, std::string fontName, int fontSize, bool visible){
	this->renderer = renderer;
	this->fontName = fontName;
	this->font = TTF_OpenFont(fontName.c_str(), fontSize);
	if (this->font == nullptr){
		std::cout << "Error Opening TTF_Font" << std::endl;
		delete this;
	}
	this->visible = visible;
	visible ? this->color = { 0, 0, 0, 255 } : this->color = { 0, 0, 0, 0 };
}

Texty::~Texty(){
	TTF_CloseFont(font);
}

void Texty::writeText(std::string text){
	int w;
	TTF_SizeText(font, text.c_str(), &w, nullptr);
	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), color);
	texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	renderTexture2(texture, renderer, nextX, nextY);
	nextX += w;
}

void Texty::write(std::string text, writeOption_t option){
	if(option == Texty::NEXT_LINE){
		nextX = lineStartX;
		nextY += TTF_FontHeight(font);
	}
	//Checks if there is a /n or if given an empty string
	if (text != "" && text.find("/n") == std::string::npos){
		writeText(text);
	}
	//Handles /n char by splitting the string into a vector of substrings and then writing each one in the vector one by one
	else if(text != ""){
		std::vector<std::string> lines;
		std::string current = text;
		while (current.find("/n") != std::string::npos){
			unsigned int index = current.find("/n");
			lines.push_back(current.substr(0, index));
			lines.push_back(current.substr(index, 2));
			current = current.substr(index + 2);
		}
		for (unsigned int i = 0; i < lines.size(); ++i){
			lines[i] == "/n" ? write("", Texty::NEXT_LINE) : write(lines[i]);
		}
		write(current);
	}
}	

void Texty::write(std::string text, int x, int y){
	lineStartX = x;
	nextX = x;
	nextY = y;
	//Checks if there is a /n or if given an empty string
	if (text != "" && text.find("/n") == std::string::npos){
		writeText(text);
	}
	//Handles /n char by splitting the string into a vector of substrings and then writing each one in the vector one by one
	else if(text != ""){
		std::vector<std::string> lines;
		std::string current = text;
		while (current.find("/n") != std::string::npos){
			unsigned int index = current.find("/n");
			lines.push_back(current.substr(0, index));
			lines.push_back(current.substr(index, 2));
			current = current.substr(index + 2);
		}
		for (unsigned int i = 0; i < lines.size(); ++i){
			lines[i] == "/n" ? write("", Texty::NEXT_LINE) : write(lines[i]);
		}
		write(current);
	}
}

void Texty::yikes(std::string message){
	std::cout << "Error: " << message << std::endl;
}