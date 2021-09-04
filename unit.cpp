
#ifndef UNIT_cpp_
#define UNIT_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<queue>
#include<map> 

#define PRO Projectile
#define PAR Particle

#define ARCHER_ID 1
#define HUNTER_ID 2
#define SLIME_ID 3
#define WARRIOR_ID 4
#define BAT_ID 5
#define SLIME_RED_ID 6

#define PLAYER1_TYPE 1
#define PLAYER2_TYPE 2
#define MONSTER_TYPE 3
#define BOT_TYPE 4

using std::to_string;
using std::queue;
using std::map;

/* Attack_type */ 
int atk_type_tot = 0;

struct Attack_type {
	int type;
	int dmg,
		recover_spd,
		mov_spd,
		range,
		used_ammo;
	SDL_Texture *model;
	SDL_Texture *model_over;
	void init_ranged(int dmg_, int recover_spd_, int mov_spd_, int range_, int used_ammo_, SDL_Texture *model_, SDL_Texture *model_over_) {
		type = 1; dmg = dmg_; recover_spd = recover_spd_; mov_spd = mov_spd_; range = range_; used_ammo = used_ammo_; 
if(ai_training_mode == 0) model = model_;
if(ai_training_mode == 0) model_over = model_over_;
	}
	void init_melee(int dmg_, int recover_spd_, SDL_Texture *model_over_) {
		type = 1; dmg = dmg_; recover_spd = recover_spd_; mov_spd = 25; range = 1; used_ammo = 0;
if(ai_training_mode == 0) model_over = model_over_;
	}
	void init_def(int recover_spd_) {
		type = 2; recover_spd = recover_spd_;
	}
	void quit() {
		SDL_DestroyTexture(model);
		SDL_DestroyTexture(model_over);
	}
};

/* Projectile */

struct Projectile {
	int number,
		x,		// ���������� (Pixels) 
		y,
		direct;
	Attack_type atk;
	void init(int number_, int pos_x, int pos_y, int direct_, Attack_type atk_type_) {
		number = number_;
		x = pos_x;
		y = pos_y;
		direct = direct_;
		atk = atk_type_;
	}
};

/* Particle */
int par_tot = 0;

struct Particle {
	int x,
		y,
		frame_cur,
		frame_spd,
		frame_spd_timer;
	Attack_type atk;
	void next_frame() {
		if(frame_spd_timer > 0) return;
		frame_spd_timer = frame_spd;
		frame_cur++;
	}
	void init(int pos_x, int pos_y, Attack_type atk_) {
		atk = atk_;
		x = pos_x;
		y = pos_y;
		frame_cur = 0;
		frame_spd = 5;
		frame_spd_timer = frame_spd;
	}
};

/* Character */
int cha_tot = 0;

