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

const ParseError ParseError::unexpected_eof(ParseError::UNEXPECTED_EOF, std::string("Wow there, I didn't expect this end-of-file!"));

ParseError ParseError::unexpected_token(const Token& token) {
    if (token.tag == Token::LITERAL) {
        return ParseError(ParseError::UNEXPECTED_TOKEN, std::string("Did quite get what you meant by this buddy: ") + token.value.get());
    } else {
        return ParseError(ParseError::UNEXPECTED_TOKEN, std::string("Hey, is this a typo?: ") + static_cast<char>(token.tag));
    }
}

// Parsers

using Iterator = std::vector<Token>::iterator&;
using Out = std::unique_ptr<JSONValue>;

static void parse_json_token(Token::Type, Iterator);
static Out parse_json_value(Iterator, Iterator);
static Out parse_json_object(Iterator, Iterator);
std::pair<JSONValue::KeyType, std::unique_ptr<JSONValue>> parse_json_object_key_value(Iterator, Iterator);
static JSONValue::KeyType parse_json_object_key(Iterator, Iterator);
static Out parse_json_array(Iterator, Iterator);
static Out parse_json_string(Iterator, Iterator);

std::unique_ptr<JSONValue> parse(const std::string& string)
{
    if (string.empty()) { throw ParseError::unexpected_eof; }

    std::vector<Token> tokens;
    tokenize(string, tokens);

    if (tokens.empty()) { throw ParseError::unexpected_eof; }

    auto begin = tokens.begin();
    auto end = tokens.end();
    return std::unique_ptr<JSONValue>(parse_json_value(begin, end));
}

std::unique_ptr<JSONValue> parse(std::istream& istream)
{
    std::vector<Token> tokens;
    tokenize(istream, tokens);

    if (tokens.empty()) { throw ParseError::unexpected_eof; }

    auto begin = tokens.begin();
    auto end = tokens.end();
    return std::unique_ptr<JSONValue>(parse_json_value(begin, end));
}

static void parse_json_token(Token::Type token, Iterator iterator)
{
    if (iterator->tag == token) {
        iterator++;
    } else {
        throw ParseError::unexpected_token(*iterator);
    }
}

Out parse_json_value(Iterator iterator, Iterator end)
{
    switch (iterator->tag) {
        case Token::DOUBLE_QUOTE:
            return parse_json_string(iterator, end);
        case Token::LITERAL:
            switch (*iterator->value.get()) {
                case 't':
                    if (strcmp(iterator->value.get(), "true") == 0) {
                        iterator++;
                        return std::make_unique<JSONValue>(true);
                    } else {
                        throw ParseError::unexpected_token(*iterator);
                    }
                    break;
                case 'f':
                    if (strcmp(iterator->value.get(), "false") == 0) {
                        iterator++;
                        return std::make_unique<JSONValue>(false);
                    } else {
                        throw ParseError::unexpected_token(*iterator);
                    }
                    break;
                case 'n':
                    if(strcmp(iterator->value.get(), "null") == 0) {
                        iterator++;
                        return std::make_unique<JSONValue>();
                    } else {
                        throw ParseError::unexpected_token(*iterator);
                    }
                    break;
                default:
                    double number = strtod(iterator->value.get(), nullptr); // TODO: Handle endptr and out-of-range errors
                    iterator++;
                    return std::make_unique<JSONValue>(number);
            }
        case Token::OBJECT_OPEN:
            return parse_json_object(iterator, end);
        case Token::ARRAY_OPEN:
            return parse_json_array(iterator, end);
        default:
            throw ParseError::unexpected_token(*iterator);
    }
}

// Parse Object

Out parse_json_object(Iterator iterator, Iterator end)
{
    auto out = std::make_unique<JSONValue>(JSONValue::OBJECT);

    parse_json_token(Token::OBJECT_OPEN, iterator);

    while (iterator < end) {
        switch (iterator->tag) {
            case Token::DOUBLE_QUOTE: {
                auto key_value = parse_json_object_key_value(iterator, end);
                out->object[key_value.first] = std::move(key_value.second);
                break;
            }
            case Token::COMMA: // TODO: Handle invalid single comma in empty object
                iterator++;
                break;
            case Token::OBJECT_CLOSE:
                iterator++;
                return out;
            default:
                throw ParseError::unexpected_token(*iterator);
        }
    }

    throw ParseError::unexpected_eof;
}

std::pair<JSONValue::KeyType, std::unique_ptr<JSONValue>> parse_json_object_key_value(Iterator iterator, Iterator end)
{
    std::string key = parse_json_object_key(iterator, end);
    parse_json_token(Token::COLON, iterator);
    auto value = parse_json_value(iterator, end);
    return std::make_pair(key, std::move(value));
}

std::string parse_json_object_key(Iterator iterator, Iterator end)
{
    std::string key;
    parse_json_token(Token::DOUBLE_QUOTE, iterator);

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    if (iterator->tag == Token::LITERAL) { key = std::string(iterator->value.get()); }
    iterator++;

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    parse_json_token(Token::DOUBLE_QUOTE, iterator);

    return key;
}

// Parse Array

static Out parse_json_array(Iterator iterator, Iterator end)
{
    auto out = std::make_unique<JSONValue>(JSONValue::ARRAY);

    parse_json_token(Token::ARRAY_OPEN, iterator);

    while (iterator < end) {
        switch (iterator->tag) {
            case Token::COMMA:
                iterator++;
                break;
            case Token::ARRAY_CLOSE:
                iterator++;
                return out;
            default:
                auto value = parse_json_value(iterator, end);
                out->array.push_back(std::move(value));
                break;
        }
    }

    throw ParseError::unexpected_eof;
}

// Parse String

Out parse_json_string(Iterator iterator, Iterator end)
{
    JSONValue::StringType value;

    parse_json_token(Token::DOUBLE_QUOTE, iterator);

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    if (iterator->tag == Token::LITERAL) { value = JSONValue::StringType(iterator->value.get()); }
    iterator++;

    if (iterator >= end) { throw ParseError::unexpected_eof; }
    parse_json_token(Token::DOUBLE_QUOTE, iterator);

    return std::make_unique<JSONValue>(value);
}

}
