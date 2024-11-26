#include "stm32f10x.h"
#include "Global.h"

void remove(void)
{
    u16 i = 0, j = 0;
    u16 record = 0;
	u16 k = 0;
	u16 sum = 0;
    for(j = 4; j < 24; j++)
    {
		sum = 0;
        for(i = 4; i < 14; i++)
        {
            if(Playfield[i][j] != 0)
                sum++;
        }
        if(sum < 10)
            continue;
        record = j;
        for(i = 4; i < 14; i++)
        {
            Playfield[i][record] = 0;
        }

        if(record < 23) // 确保不是最顶行
        {
            for(i = 4; i < 14; i++)
            {
                for(k = record; k < 23; k++) // 使用新变量 k 代替 j
                {
                    Playfield[i][k] = Playfield[i][k+1];
                }
            }
        }
        j = record - 1; // 从消除位置重新开始
        line_count++;
        Draw_playfield();
        if(gamemode == 1){
			Draw_linecount_40();
		}else{
			Draw_linecount();
		}
    }
}