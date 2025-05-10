#pragma once
#include <string>

enum class TokenType
{
	//SINGLE CHARACTER
	LEFT_BRACE,	// {
	RIGHT_BRACE,	// }
	LEFT_PAREN,		// (
	RIGHT_PAREN,	// )
	SEMICOLON,		// ;
	COMMA,			// ,
	PLUS,			// +
	MINUS,			// -
	STAR,			// *
	SLASH,			// /
	EQUAL,			// =

	// Literals
	IDENTIFIER, 
	NUMBER, 
	STRING,

	// Keywords
	LET, 
	FUNC, 
	RETURN,
	PRINT, 
	PRINTLN,
	TRUE, 
	FALSE,


	END_OF_FILE,
	UNKNOWN
};

struct Token {
	TokenType type;
	std::string lexeme;
	int line;

	Token(TokenType type, const std::string& lexeme, int line)
		: type(type), lexeme(lexeme), line(line) {
	}
};