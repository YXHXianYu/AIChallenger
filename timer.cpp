
#ifndef TIMER_cpp_
#define TIMER_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include"SDL2/SDL.h"

const int tick = 50,
		  tick_10 = 5;
int time_now = 0, time_last = 0;

Uint32 callback(Uint32 interval, void *param) {
	time_now++;
	return interval;
}

SDL_TimerID timer;
void timer_init() {
	timer = SDL_AddTimer(1000/tick, callback, NULL);
}

#endif

