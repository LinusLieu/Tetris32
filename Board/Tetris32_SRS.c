#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_SRS.h"

void rotate_clockwise(void) {
    u16 rotate_block[4][4];
    u16 temp[4][4];
    u8 i,j;
    u8 pos[4] = {1,2,2,1};
    int kicktable[8][5][2] = = {
        // Row 0 (0->R)
        {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},
        
        // Row 1 (R->0)
        {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},
        
        // Row 2 (R->2)
        {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},
        
        // Row 3 (2->R)
        {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},
        
        // Row 4 (2->L)
        {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},
        
        // Row 5 (L->2)
        {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},
        
        // Row 6 (L->0)
        {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},
        
        // Row 7 (0->L)
        {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}}
    };
    
    if(iso)return;

    for(i = 0;i<4;i++){
        for(j = 0;j<4;j++){
            rotate_block[i][j] = block[i][j];
        }
    }

    direction = (direction + 1)%4;

    block_center_x = pos[direction];
    block_center_y = pos[(direction+1)%4];


    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[j][4 - 1 - i] = rotate_block[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            rotate_block[i][j] = temp[i][j];
        }
    }
}
