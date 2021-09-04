
#ifndef yourAI_cpp_
#define yourAI_cpp_

#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<queue>
#include<map>
#include<fstream>

namespace ai1 {
	void AI(Character &t, map<string, int> &mem) {
		// 读取ai操控角色编号,坐标,血量,方向 
		int iNumber = mem["your number"],
			iX = mem["your x"],
			iY = mem["your y"],
			iHP = mem["your hp"],
			iDirect = mem["your direct"];
		// 读取角色数量
		int player_sum = mem_all["cha sum"];
		// 读取所有角色信息 
		int player_hp[player_sum+1],
			player_x[player_sum+1],
			player_y[player_sum+1];
		for(int i = 1; i <= player_sum; i++)
			player_hp[i] = mem_all["cha "+to_string(i)+" hp"],
			player_x[i] = mem_all["cha "+to_string(i)+" x"],
			player_y[i] = mem_all["cha "+to_string(i)+" y"];
		// 确定目标 
		int target = 0;
		for(int i = 1; i <= player_sum; i++)
			if(i != iNumber && player_hp[i] > 0) {
				target = i;
				//printf("[%d]\n", target);
				break;
			}
		// 输出辅助信息：ai的位置 目标的位置 
		// 追踪目标,确定前进方向 
		//int direct =  rand()%5;
		//if(direct == 4) {
			if(player_x[target] < iX)
				t.move(2);
			else
				t.move(3);
			if(player_y[target] < iY)
				t.move(0);
			else
				t.move(1);
		//}
		//t.move(direct);
		// 攻击目标
		if(player_x[target] == iX) // x轴相等，说明敌人在上方或者下方
			if(player_y[target] < iY) // 敌人在上方
				t.turn(0), t.attack(1);
			else					  // 敌人在下方
				t.turn(1), t.attack(1);
		else if(player_y[target] == iY) // y轴向等，说明敌人在左边或者右边
			if(player_x[target] < iX)
				t.turn(2), t.attack(1);
			else
				t.turn(3), t.attack(1);
		
	}
}

#define TS to_string
namespace ai2 {
	/*
	视野 (6+1+6)*13 - ... =>  挖去空格 
	*/
	/*
	int mx[80] = {},
		my[80] = {},
		to_id[14][14] = {}; // 辅助坐标和编号转换 
	float Rm[80][5] = {}, // Rm[i][j] : 敌人在位置i(1~69), 角色做出动作j的奖励; 
		  Ra[80][5] = {}; // Rm-j : 0~4静止/移动; Ra-j : 0~4静止/攻击 
	float Qm[70][5] = {},
		  Qa[70][5] = {};
	move表格，值越小， 
	*/
	void AI(Character &t, map<string, int> &mem) {
		// 初始化
		if(mem.count("init") == 0) {
			mem["init"] = 1;
			mem["stay timer move"] = 0;
			mem["stay timer attack"] = 0;
			register int i, j;
			// 学习模式
			int run_mode = 2; // 1:训练模式; 2:测试模式(成果检测); 3:纯随机模式; 4:微训练模式 
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
			input.open("ai_memory.txt", std::ios::in);
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
			iDirect = mem["your direct"];  
		int player_sum = mem_all["cha sum"]; // 读取玩家数量
		int player_hp[player_sum+1], // 读取所有玩家信息 
			player_x[player_sum+1],
			player_y[player_sum+1]; 
		for(int i = 1; i <= player_sum; i++)
			player_hp[i] = mem_all["cha "+TS(i)+" hp"],
			player_x[i] = mem_all["cha "+TS(i)+" x"],
			player_y[i] = mem_all["cha "+TS(i)+" y"];
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
					std::ofstream output("ai_memory.txt", std::ios::out);
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
			if(i != iNumber && player_hp[i] > 0) { // 玩家不是自己 并且 存活 则 选定为目标 
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
				printf("NO TARGET FOUND!\n"); 
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


#endif

