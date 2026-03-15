#ifndef SQL_PARSE_H
#define SQL_PARSE_H

// This contains functions for lexical analysis, which turns the string
// into a set of tokens to be passed off for syntactic analysis

// Is operator
// 1 = true, 0 = false
int sql_is_op(const char* str);
// Is keyword
// 1 = true, 0 = false
int sql_is_kw(const char* str);

#endif // SQL_PARSE_H