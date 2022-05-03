#include "lexer.hpp"
#include "common.hpp"

inline bool is_digit(uint8_t c) {
  return c >= '0' && c <= '9';
}

inline bool is_whitespace(uint8_t c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

inline bool is_alpha(uint8_t c) {
  return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z');
}

Token next(Lexer* l) {
  l->pending = Token{invalid};
  l->state = State::start;

  Token result = Token{
    .tag = eof,
    .start = l->loc,
  };
  uint8_t c;
  bool set_start = false;
  for (;;) {
    c = l->buffer[l->loc.offset];

    switch (l->state) {
      case start:
        if (c == 0) {
          goto loop_end;
        } else if (is_whitespace(c)) {
          set_start = true;
        //} else if (c == '0') {
        //  result.tag = int_literal;
        //  state = zero;
        } else if (is_digit(c)) {
          result.tag = int_literal;
          l->state = int_literal_dec;
        } else {
          result.tag = invalid;
          result.end = l->loc;
          l->loc.column += 1;
          l->loc.offset += 1;
          return result;
        }
        break;
      case int_literal_dec:
        if (is_digit(c)) {
        } else if (is_alpha(c)) {
          result.tag = invalid;
        } else {
          goto loop_end;
        }
        break;
      default:
        printf("in invalid state\n");
        result.tag = invalid;
        goto loop_end;
    };

    if (c == '\n') {
      l->loc.line += 1;
      l->loc.column = 0;
      l->loc.offset += 1;
    } else {
      l->loc.column += 1;
      l->loc.offset += 1;
    }

    if (set_start) {
      result.start = l->loc;
      set_start = false;
    }
  }

  loop_end:

  result.end = l->loc;
  return result;
}
