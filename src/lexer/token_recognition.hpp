/*This code is to recognize the token input and return what token is it*/
#pragma once
#include "token.hpp"

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
      case TokenType::LET:         return "LET";
      case TokenType::IDENTIFIER:  return "IDENTIFIER";
      case TokenType::NUMBER:      return "NUMBER";
      case TokenType::PRINT:       return "PRINT";
      case TokenType::PLUS:        return "PLUS";
      case TokenType::MINUS:       return "MINUS";
      case TokenType::STAR:        return "STAR";
      case TokenType::SLASH:       return "SLASH";
      case TokenType::EQUAL:       return "EQUAL";
      case TokenType::SEMICOLON:   return "SEMICOLON";
      case TokenType::LPAREN:      return "LPAREN";
      case TokenType::RPAREN:      return "RPAREN";
      case TokenType::END_OF_FILE: return "EOF";
      case TokenType::UNKNOWN:     return "UNKNOWN";
    }
    return "UNKNOWN";
}
