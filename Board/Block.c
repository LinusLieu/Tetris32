#include "stm32f10x.h"
#include "Block.h"

u16 Z[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0}};

u16 L[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {2,2,2,0},
            {0,0,2,0}};

u16 O[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {0,3,3,0},
            {0,3,3,0}};

u16 S[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {4,4,0,0},
            {0,4,4,0}};

u16 I[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {5,5,5,5},
            {0,0,0,0}};
u16 J[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {6,6,6,0},
            {6,0,0,0}};
u16 T[4][4]= {{0,0,0,0},
            {0,0,0,0},
            {0,7,0,0},
            {7,7,7,0}};