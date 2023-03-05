//
//  Tokenizer.hpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-02.
//

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

#include <vector>
#include <istream>
#include <string>

namespace jonason {

struct Token {

public:
    enum {
        LITERAL = '*',

        OBJECT_OPEN = '{',
        OBJECT_CLOSE = '}',

        ARRAY_OPEN = '[',
        ARRAY_CLOSE = ']',

        DOUBLE_QUOTE = '"',
        COLUMN = ':',
        COMMA = ','
    }
    tag;

public:
    Token(decltype(Token::LITERAL) tag) : tag(tag) {};
    Token(char* value) : tag(Token::LITERAL), value(value) {};

    Token(Token& other);
    Token& operator=(Token other);

    Token(Token&& other);
    Token& operator=(Token&& other);

    ~Token();

public:
    char* value = nullptr;
};

void tokenize(const std::string& string, std::vector<Token>& out);
void tokenize(std::istream& istream, std::vector<Token>& out);

}

#endif /* Tokenizer_hpp */
