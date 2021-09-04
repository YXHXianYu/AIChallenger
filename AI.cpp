
#ifndef AI_cpp_
#define AI_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<queue>
#include<map>
#include<fstream>

#define PRO Projectile
#define PAR Particle
#define TS to_string

using std::queue;
using std::map;

int character_team[CHA_MAX] = {};
int team_cha_number[TEAM_MAX] = {};

/* player */
Character p1;
Character p2;
void player1_control(Character &t, map<string, int> &mem) {
	int max_move = std::max(std::max(kb_stage[43],kb_stage[39]),std::max(kb_stage[21],kb_stage[24]));
	int max_atk = std::max(kb_stage[30], kb_stage[31]);
	/**/ if(max_move == kb_stage[43] && max_move != 0)
		t.move(0);
	else if(max_move == kb_stage[39] && max_move != 0)
		t.move(1);
	else if(max_move == kb_stage[21] && max_move != 0)
		t.move(2);
	else if(max_move == kb_stage[24] && max_move != 0)
		t.move(3);
	/**/ if(max_atk == kb_stage[30] && max_atk != 0)
		t.attack(1);
	else if(max_atk == kb_stage[31] && max_atk != 0)
		t.attack(2);
	
	p1 = t;
}
void player2_control(Character &t, map<string, int> &mem) {
	int max_move = std::max(std::max(kb_stage[47],kb_stage[48]),std::max(kb_stage[49],kb_stage[50]));
	int max_atk = std::max(kb_stage[51], kb_stage[52]);
	
	/**/ if(max_move == kb_stage[47] && max_move != 0)
		t.move(0);
	else if(max_move == kb_stage[48] && max_move != 0)
		t.move(1);
	else if(max_move == kb_stage[49] && max_move != 0)
		t.move(2);
	else if(max_move == kb_stage[50] && max_move != 0)
		t.move(3);
	/**/ if(max_atk == kb_stage[51] && max_atk != 0)
		t.attack(1);
	else if(max_atk == kb_stage[52] && max_atk != 0)
		t.attack(2);
	
	p2 = t;
}

/* data */
struct Information_pack_public {
	int play_mode = 0,
		player_sum = 0,
		player_type[PLAYER_MAX] = {},
		player_number[PLAYER_MAX] = {};
	queue<Projectile> map[MAP_X+1][MAP_Y+1];
	Information_pack_public(int play_mode_, int player_sum_, int player_type_[], int player_number_[]) {
		play_mode = play_mode_;
		player_sum = player_sum_;
		for(int i = 1; i <= player_sum; i++)
			player_type[i] = player_type_[i],
			player_number[i] = player_number_[i];
	}
	void init() {
		for(int i = 1; i <= MAP_X; i++)
			for(int j = 1; j <= MAP_Y; j++)
				while(!map[i][j].empty())
					map[i][j].pop();
	}
	void insert(int x, int y, Projectile t) {
		map[x][y].push(t);
	}
};
struct Information_pack_private {
	int number, x, y;
	queue<Projectile> map[9][9];
	Information_pack_private(int x_, int y_) {
		x = x_; y = y_;
	}
	void flush() {
		
	}
};

/* slime */
bool slime_ai_1_lock = 0;
void slime_ai_1(Character &t, map<string, int> &mem) {
	int think_time = 6;
	if(slime_ai_1_lock == 1)
		think_time = 50;
	int k = rand() % 50;
	if(k <= 3 && t.mov_spd_timer <= 0) t.move(k);
	if(4 <= k && k <= 6 && slime_ai_1_lock == 0 && t.can_attack(1)) {
		t.attack(1);
	}
}

namespace Slime_ai_2 { // 基础跟踪AI 
	//int think_time = 5;
	//map<string, int> mem;
	/*
	void init() {
		mem.clear();
	}
	*/
	void AI(Character &t, map<string, int> &mem) {
		int x = t.x, y = t.y;
		int xmin = std::max(1, x-4),
			xmax = std::min(MAP_X, x+4),
			ymin = std::max(1, y-4),
			ymax = std::min(MAP_Y, y+4);
		if(x == mem["target_x"] && y == mem["target_y"]) mem["target_number"] = 0;
		bool to_atk = 0;
		if(mem["target_number"] > 0) {
			if(mapC[mem["target_x"]][mem["target_y"]] == mem["target_number"])
				to_atk = 1;
			else {
				for(int i = xmin; i <= xmax; i++)
					for(int j = ymin; j <= ymax; j++)
						if(mapC[i][j] == mem["target_number"]) {
							mem["target_x"] = i;
							mem["target_y"] = j;
						}
			}
		}
		else {
			for(int i = xmin; i <= xmax; i++)
				for(int j = ymin; j <= ymax; j++)
					if(mapC[i][j] > 0 && character_team[mapC[i][j]] != t.team) {
						mem["target_number"] = mapC[i][j];
						mem["target_x"] = i;
						mem["target_y"] = j;
						//printf("Slime Found Target : %d,%d\n", mem["target_x"], mem["target_y"]);
						break;
					}
		}
		if(mem["target_number"] > 0) {
			//printf("Getting\n");
			if(mem["target_x"] == x) {
				if(mem["target_y"] < y)
					t.turn(0);
				else
					t.turn(1);
				if(to_atk)
					t.attack(1);
			}
			else if(mem["target_y"] == y) {
				if(mem["target_x"] < x)
					t.turn(2);
				else
					t.turn(3);
				if(to_atk)
					t.attack(1);
			}
			else {
				int x_ = abs(x - mem["target_x"]),
					y_ = abs(y - mem["target_y"]);
				if(y_ <= x_) {
					if(mem["target_y"] < y)
						t.turn(0);
					else
						t.turn(1);
				}
				else {
					if(mem["target_x"] < x)
						t.turn(2);
					else
						t.turn(3);
				}
			}
			if(t.can_move() == 1)
				t.move(t.direct); 
		}
		else {
			int k = rand() % 4;
			if(t.can_move(k)) {
				t.move(k);
			}
		}
	}
};

namespace Slime_ai_2_maxsight { // 基础跟踪AI 
	//int think_time = 5;
	//map<string, int> mem;
	/*
	void init() {
		mem.clear();
	}
	*/
	void AI(Character &t, map<string, int> &mem) {
		int x = t.x, y = t.y;
		int xmin = 3,
			xmax = MAP_X-2,
			ymin = 3,
			ymax = MAP_Y-2;
		if(x == mem["target_x"] && y == mem["target_y"]) mem["target_number"] = 0;
		bool to_atk = 0;
		if(mem["target_number"] > 0) {
			if(mapC[mem["target_x"]][mem["target_y"]] == mem["target_number"])
				to_atk = 1;
			else {
				for(int i = xmin; i <= xmax; i++)
					for(int j = ymin; j <= ymax; j++)
						if(mapC[i][j] == mem["target_number"]) {
							mem["target_x"] = i;
							mem["target_y"] = j;
						}
			}
		}
		else {
			for(int i = xmin; i <= xmax; i++)
				for(int j = ymin; j <= ymax; j++)
					if(mapC[i][j] > 0 && character_team[mapC[i][j]] != t.team) {
						mem["target_number"] = mapC[i][j];
						mem["target_x"] = i;
						mem["target_y"] = j;
						//printf("Slime Found Target : %d,%d\n", mem["target_x"], mem["target_y"]);
						break;
					}
		}
		if(mem["target_number"] > 0) {
			//printf("Getting\n");
			if(mem["target_x"] == x) {
				if(mem["target_y"] < y)
					t.turn(0);
				else
					t.turn(1);
				if(to_atk)
					t.attack(1);
			}
			else if(mem["target_y"] == y) {
				if(mem["target_x"] < x)
					t.turn(2);
				else
					t.turn(3);
				if(to_atk)
					t.attack(1);
			}
			else {
				int x_ = abs(x - mem["target_x"]),
					y_ = abs(y - mem["target_y"]);
				if(y_ <= x_) {
					if(mem["target_y"] < y)
						t.turn(0);
					else
						t.turn(1);
				}
				else {
					if(mem["target_x"] < x)
						t.turn(2);
					else
						t.turn(3);
				}
			}
			if(t.can_move() == 1)
				t.move(t.direct); 
		}
		else {
			int k = rand() % 4;
			if(t.can_move(k)) {
				t.move(k);
			}
		}
	}
};

