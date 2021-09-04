
#ifndef GUI_cpp_
#define GUI_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_ttf.h"

SDL_Rect clip4[4],
		 clip10[10];

void GUI_clip_init() {
	clip4[0].w = clip4[0].h = 50;
	clip4[0].x = clip4[0].y = 0;
	clip4[1].w = clip4[1].h = 50;
	clip4[1].x = 50; clip4[1].y = 0;
	clip4[2].w = clip4[2].h = 50;
	clip4[2].x = 0; clip4[2].y = 50;
	clip4[3].w = clip4[3].h = 50;
	clip4[3].x = 50; clip4[3].y = 50;
	
	for(int i = 0; i <= 4; i++)
		clip10[i].w = clip10[i].h = clip10[i+5].w = clip10[i+5].h = 50,
		clip10[i].x = clip10[i+5].x = i*50, clip10[i].y = 0, clip10[i+5].y = 50;
}

SDL_Texture *Pborder = nullptr;
SDL_Texture *Pbackground = nullptr;
SDL_Texture *Pground = nullptr;

SDL_Texture *credit_text1 = nullptr;
SDL_Texture *credit_text2 = nullptr;
SDL_Texture *instruction_text1 = nullptr;
SDL_Texture *instruction_text2 = nullptr;
SDL_Texture *congratulation_text = nullptr;

SDL_Texture *player_name[PLAYER_MAX] = {};
SDL_Texture *player_atk_ammo_text[PLAYER_MAX] = {};
SDL_Texture *player_atk_spd_timer_text[PLAYER_MAX] = {};
SDL_Texture *player_recover_timer_text[PLAYER_MAX] = {};

// ....

void GUI_init() {
	Pborder = load_image("img\\border.png");
	Pbackground = load_image("img\\background.png");
	Pground = load_image("img\\ground_50.png");
	credit_text1 = load_text("font.ttf", "Made By YXH_XianYu", 0, 0, 0, 15);
	credit_text2 = load_text("font.ttf", "        QQ:2943003", 0, 0, 0, 15);
	instruction_text1 = load_text("font.ttf", "Operation: WASD-Move  JK-Attack", 0, 0, 0, 15);
	instruction_text2 = load_text("font.ttf", "See ReadMe.txt for more modes!", 0, 0, 0, 15);
	congratulation_text = load_text("font.ttf", "Congratulations!!!", 0, 0, 0, 70);
	GUI_clip_init();
}

void GUI_quit() {
	SDL_DestroyTexture(Pborder);
	SDL_DestroyTexture(Pbackground);
	SDL_DestroyTexture(Pground);
}

#endif
