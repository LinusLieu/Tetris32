#include "stm32f10x.h"
#include "Global.h"
#include "IERG3810_Draw.h"
void PRE_write(void)
{
	int i = 0, j = 0;
	int color = 0;
    
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			//if(block_pos_x+i+1 > 0 && block_pos_x+i < 10){
			
			if(PRE_block[i][j]){
				color = switch_color(PRE_block[i][j]);
			}else{
				color = switch_color(0X00);
			}
			IERG3810_Draw_DrawSquare(color, 172+(i-1)*8, (j-3)*8+64);
			
		}
	}
}

void switch_PRE(void)
{
		u16 Z2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0}};
u16 L2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {2,2,2,0},
            {0,0,2,0}};
u16 O2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {0,3,3,0},
            {0,3,3,0}};
u16 S2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {4,4,0,0},
            {0,4,4,0}};
u16 I2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {5,5,5,5},
            {0,0,0,0}};
u16 J2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {6,6,6,0},
            {6,0,0,0}};
u16 T2[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {0,7,0,0},
            {7,7,7,0}};
		int i = 0, j = 0;
    u16 temp = 0;
    temp = Blocks[Bag7cnt];
    switch (temp)
    {
        case 0 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = Z2[i][j];
            }
        }
        break;
        case 1 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = L2[i][j];
            }
        }
        break;
        case 2 :
        iso = 1;
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = O2[i][j];
            }
        }
        break;
        case 3 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = S2[i][j];
            }
        }
        break;
        case 4 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = I2[i][j];
            }
        }
        break;
        case 5 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = J2[i][j];
            }
        }
        break;
        case 6 :
        for(i = 0;i<4;i++){
            for(j = 0; j < 4; j++){
                PRE_block[j][i] = T2[i][j];
            }
        }
    }
    PRE_write();
}