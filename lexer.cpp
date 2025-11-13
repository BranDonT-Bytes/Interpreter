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

    void scanToken() {
        char c = advance();
        switch (c) {
            // Single-character tokens
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;


            case '!':
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;

            case '/':
                if (match('/')) {

                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;

            case '"': stringLiteral(); break;

            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;

            default:
                if (isdigit(c)) {
                    number();
                } else if (isalpha(c) || c == '_') {
                    identifier();
                } else {
                    cerr << "Unexpected character '" << c << "' at line " << line << endl;
                }
                break;
        }
    }

    void stringLiteral() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            cerr << "Unterminated string at line " << line << endl;
            return;
        }

        advance();


        string value = source.substr(start + 1, current - start - 2);
        tokens.emplace_back(TokenType::STRING, value, line);
    }

    void number() {
        while (isdigit(peek())) advance();

        // Fractional part
        if (peek() == '.' && isdigit(peekNext())) {
            advance(); // consume '.'
            while (isdigit(peek())) advance();
        }

        addToken(TokenType::NUMBER);
    }


    void identifier() {
        while (isalnum(peek()) || peek() == '_') advance();

        string text = source.substr(start, current - start);
        auto keywordIt = keywords.find(text);
        TokenType type = (keywordIt != keywords.end()) ? keywordIt->second : TokenType::IDENTIFIER;
        tokens.emplace_back(type, text, line);
    }
};


int main() {
    string code = R"(
        let username = "Alice";
        let count = 42.5;
        if (count >= 10) {
            print username;
        } else {
            print "small";
        }
        // this is a comment
    )";

    Lexer lexer(code);
    vector<Token> tokens = lexer.scanTokens();

    for (const auto& token : tokens) {
        cout << "Token(" << static_cast<int>(token.type)
             << ", \"" << token.lexeme << "\", line " << token.line << ")\n";
    }

    return 0;
}
