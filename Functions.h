//
//  Functions.h
//  mcts_sc
//
//  Created by 松岡　尚典 on 2021/05/14.
//

#ifndef Functions_h
#define Functions_h
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int get_coo_for_prg(int x, int y);              //プログラム用に座標変換
int get_coo_for_man(int coo);                   //人間用に座標変換
int put_time(int x);                            //盤面にサイクル数を配置
int playout(int *inputDfg, int *nowBoard, int initCycle);                             //プレイアウト
void print_board(int *nowBoard);                         //盤面表示
int select_cycle(int *nowBoard, int nodeNum);
bool check_cycle(int *nowBoard, int cycle, int nodeNum);

#endif /* Functions_h */
