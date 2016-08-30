#ifndef _MULTIGRID_TEST_DATA_H_
# define _MULTIGRID_TEST_DATA_H_

# include "multigrid/multigrid.h"

# define DIGIT(n)    n
# define V1(n)      DIGIT(n)
# define V2(n)      V1(n),V1(n)
# define V3(n)      V2(n),V1(n)
# define V4(n)      V3(n),V1(n)
# define V5(n)      V4(n),V1(n)
# define V6(n)      V5(n),V1(n)
# define V7(n)      V6(n),V1(n)
# define V8(n)      V7(n),V1(n)
# define V9(n)      V8(n),V1(n)
# define V10(n)     V9(n),V1(n)
# define V11(n)     V10(n),V1(n)
# define V12(n)     V11(n),V1(n)
# define V13(n)     V12(n),V1(n)
# define V14(n)     V13(n),V1(n)
# define V15(n)     V14(n),V1(n)
# define V16(n)     V15(n),V1(n)
# define V17(n)     V16(n),V1(n)
# define V18(n)     V17(n),V1(n)
# define V19(n)     V18(n),V1(n)
# define V20(n)     V19(n),V1(n)
# define V21(n)     V20(n),V1(n)
# define V22(n)     V21(n),V1(n)
# define V23(n)     V22(n),V1(n)
# define V24(n)     V23(n),V1(n)
# define V25(n)     V24(n),V1(n)
# define V26(n)     V25(n),V1(n)
# define V27(n)     V26(n),V1(n)
# define V28(n)     V27(n),V1(n)
# define V29(n)     V28(n),V1(n)
# define V30(n)     V29(n),V1(n)

# define Z16 V16(0)

extern const grid_level_t MAX_LEVEL;
extern const const division_scheme_t ds;
extern const grid_value_t PIC1_VAL[];
extern const grid_value_t PIC2_VAL[];

extern const picture_t PIC1;
extern const picture_t PIC2;

#endif /* _MULTIGRID_TEST_DATA_H_ */