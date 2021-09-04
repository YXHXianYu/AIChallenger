
#ifndef INPUT_cpp_
#define INPUT_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>

int kb_stage[100];
/*
10 + 26 + 4 + 10

11 ~ 20 : '1' ~ '0'
21 ~ 46 : 'A' ~ 'Z'
47 ~ 50 : up down left right
51 ~ 60 : '1' ~ '0' (numeric keypad) 
*/
int kb_input(SDL_Keycode sym, int t) {
	int va = -1;
	//printf(".\n");
	switch(sym) {
		// wasd : 43,39,21,24
		case SDLK_w: va = 43; break;
		case SDLK_s: va = 39; break;
		case SDLK_a: va = 21; break;
		case SDLK_d: va = 24; break;
		case SDLK_j: va = 30; break;
		case SDLK_k: va = 31; break;
		
		case SDLK_UP: va = 47; break;
		case SDLK_DOWN: va = 48; break;
		case SDLK_LEFT: va = 49; break;
		case SDLK_RIGHT: va = 50; break;
		case SDLK_KP_1: va = 51; break;
		case SDLK_KP_2: va = 52; break;
		
		case SDLK_l: va = 32; break;
	}
	kb_stage[va] = t;
	return va;
}


#endif

