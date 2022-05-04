#include "lexer.hpp"
#include "common.hpp"

inline bool is_digit(uint8_t c) {
  return c >= '0' && c <= '9';
}

inline bool is_nz_digit(uint8_t c) {
  return c >= '1' && c <= '9';
}

inline bool is_whitespace(uint8_t c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

inline bool is_alpha(uint8_t c) {
  return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z');
}

inline void inc_loc(Lexer* l) {
  l->loc.column += 1;
  l->loc.offset += 1;
}

inline void dec_loc(Lexer* l) {
  l->loc.column -= 1;
  l->loc.offset -= 1;
}

Token next(Lexer* l) {
  l->pending = Token{invalid};
  l->state = State::start;

  Token result = Token{
    .tag = eof,
    .start = l->loc,
  };
  uint8_t c;
  for (;;) {
    c = l->buffer[l->loc.offset];

    switch (l->state) {
      case start:
        if (c == 0) {
          goto loop_end;
        } else if (c == '\n') {
          l->loc.line += 1;
          l->loc.column = 0;
          l->loc.offset += 1;
          result.start = l->loc;
          continue;
        } else if (is_alpha(c)) {
          result.tag = identifier;
          l->state = identifier_dec;
        } else if (is_whitespace(c)) {
          inc_loc(l);
          result.start = l->loc;
          continue;
        } else if (c == '0') {
          result.tag = int_literal;
          l->state = zero;
        } else if (is_nz_digit(c)) {
          result.tag = int_literal;
          l->state = int_literal_dec;
        } else if (c == '(') {
          result.tag = l_paren;
          inc_loc(l);
          goto loop_end;
        } else if (c == ')') {
          result.tag = r_paren;
          inc_loc(l);
          goto loop_end;
        } else if (c == '{') {
          result.tag = l_brace;
          inc_loc(l);
          goto loop_end;
        } else if (c == '}') {
          result.tag = r_brace;
          inc_loc(l);
          goto loop_end;
        } else if (c == '[') {
          result.tag = l_bracket;
          inc_loc(l);
          goto loop_end;
        } else if (c == ']') {
          result.tag = r_bracket;
          inc_loc(l);
          goto loop_end;
        } else if (c == '`') {
          result.tag = grave;
          inc_loc(l);
          goto loop_end;
        } else if (c == '~') {
          result.tag = tilde;
          inc_loc(l);
          goto loop_end;
        } else if (c == '!') {
          result.tag = bang;
          inc_loc(l);
          goto loop_end;
        } else if (c == '@') {
          result.tag = at;
          inc_loc(l);
          goto loop_end;
        } else if (c == '#') {
          result.tag = pound;
          inc_loc(l);
          goto loop_end;
        } else if (c == '$') {
          result.tag = dollar;
          inc_loc(l);
          goto loop_end;
        } else if (c == '%') {
          result.tag = percent;
          inc_loc(l);
          goto loop_end;
        } else if (c == '^') {
          result.tag = caret;
          inc_loc(l);
          goto loop_end;
        } else if (c == '&') {
          result.tag = ampersand;
          inc_loc(l);
          goto loop_end;
        } else if (c == '*') {
          result.tag = asterisk;
          inc_loc(l);
          goto loop_end;
        } else if (c == '+') {
          result.tag = plus;
          inc_loc(l);
          goto loop_end;
        } else if (c == '-') {
          result.tag = minus;
          inc_loc(l);
          goto loop_end;
        } else if (c == '=') {
          result.tag = equals;
          inc_loc(l);
          goto loop_end;
        } else if (c == ';') {
          result.tag = semi_colon;
          inc_loc(l);
          goto loop_end;
        } else if (c == ':') {
          result.tag = colon;
          inc_loc(l);
          goto loop_end;
        } else if (c == '/') {
          result.tag = slash;
          inc_loc(l);
          goto loop_end;
        } else if (c == '\\') {
          result.tag = backslash;
          inc_loc(l);
          goto loop_end;
        } else if (c == '|') {
          result.tag = pipe;
          inc_loc(l);
          goto loop_end;
        } else if (c == ',') {
          result.tag = comma;
          inc_loc(l);
          goto loop_end;
        } else if (c == '.') {
          result.tag = period;
          inc_loc(l);
          goto loop_end;
        } else if (c == '<') {
          result.tag = angle_left;
          inc_loc(l);
          goto loop_end;
        } else if (c == '>') {
          result.tag = angle_right;
          inc_loc(l);
          goto loop_end;
        } else if (c == '?') {
          result.tag = question_mark;
          inc_loc(l);
          goto loop_end;
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
        } else if (c == '.') {
          result.tag = invalid;
          l->state = num_dot;
        } else {
          goto loop_end;
        }
        break;

      case zero:
        if (is_digit(c) || c == '.') {
          dec_loc(l);
          l->state = int_literal_dec;
        } else if (is_alpha(c)) {
          // TODO: hex numbers and stuff
          result.tag = invalid;
        } else {
          goto loop_end;
        }
        break;

      case num_dot:
        if (is_digit(c)) {
          result.tag = float_literal;
          l->state = float_fraction_dec;
        } else {
          result.tag = invalid;
          goto loop_end;
        }
        break;

      case float_fraction_dec:
        if (is_digit(c)) {
          result.tag = float_literal;
          l->state = float_fraction_dec;
        } else if (c == '.') {
          result.tag = invalid;
          goto loop_end;
        } else {
          goto loop_end;
        }
        break;

      case identifier_dec: 
        if (is_alpha(c) || is_digit(c) || c == '_' || c == '-') {
        } else {
          // TODO: check against keywords
          goto loop_end;
        }
        break;
    };

    inc_loc(l);
  }

  loop_end:

  result.end = l->loc;
  return result;
}