namespace Slime_ai_3 {
	//int think_time = 5;
	//map<string, int> mem;
	/*
	void init() {
		mem.clear();
	}
	*/
	void AI(Character &t, map<string, int> &mem) {
		if(mem.count("init") == 0) {
			mem["init"] = 1;
			// 模糊逻辑处理
			// 因为怪物有多波，所以目的是尽量消耗ai血量即可胜利。
			// 故怪物血量高时偏向于激进打法，血量低时采取稳健打法 
			for(int i = 0; i <= 2; i++) {mem["hpfuzzy safe"+TS(i)] = 0; mem["hpfuzzy unsafe"+TS(i)] = 10;}
			mem["hpfuzzy safe3"] = 1; mem["hpfuzzy unsafe3"] = 9;
			mem["hpfuzzy safe4"] = 3; mem["hpfuzzy unsafe4"] = 7;
			mem["hpfuzzy safe5"] = 5; mem["hpfuzzy unsafe5"] = 5;
			mem["hpfuzzy safe6"] = 7; mem["hpfuzzy unsafe6"] = 3;
			mem["hpfuzzy safe7"] = 9; mem["hpfuzzy unsafe7"] = 1;
			for(int i = 8; i <= 20; i++) {mem["hpfuzzy safe"+TS(i)] = 10; mem["hpfuzzy unsafe"+TS(i)] = 0;}
			for(int i = 0; i <= 2; i++) {mem["disfuzzy safe"+TS(i)] = 0; mem["disfuzzy unsafe"+TS(i)] = 10;}
			for(int i = 3; i <= 11; i++) {mem["disfuzzy safe"+TS(i)] = i-2; mem["disfuzzy unsafe"+TS(i)] = 12-i;}
			for(int i = 12; i <= 50; i++) {mem["disfuzzy safe"+TS(i)] = 10; mem["disfuzzy unsafe"+TS(i)] = 0;}
			mem["fuzzy safe ow"] = 4; 
			mem["fuzzy unsafe ow"] = 6;
			// midvalue : hp=5; dis=7;
		}
		int x = t.x, y = t.y;
		int xmin = std::max(1, x-10),
			xmax = std::min(MAP_X, x+10),
			ymin = std::max(1, y-10),
			ymax = std::min(MAP_Y, y+10);
		if(x == mem["target_x"] && y == mem["target_y"]) mem["target_number"] = 0;
		bool to_atk = 0;
		if(mem["target_number"] > 0) {
			if(mapC[mem["target_x"]][mem["target_y"]] == mem["target_number"])
				to_atk = 1;
			else {
				for(int i = xmin; i <= xmax; i++)
					for(int j = ymin; j <= ymax; j++)
						if(mapC[i][j] == mem["target_number"]) {
							mem["target_x"] = i;
							mem["target_y"] = j;
						}
			}
		}
		else {
			for(int i = xmin; i <= xmax; i++)
				for(int j = ymin; j <= ymax; j++)
					if(mapC[i][j] > 0 && character_team[mapC[i][j]] != t.team) {
						mem["target_number"] = mapC[i][j];
						mem["target_x"] = i;
						mem["target_y"] = j;
						//printf("Slime Found Target : %d,%d\n", mem["target_x"], mem["target_y"]);
						break;
					}
		}
		if(mem["target_number"] > 0) {
			int fhp = mem["your hp"],
				fdis = abs(x-mem["target_x"])+abs(y-mem["target_y"]);
			int fhpsafe = mem["hpfuzzy safe"+TS(fhp)],
				fhpunsafe = mem["hpfuzzy unsafe"+TS(fhp)],
				fdissafe = mem["disfuzzy safe"+TS(fdis)],
				fdisunsafe = mem["disfuzzy unsafe"+TS(fdis)];
			int output1 = 0,
				output2 = 0;
			output1 += std::min(fhpsafe, fdissafe) * mem["fuzzy safe ow"];
			output2 += std::min(fhpsafe, fdissafe);
			output1 += std::min(fhpsafe, fdisunsafe) * ((fhpsafe <= fdisunsafe)?(mem["fuzzy safe ow"]):(mem["fuzzy unsafe ow"]));
			output2 += std::min(fhpsafe, fdisunsafe);
			output1 += std::min(fhpunsafe, fdissafe) * ((fhpunsafe <= fdissafe)?(mem["fuzzy safe ow"]):(mem["fuzzy unsafe ow"]));
			output2 += std::min(fhpunsafe, fdissafe);
			output1 += std::min(fhpunsafe, fdisunsafe) * mem["fuzzy unsafe ow"];
			output2 += std::min(fhpunsafe, fdisunsafe);
			int output = output1/output2;
			//printf("output = %d\n", output);
			if(output <= 5) {
				//printf("Getting\n");
				if(mem["target_x"] == x) {
					if(mem["target_y"] < y)
						t.turn(0);
					else
						t.turn(1);
					if(to_atk)
						t.attack(1);
				}
				else if(mem["target_y"] == y) {
					if(mem["target_x"] < x)
						t.turn(2);
					else
						t.turn(3);
					if(to_atk)
						t.attack(1);
				}
				else {
					int x_ = abs(x - mem["target_x"]),
						y_ = abs(y - mem["target_y"]);
					if(y_ <= x_) {
						if(mem["target_y"] < y)
							t.turn(0);
						else
							t.turn(1);
					}
					else {
						if(mem["target_x"] < x)
							t.turn(2);
						else
							t.turn(3);
					}
				}
				if(t.can_move() == 1)
					t.move(t.direct); 
			} else {
				if(y == mem["target_y"]) {
					if(x < mem["target_x"])
						t.turn(3), t.attack(1);
					else
						t.turn(2), t.attack(1);
					int rand_v = rand() % 2;
					if(rand_v == 0 && map_reach(x, y-1))
						t.move(0);
					else
						t.move(1);
				} else if(x == mem["target_x"]) {
					if(y < mem["target_y"])
						t.turn(1), t.attack(1);
					else
						t.turn(0), t.attack(1);
					int rand_v = rand() % 2;
					if(rand_v == 0 && map_reach(x-1, y))
						t.move(2);
					else
						t.move(3);
				} else { // 害怕了 
					int k = rand() % 4;
					if(t.can_move(k)) {
						t.move(k);
					}
				}
			}
		}
		else {
			int k = rand() % 4;
			if(t.can_move(k)) {
				t.move(k);
			}
		}
	}
};

