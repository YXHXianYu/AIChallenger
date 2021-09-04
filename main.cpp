#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<ctime>
#include<queue>
#include<map>
#include"SDL2/SDL.h"
#include"SDL2/SDL_image.h"
#include"SDL2/SDL_ttf.h"

using std::cout;
using std::endl;
using std::string;
using std::queue;
using std::map;

const int PLAYER_MAX = 4+1; // 玩家数量限制 
const int CHA_MAX = 110;    // 角色数量限制（玩家+怪物） 
const int PRO_MAX = 110;    // 子弹数量限制 
const int PAR_MAX = 110;    // 动画数量限制 
const int TEAM_MAX = 10;    // 队伍数量限制 
const int mx[4] = {0, 0, -1, 1},
		  my[4] = {-1, 1, 0, 0};
int WINDOW_MODE = 0;
/* AI TRAINING*/
int ai_training_mode = 0;
int ai_training_times = 0;
int ai_training_times_cur = 0;
int ai_training_kill[1000] = {}; 

struct Character;
struct Projectile;
struct Particle;

queue<Character> cha_list;
queue<Projectile> pro_list;
queue<Particle> par_list;
queue<Character> cha_list_;
queue<Projectile> pro_list_;
queue<Particle> par_list_;
//queue<string> message_box;
map<string, int> mem_all;

#include"SDL.cpp"
#include"GUI.cpp"
#include"timer.cpp"
#include"map.cpp"
#include"unit.cpp"
#include"input.cpp"
#include"AI.cpp"
#include"yourAI.cpp"

void init() {
	if(ai_training_mode == 0) if(SDL_init()) {exit(1);}
	if(ai_training_mode == 0) GUI_init();
	map_init();
	if(ai_training_mode == 0) timer_init();	
}

void quit() {
	if(ai_training_mode == 0) GUI_quit();
	if(ai_training_mode == 0) SDL_quit();
}

/* basic var */
int game_state; // 1正在进行游戏 2游戏通关 3游戏失败 
int dmg_list[CHA_MAX] = {};
int map_x,
	map_y;
/* player mode etc */
int play_mode = 0,
	player_sum = 0,
	player_type[PLAYER_MAX] = {},
	player_number[PLAYER_MAX] = {};
/* monster var */
int wave = 1, monster_sum = 0;
bool bo_pass = 0; 
int monster_difficulty = 0;

