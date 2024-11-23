#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_SRS.h"

void rotate_clockwise(void) {
    u16 rotate_block[4][4];
    u16 temp[4][4];
    u8 i,j;
    u8 pos[4] = {1,2,2,1};
    u8 pre_direction,pre_center_x,pre_center_y;
    u8 tmp_srs,tmp1,tmp2;
    u8 dir[12] = {68,105,114,0,0,0,0,0,0,0,0,0};
    int kicktable[2][4][5][2] = {
        //[a][b][c][d]
        //a for rorate clockwise[0] or anti-clockwise[1]
        //b for status
        //c for case
        {
        // Row 0 (0->R)
        {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},
        
        // Row 2 (R->2)
        {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},
        
        // Row 4 (2->L)
        {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},
        
        // Row 6 (L->0)
        {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},
        },{
        // Row 7 (0->L)
        {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},

        // Row 1 (R->0)
        {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},
                
        // Row 3 (2->R)
        {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},

         // Row 5 (L->2)
        {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},

        }

    };

    for(i = 0;i<4;i++){
        for(j = 0; j < 4; j++){
            temp[i][j] = 0;
            rotate_block[i][j] = 0;
        }
    }
    
    if(iso)return;

    pre_direction = direction;
    direction = (direction + 1)%4;
    block_center_x = pos[direction];
    block_center_y = pos[(direction+1)%4];

    dir[9] = direction+48;

    for(i = 0;i<12;i++)
	{
		IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,260,dir[i],0xFFFF,0x0000);
	}

    for(i = 0;i<4;i++){
        for(j = 0;j<4;j++){
            rotate_block[i][j] = block[i][j];
        }
    }

    pre_center_x = block_center_x;
    pre_center_y = block_center_y;
    

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
    
    for(i = 0;i<5;i++){
        block_center_offset_x = kicktable[0][pre_direction][i][0];
        block_center_offset_y = kicktable[0][pre_direction][i][1];
        tmp_srs = Rotate_check();
        tmp1 = tmp_srs / 10 % 10;
        tmp2 = tmp_srs % 10;
        IERG3810_TFTLCD_ShowChar(10,80,block_center_offset_x+48+5,0xFFFF,0x0000);
        IERG3810_TFTLCD_ShowChar(20,80,block_center_offset_y+48+5,0xFFFF,0x0000);
        IERG3810_TFTLCD_ShowChar(10,60,tmp1+48,0xFFFF,0x0000);
        IERG3810_TFTLCD_ShowChar(20,60,tmp2+48,0xFFFF,0x0000);
        if(tmp_srs == 0){
            block_pos_x += block_center_offset_x;
            block_pos_y += block_center_offset_y;
            break;
        }
        
    }
        if(tmp_srs){
            for(i = 0;i<4;i++){
                for(j = 0;j<4;j++){
                    block[i][j] = rotate_block[i][j];
                }
            }
            block_center_x = pre_center_x;
            block_center_y = pre_center_y;
            direction = pre_direction;
            return;
        }
        Draw_playfield();   
        return;
}

void rotate_anticlockwise(void) {
    u16 rotate_block[4][4];
    u16 temp[4][4];
    u8 i,j;
    u8 pos[4] = {1,2,2,1};
    u8 pre_direction,pre_center_x,pre_center_y,pre_pos_x,pre_pos_y;
    u8 tmp_srs,tmp1,tmp2;
    u8 dir[12] = {68,105,114,0,0,0,0,0,0,0,0,0};
    int kicktable[2][4][5][2] = {
        //[a][b][c][d]
        //a for rorate clockwise[0] or anti-clockwise[1]
        //b for status
        //c for case
        {
        // Row 0 (0->R)
        {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},
        
        // Row 2 (R->2)
        {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},
        
        // Row 4 (2->L)
        {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},
        
        // Row 6 (L->0)
        {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},
        },{
        // Row 7 (0->L)
        {{0, 0}, {+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},

        // Row 1 (R->0)
        {{0, 0}, {+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},
                
        // Row 3 (2->R)
        {{0, 0}, {-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},

         // Row 5 (L->2)
        {{0, 0}, {-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},

        }

    };

    for(i = 0;i<4;i++){
        for(j = 0; j < 4; j++){
            temp[i][j] = 0;
            rotate_block[i][j] = 0;
        }
    }
    
    if(iso)return;

    pre_direction = direction;
    direction = (direction + 3)%4;
    block_center_x = pos[direction];
    block_center_y = pos[(direction+1)%4];

    dir[9] = direction+48;

    for(i = 0;i<12;i++)
	{
		IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,260,dir[i],0xFFFF,0x0000);
	}

    for(i = 0;i<4;i++){
        for(j = 0;j<4;j++){
            rotate_block[i][j] = block[i][j];
        }
    }

    pre_center_x = block_center_x;
    pre_center_y = block_center_y;
    

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[4 - 1 - j][i] = block[i][j];
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            block[i][j] = temp[i][j];
        }
    }
    
    for(i = 0;i<5;i++){
        block_center_offset_x = kicktable[1][pre_direction][i][0];
        block_center_offset_y = kicktable[1][pre_direction][i][1];
        tmp_srs = Rotate_check();
        tmp1 = tmp_srs / 10 % 10;
        tmp2 = tmp_srs % 10;
        IERG3810_TFTLCD_ShowChar(10,80,block_center_offset_x+48+5,0xFFFF,0x0000);
        IERG3810_TFTLCD_ShowChar(20,80,block_center_offset_y+48+5,0xFFFF,0x0000);
        IERG3810_TFTLCD_ShowChar(10,60,tmp1+48,0xFFFF,0x0000);
        IERG3810_TFTLCD_ShowChar(20,60,tmp2+48,0xFFFF,0x0000);
        if(tmp_srs == 0){
            block_pos_x += block_center_offset_x;
            block_pos_y += block_center_offset_y;
            break;
        }
        
    }
        if(tmp_srs){
            for(i = 0;i<4;i++){
                for(j = 0;j<4;j++){
                    block[i][j] = rotate_block[i][j];
                }
            }
            block_center_x = pre_center_x;
            block_center_y = pre_center_y;
            direction = pre_direction;
            return;
        }
        Draw_playfield();   
        return;
    
}