struct Character {
	int number,
		type,	// ��λ����
		team,   // ��λ��Ӫ 
		x,		// ��λ���� (Blocks)
		y,
		direct, // ��λ���� 
		hp,
		max_hp,
		sp,
		max_sp,
		mov_spd,	// �ƶ��ٶ� (n ticks һ���ƶ�)
		mov_spd_timer,
		atk_dmg,	// �����˺� 
		atk_ammo,	// ������ҩ 
		atk_ammo_lim, // ������ҩ���� 
		atk_spd,	// �����ٶ� (n ticks һ�ι���)
		atk_spd_timer, // �����ٶȼ�ʱ�� (timer = 0 ���Թ���) 
		atk_recover_spd_timer, // ����Ӳֱʱ�� 
		def,
		def_spd,
		def_spd_timer;
	int is_def;
	SDL_Texture *model;
	SDL_Texture *model_def;
	SDL_Texture *hp_text; // hp & max_hp
	SDL_Texture *atk_ammo_text; // atk_ammo & atk_ammo_lim 
	SDL_Texture *atk_spd_timer_text; // atk_spd_timer
	SDL_Texture *recover_timer_text; // atk_recover_spd_timer
	map<string, int> mem;
	/*
	tips:
		�ٶȻ���: ֻ�����캯��������timer��ÿ��tickʹtimer-1 
	*/
	Attack_type atk_type[PRO_MAX] = {};
	int atk_type_sum = 0;
	void (*AI)(Character &t, map<string, int> &mem);
	Character() {
		x = y = direct = type = hp = max_hp = sp = max_sp = 0;
		mov_spd = mov_spd_timer = 0;
		atk_dmg = atk_spd = atk_spd_timer = 0;
		def = 0;
	}
	void flush() {
		is_def -= (is_def > 0);
		mov_spd_timer -= (mov_spd_timer > 0);
		def_spd_timer -= (def_spd_timer > 0);
		atk_spd_timer -= (atk_spd_timer > 0 && atk_ammo < atk_ammo_lim);
		atk_recover_spd_timer -= (atk_recover_spd_timer > 0);
		if(atk_spd_timer == 0 && atk_ammo < atk_ammo_lim) {
			atk_ammo++;
			atk_spd_timer = atk_spd;
		}
if(ai_training_mode == 0) {
		if(type == PLAYER1_TYPE || type == PLAYER2_TYPE) {
			int R = 0, G = 0, B = 0;
			atk_ammo_text = load_text("font.ttf", "ammo : " + to_string(atk_ammo) + "/" + to_string(atk_ammo_lim), R, G, B, 15);
			atk_spd_timer_text = load_text("font.ttf", "add ammo CD : " + to_string(atk_spd_timer/tick) + "." + to_string(atk_spd_timer%tick/tick_10), R, G, B, 15);
			if(atk_recover_spd_timer == 0)
				R = 150;
			recover_timer_text = load_text("font.ttf", "attack CD : " + to_string(atk_recover_spd_timer/tick) + "." + to_string(atk_recover_spd_timer%tick/tick_10), R, G, B, 15);
		}
}
		mem["your number"] = number;
		mem["your hp"] = hp;
		mem["your x"] = x;
		mem["your y"] = y;
		mem["your direct"] = direct;
	}
	bool turn(int k) {
		direct = k;
		return true;
	}
	bool move(int k) {
		direct = k;
		if(k > 3) {printf("direct4 move!\n");return false;}
		if(mov_spd_timer == 0 && map_reach(x + mx[k], y + my[k])) {
			mov_spd_timer = mov_spd;
			mapC[x][y] = 0;
			x = x + mx[k];
			y = y + my[k];
			mapC[x][y] = number;
			return true;
		}
		else
			return false;
	}
	int can_move(int k) {
		if(mov_spd_timer > 0)
			return 0;
		if(!map_reach(x + mx[k], y + my[k]))
			return -1;
		return 1;
	}
	int can_move() {
		if(mov_spd_timer > 0)
			return 0;
		//if(!map_reach(x + mx[direct], y + my[direct]))
		//	return -1;
		return 1;
	}
	bool attack(int kind_) {
		//if(atk_spd_timer > 0) return;
		if(atk_type[kind_].type == 1) {
			if(atk_ammo >= atk_type[kind_].used_ammo && atk_recover_spd_timer == 0) {
				atk_ammo -= atk_type[kind_].used_ammo;
				atk_recover_spd_timer = atk_type[kind_].recover_spd;
				Projectile t;
				t.init(number, 50*x + 25*mx[direct], 50*y + 25*my[direct], direct, atk_type[kind_]);
				//if(type == 1) printf("PlayerPosition:(%d,%d)\nShotPosition:(%d,%d)", x, y, 50*x + 25*mx[direct], 50*y + 25*my[direct]);
				pro_list_.push(t);
				return true;
			}
			else
				return false;
		}
		if(atk_type[kind_].type == 2) {
			if(def_spd_timer == 0) {
				is_def = 2;
				return true;
			}
			else
				return false;
		}
		return false;
	}
	bool can_attack(int kind_) {
		if(atk_type[kind_].type == 1) {
			if(atk_ammo >= atk_type[kind_].used_ammo && atk_recover_spd_timer == 0)
				return true;
			else
				return false;
		}
		if(atk_type[kind_].type == 2) {
			if(def_spd_timer == 0)
				return true;
			else
				return false;
		}
		return false;
	}
	