/* play mode & birth character */ 
// 初始化人物
void birth_player() {
	if(player_type[1] != 0) {
		if(play_mode == 4 || play_mode == 5 || play_mode == 7)
			player_number[1] = p1.init(player_type[1], 13, 6, PLAYER1_TYPE, 1, ai2::AI); 
		else
			player_number[1] = p1.init(player_type[1], 13, 6, PLAYER1_TYPE, 1, player1_control);
		character_team[p1.number] = 1;
		team_cha_number[1]++;
if(ai_training_mode == 0) player_name[1] = load_text("font.ttf", "Player1", 0, 0, 0, 20);
		cha_list.push(p1);
	}
	if(player_type[2] != 0) {
		if(play_mode == 5)
			player_number[2] = p2.init(player_type[2], 13, 8, PLAYER2_TYPE, 1, ai2::AI);
		else
			player_number[2] = p2.init(player_type[2], 13, 8, PLAYER2_TYPE, 1, player2_control);
		character_team[p2.number] = 1;
		team_cha_number[1]++;
if(ai_training_mode == 0) player_name[2] = load_text("font.ttf", "Player2", 0, 0, 0, 20);
		cha_list.push(p2);
	}
}
// 初始化怪物
void bm1(int k, int x, int y, int type, int team, void (*AI_)(Character &t, map<string, int> &mem)) {
	Character pm;
	pm.init(k, x, y, type, team, AI_);
	character_team[pm.number] = team;
	team_cha_number[team]++;
	cha_list.push(pm);
}
void birth_monster() {
	if(bo_pass == 1 || monster_sum > 0 || play_mode == 3 || play_mode == 5) return;
	Character pm;
	if(monster_difficulty == 0) { // 怪物难度 0 简单 
		if(wave == 1) {
			monster_sum = 1;
			
			int pos = rand()%4, // 随机出生点位 
				m1x, m1y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;break;}
				case 1:{m1x=3;m1y=9;break;}
				case 2:{m1x=23;m1y=3;break;}
				case 3:{m1x=23;m1y=9;break;}
			}
			bm1(SLIME_ID, m1x, m1y, MONSTER_TYPE, 2, Slime_ai_2::AI);
		}
		if(wave == 2) {
			bo_pass = 1;
			game_state = 2;
			return;
		}
	}
	else if(monster_difficulty == 1) { // 怪物难度 1 普通难度 
		if(wave == 1) {
			monster_sum = 1;
			
			int pos = rand()%4, // 随机出生点位 
				m1x, m1y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;break;}
				case 1:{m1x=3;m1y=9;break;}
				case 2:{m1x=23;m1y=3;break;}
				case 3:{m1x=23;m1y=9;break;}
			}
			bm1(SLIME_ID, m1x, m1y, MONSTER_TYPE, 2, Slime_ai_2::AI);
		}
		if(wave == 2) {
			monster_sum = 2;
			
			//Slime_ai_2::init();
			int pos = rand()%6, // 随机出生点位 
				m1x, m1y, m2x, m2y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;m2x=3;m2y=9;break;}
				case 1:{m1x=3;m1y=3;m2x=23;m2y=3;break;}
				case 2:{m1x=3;m1y=3;m2x=23;m2y=9;break;}
				case 3:{m1x=3;m1y=9;m2x=23;m2y=3;break;}
				case 4:{m1x=3;m1y=9;m2x=23;m2y=9;break;}
				case 5:{m1x=23;m1y=3;m2x=23;m2y=9;break;}
			}
			bm1(SLIME_ID, m1x, m1y, MONSTER_TYPE, 2, Slime_ai_2::AI);
			bm1(SLIME_ID, m2x, m2y, MONSTER_TYPE, 2, Slime_ai_2::AI);
		}
		if(wave == 3) {
			monster_sum = 4;
			
			bm1(SLIME_ID, 3, 3, MONSTER_TYPE, 2, Slime_ai_2::AI);
			bm1(SLIME_ID, 3, 9, MONSTER_TYPE, 2, Slime_ai_2::AI);
			bm1(SLIME_ID, 23, 3, MONSTER_TYPE, 2, Slime_ai_2::AI);
			bm1(SLIME_ID, 23, 9, MONSTER_TYPE, 2, Slime_ai_2::AI);
		}
		if(wave == 4) {
			bo_pass = 1;
			game_state = 2;
			return; 
		}
	}
	else if(monster_difficulty == 2) { // 怪物难度 2 困难难度 
		if(wave == 1) {
			monster_sum = 1;
			
			int pos = rand()%4, // 随机出生点位 
				m1x, m1y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;break;}
				case 1:{m1x=3;m1y=9;break;}
				case 2:{m1x=23;m1y=3;break;}
				case 3:{m1x=23;m1y=9;break;}
			}
			
			bm1(SLIME_ID, m1x, m1y, MONSTER_TYPE, 2, Slime_ai_3::AI);
		}
		if(wave == 2) {
			monster_sum = 2;
			
			//Slime_ai_2::init();
			int pos = rand()%6, // 随机出生点位 
				m1x, m1y, m2x, m2y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;m2x=3;m2y=9;break;}
				case 1:{m1x=3;m1y=3;m2x=23;m2y=3;break;}
				case 2:{m1x=3;m1y=3;m2x=23;m2y=9;break;}
				case 3:{m1x=3;m1y=9;m2x=23;m2y=3;break;}
				case 4:{m1x=3;m1y=9;m2x=23;m2y=9;break;}
				case 5:{m1x=23;m1y=3;m2x=23;m2y=9;break;}
			}
			
			bm1(SLIME_ID, m1x, m1y, MONSTER_TYPE, 2, Slime_ai_3::AI);
			bm1(SLIME_ID, m2x, m2y, MONSTER_TYPE, 2, Slime_ai_3::AI);
		}
		if(wave == 3) {
			monster_sum = 4;
			
			//Slime_ai_2::init();
			bm1(SLIME_ID, 3, 3, MONSTER_TYPE, 2, Slime_ai_3::AI);
			bm1(SLIME_ID, 3, 9, MONSTER_TYPE, 2, Slime_ai_3::AI);
			bm1(SLIME_ID, 23, 3, MONSTER_TYPE, 2, Slime_ai_3::AI);
			bm1(SLIME_ID, 23, 9, MONSTER_TYPE, 2, Slime_ai_3::AI);
		}
		if(wave == 4) {
			bo_pass = 1;
			game_state = 2;
			return; 
		}
	}
	else if(monster_difficulty == 3) { // 怪物难度 3 智慧难度 
		if(wave == 1) {
			monster_sum = 1;
			
			int pos = rand()%4, // 随机出生点位 
				m1x, m1y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;break;}
				case 1:{m1x=3;m1y=9;break;}
				case 2:{m1x=23;m1y=3;break;}
				case 3:{m1x=23;m1y=9;break;}
			}
			bm1(SLIME_ID, m1x, m1y, MONSTER_TYPE, 2, QLearning_AI_slime::AI);
		}
		if(wave == 2) {
			monster_sum = 1;
			
			int pos = rand()%4, // 随机出生点位 
				m1x, m1y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;break;}
				case 1:{m1x=3;m1y=9;break;}
				case 2:{m1x=23;m1y=3;break;}
				case 3:{m1x=23;m1y=9;break;}
			}
			bm1(ARCHER_ID, m1x, m1y, MONSTER_TYPE, 2, QLearning_AI_archer::AI);
		}
		if(wave == 3) {
			monster_sum = 2;
			
			//Slime_ai_2::init();
			int pos = rand()%6, // 随机出生点位 
				m1x, m1y, m2x, m2y;
			switch(pos) {
				case 0:{m1x=3;m1y=3;m2x=3;m2y=9;break;}
				case 1:{m1x=3;m1y=3;m2x=23;m2y=3;break;}
				case 2:{m1x=3;m1y=3;m2x=23;m2y=9;break;}
				case 3:{m1x=3;m1y=9;m2x=23;m2y=3;break;}
				case 4:{m1x=3;m1y=9;m2x=23;m2y=9;break;}
				case 5:{m1x=23;m1y=3;m2x=23;m2y=9;break;}
			}
			bm1(ARCHER_ID, m1x, m1y, MONSTER_TYPE, 2, QLearning_AI_archer::AI);
			bm1(ARCHER_ID, m2x, m2y, MONSTER_TYPE, 2, QLearning_AI_archer::AI);
		}
		if(wave == 4) {
			bo_pass = 1;
			game_state = 2;
			return; 
		}
	}
	else if(monster_difficulty == 4) { // 绞肉难度(4)
		if(wave == 1) {
			monster_sum = 6;
			/*
			for(int i = 3; i <= 4; i++) {
				for(int j = 3; j <= 9; j++) {
					pm.init(SLIME_ID, i, j, MONSTER_TYPE, 2, Slime_ai_4::AI);
					character_team[pm.number] = 2;
					cha_list.push(pm);
				}
			}
			*/
			bm1(HUNTER_ID, 3, 5, BOT_TYPE, 4, QLearning_AI_hunter::AI);
			bm1(HUNTER_ID, 5, 6, BOT_TYPE, 4, QLearning_AI_hunter::AI);
			bm1(HUNTER_ID, 3, 7, BOT_TYPE, 4, QLearning_AI_hunter::AI);
			/*
			for(int i = 3; i <= 3; i++) {
				for(int j = 4; j <= 8; j += 2) {
					pm.init(ARCHER_ID, i, j, MONSTER_TYPE, 4, QLearning_AI_archer::AI);
					character_team[pm.number] = 4;
					cha_list.push(pm);
				}
			}
			*/
			for(int i = 23; i <= 23; i++) {
				for(int j = 4; j <= 8; j += 2) {
					bm1(SLIME_RED_ID, i, j, MONSTER_TYPE, 3, QLearning_AI_slime::AI);
				}
			}
		}
	}
	wave++;
	//system("CLS");
}

