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
#include <memory>

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
        COLON = ':',
        COMMA = ','
    }
    tag;

    using Type = decltype(Token::LITERAL);

    std::unique_ptr<char[]> value;

public:
    Token() : tag(Token::LITERAL) {};
    Token(Type tag) : tag(tag) {};
    Token(std::unique_ptr<char[]> value) : tag(Token::LITERAL), value(std::move(value)) {};

    Token(Token& other) = delete;
    Token& operator=(Token other) = delete;

    Token(Token&& other);
    Token& operator=(Token&& other);
};

void tokenize(const std::string& string, std::vector<Token>& out);
void tokenize(std::istream& istream, std::vector<Token>& out);

}

#endif /* Tokenizer_hpp */
