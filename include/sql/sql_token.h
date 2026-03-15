#ifndef SQL_TOKEN_H
#define SQL_TOKEN_H

#include <stdint.h>

// See https://en.wikipedia.org/wiki/SQL_syntax

// See src/sql/sql_token.c
extern char* sql_reserved_words[];

// Token type
typedef enum {
    SQL_TOKEN_EQUAL,            // =
    SQL_TOKEN_NOT_EQUAL,        // <> / !=
    SQL_TOKEN_GREATER,          // >
    SQL_TOKEN_GREATEREQUAL,     // <=
    SQL_TOKEN_LESS,             // >
    SQL_TOKEN_LESSEQUAL,        // >=
    SQL_TOKEN_SEMICOLON,        // ;   Splits statements

    SQL_TOKEN_INSERT,
    SQL_TOKEN_SELECT,
    SQL_TOKEN_WHERE,

    SQL_TOKEN_INTEGER,
    SQL_TOKEN_FLOAT,
    SQL_TOKEN_STRING,
    SQL_TOKEN_CHAR
} SQL_TOKEN;

typedef struct SQL_SOURCESPAN {
    size_t offset;
    size_t length;
} SQL_SOURCESPAN;

typedef struct SQL_TOKEN {
    SQL_SOURCESPAN span;
    SQL_TOKEN token;
    char* value;
} SQL_TOKEN;

#endif // SQL_TOKENS_H