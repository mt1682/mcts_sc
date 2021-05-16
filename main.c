//
//  main.c
//  mcts_sc
//
//  Created by 松岡　尚典 on 2021/05/14.
//

#include <stdio.h>
#include "Constants.h"

const int BOARD_SIZE=INPUT_NODE_QUANTITY*INPUT_NODE_QUANTITY;

int main(int argc, const char * argv[]) {
    int inputDfg[BOARD_SIZE]={
        0, 1, 0, 0,
        0, 0, 0, 1,
        0, 1, 0, 0,
        0, 0, 0, 0
    };
    
    int resultDfg[BOARD_SIZE]={
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    
    return 0;
}