/* hit_checker */
bool hit_checker(int mapID, int shooterID) {
	if(character_team[mapID] == character_team[shooterID]) return false;
	/*
	if(play_mode == 2) { // dont have friendly fire
		bool b1 = 0, b2 = 0;
		for(int i = 1; i <= player_sum; i++) {
			if(player_number[i] == mapID)
				b1 = 1;
			if(player_number[i] == shooterID)
				b2 = 1;
			}
		return !(b1&b2);
	}
	*/
	return true;
}

/* flush */
void flush() {
	
	birth_monster();
	//mem_all.clear(); /* memory */
	mem_all["cha sum"] = cha_tot; /* memory */
	int mem_pro_sum = 0; /* memory */
	int mem_event_sum = 0;
	int true_x, true_y;
	
if(ai_training_mode == 0) {
	/* draw ground */
	if(WINDOW_MODE == 0) {
		if(p1.x - map_x > 5) map_x = p1.x - 5;
		if(map_x - p1.x > 5) map_x = p1.x + 5;
		if(p1.y - map_y > 1) map_y = p1.y - 1;
		if(map_y - p1.y > 1) map_y = p1.y + 1;
		true_x = std::max(0, std::min(MAP_X - 25, map_x - 13)),
		true_y = std::max(0, std::min(MAP_Y - 11, map_y - 6));
		for(int i = 1; i <= 25; i++)
			for(int j = 1; j <= 11; j++) {
				render_texture(Pground, i*50-35, j*50-45, &clip4[mapB[true_x+i][true_y+j]]);
			}
	} else if(WINDOW_MODE == 1) {
		for(int i = 0; i <= 29; i++)
			for(int j = 0; j <= 14; j++) {
				render_texture(Pground, i*50, j*50, &clip4[mapB[i+1][j+1]]);
			}
	}
}
	/* draw unit */
	// character
	while(!cha_list.empty()) {
		Character t = cha_list.front();
		cha_list.pop();
		//if(t.type == 1) printf("[%d,%d] ", t.x, t.y);
		//else printf("(%d, %d) ", t.x, t.y);
		if(dmg_list[t.number] > 0) {
			if(t.is_def == 0) 
				t.hp -= dmg_list[t.number];
			else
				t.def_spd_timer = t.def_spd;
			dmg_list[t.number] = 0;
if(ai_training_mode == 0) t.hp_text = load_text("font.ttf", std::to_string(t.hp) + "/" + std::to_string(t.max_hp), 0, 0, 0, 10);
		}
		t.flush();
		if(t.hp <= 0) {
			mem_all["cha "+to_string(t.number)+" hp"] = t.hp; /* memory */
			mapC[t.x][t.y] = 0;
			mem_event_sum++; /* memory */
			mem_all["event "+to_string(mem_event_sum)] = 2;
			mem_all["event "+to_string(mem_event_sum)+" A"] = t.number;
			team_cha_number[t.team]--;
			//broadcast.push(to_string(t.number)+" was killed.");
			if(t.type == 1) {
				cout << "You Died." << endl;
				mem_all["event sum"] = mem_event_sum;
				t.AI(t, t.mem);
				game_state = 3;
			}
			if(t.type == MONSTER_TYPE) {
				monster_sum--;
				cout << "You have killed a monster !" << endl;
				ai_training_kill[++ai_training_kill[0]] = ai_training_times_cur;
			}
			
			// render_texture slime dead film
			// unit_list_.push(film)
			continue;
		}
if(ai_training_mode == 0) {
		if(WINDOW_MODE == 0) {
			render_texture((t.is_def==0?t.model:t.model_def), 50*(t.x-true_x)-35, 50*(t.y-true_y)-45, &clip4[t.direct]);
			render_texture(t.hp_text, 50*(t.x-true_x)-33, 50*(t.y-true_y)-8, NULL);
		}
		else {
			render_texture((t.is_def==0?t.model:t.model_def), 50*t.x-50, 50*t.y-50, &clip4[t.direct]);
			render_texture(t.hp_text, 50*t.x-45, 50*t.y-50, NULL);
		}
}
		
		t.mem["your number"] = t.number; /* memory */
		t.mem["your team"] = t.team;
		t.AI(t, t.mem);
		mem_all["cha "+to_string(t.number)+" team"] = t.team;
		mem_all["cha "+to_string(t.number)+" hp"] = t.hp; /* memory */
		mem_all["cha "+to_string(t.number)+" x"] = t.x; /* memory */
		mem_all["cha "+to_string(t.number)+" y"] = t.y; /* memory */
		
if(ai_training_mode == 0) {
		if(WINDOW_MODE == 0) {
			if(t.type == PLAYER1_TYPE) {
				player_atk_ammo_text[1] = t.atk_ammo_text;
				player_atk_spd_timer_text[1] = t.atk_spd_timer_text;
				player_recover_timer_text[1] = t.recover_timer_text;
			}
			if(t.type == PLAYER2_TYPE) {
				player_atk_ammo_text[2] = t.atk_ammo_text;
				player_atk_spd_timer_text[2] = t.atk_spd_timer_text;
				player_recover_timer_text[2] = t.recover_timer_text;
			}
		}
}
		cha_list_.push(t);
	}
	//printf("\n");
	cha_list = cha_list_;
	while(!cha_list_.empty()) cha_list_.pop();
	// projectile
	while(!pro_list.empty()) {
		Projectile t = pro_list.front();
		pro_list.pop();
		t.x += t.atk.mov_spd * mx[t.direct];
		t.y += t.atk.mov_spd * my[t.direct];
		t.atk.range -= t.atk.mov_spd;
		int tx = (t.x)/50,
			ty = (t.y)/50;
		if(tx < 1 || ty < 1 || tx > MAP_X || ty > MAP_Y)
			continue;
		if(mapC[tx][ty] == -1) {
			Particle pt;
			pt.init(t.x, t.y, t.atk);
			par_list_.push(pt);
			continue;
		}
		if(mapC[tx][ty] > 0 && hit_checker(mapC[tx][ty], t.number)) { // 当前格子存在character 
			dmg_list[mapC[tx][ty]] += t.atk.dmg;
			mem_event_sum++; /* memory */
			mem_all["event "+to_string(mem_event_sum)] = 1;
			mem_all["event "+to_string(mem_event_sum)+" A"] = t.number;
			mem_all["event "+to_string(mem_event_sum)+" B"] = mapC[tx][ty];
			mem_all["event "+to_string(mem_event_sum)+" n"] = t.atk.dmg;
			//cout << t.number << " hitted " << mapC[tx][ty] << " in (" << tx << "," << ty << endl; 
			Particle pt;
			pt.init(t.x, t.y, t.atk);
			par_list_.push(pt);
			continue;
		}
		if(t.atk.range <= 0) {
			Particle pt;
			pt.init(t.x, t.y, t.atk);
			par_list_.push(pt);
			continue;
		}
		mem_pro_sum++; /* memory */
		mem_all["pro "+to_string(mem_pro_sum)+" father number"] = t.number;
		mem_all["pro "+to_string(mem_pro_sum)+" x"] = t.x/50;
		mem_all["pro "+to_string(mem_pro_sum)+" y"] = t.y/50;
		mem_all["pro "+to_string(mem_pro_sum)+" direct"] = t.direct;
		mem_all["pro "+to_string(mem_pro_sum)+" damage"] = t.atk.dmg;
		pro_list_.push(t);
if(ai_training_mode == 0) {
		if(WINDOW_MODE == 0) {
			if(t.x-50*true_x-35 < 0 || t.y-50*true_y-45 < 0 || t.x-50*true_x-35 > 1230 || t.y-50*true_y-45 > 520)
				continue;
			render_texture(t.atk.model, t.x-50*true_x-35, t.y-50*true_y-45, &clip4[t.direct]);
		}
		else {
			render_texture(t.atk.model, t.x-50, t.y-50, &clip4[t.direct]);
		}
}
	}
	mem_all["event sum"] = mem_event_sum;
	pro_list = pro_list_;
	mem_all["pro sum"] = mem_pro_sum;
	while(!pro_list_.empty()) pro_list_.pop();
if(ai_training_mode == 0) {
	// Particle
	while(!par_list.empty()) {
		Particle t = par_list.front();
		par_list.pop();
		t.frame_spd_timer--;
		t.next_frame();
		if(t.frame_cur <= 9) {
			if(WINDOW_MODE == 0)
				render_texture(t.atk.model_over, t.x-50*true_x-35, t.y-50*true_y-45, &clip10[t.frame_cur]);
			else
				render_texture(t.atk.model_over, t.x-50, t.y-50, &clip10[t.frame_cur]);
			par_list_.push(t);
		}
	}
}
	par_list = par_list_;
	while(!par_list_.empty()) par_list_.pop();
if(ai_training_mode == 0) {															// ai training no need
	if(WINDOW_MODE == 0) {															// ai training no need															
		/* draw border */															// ai training no need															
		render_texture(Pborder, -1, -1, NULL);										// ai training no need
		render_texture(credit_text1, 1020, 620, NULL);								// ai training no need
		render_texture(credit_text2, 1020, 650, NULL);								// ai training no need
		render_texture(instruction_text1, 720, 620, NULL);							// ai training no need
		render_texture(instruction_text2, 720, 650, NULL);							// ai training no need
		if(bo_pass) render_texture(congratulation_text, -1, -1, NULL);				// ai training no need
		/* show UI */																// ai training no need
		if(player_type[1] > 0) {													// ai training no need
			render_texture(player_name[1], 50, 540, NULL);							// ai training no need
			render_texture(player_atk_ammo_text[1], 150, 540, NULL);				// ai training no need
			render_texture(player_atk_spd_timer_text[1], 150, 560, NULL);			// ai training no need
			render_texture(player_recover_timer_text[1], 150, 580, NULL);			// ai training no need
		}																			// ai training no need
		if(player_type[2] > 0) {													// ai training no need
			render_texture(player_name[2], 50, 610, NULL);							// ai training no need
			render_texture(player_atk_ammo_text[2], 150, 610, NULL);				// ai training no need
			render_texture(player_atk_spd_timer_text[2], 150, 630, NULL);			// ai training no need
			render_texture(player_recover_timer_text[2], 150, 650, NULL);			// ai training no need
		}																			// ai training no need
	}																				// ai training no need
	/* show it */																	// ai training no need
	SDL_RenderPresent(renderer);													// ai training no need
}																					// ai training no need

}

