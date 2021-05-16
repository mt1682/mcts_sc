//
//  Functions.c
//  mcts_sc
//
//  Created by 松岡　尚典 on 2021/05/14.
//

#include "Functions.h"
#include "Constants.h"
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int BOARD_SIZE=INPUT_NODE_QUANTITY*INPUT_NODE_QUANTITY;

int get_coo_for_prg(int x, int y) {
    return x*INPUT_NODE_QUANTITY+y;
}

void print_board(int *nowBoard) {
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        for (int j=0; j<INPUT_NODE_QUANTITY; j++) {
            printf("%d, ", nowBoard[get_coo_for_prg(i, j)]);
        }
        printf("\n");
    }
}

//ランダムにサイクル数を求める
int select_cycle(int *nowBoard, int nodeNum) {
    int cycle;
    //ここを再帰からfor or while に書き換え
    cycle=1+rand()%TIME_CONSTRAIN;
    if (check_cycle(nowBoard, cycle, nodeNum)==0) {
        return select_cycle(nowBoard, nodeNum);
    } else {
        return cycle;
    }
}

bool check_cycle(int *nowBoard, int cycle, int nodeNum) {
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        //対象ノードの入力ノードのサイクル数が対象ノードのサイクル数以下なら0, 大きければ1を返す
        if (nowBoard[i*INPUT_NODE_QUANTITY+nodeNum]<=cycle) {
            return 0;
        }
    }
    return 1;
}

//評価値を返す
int evaluate_board(int *nowBoard) {
    return 0;
}

//探索するノードは1から順番にする
//ランダムにするのはサイクル数のみ
//playout内はforで1からINPUT_NODE_QUANTITYまで回してその中でselectcycleを使う

int playout(int *inputDfg, int *nowBoard, int initCycle) {
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        for (int j=0; j<INPUT_NODE_QUANTITY; j++) {
            if (inputDfg[get_coo_for_prg(j, i)]!=0&&nowBoard[get_coo_for_prg(j, i)]==0) {
                select_cycle(nowBoard, get_coo_for_prg(j, i));
            }
        }
    }
    
    return evaluate_board(nowBoard);
}


//探索対象はサイクル数
//最初のノードのサイクル数ごとに盤面ノード
//cycle n={盤面, 最終サイクル数, 最終ノード数}
int org_mc(int *inputDfg, int *nowBoard) {
    int initCycle;
    //ここで最初のノードのサイクル数を指定
    for (int i=0; i<PLAYOUT_TIME; i++) {
        initCycle=rand()%TIME_CONSTRAIN;
        playout(inputDfg, nowBoard, initCycle);
    }
    return 0;
}