	int init(int kind_, int pos_x, int pos_y, int type_, int team_, void (*AI_)(Character &t, map<string, int> &mem)) {
		atk_type_sum = 0;
		AI = AI_;
		number = ++cha_tot;
		//printf("give number : %d\n", number);
		type = type_;
		team = team_;
		x = pos_x;
		y = pos_y;
		direct = 0;
		mapC[x][y] = number;
		is_def = 0;
		if(kind_ == ARCHER_ID) { // archer
			hp = max_hp = 3;
			sp = max_sp = 0;
			mov_spd = 10;
			atk_ammo_lim = 1;
			atk_spd = 50;
if(ai_training_mode == 0) model = load_image("img\\character\\archer.png");
if(ai_training_mode == 0) hp_text = load_text("font.ttf", std::to_string(hp) + "/" + std::to_string(max_hp), 0, 0, 0, 10);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_ranged(1, 50, 10, 500, 1, load_image("img\\projectile\\arrow.png"), load_image("img\\particle\\arrow_boom.png"));
else atk_type[++atk_type_sum].init_ranged(1, 50, 10, 500, 1, nullptr, nullptr);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_ranged(3, 100, 25, 1, 0, load_image("img\\projectile\\archer_kick.png"), load_image("img\\particle\\archer_kick.png"));
else atk_type[++atk_type_sum].init_ranged(3, 100, 25, 1, 0, nullptr, nullptr);
		}
		if(kind_ == HUNTER_ID) { // hunter
			hp = max_hp = 2;
			sp = max_sp = 0;
			mov_spd = 10;
			atk_ammo_lim = 2;
			atk_spd = 50;
if(ai_training_mode == 0) model = load_image("img\\character\\hunter.png");
if(ai_training_mode == 0) hp_text = load_text("font.ttf", std::to_string(hp) + "/" + std::to_string(max_hp), 0, 0, 0, 10);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_ranged(1, 5, 25, 200, 1, load_image("img\\projectile\\shotgun.png"), load_image("img\\particle\\shotgun_boom.png"));
else atk_type[++atk_type_sum].init_ranged(1, 5, 25, 200, 1, nullptr, nullptr);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_ranged(3, 100, 10, 500, 2, load_image("img\\projectile\\shotgun.png"), load_image("img\\particle\\shotgun_boom.png"));
else atk_type[++atk_type_sum].init_ranged(3, 100, 10, 500, 2, nullptr, nullptr);
		}
		if(kind_ == WARRIOR_ID) { // warrior
			hp = max_hp = 7;
			sp = max_sp = 0;
			mov_spd = 10;
			atk_ammo_lim = 0;
			atk_spd = 0;
			def_spd = 100;
if(ai_training_mode == 0) model = load_image("img\\character\\guard.png");
if(ai_training_mode == 0) model_def = load_image("img\\character\\guard_def.png");
if(ai_training_mode == 0) hp_text = load_text("font.ttf", std::to_string(hp) + "/" + std::to_string(max_hp), 0, 0, 0, 10);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_melee(2, 50, load_image("img\\particle\\archer_kick.png"));
else atk_type[++atk_type_sum].init_melee(2, 50, nullptr);
		atk_type[++atk_type_sum].init_def(50);
		}
		if(kind_ == SLIME_ID) { // slime
			hp = max_hp = 6+(rand()%5);
			sp = max_sp = 0;
			mov_spd = 20;
			atk_ammo_lim = 1;
			atk_spd = 50;
if(ai_training_mode == 0) model = load_image("img\\character\\slime.png");
if(ai_training_mode == 0) hp_text = load_text("font.ttf", std::to_string(hp) + "/" + std::to_string(max_hp), 0, 0, 0, 10);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_ranged(1, 50, 5, 250, 1, load_image("img\\projectile\\slime_ball.png"), load_image("img\\particle\\slime_ball_boom.png"));
else atk_type[++atk_type_sum].init_ranged(1, 50, 5, 250, 1, nullptr, nullptr);
		}
		if(kind_ == BAT_ID) { // skeleton
			hp = max_hp = 1;
			sp = max_sp = 0;
			mov_spd = 13;
			atk_ammo_lim = 0;
			atk_spd = 0;
			// not done
			// not done
			// not done
		}
		if(kind_ == SLIME_RED_ID) { // slime
			hp = max_hp = 6+(rand()%5);
			sp = max_sp = 0;
			mov_spd = 20;
			atk_ammo_lim = 1;
			atk_spd = 50;
if(ai_training_mode == 0) model = load_image("img\\character\\slime_red.png");
if(ai_training_mode == 0) hp_text = load_text("font.ttf", std::to_string(hp) + "/" + std::to_string(max_hp), 0, 0, 0, 10);
if(ai_training_mode == 0) atk_type[++atk_type_sum].init_ranged(1, 50, 5, 250, 1, load_image("img\\projectile\\slime_red_ball.png"), load_image("img\\particle\\slime_red_ball_boom.png"));
else atk_type[++atk_type_sum].init_ranged(1, 50, 5, 250, 1, nullptr, nullptr);
		}
		mov_spd_timer = mov_spd;
		atk_spd_timer = atk_spd;
		def_spd_timer = def_spd;
		atk_ammo = atk_ammo_lim;
		atk_recover_spd_timer = 0;
		mem_all["cha "+to_string(number)+" hp"] = hp; /* memory */
		mem_all["cha "+to_string(number)+" x"] = x; /* memory */
		mem_all["cha "+to_string(number)+" y"] = y; /* memory */
		mem_all["cha "+to_string(number)+" team"] = team;
		return number;
	}
	void quit() {
		SDL_DestroyTexture(model);
		SDL_DestroyTexture(hp_text);
		for(int i = 1; i <= atk_type_sum; i++)
			atk_type[i].quit();
	}
};

#undef PRO
#undef PAR

#endif