namespace Slime_ai_4 {
	//int think_time = 5;
	//map<string, int> mem;
	/*
	void init() {
		mem.clear();
	}
	*/
	void AI(Character &t, map<string, int> &mem) {
		if(mem.count("init") == 0) {
			mem["init"] = 1;
			// 模糊逻辑处理
			// 因为怪物有多波，所以目的是尽量消耗ai血量即可胜利。
			// 故怪物血量高时偏向于激进打法，血量低时采取稳健打法 
			for(int i = 0; i <= 2; i++) {mem["hpfuzzy safe"+TS(i)] = 0; mem["hpfuzzy unsafe"+TS(i)] = 10;}
			mem["hpfuzzy safe3"] = 1; mem["hpfuzzy unsafe3"] = 9;
			mem["hpfuzzy safe4"] = 3; mem["hpfuzzy unsafe4"] = 7;
			mem["hpfuzzy safe5"] = 5; mem["hpfuzzy unsafe5"] = 5;
			mem["hpfuzzy safe6"] = 7; mem["hpfuzzy unsafe6"] = 3;
			mem["hpfuzzy safe7"] = 9; mem["hpfuzzy unsafe7"] = 1;
			for(int i = 8; i <= 20; i++) {mem["hpfuzzy safe"+TS(i)] = 10; mem["hpfuzzy unsafe"+TS(i)] = 0;}
			for(int i = 0; i <= 2; i++) {mem["disfuzzy safe"+TS(i)] = 0; mem["disfuzzy unsafe"+TS(i)] = 10;}
			for(int i = 3; i <= 11; i++) {mem["disfuzzy safe"+TS(i)] = i-2; mem["disfuzzy unsafe"+TS(i)] = 12-i;}
			for(int i = 12; i <= 50; i++) {mem["disfuzzy safe"+TS(i)] = 10; mem["disfuzzy unsafe"+TS(i)] = 0;}
			mem["fuzzy safe ow"] = 4; 
			mem["fuzzy unsafe ow"] = 6;
			// midvalue : hp=5; dis=7;
		}
		int x = t.x, y = t.y;
		int xmin = 3,
			xmax = MAP_X-2,
			ymin = 3,
			ymax = MAP_Y-2;
		if(x == mem["target_x"] && y == mem["target_y"]) mem["target_number"] = 0;
		bool to_atk = 0;
		if(mem["target_number"] > 0) {
			if(mapC[mem["target_x"]][mem["target_y"]] == mem["target_number"])
				to_atk = 1;
			else {
				for(int i = xmin; i <= xmax; i++)
					for(int j = ymin; j <= ymax; j++)
						if(mapC[i][j] == mem["target_number"]) {
							mem["target_x"] = i;
							mem["target_y"] = j;
						}
			}
		}
		else {
			for(int i = xmin; i <= xmax; i++)
				for(int j = ymin; j <= ymax; j++)
					if(mapC[i][j] > 0 && character_team[mapC[i][j]] != t.team) {
						mem["target_number"] = mapC[i][j];
						mem["target_x"] = i;
						mem["target_y"] = j;
						//printf("Slime Found Target : %d,%d\n", mem["target_x"], mem["target_y"]);
						break;
					}
		}
		if(mem["target_number"] > 0) {
			int fhp = mem["your hp"],
				fdis = abs(x-mem["target_x"])+abs(y-mem["target_y"]);
			int fhpsafe = mem["hpfuzzy safe"+TS(fhp)],
				fhpunsafe = mem["hpfuzzy unsafe"+TS(fhp)],
				fdissafe = mem["disfuzzy safe"+TS(fdis)],
				fdisunsafe = mem["disfuzzy unsafe"+TS(fdis)];
			int output1 = 0,
				output2 = 0;
			output1 += std::min(fhpsafe, fdissafe) * mem["fuzzy safe ow"];
			output2 += std::min(fhpsafe, fdissafe);
			output1 += std::min(fhpsafe, fdisunsafe) * ((fhpsafe <= fdisunsafe)?(mem["fuzzy safe ow"]):(mem["fuzzy unsafe ow"]));
			output2 += std::min(fhpsafe, fdisunsafe);
			output1 += std::min(fhpunsafe, fdissafe) * ((fhpunsafe <= fdissafe)?(mem["fuzzy safe ow"]):(mem["fuzzy unsafe ow"]));
			output2 += std::min(fhpunsafe, fdissafe);
			output1 += std::min(fhpunsafe, fdisunsafe) * mem["fuzzy unsafe ow"];
			output2 += std::min(fhpunsafe, fdisunsafe);
			int output = output1/output2;
			//printf("output = %d\n", output);
			if(output <= 5) {
				//printf("Getting\n");
				if(mem["target_x"] == x) {
					if(mem["target_y"] < y)
						t.turn(0);
					else
						t.turn(1);
					if(to_atk)
						t.attack(1);
				}
				else if(mem["target_y"] == y) {
					if(mem["target_x"] < x)
						t.turn(2);
					else
						t.turn(3);
					if(to_atk)
						t.attack(1);
				}
				else {
					int x_ = abs(x - mem["target_x"]),
						y_ = abs(y - mem["target_y"]);
					if(y_ <= x_) {
						if(mem["target_y"] < y)
							t.turn(0);
						else
							t.turn(1);
					}
					else {
						if(mem["target_x"] < x)
							t.turn(2);
						else
							t.turn(3);
					}
				}
				if(t.can_move() == 1)
					t.move(t.direct); 
			} else {
				if(y == mem["target_y"]) {
					if(x < mem["target_x"])
						t.turn(3), t.attack(1);
					else
						t.turn(2), t.attack(1);
					int rand_v = rand() % 2;
					if(rand_v == 0 && map_reach(x, y-1))
						t.move(0);
					else
						t.move(1);
				} else if(x == mem["target_x"]) {
					if(y < mem["target_y"])
						t.turn(1), t.attack(1);
					else
						t.turn(0), t.attack(1);
					int rand_v = rand() % 2;
					if(rand_v == 0 && map_reach(x-1, y))
						t.move(2);
					else
						t.move(3);
				} else { // 害怕了 
					int k = rand() % 4;
					if(t.can_move(k)) {
						t.move(k);
					}
				}
			}
		}
		else {
			int k = rand() % 4;
			if(t.can_move(k)) {
				t.move(k);
			}
		}
	}
};

