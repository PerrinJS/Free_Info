#ifndef FREEINFO
#define FREEINFO
#include <stdlib.h>
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))

enum MainError_t {
    NO_ERROR = 0,
    KVER_ERROR = 0b0001,
    LIN_NAME_ERROR = 0b0010,
    TIME_STR_ERROR = 0b0100,
    PARSE_ERROR = 0b1000,
};
typedef enum MainError_t InfoError;

/*
const char * INFO_ERROR_NAMES[] = {
    "No",
    "Kernel Version",
    "Linux Name",
    "Time String",
    "Parse Error",
    "Multiple Errors"
};
const size_t INFO_ERROR_NAMES_LEN = ARRAY_SIZE(INFO_ERROR_NAMES);
*/

enum ParseError_t {
    NO_P_ERROR = 0,
    INSERT_FUNC_NOT_SUPPORTED,
};
typedef enum ParseError_t ParseError;

#define IS_ZERO_OR(x,y) ((x) ? NO_ERROR : (y))
#define IS_ZERO(x) ((x) ? false : true)
#define NOT_EOF_NL(x) (((x) != EOF) && ((x) != '\n'))

#endif
