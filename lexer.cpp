#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

using namespace std;

// 1. Enum class TokenType

enum class TokenType {
    
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, NUMBER, STRING,

    AND, ELSE, FALSE, IF, LET, NIL, OR, TRUE, WHILE, PRINT,

    END_OF_FILE
};

// 2. Struct Token

struct Token {
    TokenType type;
    string lexeme;
    int line;

    Token(TokenType t, const string& lex, int ln)
        : type(t), lexeme(lex), line(ln) {}
};


// 3. Lexer class

class Lexer {
public:
    // Constructor
    Lexer(const string& src)
        : source(src), start(0), current(0), line(1) {
        keywords = {
            {"and", TokenType::AND},
            {"else", TokenType::ELSE},
            {"false", TokenType::FALSE},
            {"if", TokenType::IF},
            {"let", TokenType::LET},
            {"nil", TokenType::NIL},
            {"or", TokenType::OR},
            {"true", TokenType::TRUE},
            {"while", TokenType::WHILE},
            {"print", TokenType::PRINT}
        };
    }

    vector<Token> scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }
        tokens.emplace_back(TokenType::END_OF_FILE, "", line);
        return tokens;
    }

private:
    // Private members
    const string source;
    vector<Token> tokens;
    size_t start = 0;
    size_t current = 0;
    int line = 1;
    unordered_map<string, TokenType> keywords;


    bool isAtEnd() const {
        return current >= source.length();
    }

    char advance() {
        return source[current++];
    }

    void addToken(TokenType type) {
        string text = source.substr(start, current - start);
        tokens.emplace_back(type, text, line);
    }

    bool match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;
        current++;
        return true;
    }

    char peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    char peekNext() const {
        if (current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }

