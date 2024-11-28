#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_Preview.h"
#include "Block.h"

void Draw_Preview(void){
    u8 nextblock = Blocks[Bag7cnt];
    u8 i,j;
    switch (nextblock)
    {
        case 0 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = Z[i][j];
            }
        }
        break;
        case 1 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = L[i][j];
            }
        }
        break;
        case 2 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = O[i][j];
            }
        }
        break;
        case 3 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = S[i][j];
            }
        }
        break;
        case 4 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = I[i][j];
            }
        }
        break;
        case 5 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = J[i][j];
            }
        }
        break;
        case 6 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = T[i][j];
            }
        }
        break;
    }
    Draw_Preview_2();
}
