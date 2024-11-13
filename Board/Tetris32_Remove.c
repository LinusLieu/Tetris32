#include "stm32f10x.h"
#include "Global.h"

void remove(void)
{
    u16 i = 0, j = 0;
    u16 record = 0;
    //你那个18x28的数组太变态了，我仿照里头的10x24写的
    for(j = 0; j < 24; j++ )
    {
        for(i = 4; i < 14; i++)
        {
            if(Playfield[i][j] == 0)
            break;
        }
        if(Playfield[i][j] != 0)
        continue;
        else{
            record = j;
            for(i = 4; i < 14; i++){
                Playfield[i][record] == 0;
            }//全部为0，就是消除掉
            if(record == 23){//如果消除行是最顶上的
                continue;
            }
            else//如果是正常情况
            {
                for(i = 4; i < 14; i++){
                    for(j = record; j < 24; j++){
                        Playfield[i][j] = Playfield[i][j+1];
                    }
                }//消除行以上全部下移一位
            }
             
            j = record - 1;//从消除位置重新开始
            
        }
    }
}