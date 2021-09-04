
#ifndef MAP_cpp_
#define MAP_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<queue>

const int MAP_X = 25;
const int MAP_Y = 11;
int mapB[MAP_X+1][MAP_Y+1] = {}; // ÌùÍ¼ 
int mapC[MAP_X+1][MAP_Y+1] = {}; // ±àºÅ 

void map_init() {
	for(int i = 1; i <= MAP_X; i++)
		mapB[i][1] = mapB[i][2] = mapB[i][MAP_Y] = mapB[i][MAP_Y-1] = 1;
	for(int i = 1; i <= MAP_Y; i++)
		mapB[1][i] = mapB[2][i] = mapB[MAP_X][i] = mapB[MAP_X-1][i] = 1;
	/*
	mapB[20][5] = 1;
	mapB[20][6] = 1;
	mapB[20][7] = 1;
	mapB[20][8] = 1;
	mapB[20][9] = 1;
	mapB[21][8] = 1;
	mapB[21][9] = 1;
	mapB[21][10] = 1;
	mapB[22][7] = 1;
	mapB[22][8] = 1;
	mapB[22][9] = 1;
	mapB[22][10] = 1;
	*/
	
	//mapB[10][7] = 1;
	//mapB[11][7] = 1;
	//mapB[10][8] = 1;
	//mapB[11][8] = 1;
	
	mapB[12][3] = 2;
	mapB[12][4] = 2;
	mapB[12][5] = 2;
	mapB[12][6] = 2;
	mapB[12][7] = 2;
	mapB[12][8] = 2;
	mapB[12][9] = 2;
	mapB[11][3] = 2;
	mapB[11][5] = 2;
	mapB[11][6] = 2;
	mapB[10][4] = 2;
	mapB[10][5] = 2;
	mapB[10][6] = 2;
	
	for(int i = 1; i <= MAP_X; i++)
		for(int j = 1; j <= MAP_Y; j++)
			if(mapB[i][j] == 1)
				mapC[i][j] = -1;
}

bool map_reach(int x, int y) {
	if(mapC[x][y] != 0) return false;
	if(mapB[x][y] == 1) return false;
	return true;
}

/*
mapB:
	0 light green glass
	1 stone
	2 green glass
mapC:
	-1 unpassable (stone)
	0 passable
	>0 unpassable (character)
*/

#endif

