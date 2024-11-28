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
//outside_design_u16 blocks[7] = {1,2,3,4,5,6,7};
void generate_7bag(void)
{
		u16 i = 0;
		u16 temp = 0;
		u16 j = 0;
    for (i = 6; i > 0; i--) {
        cnt = generate_random();       // generate random
        j = cnt % (i + 1);        // range 0 to i 
        
        // switch_blocks[i] 和 blocks[j]
        temp = Blocks[i];
        Blocks[i] = Blocks[j];
        Blocks[j] = temp;
    }
}