int to_int(char * s) {
	int len = strlen(s);
	int x = 0;
	for(int i = 0; i < len; i++) {
		if(s[i] >= '0' && s[i] <= '9')
			x = 10 * x + s[i] - '0';
	}
	return x;
}

void restart() {
	map_init();
	while(!cha_list.empty()) cha_list.pop();
	while(!pro_list.empty()) pro_list.pop();
	while(!par_list.empty()) par_list.pop();
	while(!cha_list_.empty()) cha_list_.pop();
	while(!pro_list_.empty()) pro_list_.pop();
	while(!par_list_.empty()) par_list_.pop();
	memset(dmg_list, 0, sizeof dmg_list);
	memset(mapB, 0, sizeof mapB);
	memset(mapC, 0, sizeof mapC);
	memset(team_cha_number, 0, sizeof team_cha_number);
	map_init();
	mem_all.clear();
	wave = 1;
	monster_sum = 0;
	cha_tot = 0;
	birth_player(); 
	bo_pass = 0;
	game_state = 1;
	par_tot = 0;
	atk_type_tot = 0; 
}

int main(int argc, char** argv) {
	// 0 : exercise path
	// 1 : play_mode [1 pve(3); 2 ppve(4); 3 pvp(4)]
	// 2 : player1_type
	// 3 : player2_type
	/* check */
	//return 0;
	//freopen("output.txt", "w", stdout);
	if(argc >= 2) {
		play_mode = argv[1][0]-'0';
		if(play_mode == 1) {
			WINDOW_MODE = 0; player_sum = 1; player_type[1] = argv[2][0]-'0'; player_type[2] = 0; monster_difficulty = argv[4][0]-'0'; 
		}
		else if(play_mode == 2) {
			WINDOW_MODE = 0; player_sum = 2; player_type[1] = argv[2][0]-'0'; player_type[2] = argv[3][0]-'0'; monster_difficulty = argv[4][0]-'0';
		}
		else if(play_mode == 3) {
			WINDOW_MODE = 0; player_sum = 2; player_type[1] = argv[2][0]-'0'; player_type[2] = argv[3][0]-'0';
		}
		else if(play_mode == 4) { // ai pve mode
			WINDOW_MODE = 0; player_sum = 1; player_type[1] = argv[2][0]-'0'; player_type[2] = 0; monster_difficulty = argv[4][0]-'0';
		}
		else if(play_mode == 5) { // ai pvp mode
			WINDOW_MODE = 0; player_sum = 2; player_type[1] = argv[2][0]-'0'; player_type[2] = argv[3][0]-'0';
		}
		else if(play_mode == 6) { // 50v50 mode
			WINDOW_MODE = 0; player_sum = 0; monster_difficulty = 4;
		}
		else if(play_mode == 7) { // ai training mode
			ai_training_mode = 1; WINDOW_MODE = -1;/*指代没有窗口*/ player_sum = 1;
			player_type[1] = argv[2][0]-'0'; ai_training_times = to_int(argv[3]); 
			monster_difficulty = argv[4][0]-'0';
		}
		else {printf("Mode Error!\n"); system("pause"); return 1;}
	}
	else {
		play_mode = 6; WINDOW_MODE = 0; player_sum = 0; monster_difficulty = 4;
		//play_mode = 1; WINDOW_MODE = 0; player_sum = 1; player_type[1] = 1; player_type[2] = 0;
		//scanf("%d", &player1_type);
	}
	if(monster_difficulty < 0 || monster_difficulty > 4) monster_difficulty = 0;
	printf("monster_difficulty : %d\n", monster_difficulty); 
	srand(time(0));
	init();
	birth_player();
	birth_monster();
	game_state = 1;
	printf("ai_training_mode = %d\n", ai_training_mode);
	if(ai_training_mode == 1) {
		ai_training_times_cur = 0;
		while(ai_training_times_cur < ai_training_times) {
			//初始化函数; 
			restart();
			while(game_state == 1) {
				flush();
				int team_count = 0;
				for(int i = 1; i <= 10; i++) {
					if(team_cha_number[i] != 0)
						team_count++;
				}
				if(team_count <= 1) game_state = 4; // 不确定谁赢，但打完了 
			}
			printf("%d / %d\n", ai_training_times_cur+1, ai_training_times);
			//system("pause");
			ai_training_times_cur++;
		}
		printf("You have killed %d monsters.\n", ai_training_kill[0]);
		for(int i = 1; i <= ai_training_kill[0]; i++)
			printf("%d,", ai_training_kill[i]);
		system("pause");
		return 0;
	}
	
	bool bo_quit = false;
	SDL_Event e;
	while(!bo_quit) {
		while(SDL_PollEvent(&e)) {
			//event type
			if(e.type == SDL_QUIT) {
				bo_quit = true;
				continue;
			}
			if(e.type == SDL_KEYDOWN) {
				int input = kb_input(e.key.keysym.sym, time_now);
				//printf("pressed : %d\n", input);
				//if(input == 32) slime_ai_1_lock ^= 1;
				//if(input == 32) bo_pass = 1;
				continue;
			}
			if(e.type == SDL_KEYUP) {
				int input = kb_input(e.key.keysym.sym, 0);
				continue;
			}
		}
		flush();
		// make tick (50tick/s)
		while(time_now == time_last) SDL_Delay(1);
		time_last = time_now;
	}
	
	quit();
	
	return 0;
}




