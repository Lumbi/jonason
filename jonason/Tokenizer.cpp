//
//  Tokenizer.cpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-02.
//

#include "Tokenizer.hpp"

#include <sstream>

namespace jonason {

Token::Token(Token&& other) {
    tag = std::move(other.tag);
    value = std::move(other.value);
    other.value = nullptr;
}

Token& Token::operator=(Token&& other) {
    if (this == &other) { return *this; }
    tag = std::move(other.tag);
    value = std::move(other.value);
    return *this;
}

constexpr bool is_ws(char c) {
    return c == 0x20 || c == 0x0a || c == 0x0d || c == 0x09;
}

// Based on MSVC's STL implementation
void skip_ws(std::istream& istream) {
    if (istream.good()) {
        std::ios_base::iostate state = std::ios_base::goodbit;
        try {
            for (auto c = istream.rdbuf()->sgetc();; c = istream.rdbuf()->snextc()) {
                if (c == EOF) {
                    state |= std::ios_base::eofbit;
                    break;
                } else if (!is_ws(c)) {
                    break;
                }
            }
        } catch (...) {
            istream.setstate(std::ios_base::badbit);
            throw;
        }
        istream.setstate(state);
    }
}

Token Tokenizer::get_next() {
    skip_ws(istream);
    if (!istream.good()) { return { Token::END_OF_FILE }; }
    istream.get(char_buffer);
    switch (char_buffer) {
        case Token::OBJECT_OPEN: return { Token::OBJECT_OPEN };
        case Token::OBJECT_CLOSE: return { Token::OBJECT_CLOSE };
        case Token::ARRAY_OPEN: return { Token::ARRAY_OPEN };
        case Token::ARRAY_CLOSE: return { Token::ARRAY_CLOSE };
        case Token::COMMA: return { Token::COMMA };
        case Token::COLON: return { Token::COLON };
        case '"': return read_string_literal(char_buffer);
        default: return read_any_literal(char_buffer);
    }
}

Token Tokenizer::read_string_literal(char start_char) {
    value_buffer_count = 0;

    char char_buffer;
    while (istream.good()) {
        char peek = istream.peek();

        if (peek == '"') {
            istream.get();
            break;
        } else {
            istream.get(char_buffer);
            if (value_buffer_count < MAX_LITERAL_SIZE) {
                value_buffer[value_buffer_count] = char_buffer;
                value_buffer_count++;
            }
        }
    }

    if (value_buffer_count > 0) {
        auto value = std::make_unique<char[]>(value_buffer_count + 1);
        std::copy(value_buffer.get(), value_buffer.get() + value_buffer_count, value.get());
        value[value_buffer_count] = '\0';
        return { Token::STRING, std::move(value) };
    } else {
        return { Token::END_OF_FILE };
    }
}

Token Tokenizer::read_any_literal(char start_char) {
    value_buffer[0] = start_char;
    value_buffer_count = 1;

    char char_buffer;
    while (istream.good()) {
        char peek = istream.peek();

        if (is_ws(peek) || peek == Token::COMMA || peek == Token::OBJECT_CLOSE || peek == Token::ARRAY_CLOSE) {
            break;
        } else {
            istream.get(char_buffer);
            if (value_buffer_count < MAX_LITERAL_SIZE) {
                value_buffer[value_buffer_count] = char_buffer;
                value_buffer_count++;
            }
        }
    }

    if (value_buffer_count > 0) {
        auto value = std::make_unique<char[]>(value_buffer_count + 1);
        std::copy(value_buffer.get(), value_buffer.get() + value_buffer_count, value.get());
        value[value_buffer_count] = '\0';
        return { Token::LITERAL, std::move(value) };
    } else {
        return { Token::END_OF_FILE };
    }
}

void tokenize(std::istream& istream, std::vector<Token>& out)
{
    Tokenizer tokenizer(istream);
    bool is_eof = true;
    do {
        Token token = tokenizer.get_next();
        is_eof = token.is_eof();
        if (!is_eof) { out.push_back(std::move(token)); }
    }
    while (!is_eof);
}

}
