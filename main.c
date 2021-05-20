//
//  main.c
//  mcts_sc
//
//  Created by 松岡　尚典 on 2021/05/14.
//

#include <stdio.h>
#include "Constants.h"
#include "Functions.h"
#include <time.h>

static const int BOARD_SIZE=INPUT_NODE_QUANTITY*INPUT_NODE_QUANTITY;

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
    
    /*
    int inputDfg[BOARD_SIZE]={
        0, 1, 1, 0,
        0, 0, 0, 1,
        0, 0, 0, 1,
        0, 0, 0, 0
    };
     */
    
     int inputDfg[BOARD_SIZE]={
         0, 1, 0, 0,
         0, 0, 0, 1,
         0, 1, 0, 0,
         0, 0, 0, 0
     };
     
    //烈jが表すもの: jに入ってくる元
    //行iが表すもの: iから出ていく先
    //横側の番号(i)
    //上の例では0->3のところの1をスケジュールする時にj=0の列が全部スケジュール済みであることを確認する.
    //スケジュール済みかどうかだけでなく, 入力側のサイクル数がちゃんと下回ってるかを確認する.
    
    int resultDfg[BOARD_SIZE]={
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    
    clock_t start = clock();
    org_mc(inputDfg, resultDfg);
    playout(inputDfg, resultDfg, 1);
    clock_t end = clock();
    printf("main=%d\n", end-start);
    print_board(resultDfg);
    
    return 0;
}
