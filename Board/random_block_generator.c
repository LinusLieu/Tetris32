#include "stm32f10x.h"
#include "Global.h"
#include "Block.h"

void random_block_generator(void)
{
    u16 temp = 0;
    int i = 0, j = 0;
    temp = cnt % 7;
    switch (temp)
    {
        case 0 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = Z[i][j];
            }
        }
        break;
        case 1 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = L[i][j];
            }
        }
        break;
        case 2 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = O[i][j];
            }
        }
        break;
        case 3 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = S[i][j];
            }
        }
        break;
        case 4 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = I[i][j];
            }
        }
        break;
        case 5 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = J[i][j];
            }
        }
        break;
        case 6 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                block[i][j] = T[i][j];
            }
        }
        break;
    }
}