namespace QLearning_AI_slime {
	void AI(Character &t, map<string, int> &mem) {
		// 初始化
		if(mem.count("init") == 0) {
			mem["init"] = 1;
			mem["stay timer move"] = 0;
			mem["stay timer attack"] = 0;
			register int i, j;
			// 学习模式
			int run_mode = 4; // 1:训练模式; 2:测试模式(成果检测); 3:纯随机模式; 4:微训练模式 
			if(run_mode == 1) {
				mem["epsilon"] = 50; // 百分值 
				mem["alpha"] = 50;
				mem["gamma"] = 100;
			} else if(run_mode == 2) {
				mem["epsilon"] = 0;
				mem["alpha"] = 0;
				mem["gamma"] = 100;
			} else if(run_mode == 3) {
				mem["epsilon"] = 100;
				mem["alpha"] = 0;
				mem["gamma"] = 100;
			} else if(run_mode == 4) {
				mem["epsilon"] = 20;
				mem["alpha"] = 20;
				mem["gamma"] = 100;
			}
			// 视野范围对应表格
			for(i = 1; i <= 17; i++) {mem["mx"+TS(i)] = -1; mem["my"+TS(i)] = i-9;} 
			for(i = 18; i <= 34; i++) {mem["mx"+TS(i)] = 0; mem["my"+TS(i)] = i-26;} 
			for(i = 35; i <= 51; i++) {mem["mx"+TS(i)] = 1; mem["my"+TS(i)] = i-43;} 
			for(i = 52; i <= 58; i++) {mem["mx"+TS(i)] = i-60; mem["my"+TS(i)] = -1;} 
			for(i = 59; i <= 65; i++) {mem["mx"+TS(i)] = i-57; mem["my"+TS(i)] = -1;} 
			for(i = 66; i <= 72; i++) {mem["mx"+TS(i)] = i-74; mem["my"+TS(i)] = 0;} 
			for(i = 73; i <= 79; i++) {mem["mx"+TS(i)] = i-71; mem["my"+TS(i)] = 0;}
			for(i = 80; i <= 86; i++) {mem["mx"+TS(i)] = i-88; mem["my"+TS(i)] = 1;} 
			for(i = 87; i <= 93; i++) {mem["mx"+TS(i)] = i-85; mem["my"+TS(i)] = 1;}
			mem["mx94"] = -2; mem["my94"] = -2;
			mem["mx95"] = 2; mem["my95"] = -2;
			mem["mx96"] = -2; mem["my96"] = 2;
			mem["mx97"] = 2; mem["my97"] = 2;
			for(i = 1; i <= 97; i++)
				mem["to_id"+TS(mem["mx"+TS(i)])+","+TS(mem["my"+TS(i)])] = i; 
			// R 表格
			/*
			for(i = 1; i <= 97; i++)
				mem["Rm"+TS(i)+","+TS(0)] = 0,
				mem["Ra"+TS(i)+","+TS(0)] = 0;
			for(i = 1; i <= 97; i++)
				mem["Rm"+TS(i)+","+TS(1)] = mem["Rm"+TS(i)+","+TS(2)] = mem["Rm"+TS(i)+","+TS(3)] = mem["Rm"+TS(i)+","+TS(4)] = 0;
			for(i = 1; i <= 97; i++) {
				if((i >= 2 && i <= 16) || (i >= 22 && i <= 24))
					mem["Ra"+TS(i)+","+TS(1)] = 10;
				if((i >= 55 && i <= 69) || (i >= 47 && i <= 49))
					mem["Ra"+TS(i)+","+TS(2)] = 10;
				if((i >= 17 && i <= 22) || (i >= 30 && i <= 35) || (i >= 42 && i <= 47))
					mem["Ra"+TS(i)+","+TS(3)] = 10;
				if((i >= 24 && i <= 29) || (i >= 36 && i <= 41) || (i >= 49 && i <= 54))
					mem["Ra"+TS(i)+","+TS(4)] = 10;
			}
			*/
			// Q 表格 
			std::ifstream input;
			input.open("ai_memory_monster_slime.txt", std::ios::in);
			if(input.is_open()) {
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						input >> mem["Qm"+TS(i)+","+TS(j)];
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						input >> mem["Qa"+TS(i)+","+TS(j)];
			} else {
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						mem["Qm"+TS(i)+","+TS(j)] = 0;
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						mem["Qa"+TS(i)+","+TS(j)] = 0;
			}
			input.close();
		}
		// 刷新
		int iNumber = mem["your number"], // 读取ai操控角色编号,坐标,血量,方向
			iX = mem["your x"],
			iY = mem["your y"],
			iHP = mem["your hp"],
			iTeam = mem["your team"],
			iDirect = mem["your direct"];  
		int player_sum = mem_all["cha sum"]; // 读取玩家数量
		int player_hp[player_sum+1], // 读取所有玩家信息 
			player_x[player_sum+1],
			player_y[player_sum+1],
			player_team[player_sum+1]; 
		for(int i = 1; i <= player_sum; i++)
			player_hp[i] = mem_all["cha "+TS(i)+" hp"],
			player_x[i] = mem_all["cha "+TS(i)+" x"],
			player_y[i] = mem_all["cha "+TS(i)+" y"],
			player_team[i] = mem_all["cha "+TS(i)+" team"];
		if(mem["stay timer move"] > 0) mem["stay timer move"] -= 1;
		if(mem["stay timer attack"] > 0) mem["stay timer attack"] -= 1;
		// --- 处理事件 --- 
		// 延迟奖励：对战游戏中，奖励均存在一定的延迟性。
		//           算法无法即时了解做出动作的优劣，所以引入延迟奖励，在命中敌人/被敌人命中之后，进行学习。 
		int event_sum = mem_all["event sum"];
		//printf("event = %d\n", event_sum);
		for(int i = 1; i <= event_sum; i++) {
			//printf("[event kind = %d]\n", mem_all["event "+TS(i)]);
			if(mem_all["event "+TS(i)] == 1) {
				if(mem_all["event "+TS(i)+" B"] == iNumber) { // "我"被击中 
					mem["last move"] = 0;
					int action = mem["last move action"],
						state = mem["last move state"];
					//	state_next = mem["last move state_next"];
					//int min_q = 1e8;
					//for(int i = 0; i <= 4; i++) min_q = std::min(min_q, mem["Qm"+TS(state_next)+","+TS(i)]);
					// 因为移动行为没有初始Rewards表格，所以QLearning公式对应做出改变 
					mem["Qm"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qm"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (5000);  
				}
				if(mem_all["event "+TS(i)+" A"] == iNumber && mem_all["event "+TS(i)+" B"] == mem["last attack target"]) { // "我"击中敌人 
					mem["last attack"] = 0;
					int action = mem["last attack action"],
						state = mem["last attack state"];
					printf("state = %d; action = %d; hit!\n", state, action);
					// 因为攻击行为对状态没有影响 所以QLearning公式对应做出改变 
					//printf("count\n");
					mem["Qa"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qa"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (5000);
				}
			} else if(mem_all["event "+TS(i)] == 2) {
				//printf("sd\n");
				if(mem_all["event "+TS(i)+" A"] == iNumber) { // 本人死亡，保存数据 
					std::ofstream output("ai_memory_monster_slime.txt", std::ios::out);
					printf("saved\n");
					register int i, j;
					for(i = 1; i <= 97; i++) {
						for(j = 0; j <= 4; j++)
							output << mem["Qm"+TS(i)+","+TS(j)] << " ";
						output << "\n";
					}
					output << "\n";
					for(i = 1; i <= 97; i++) {
						for(j = 0; j <= 4; j++)
							output << mem["Qa"+TS(i)+","+TS(j)] << " ";
						output << "\n";
					}
					output.close();
				}
			}
		}
		// 无法操作 则跳过本次行动
		if(!t.can_move() && !t.can_attack(1)) return;
		// 操作 
		
		// 确定目标 
		int tar_num = 0, tar_x = 0, tar_y = 0, tar_dx = 0, tar_dy = 0,
			near_num = 0, near_x = 999, near_y = 999, near_x_delta = 999, near_y_delta = 999;
		// 注意 tar_x 储存的是相对距离 
		for(int i = 1; i <= player_sum; i++)
			if(iTeam != player_team[i] && player_hp[i] > 0) { // 玩家不是自己 并且 存活 则 选定为目标 
				if((near_x_delta + near_y_delta) > (abs(player_x[i] - iX) + abs(player_y[i] - iY))) {
					near_num = i;
					near_x = player_x[i];
					near_y = player_y[i];
					near_x_delta = abs(player_x[i] - iX);
					near_y_delta = abs(player_y[i] - iY);
				}
				int dx = player_x[i] - iX,
					dy = player_y[i] - iY;
				if(dx < -5 || dy < -5 || dx > 5 || dy > 5) continue; // 超出视野距离 
				if(mem["to_id"+TS(dx)+","+TS(dy)] == 0) continue;
				if(tar_num == 0) {
					tar_num = i;
					tar_x = player_x[i];
					tar_y = player_y[i];
					tar_dx = dx;
					tar_dy = dy;
				} else if((abs(tar_x - iX) + abs(tar_y - iY)) > (abs(player_x[i] - iX) + abs(player_y[i] - iY))) {
					tar_num = i;
					tar_x = player_x[i];
					tar_y = player_y[i];
					tar_dx = dx;
					tar_dy = dy;
				}
			}
		//printf("find target (%d in %d,%d)\n", tar_num, tar_x, tar_y);
		if(tar_num == 0) { // 视野内无目标 
			if(near_num == 0) {
				//printf("NO TARGET FOUND!\n"); 
				return;
			}
			if(t.can_move()) {
				// 接近最近目标
				if(abs(near_x - iX) <= abs(near_y - iY)) {
					if(near_x < iX)
						t.move(2);
					else if(iX < near_x)
						t.move(3);
					else if(near_y < iY)
						t.move(0);
					else
						t.move(1); 
				} else {
					if(near_y < iY)
						t.move(0);
					else if(iY < near_y)
						t.move(1);
					else if(near_x < iX)
						t.move(2);
					else 
						t.move(3);
				}
			}
			if(t.can_attack(1)) {
				if(abs(near_x - iX) <= abs(near_y - iY)) {
					if(near_y < iY) {
						t.turn(0);
						t.attack(1);
					} else {
						t.turn(1);
						t.attack(1);
					}
				} else {
					if(near_x < iX) {
						t.turn(2);
						t.attack(1);
					} else {
						t.turn(3);
						t.attack(1);
					}
				}
			}
		} else { // 视野内存在目标 (此时可以进行移动或攻击(或皆可))
			int state = mem["to_id"+TS(tar_dx)+","+TS(tar_dy)];
			bool explore = (rand()%100 < mem["epsilon"]);
			// --- 移动 ---  
			if(t.can_move() == 1 && mem["stay timer move"] == 0) {
				if(mem["last move"] == 1) {
					mem["last move"] = 0;
					int action = mem["last move action"],
						state = mem["last move state"];
					//	state_next = mem["last move state_next"];
					//int min_q = 1e8;
					//for(int i = 0; i <= 4; i++) min_q = std::min(min_q, mem["Qm"+TS(state_next)+","+TS(i)]);
					// 因为移动行为没有初始Rewards表格，所以QLearning公式对应做出改变 
					mem["Qm"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qm"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (-10); 
				}
				int action;
				if(explore == 1)
					action = rand()%5;
				else {
					int action_q = 1e8;
					for(int i = 0; i <= 3; i++) {
						if(t.can_move(i) == 1 && action_q > mem["Qm"+TS(state)+","+TS(i)]) {
							action_q = mem["Qm"+TS(state)+","+TS(i)];
							action = i;
						}
					}
					if(action_q > mem["Qm"+TS(state)+","+TS(4)]) {
						action = 4;
					}
					if(action_q == 1e8)
						printf("action choosing error.\n");
				}
				if(action == 4) {
					mem["stay timer move"] = 9;
				} else {
					if(t.move(action) == false)
						;//printf("fail action.\n");
				}
				//int state_next;
				//if(action == 0) state_next = mem["to_id"+TS(tar_x)+","+TS(tar_y+1)];
				//else if(action == 1) state_next = mem["to_id"+TS(tar_x)+","+TS(tar_y-1)];
				//else if(action == 2) state_next = mem["to_id"+TS(tar_x+1)+","+TS(tar_y)];
				//else if(action == 3) state_next = mem["to_id"+TS(tar_x-1)+","+TS(tar_y)];
				//else state_next = state;
				mem["last move state"] = state;
				mem["last move action"] = action;
				//mem["last move state_next"] = state_next;
				mem["last move"] = 1;
			}
			// --- 攻击 --- 
			if(t.can_attack(1) && mem["stay timer attack"] == 0) {
				if(mem["last attack"] == 1) {
					mem["last attack"] = 0;
					int action = mem["last attack action"],
						state = mem["last attack state"];
					// 因为攻击行为对状态没有影响 所以QLearning公式对应做出改变 
					mem["Qa"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qa"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (0);
				}
				int action;
				if(explore == 1)
					action = rand()%5;
				else {
					int action_q = -1;
					for(int i = 0; i <= 4; i++)
						if(action_q < mem["Qa"+TS(state)+","+TS(i)]) {
							action_q = mem["Qa"+TS(state)+","+TS(i)];
							action = i;
						}
				}
				if(action == 4) {
					mem["stay timer attack"] = 9;
				} else {
					t.turn(action);
					t.attack(1);
				}
				mem["last attack state"] = state;
				mem["last attack action"] = action;
				mem["last attack target"] = tar_num;
				mem["last attack"] = 1;
			}
		}
	}
} 

namespace QLearning_AI_archer {
	void AI(Character &t, map<string, int> &mem) {
		// 初始化
		if(mem.count("init") == 0) {
			mem["init"] = 1;
			mem["stay timer move"] = 0;
			mem["stay timer attack"] = 0;
			register int i, j;
			// 学习模式
			int run_mode = 4; // 1:训练模式; 2:测试模式(成果检测); 3:纯随机模式; 4:微训练模式 
			if(run_mode == 1) {
				mem["epsilon"] = 50; // 百分值 
				mem["alpha"] = 50;
				mem["gamma"] = 100;
			} else if(run_mode == 2) {
				mem["epsilon"] = 0;
				mem["alpha"] = 0;
				mem["gamma"] = 100;
			} else if(run_mode == 3) {
				mem["epsilon"] = 100;
				mem["alpha"] = 0;
				mem["gamma"] = 100;
			} else if(run_mode == 4) {
				mem["epsilon"] = 20;
				mem["alpha"] = 20;
				mem["gamma"] = 100;
			}
			// 视野范围对应表格
			for(i = 1; i <= 17; i++) {mem["mx"+TS(i)] = -1; mem["my"+TS(i)] = i-9;} 
			for(i = 18; i <= 34; i++) {mem["mx"+TS(i)] = 0; mem["my"+TS(i)] = i-26;} 
			for(i = 35; i <= 51; i++) {mem["mx"+TS(i)] = 1; mem["my"+TS(i)] = i-43;} 
			for(i = 52; i <= 58; i++) {mem["mx"+TS(i)] = i-60; mem["my"+TS(i)] = -1;} 
			for(i = 59; i <= 65; i++) {mem["mx"+TS(i)] = i-57; mem["my"+TS(i)] = -1;} 
			for(i = 66; i <= 72; i++) {mem["mx"+TS(i)] = i-74; mem["my"+TS(i)] = 0;} 
			for(i = 73; i <= 79; i++) {mem["mx"+TS(i)] = i-71; mem["my"+TS(i)] = 0;}
			for(i = 80; i <= 86; i++) {mem["mx"+TS(i)] = i-88; mem["my"+TS(i)] = 1;} 
			for(i = 87; i <= 93; i++) {mem["mx"+TS(i)] = i-85; mem["my"+TS(i)] = 1;}
			mem["mx94"] = -2; mem["my94"] = -2;
			mem["mx95"] = 2; mem["my95"] = -2;
			mem["mx96"] = -2; mem["my96"] = 2;
			mem["mx97"] = 2; mem["my97"] = 2;
			for(i = 1; i <= 97; i++)
				mem["to_id"+TS(mem["mx"+TS(i)])+","+TS(mem["my"+TS(i)])] = i; 
			// R 表格
			/*
			for(i = 1; i <= 97; i++)
				mem["Rm"+TS(i)+","+TS(0)] = 0,
				mem["Ra"+TS(i)+","+TS(0)] = 0;
			for(i = 1; i <= 97; i++)
				mem["Rm"+TS(i)+","+TS(1)] = mem["Rm"+TS(i)+","+TS(2)] = mem["Rm"+TS(i)+","+TS(3)] = mem["Rm"+TS(i)+","+TS(4)] = 0;
			for(i = 1; i <= 97; i++) {
				if((i >= 2 && i <= 16) || (i >= 22 && i <= 24))
					mem["Ra"+TS(i)+","+TS(1)] = 10;
				if((i >= 55 && i <= 69) || (i >= 47 && i <= 49))
					mem["Ra"+TS(i)+","+TS(2)] = 10;
				if((i >= 17 && i <= 22) || (i >= 30 && i <= 35) || (i >= 42 && i <= 47))
					mem["Ra"+TS(i)+","+TS(3)] = 10;
				if((i >= 24 && i <= 29) || (i >= 36 && i <= 41) || (i >= 49 && i <= 54))
					mem["Ra"+TS(i)+","+TS(4)] = 10;
			}
			*/
			// Q 表格 
			std::ifstream input;
			input.open("ai_memory_monster_archer.txt", std::ios::in);
			if(input.is_open()) {
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						input >> mem["Qm"+TS(i)+","+TS(j)];
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						input >> mem["Qa"+TS(i)+","+TS(j)];
			} else {
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						mem["Qm"+TS(i)+","+TS(j)] = 0;
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						mem["Qa"+TS(i)+","+TS(j)] = 0;
			}
			input.close();
		}
		// 刷新
		int iNumber = mem["your number"], // 读取ai操控角色编号,坐标,血量,方向
			iX = mem["your x"],
			iY = mem["your y"],
			iHP = mem["your hp"],
			iTeam = mem["your team"],
			iDirect = mem["your direct"];  
		int player_sum = mem_all["cha sum"]; // 读取玩家数量
		int player_hp[player_sum+1], // 读取所有玩家信息 
			player_x[player_sum+1],
			player_y[player_sum+1],
			player_team[player_sum+1]; 
		for(int i = 1; i <= player_sum; i++)
			player_hp[i] = mem_all["cha "+TS(i)+" hp"],
			player_x[i] = mem_all["cha "+TS(i)+" x"],
			player_y[i] = mem_all["cha "+TS(i)+" y"],
			player_team[i] = mem_all["cha "+TS(i)+" team"];
		if(mem["stay timer move"] > 0) mem["stay timer move"] -= 1;
		if(mem["stay timer attack"] > 0) mem["stay timer attack"] -= 1;
		// --- 处理事件 --- 
		// 延迟奖励：对战游戏中，奖励均存在一定的延迟性。
		//           算法无法即时了解做出动作的优劣，所以引入延迟奖励，在命中敌人/被敌人命中之后，进行学习。 
		int event_sum = mem_all["event sum"];
		//printf("event = %d\n", event_sum);
		for(int i = 1; i <= event_sum; i++) {
			//printf("[event kind = %d]\n", mem_all["event "+TS(i)]);
			if(mem_all["event "+TS(i)] == 1) {
				if(mem_all["event "+TS(i)+" B"] == iNumber) { // "我"被击中 
					mem["last move"] = 0;
					int action = mem["last move action"],
						state = mem["last move state"];
					//	state_next = mem["last move state_next"];
					//int min_q = 1e8;
					//for(int i = 0; i <= 4; i++) min_q = std::min(min_q, mem["Qm"+TS(state_next)+","+TS(i)]);
					// 因为移动行为没有初始Rewards表格，所以QLearning公式对应做出改变 
					mem["Qm"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qm"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (5000);  
				}
				if(mem_all["event "+TS(i)+" A"] == iNumber && mem_all["event "+TS(i)+" B"] == mem["last attack target"]) { // "我"击中敌人 
					mem["last attack"] = 0;
					int action = mem["last attack action"],
						state = mem["last attack state"];
					printf("state = %d; action = %d; hit!\n", state, action);
					// 因为攻击行为对状态没有影响 所以QLearning公式对应做出改变 
					//printf("count\n");
					mem["Qa"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qa"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (5000);
				}
			} else if(mem_all["event "+TS(i)] == 2) {
				//printf("sd\n");
				if(mem_all["event "+TS(i)+" A"] == iNumber) { // 本人死亡，保存数据 
					std::ofstream output("ai_memory_monster_archer.txt", std::ios::out);
					printf("saved\n");
					register int i, j;
					for(i = 1; i <= 97; i++) {
						for(j = 0; j <= 4; j++)
							output << mem["Qm"+TS(i)+","+TS(j)] << " ";
						output << "\n";
					}
					output << "\n";
					for(i = 1; i <= 97; i++) {
						for(j = 0; j <= 4; j++)
							output << mem["Qa"+TS(i)+","+TS(j)] << " ";
						output << "\n";
					}
					output.close();
				}
			}
		}
		// 无法操作 则跳过本次行动
		if(!t.can_move() && !t.can_attack(1)) return;
		// 操作 
		
		// 确定目标 
		int tar_num = 0, tar_x = 0, tar_y = 0, tar_dx = 0, tar_dy = 0,
			near_num = 0, near_x = 999, near_y = 999, near_x_delta = 999, near_y_delta = 999;
		// 注意 tar_x 储存的是相对距离 
		for(int i = 1; i <= player_sum; i++)
		///////////////////////////////////////////////////////////////////////
			if(iTeam != player_team[i] && player_hp[i] > 0) { // 玩家不是自己 并且 存活 则 选定为目标 
				if((near_x_delta + near_y_delta) > (abs(player_x[i] - iX) + abs(player_y[i] - iY))) {
					near_num = i;
					near_x = player_x[i];
					near_y = player_y[i];
					near_x_delta = abs(player_x[i] - iX);
					near_y_delta = abs(player_y[i] - iY);
				}
				int dx = player_x[i] - iX,
					dy = player_y[i] - iY;
				if(dx < -8 || dy < -8 || dx > 8 || dy > 8) continue; // 超出视野距离 
				if(mem["to_id"+TS(dx)+","+TS(dy)] == 0) continue;
				if(tar_num == 0) {
					tar_num = i;
					tar_x = player_x[i];
					tar_y = player_y[i];
					tar_dx = dx;
					tar_dy = dy;
				} else if((abs(tar_x - iX) + abs(tar_y - iY)) > (abs(player_x[i] - iX) + abs(player_y[i] - iY))) {
					tar_num = i;
					tar_x = player_x[i];
					tar_y = player_y[i];
					tar_dx = dx;
					tar_dy = dy;
				}
			}
		//printf("find target (%d in %d,%d)\n", tar_num, tar_x, tar_y);
		if(tar_num == 0) { // 视野内无目标 
			if(near_num == 0) {
				//printf("NO TARGET FOUND!\n"); 
				return;
			}
			if(t.can_move()) {
				// 接近最近目标
				if(abs(near_x - iX) <= abs(near_y - iY)) {
					if(near_x < iX)
						t.move(2);
					else if(iX < near_x)
						t.move(3);
					else if(near_y < iY)
						t.move(0);
					else
						t.move(1); 
				} else {
					if(near_y < iY)
						t.move(0);
					else if(iY < near_y)
						t.move(1);
					else if(near_x < iX)
						t.move(2);
					else 
						t.move(3);
				}
			}
			if(t.can_attack(1)) {
				if(abs(near_x - iX) <= abs(near_y - iY)) {
					if(near_y < iY) {
						t.turn(0);
						t.attack(1);
					} else {
						t.turn(1);
						t.attack(1);
					}
				} else {
					if(near_x < iX) {
						t.turn(2);
						t.attack(1);
					} else {
						t.turn(3);
						t.attack(1);
					}
				}
			}
		} else { // 视野内存在目标 (此时可以进行移动或攻击(或皆可))
			int state = mem["to_id"+TS(tar_dx)+","+TS(tar_dy)];
			bool explore = (rand()%100 < mem["epsilon"]);
			// --- 移动 ---  
			if(t.can_move() == 1 && mem["stay timer move"] == 0) {
				if(mem["last move"] == 1) {
					mem["last move"] = 0;
					int action = mem["last move action"],
						state = mem["last move state"];
					//	state_next = mem["last move state_next"];
					//int min_q = 1e8;
					//for(int i = 0; i <= 4; i++) min_q = std::min(min_q, mem["Qm"+TS(state_next)+","+TS(i)]);
					// 因为移动行为没有初始Rewards表格，所以QLearning公式对应做出改变 
					mem["Qm"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qm"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (-10); 
				}
				int action;
				if(explore == 1)
					action = rand()%5;
				else {
					int action_q = 1e8;
					for(int i = 0; i <= 3; i++) {
						if(t.can_move(i) == 1 && action_q > mem["Qm"+TS(state)+","+TS(i)]) {
							action_q = mem["Qm"+TS(state)+","+TS(i)];
							action = i;
						}
					}
					if(action_q > mem["Qm"+TS(state)+","+TS(4)]) {
						action = 4;
					}
					if(action_q == 1e8)
						printf("action choosing error.\n");
				}
				if(action == 4) {
					mem["stay timer move"] = 9;
				} else {
					if(t.move(action) == false)
						;//printf("fail action.\n");
				}
				//int state_next;
				//if(action == 0) state_next = mem["to_id"+TS(tar_x)+","+TS(tar_y+1)];
				//else if(action == 1) state_next = mem["to_id"+TS(tar_x)+","+TS(tar_y-1)];
				//else if(action == 2) state_next = mem["to_id"+TS(tar_x+1)+","+TS(tar_y)];
				//else if(action == 3) state_next = mem["to_id"+TS(tar_x-1)+","+TS(tar_y)];
				//else state_next = state;
				mem["last move state"] = state;
				mem["last move action"] = action;
				//mem["last move state_next"] = state_next;
				mem["last move"] = 1;
			}
			// --- 攻击 --- 
			if(t.can_attack(1) && mem["stay timer attack"] == 0) {
				if(mem["last attack"] == 1) {
					mem["last attack"] = 0;
					int action = mem["last attack action"],
						state = mem["last attack state"];
					// 因为攻击行为对状态没有影响 所以QLearning公式对应做出改变 
					mem["Qa"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qa"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (0);
				}
				int action;
				if(explore == 1)
					action = rand()%5;
				else {
					int action_q = -1;
					for(int i = 0; i <= 4; i++)
						if(action_q < mem["Qa"+TS(state)+","+TS(i)]) {
							action_q = mem["Qa"+TS(state)+","+TS(i)];
							action = i;
						}
				}
				if(action == 4) {
					mem["stay timer attack"] = 9;
				} else {
					t.turn(action);
					t.attack(1);
				}
				mem["last attack state"] = state;
				mem["last attack action"] = action;
				mem["last attack target"] = tar_num;
				mem["last attack"] = 1;
			}
		}
	}
} 

namespace QLearning_AI_hunter {
	void AI(Character &t, map<string, int> &mem) {
		// 初始化
		if(mem.count("init") == 0) {
			mem["init"] = 1;
			mem["stay timer move"] = 0;
			mem["stay timer attack"] = 0;
			register int i, j;
			// 学习模式
			int run_mode = 4; // 1:训练模式; 2:测试模式(成果检测); 3:纯随机模式; 4:微训练模式 
			if(run_mode == 1) {
				mem["epsilon"] = 50; // 百分值 
				mem["alpha"] = 50;
				mem["gamma"] = 100;
			} else if(run_mode == 2) {
				mem["epsilon"] = 0;
				mem["alpha"] = 0;
				mem["gamma"] = 100;
			} else if(run_mode == 3) {
				mem["epsilon"] = 100;
				mem["alpha"] = 0;
				mem["gamma"] = 100;
			} else if(run_mode == 4) {
				mem["epsilon"] = 20;
				mem["alpha"] = 20;
				mem["gamma"] = 100;
			}
			// 视野范围对应表格
			for(i = 1; i <= 17; i++) {mem["mx"+TS(i)] = -1; mem["my"+TS(i)] = i-9;} 
			for(i = 18; i <= 34; i++) {mem["mx"+TS(i)] = 0; mem["my"+TS(i)] = i-26;} 
			for(i = 35; i <= 51; i++) {mem["mx"+TS(i)] = 1; mem["my"+TS(i)] = i-43;} 
			for(i = 52; i <= 58; i++) {mem["mx"+TS(i)] = i-60; mem["my"+TS(i)] = -1;} 
			for(i = 59; i <= 65; i++) {mem["mx"+TS(i)] = i-57; mem["my"+TS(i)] = -1;} 
			for(i = 66; i <= 72; i++) {mem["mx"+TS(i)] = i-74; mem["my"+TS(i)] = 0;} 
			for(i = 73; i <= 79; i++) {mem["mx"+TS(i)] = i-71; mem["my"+TS(i)] = 0;}
			for(i = 80; i <= 86; i++) {mem["mx"+TS(i)] = i-88; mem["my"+TS(i)] = 1;} 
			for(i = 87; i <= 93; i++) {mem["mx"+TS(i)] = i-85; mem["my"+TS(i)] = 1;}
			mem["mx94"] = -2; mem["my94"] = -2;
			mem["mx95"] = 2; mem["my95"] = -2;
			mem["mx96"] = -2; mem["my96"] = 2;
			mem["mx97"] = 2; mem["my97"] = 2;
			for(i = 1; i <= 97; i++)
				mem["to_id"+TS(mem["mx"+TS(i)])+","+TS(mem["my"+TS(i)])] = i; 
			// R 表格
			/*
			for(i = 1; i <= 97; i++)
				mem["Rm"+TS(i)+","+TS(0)] = 0,
				mem["Ra"+TS(i)+","+TS(0)] = 0;
			for(i = 1; i <= 97; i++)
				mem["Rm"+TS(i)+","+TS(1)] = mem["Rm"+TS(i)+","+TS(2)] = mem["Rm"+TS(i)+","+TS(3)] = mem["Rm"+TS(i)+","+TS(4)] = 0;
			for(i = 1; i <= 97; i++) {
				if((i >= 2 && i <= 16) || (i >= 22 && i <= 24))
					mem["Ra"+TS(i)+","+TS(1)] = 10;
				if((i >= 55 && i <= 69) || (i >= 47 && i <= 49))
					mem["Ra"+TS(i)+","+TS(2)] = 10;
				if((i >= 17 && i <= 22) || (i >= 30 && i <= 35) || (i >= 42 && i <= 47))
					mem["Ra"+TS(i)+","+TS(3)] = 10;
				if((i >= 24 && i <= 29) || (i >= 36 && i <= 41) || (i >= 49 && i <= 54))
					mem["Ra"+TS(i)+","+TS(4)] = 10;
			}
			*/
			// Q 表格 
			std::ifstream input;
			input.open("ai_memory_monster_hunter.txt", std::ios::in);
			if(input.is_open()) {
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						input >> mem["Qm"+TS(i)+","+TS(j)];
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						input >> mem["Qa"+TS(i)+","+TS(j)];
			} else {
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						mem["Qm"+TS(i)+","+TS(j)] = 0;
				for(i = 1; i <= 97; i++)
					for(j = 0; j <= 4; j++)
						mem["Qa"+TS(i)+","+TS(j)] = 0;
			}
			input.close();
		}
		// 刷新
		int iNumber = mem["your number"], // 读取ai操控角色编号,坐标,血量,方向
			iX = mem["your x"],
			iY = mem["your y"],
			iHP = mem["your hp"],
			iTeam = mem["your team"],
			iDirect = mem["your direct"];  
		int player_sum = mem_all["cha sum"]; // 读取玩家数量
		int player_hp[player_sum+1], // 读取所有玩家信息 
			player_x[player_sum+1],
			player_y[player_sum+1],
			player_team[player_sum+1]; 
		for(int i = 1; i <= player_sum; i++)
			player_hp[i] = mem_all["cha "+TS(i)+" hp"],
			player_x[i] = mem_all["cha "+TS(i)+" x"],
			player_y[i] = mem_all["cha "+TS(i)+" y"],
			player_team[i] = mem_all["cha "+TS(i)+" team"];
		if(mem["stay timer move"] > 0) mem["stay timer move"] -= 1;
		if(mem["stay timer attack"] > 0) mem["stay timer attack"] -= 1;
		// --- 处理事件 --- 
		// 延迟奖励：对战游戏中，奖励均存在一定的延迟性。
		//           算法无法即时了解做出动作的优劣，所以引入延迟奖励，在命中敌人/被敌人命中之后，进行学习。 
		int event_sum = mem_all["event sum"];
		//printf("event = %d\n", event_sum);
		for(int i = 1; i <= event_sum; i++) {
			//printf("[event kind = %d]\n", mem_all["event "+TS(i)]);
			if(mem_all["event "+TS(i)] == 1) {
				if(mem_all["event "+TS(i)+" B"] == iNumber) { // "我"被击中 
					mem["last move"] = 0;
					int action = mem["last move action"],
						state = mem["last move state"];
					//	state_next = mem["last move state_next"];
					//int min_q = 1e8;
					//for(int i = 0; i <= 4; i++) min_q = std::min(min_q, mem["Qm"+TS(state_next)+","+TS(i)]);
					// 因为移动行为没有初始Rewards表格，所以QLearning公式对应做出改变 
					mem["Qm"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qm"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (5000);  
				}
				if(mem_all["event "+TS(i)+" A"] == iNumber && mem_all["event "+TS(i)+" B"] == mem["last attack target"]) { // "我"击中敌人 
					mem["last attack"] = 0;
					int action = mem["last attack action"],
						state = mem["last attack state"];
					printf("state = %d; action = %d; hit!\n", state, action);
					// 因为攻击行为对状态没有影响 所以QLearning公式对应做出改变 
					//printf("count\n");
					mem["Qa"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qa"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (5000);
				}
			} else if(mem_all["event "+TS(i)] == 2) {
				//printf("sd\n");
				if(mem_all["event "+TS(i)+" A"] == iNumber) { // 本人死亡，保存数据 
					std::ofstream output("ai_memory_monster_hunter.txt", std::ios::out);
					printf("saved\n");
					register int i, j;
					for(i = 1; i <= 97; i++) {
						for(j = 0; j <= 4; j++)
							output << mem["Qm"+TS(i)+","+TS(j)] << " ";
						output << "\n";
					}
					output << "\n";
					for(i = 1; i <= 97; i++) {
						for(j = 0; j <= 4; j++)
							output << mem["Qa"+TS(i)+","+TS(j)] << " ";
						output << "\n";
					}
					output.close();
				}
			}
		}
		// 无法操作 则跳过本次行动
		if(!t.can_move() && !t.can_attack(2)) return;
		// 操作 
		
		// 确定目标 
		int tar_num = 0, tar_x = 0, tar_y = 0, tar_dx = 0, tar_dy = 0,
			near_num = 0, near_x = 999, near_y = 999, near_x_delta = 999, near_y_delta = 999;
		// 注意 tar_x 储存的是相对距离 
		for(int i = 1; i <= player_sum; i++)
			if(iTeam != player_team[i] && player_hp[i] > 0) { // 玩家不是自己 并且 存活 则 选定为目标 
				if((near_x_delta + near_y_delta) > (abs(player_x[i] - iX) + abs(player_y[i] - iY))) {
					near_num = i;
					near_x = player_x[i];
					near_y = player_y[i];
					near_x_delta = abs(player_x[i] - iX);
					near_y_delta = abs(player_y[i] - iY);
				}
				int dx = player_x[i] - iX,
					dy = player_y[i] - iY;
				if(dx < -8 || dy < -8 || dx > 8 || dy > 8) continue; // 超出视野距离 
				if(mem["to_id"+TS(dx)+","+TS(dy)] == 0) continue;
				if(tar_num == 0) {
					tar_num = i;
					tar_x = player_x[i];
					tar_y = player_y[i];
					tar_dx = dx;
					tar_dy = dy;
				} else if((abs(tar_x - iX) + abs(tar_y - iY)) > (abs(player_x[i] - iX) + abs(player_y[i] - iY))) {
					tar_num = i;
					tar_x = player_x[i];
					tar_y = player_y[i];
					tar_dx = dx;
					tar_dy = dy;
				}
			}
		//printf("find target (%d in %d,%d)\n", tar_num, tar_x, tar_y);
		if(tar_num == 0) { // 视野内无目标 
			if(near_num == 0) {
				//printf("NO TARGET FOUND!\n"); 
				return;
			}
			if(t.can_move()) {
				// 接近最近目标
				if(abs(near_x - iX) <= abs(near_y - iY)) {
					if(near_x < iX)
						t.move(2);
					else if(iX < near_x)
						t.move(3);
					else if(near_y < iY)
						t.move(0);
					else
						t.move(1); 
				} else {
					if(near_y < iY)
						t.move(0);
					else if(iY < near_y)
						t.move(1);
					else if(near_x < iX)
						t.move(2);
					else 
						t.move(3);
				}
			}
			if(t.can_attack(2)) {
				if(abs(near_x - iX) <= abs(near_y - iY)) {
					if(near_y < iY) {
						t.turn(0);
						t.attack(2);
					} else {
						t.turn(1);
						t.attack(2);
					}
				} else {
					if(near_x < iX) {
						t.turn(2);
						t.attack(2);
					} else {
						t.turn(3);
						t.attack(2);
					}
				}
			}
		} else { // 视野内存在目标 (此时可以进行移动或攻击(或皆可))
			int state = mem["to_id"+TS(tar_dx)+","+TS(tar_dy)];
			bool explore = (rand()%100 < mem["epsilon"]);
			// --- 移动 ---  
			if(t.can_move() == 1 && mem["stay timer move"] == 0) {
				if(mem["last move"] == 1) {
					mem["last move"] = 0;
					int action = mem["last move action"],
						state = mem["last move state"];
					//	state_next = mem["last move state_next"];
					//int min_q = 1e8;
					//for(int i = 0; i <= 4; i++) min_q = std::min(min_q, mem["Qm"+TS(state_next)+","+TS(i)]);
					// 因为移动行为没有初始Rewards表格，所以QLearning公式对应做出改变 
					mem["Qm"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qm"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (-10); 
				}
				int action;
				if(explore == 1)
					action = rand()%5;
				else {
					int action_q = 1e8;
					for(int i = 0; i <= 3; i++) {
						if(t.can_move(i) == 1 && action_q > mem["Qm"+TS(state)+","+TS(i)]) {
							action_q = mem["Qm"+TS(state)+","+TS(i)];
							action = i;
						}
					}
					if(action_q > mem["Qm"+TS(state)+","+TS(4)]) {
						action = 4;
					}
					if(action_q == 1e8)
						printf("action choosing error.\n");
				}
				if(action == 4) {
					mem["stay timer move"] = 9;
				} else {
					if(t.move(action) == false)
						;//printf("fail action.\n");
				}
				//int state_next;
				//if(action == 0) state_next = mem["to_id"+TS(tar_x)+","+TS(tar_y+1)];
				//else if(action == 1) state_next = mem["to_id"+TS(tar_x)+","+TS(tar_y-1)];
				//else if(action == 2) state_next = mem["to_id"+TS(tar_x+1)+","+TS(tar_y)];
				//else if(action == 3) state_next = mem["to_id"+TS(tar_x-1)+","+TS(tar_y)];
				//else state_next = state;
				mem["last move state"] = state;
				mem["last move action"] = action;
				//mem["last move state_next"] = state_next;
				mem["last move"] = 1;
			}
			// --- 攻击 --- 
			if(t.can_attack(2) && mem["stay timer attack"] == 0) {
				if(mem["last attack"] == 1) {
					mem["last attack"] = 0;
					int action = mem["last attack action"],
						state = mem["last attack state"];
					// 因为攻击行为对状态没有影响 所以QLearning公式对应做出改变 
					mem["Qa"+TS(state)+","+TS(action)] = (1.0 - 0.01 * mem["alpha"]) * mem["Qa"+TS(state)+","+TS(action)] + 0.01 * mem["alpha"] * (0);
				}
				int action;
				if(explore == 1)
					action = rand()%5;
				else {
					int action_q = -1;
					for(int i = 0; i <= 4; i++)
						if(action_q < mem["Qa"+TS(state)+","+TS(i)]) {
							action_q = mem["Qa"+TS(state)+","+TS(i)];
							action = i;
						}
				}
				if(action == 4) {
					mem["stay timer attack"] = 9;
				} else {
					t.turn(action);
					t.attack(2);
				}
				mem["last attack state"] = state;
				mem["last attack action"] = action;
				mem["last attack target"] = tar_num;
				mem["last attack"] = 1;
			}
		}
	}
} 

#undef PRO
#undef PAR

#endif

