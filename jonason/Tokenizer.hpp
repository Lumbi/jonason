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
        LITERAL,
        STRING,

        OBJECT_OPEN = '{',
        OBJECT_CLOSE = '}',

        ARRAY_OPEN = '[',
        ARRAY_CLOSE = ']',

        COLON = ':',
        COMMA = ',',

        END_OF_FILE
    }
    tag;

    using Type = decltype(Token::LITERAL);

    std::unique_ptr<char[]> value;

    bool is_eof() const { return tag == END_OF_FILE; };

public:
    Token(Type tag) : tag(tag) {};
    Token(Type tag, std::unique_ptr<char[]> value) : tag(tag), value(std::move(value)) {};

    Token(Token& other) = delete;
    Token& operator=(Token other) = delete;

    Token(Token&& other);
    Token& operator=(Token&& other);
};

class Tokenizer {

public:
    const static int MAX_LITERAL_SIZE = 1 << 16; // about 65KB

public:
    Tokenizer(std::istream& istream): istream(istream) {};
    Token get_next();

private:
    Token read_string_literal(char start_char);
    Token read_any_literal(char start_char);

private:
    std::istream& istream;
    char char_buffer;
    std::unique_ptr<char[]> value_buffer = std::make_unique<char[]>(MAX_LITERAL_SIZE);
    int value_buffer_count = 0;
};

// For test compability
void tokenize(std::istream& istream, std::vector<Token>& out);

}

#endif /* Tokenizer_hpp */
