//
//  Tokenizer.cpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-02.
//

#include "Tokenizer.hpp"

#include <sstream>

namespace jonason {

Token::Token(Token& other) {
    if (this == &other) { return; }

    tag = other.tag;

    delete value;
    value = nullptr;
    size_t size = strlen(other.value);
    value = size ? new char[size] : nullptr;
    std::copy(other.value, other.value + size, value);
}

Token& Token::operator=(Token other) {
    using std::swap;
    swap(tag, other.tag);
    swap(value, other.value);
    return *this;
}

Token::Token(Token&& other) {
    tag = std::move(other.tag);
    value = std::move(other.value);
    other.value = nullptr;
}

Token& Token::operator=(Token&& other) {
    tag = std::move(other.tag);
    value = std::move(other.value);
    other.value = nullptr;
    return *this;
}

Token::~Token() {
    delete[] value;
    value = nullptr;
}

constexpr bool is_ws(char character) {
    return character == 0x20 || character == 0x0a || character == 0x0d || character == 0x09;
}

static void read_literal(std::istream& istream, std::vector<char>& buffer, std::predicate<char> auto is_delimiter, std::vector<Token>& out);

void tokenize(const std::string& string, std::vector<Token>& out)
{
    std::istringstream istream(string);
    tokenize(istream, out);
}

void tokenize(std::istream& istream, std::vector<Token>& out) {
    char char_buffer;
    std::vector<char> value_buffer;

    istream >> std::ws;
    while (istream.get(char_buffer)) {
        switch (char_buffer) {
            case Token::OBJECT_OPEN: out.push_back({ Token::OBJECT_OPEN }); break;
            case Token::OBJECT_CLOSE: out.push_back({ Token::OBJECT_CLOSE }); break;
            case Token::ARRAY_OPEN: out.push_back({ Token::ARRAY_OPEN }); break;
            case Token::ARRAY_CLOSE: out.push_back({ Token::ARRAY_CLOSE }); break;
            case Token::COMMA: out.push_back({ Token::COMMA }); break;
            case Token::COLUMN: out.push_back({ Token::COLUMN }); break;

            case Token::DOUBLE_QUOTE:
                out.push_back({ Token::DOUBLE_QUOTE });
                if (value_buffer.empty()) {
                    read_literal(istream, value_buffer, [](char c){ return c == Token::DOUBLE_QUOTE; }, out);
                } else {
                    value_buffer.clear();
                }
                break;

            default:
                value_buffer.clear();
                value_buffer.push_back(char_buffer);
                read_literal(istream, value_buffer, [](char c) { return is_ws(c) || c == Token::COMMA || c == Token::OBJECT_CLOSE; }, out);
                break;
        }

        istream >> std::ws;
    }
}

void read_literal(std::istream& istream, std::vector<char>& buffer, std::predicate<char> auto is_delimiter, std::vector<Token>& out)
{
    char char_buffer;
    while (istream.good()) {
        char peek = istream.peek();
        if (is_delimiter(peek)) {
            break;
        } else {
            istream.get(char_buffer);
            buffer.push_back(char_buffer);
        }
    }
    char* value = new char[buffer.size() + 1];
    std::copy(buffer.begin(), buffer.end(), value);
    value[buffer.size()] = '\0';
    out.push_back({ value });
}

}
