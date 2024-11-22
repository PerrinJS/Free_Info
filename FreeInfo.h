#ifndef FREEINFO
#define FREEINFO
#include <stdlib.h>
#include <stdint.h>
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))


enum MainError_t {
    NO_ERROR        = 0,
    KVER_ERROR      = 0b00000001,
    LIN_NAME_ERROR  = 0b00000010,
    TIME_STR_ERROR  = 0b00000100,
    SCRIPT_ERROR    = 0b00001000,
    PARSE_ERROR     = 0b00010000,
    FEATURE_ERROR   = 0b00100000,
    FIRST_ERROR     = KVER_ERROR,
    FINAL_ERROR     = FEATURE_ERROR
};
typedef enum MainError_t InfoError_t;
typedef uint8_t InfoError;
#define INFO_ERROR_NUM_BITS sizeof(InfoError)*8

extern const char* INFO_ERROR_NAMES[];
[[maybe_unused]] extern const size_t INFO_ERROR_NAMES_LEN;

enum ParseError_t {
    NO_P_ERROR = 0,
    INSERT_FUNC_NOT_SUPPORTED,
};
typedef enum ParseError_t ParseError;

#define IS_ZERO_OR(x,y) ((x) ? NO_ERROR : (y))
#define IS_ZERO(x) ((x) ? false : true)
#define NOT_EOF_NL(x) (((x) != EOF) && ((x) != '\n'))

#endif
