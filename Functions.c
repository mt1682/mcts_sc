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
#include <limits.h>

int BOARD_SIZE=INPUT_NODE_QUANTITY*INPUT_NODE_QUANTITY;

int get_coo_for_prg(int x, int y) {
    return x*INPUT_NODE_QUANTITY+y;
}

int get_coo_for_man(int coo) {
    return coo/INPUT_NODE_QUANTITY;
}

void print_board(int *nowBoard) {
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        for (int j=0; j<INPUT_NODE_QUANTITY; j++) {
            printf("%d, ", nowBoard[get_coo_for_prg(i, j)]);
        }
        printf("\n");
    }
}

void check_duplicate(int *inputDfg, int *nowBoard, int nodeNum, int cycle) {
    int y=nodeNum/INPUT_NODE_QUANTITY;
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        if (inputDfg[i+INPUT_NODE_QUANTITY*y]!=0&&nowBoard[i+INPUT_NODE_QUANTITY*y]==0) {
            nowBoard[i+INPUT_NODE_QUANTITY*y]=cycle;
        }
        //ここで横全部埋まるからplayout中のforループは最初の
    }
}
//ランダムにサイクル数を求めて盤面に配置
//多分voidでもおk
int select_cycle(int *inputDfg, int *nowBoard, int nodeNum) {
    int cycle;
    int flag=-1, nodeNumStack[10000];
    int checker;
    int cnt=0;
    //ここを再帰からfor or while に書き換え
    //規定回数やっても終わらなかったら盤面を初期化する処理を入れる
    while (1) {
        cycle=1+rand()%TIME_CONSTRAIN;
        checker=check_input(inputDfg, nowBoard, cycle, nodeNum);
        //printf("cycle=%d, check=%d\n", cycle, checker);
        if (checker==-2) {
            //cycleに問題がない場合
            check_duplicate(inputDfg, nowBoard, nodeNum, cycle);
            nowBoard[nodeNum]=cycle;
            if (flag==-1) {
                //入力に未処理がない場合
                break;
            } else {
                //入力に未処理がある場合
                nodeNum=nodeNumStack[flag];
                flag--;
            }
        } else if (checker!=-1) {
            //入力が未スケジュールの場合
            flag++;
            nodeNumStack[flag]=nodeNum;
            nodeNum=checker;
        }
        cnt++;
        if (cnt>10) {
            return -1;
        }
    }
    return 0;
}

//対象ノードの入力ノードのサイクル数が対象ノードのサイクル数以下なら-1, 未スケジュールならその配列番号, 問題なければ-2を返す
int check_input(int *inputDfg, int *nowBoard, int cycle, int nodeNum) {
    
    nodeNum=get_coo_for_man(nodeNum);
    int coo;
    
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        coo=get_coo_for_prg(i, nodeNum);
        if (inputDfg[coo]!=0) {
            if (nowBoard[coo]==0) {
                //未スケジュール時
                return coo;
            } else if (nowBoard[coo]>=cycle) {
                //サイクル数不合時
                return -1;
            }
        }
    }
    return -2;
}

//評価値を返す
int evaluate_board(int *nowBoard) {
    int cmax=INT_MIN;
    int emax=INT_MIN;
    int cntEl[TIME_CONSTRAIN+1];
    for (int i=0; i<BOARD_SIZE; i++) {
        cntEl[nowBoard[i]]++;
        if (cmax<nowBoard[i]) {
            cmax=nowBoard[i];
        }
    }
    for (int i=0; i<TIME_CONSTRAIN; i++) {
        if (emax<cntEl[i]) {
            emax=cntEl[i];
        }
    }
    
    return cmax;
}

//探索するノードは1から順番にする
//ランダムにするのはサイクル数のみ
//playout内はforで1からINPUT_NODE_QUANTITYまで回してその中でselectcycleを使う

//縦方向優先
int playout(int *inputDfg, int *nowBoard, int initCycle) {
    clock_t mcstart;
    clock_t mcend;
    int coo;
    int jud;
    mcstart=clock();
    nowBoard[1]=initCycle;
    check_duplicate(inputDfg, nowBoard, 1, initCycle);
    for (int i=0; i<INPUT_NODE_QUANTITY; i++) {
        for (int j=0; j<INPUT_NODE_QUANTITY; j++) {
            //printf("%d, %d\n", i, j);
            coo=get_coo_for_prg(j, i);
            if (inputDfg[coo]!=0&&nowBoard[coo]==0) {
                jud=select_cycle(inputDfg, nowBoard, coo);
                if (jud==-1) {
                    return -1;
                }
            }
        }
    }
    mcend=clock();
    printf("%d\n", mcend-mcstart);
    print_board(nowBoard);
    return evaluate_board(nowBoard);
}


//探索対象はサイクル数
//最初のノードのサイクル数ごとに盤面ノード
//cycle n={盤面, 最終サイクル数, 最終ノード数}
//1番目のノードにinitcycleを当てて探索→1番評価の良かったinitcycleを1番に割り当てて, 2番目のinitcycleを当て探索→1番評価の良かったinitcycleを...以下同
int org_mc(int *inputDfg, int *nowBoard) {
    int initCycle;
    int eva;
    int copyBoard[BOARD_SIZE];
    int ansBoard[BOARD_SIZE];
    int evaMin=INT_MAX;
    copy_board(copyBoard, nowBoard);
    print_board(copyBoard);
    printf("\n");
    //ここで最初のノードのサイクル数を指定
    for (int i=0; i<PLAYOUT_TIME; i++) {
        initCycle=1+rand()%TIME_CONSTRAIN;
        eva=playout(inputDfg, nowBoard, initCycle);
        if (eva==-1) {
            i--;
        }
        if (evaMin>eva) {
            evaMin=eva;
            copy_board(ansBoard, nowBoard);
        }
        copy_board(nowBoard, copyBoard);
    }
    return 0;
}

void copy_board(int *dst, int *src) {
    for (int i=0; i<BOARD_SIZE; i++) {
        dst[i]=src[i];
    }
}
