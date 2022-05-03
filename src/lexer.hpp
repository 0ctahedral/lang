#include "common.hpp"

#ifndef lexer_h
#define lexer_h

enum Tag : uint8_t {
  int_literal,

  eof,
  invalid,
};

enum State : uint8_t {
  // starting a token
  start,

  // beginning an int literal
  int_literal_dec,
  // just saw a zero
  zero,
};

// Source location in file
// start and end offset in bytes in the current buffer
struct Loc {
  uint64_t line = 0;
  uint64_t column = 0;
  uint64_t offset = 0;
};

// Where a token occured in a file
struct Token {
  Tag tag = eof;
  Loc start = {};
  Loc end = {};
};


// lexes a file from a character pointer
struct Lexer {

  // Start the lexer with a source string
  Lexer(const char* src) : buffer(src), loc({}), state(start) { }

  // buffer to read from
  const char* buffer;

  // index into the buffer
  Loc loc = {};

  // current token we are working on
  Token pending = {};

  State state = {};

  // retrieves the next token in the buffer
  Token next();
};


#endif
