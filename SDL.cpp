
#ifndef SDL_cpp_
#define SDL_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_ttf.h"

// window mode : 0 normal; 1 vast; 2 special;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

const int WINDOW_SHOW_X = 400;
const int WINDOW_SHOW_Y = 200;
const char WINDOW_TITLE[50] = "AI Challenger !";

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

int SDL_init() {
	if(WINDOW_MODE == -1) {
		WINDOW_WIDTH = 1280;
		WINDOW_HEIGHT = 720;
		cout << "Error (SDL.cpp - SDL_init) : Window shouldn't be opened." << endl;
	}
	else if(WINDOW_MODE == 0) {
		WINDOW_WIDTH = 1280;
		WINDOW_HEIGHT = 720;
	}
	else if(WINDOW_MODE == 1) { // 这玩意没写完，没贴图，会错乱 
		WINDOW_WIDTH = 1500;
		WINDOW_HEIGHT = 750;
	}
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		//cout << "Error (SDL.cpp - SDL_init) : " << SDL_GetError() << endl;
		return 1;
	}
	if(TTF_Init() == -1) {
		//cout << "Error (SDL.cpp - SDL_init) : " << TTF_GetError() << endl;
		return 1;
	}
	window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_SHOW_X, WINDOW_SHOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == nullptr) {
		//cout << "Error (SDL.cpp - SDL_init) : " << SDL_GetError() << endl;
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == nullptr) {
		//cout << "Error (SDL.cpp - SDL_init) : " << SDL_GetError() << endl;
		return 1;
	}
	return 0;
}

void SDL_quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

SDL_Texture *load_image(std::string path) {
	SDL_Texture *tex = IMG_LoadTexture(renderer, path.c_str());
	if(tex == nullptr) {
		//cout << "Error (SDL.cpp - load_image) : " << IMG_GetError() << endl;
		return nullptr; 
	}
	return tex;
}

SDL_Texture *load_text(std::string path, std::string message, Uint8 R, Uint8 G, Uint8 B, int font_size) {
	TTF_Font *font = TTF_OpenFont(path.c_str(), font_size);
	if(font == nullptr) {
		//cout << "Error (SDL.cpp - load_text) : " << TTF_GetError() << endl;
		return nullptr;
	}
	SDL_Color color = {R, G, B};
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	TTF_CloseFont(font);
	if(surf == nullptr) {
		//cout << "Error (SDL.cpp - load_text) : " << TTF_GetError() << endl;
		return nullptr;
	}
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surf);
	if(text == nullptr) {
		//cout << "Error (SDL.cpp - load_text) : " << SDL_GetError() << endl;
	}
	SDL_FreeSurface(surf);
	return text;
}

void render_texture(SDL_Texture *tex, int x, int y) {
	SDL_Rect pos;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	pos.x = (x != -1)?(x):((WINDOW_WIDTH - pos.w) / 2);
	pos.y = (y != -1)?(y):((WINDOW_HEIGHT - pos.h) / 2);
	SDL_RenderCopy(renderer, tex, NULL, &pos);
}

void render_texture(SDL_Texture *tex, int x, int y, SDL_Rect *clip = nullptr) {
	SDL_Rect pos;
	if(clip == nullptr)
		SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	else {
		pos.w = clip->w;
		pos.h = clip->h;
	}
	pos.x = (x != -1)?(x):((WINDOW_WIDTH - pos.w) / 2);
	pos.y = (y != -1)?(y):((WINDOW_HEIGHT - pos.h) / 2);
	SDL_RenderCopy(renderer, tex, clip, &pos);
}

#endif

