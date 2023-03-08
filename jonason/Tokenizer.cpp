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
    other.value = nullptr;
    return *this;
}

Token::~Token() {
    delete[] value;
}

constexpr bool is_ws(char character) {
    return character == 0x20 || character == 0x0a || character == 0x0d || character == 0x09;
}

static void read_literal(std::istream& istream, char*& buffer, int& buffer_size, std::predicate<char> auto is_delimiter, std::vector<Token>& out);

void tokenize(const std::string& string, std::vector<Token>& out)
{
    std::istringstream istream(string);
    tokenize(istream, out);
}

const int MAX_LITERAL_SIZE = 1 << 16; // about 65KB

void tokenize(std::istream& istream, std::vector<Token>& out) {
    char char_buffer;
    char* value_buffer = new char[MAX_LITERAL_SIZE];
    int value_buffer_count = 0;

    istream >> std::ws;
    while (istream.get(char_buffer)) {
        switch (char_buffer) {
            case Token::OBJECT_OPEN: out.push_back({ Token::OBJECT_OPEN }); break;
            case Token::OBJECT_CLOSE: out.push_back({ Token::OBJECT_CLOSE }); break;
            case Token::ARRAY_OPEN: out.push_back({ Token::ARRAY_OPEN }); break;
            case Token::ARRAY_CLOSE: out.push_back({ Token::ARRAY_CLOSE }); break;
            case Token::COMMA: out.push_back({ Token::COMMA }); break;
            case Token::COLON: out.push_back({ Token::COLON }); break;

            case Token::DOUBLE_QUOTE:
                out.push_back({ Token::DOUBLE_QUOTE });
                if (value_buffer_count == 0) {
                    read_literal(istream, value_buffer, value_buffer_count, [](char c){ return c == Token::DOUBLE_QUOTE; }, out);
                } else {
                    value_buffer_count = 0;
                }
                break;

            default:
                value_buffer[0] = char_buffer;
                value_buffer_count = 1;
                read_literal(istream, value_buffer, value_buffer_count, [](char c) { return is_ws(c) || c == Token::COMMA || c == Token::OBJECT_CLOSE || c == Token::ARRAY_CLOSE; }, out);
                break;
        }

        istream >> std::ws;
    }

    delete[] value_buffer;
}

void read_literal(std::istream& istream, char*& buffer, int& buffer_size, std::predicate<char> auto is_delimiter, std::vector<Token>& out)
{
    char char_buffer;
    while (istream.good()) {
        char peek = istream.peek();
        if (is_delimiter(peek)) {
            break;
        } else {
            istream.get(char_buffer);
            if (buffer_size < MAX_LITERAL_SIZE) {
                buffer[buffer_size] = char_buffer;
                buffer_size++;
            }
        }
    }
    char* value = new char[buffer_size + 1];
    std::copy(buffer, buffer + buffer_size, value);
    value[buffer_size] = '\0';
    out.push_back({ value });
}

}
