#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_random_block_generator.h"
u16 generate_random(void)
{
    u16 x = 1511;
    u16 y = 1033;
    u16 d = 32768;
    cnt = (y*cnt + x) % d;

    return cnt;
}
//需要在外面定义u16 blocks[7] = {1,2,3,4,5,6,7};
void generate_7bag(u16 blocks[7])
{
    for (int i = 6; i > 0; i--) {
        cnt = generate_random(cnt);       // 生成随机数
        u16 j = cnt % (i + 1);        // 生成范围 0 到 i 的随机索引
        
        // 交换 blocks[i] 和 blocks[j]
        char temp = blocks[i];
        blocks[i] = blocks[j];
        blocks[j] = temp;
    }
    return blocks
}

void turn_blocks_number(u16 blocks[7])
{
    u16 i = 0;

    for(i = 0; i < 7; i++){
        cnt = blocks[i];
        random_block_generator();
    }
    //结束了就重新generate一遍，不晓得是什么thread，请饱饱统筹
}