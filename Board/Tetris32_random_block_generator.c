#include "stm32f10x.h"
#include "Global.h"
#include "Block.h"
#include "Tetris32_random_block_generator.h"

void random_block_generator(void)
{
    u16 temp = 0;
    int i = 0, j = 0;
    temp = cnt % 7;
    direction = 0;
    block_center_x = 1;
    block_center_y = 2;
    switch (temp)
    {
        case 0 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = Z[i][j];
            }
        }
        break;
        case 1 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = L[i][j];
            }
        }
        break;
        case 2 :
        iso = 1;
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = O[i][j];
            }
        }
        break;
        case 3 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = S[i][j];
            }
        }
        break;
        case 4 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = I[i][j];
            }
        }
        break;
        case 5 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = J[i][j];
            }
        }
        break;
        case 6 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[j][i] = T[i][j];
            }
        }
        break;
    }
}
