#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_SRS.h"

void rotate_clockwise(void) {
    u16 temp[4][4];
    u8 i,j;
    u8 pos[4] = {1,2,2,1};
    if(iso)return;
    direction = (direction + 1)%4;

    block_center_x = pos[direction];
    block_center_y = pos[(direction+1)%4];


    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[j][4 - 1 - i] = block[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            block[i][j] = temp[i][j];
        }
    }
}
