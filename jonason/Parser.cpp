//
//  Parser.cpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#include "Parser.hpp"

#include "Tokenizer.hpp"

namespace jonason {

// Errors

const ParseError ParseError::unexpected_eof(ParseError::UNEXPECTED_EOF, std::string("Unexpected end-of-file"));

ParseError ParseError::unexpected_token(const Token& token) {
    if (token.tag == Token::LITERAL) {
        return ParseError(ParseError::UNEXPECTED_TOKEN, std::string("Unexpected token: ") + token.value);
    } else {
        return ParseError(ParseError::UNEXPECTED_TOKEN, std::string("Unexpected token: ") + static_cast<char>(token.tag));
    }
}

const ParseError ParseError::unknown_error(ParseError::UNKNOWN, std::string("Unknown error"));

// Parsers

using Iterator = std::vector<Token>::iterator&;
using Out = JSONValue&;

static void parse_json_token(Token::Type, Iterator);
static void parse_json_value(Iterator, Iterator, Out);
static void parse_json_object(Iterator, Iterator, Out);
static void parse_json_object_key_value(Iterator, Iterator, Out);
static void parse_json_object_key(Iterator, Iterator, JSONValue::KeyType& out);
static void parse_json_array(Iterator, Iterator, Out);
static void parse_json_string(Iterator, Iterator, Out);

void parse(const std::string& string, JSONValue& out)
{
    if (string.empty()) { throw ParseError::unexpected_eof; }

    std::vector<Token> tokens;
    tokenize(string, tokens);

    if (tokens.empty()) { throw ParseError::unexpected_eof; }

    auto begin = tokens.begin();
    auto end = tokens.end();
    parse_json_value(begin, end, out);
}

static void parse_json_token(Token::Type token, Iterator iterator)
{
    if (iterator->tag == token) {
        iterator++;
    } else {
        throw ParseError::unexpected_token(*iterator);
    }
}

void parse_json_value(Iterator iterator, Iterator end, Out out)
{
    switch (iterator->tag) {
        case Token::OBJECT_OPEN:
            parse_json_object(iterator, end, out);
            break;
        case Token::ARRAY_OPEN:
            parse_json_array(iterator, end, out);
            return;
        case Token::DOUBLE_QUOTE:
            parse_json_string(iterator, end, out);
            break;
        case Token::LITERAL:
            // TODO: parse literal (true, false, number or null)
            return;
        default:
            throw ParseError::unexpected_token(*iterator);
    }
}

// Parse Object

void parse_json_object(Iterator iterator, Iterator end, Out out)
{
    out = JSONValue(JSONValue::OBJECT);

    parse_json_token(Token::OBJECT_OPEN, iterator);

    while (iterator < end) {
        switch (iterator->tag) {
            case Token::DOUBLE_QUOTE:
                parse_json_object_key_value(iterator, end, out);
                break;
            case Token::COMMA: // TODO: Handle invalid single comma in empty object
                iterator++;
                break;
            case Token::OBJECT_CLOSE:
                iterator++;
                return;
            default:
                throw ParseError::unexpected_token(*iterator);
        }
    }

    throw ParseError::unexpected_eof;
}

void parse_json_object_key_value(Iterator iterator, Iterator end, Out out)
{
    std::string key;
    parse_json_object_key(iterator, end, key);
    parse_json_token(Token::COLUMN, iterator);

    JSONValue value;
    parse_json_value(iterator, end, value);

    out.set(key, std::move(value));
}

void parse_json_object_key(Iterator iterator, Iterator end, std::string& out)
{
    parse_json_token(Token::DOUBLE_QUOTE, iterator);

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    if (iterator->tag == Token::LITERAL) { out = std::string(iterator->value); }
    iterator++;

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    parse_json_token(Token::DOUBLE_QUOTE, iterator);
}

// Parse Array

static void parse_json_array(Iterator iterator, Iterator end, Out out)
{
    out = JSONValue(JSONValue::ARRAY);

    parse_json_token(Token::ARRAY_OPEN, iterator);

    while (iterator < end) {
        switch (iterator->tag) {
            case Token::COMMA:
                iterator++;
                break;
            case Token::ARRAY_CLOSE:
                iterator++;
                return;
            default:
                JSONValue value;
                parse_json_value(iterator, end, value);
                out.array.push_back(std::move(value));
                break;
        }
    }

    throw ParseError::unexpected_eof;
}

// Parse String

void parse_json_string(Iterator iterator, Iterator end, Out out)
{
    parse_json_token(Token::DOUBLE_QUOTE, iterator);

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    if (iterator->tag == Token::LITERAL) { out = JSONValue(std::string(iterator->value)); }
    iterator++;

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    parse_json_token(Token::DOUBLE_QUOTE, iterator);
}

}
