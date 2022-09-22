#if defined(CALC_ASM_OPTIMIZE) 
#include "./__asm_opt.cc"
#elif defined(CALC_INT128_OPTIMIZE)
#include "./__uint128_opt.cc"
#endif