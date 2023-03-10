//
//  Parser.cpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#include "Parser.hpp"

#include "Tokenizer.hpp"

#include <cassert>
#include <sstream>

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

using Out = std::unique_ptr<JSONValue>;

static Out parse_json_value(Token&, Tokenizer&);
static Out parse_json_object(Tokenizer&);
static Out parse_json_array(Tokenizer&);
static Out parse_json_string(Token&);
static Out parse_json_literal(Token&);

std::unique_ptr<JSONValue> parse(const std::string& string)
{
    std::istringstream istream(string);
    return parse(istream);
}

std::unique_ptr<JSONValue> parse(std::istream& istream)
{
    Tokenizer tokenizer(istream);
    Token token = tokenizer.get_next();
    return std::unique_ptr<JSONValue>(parse_json_value(token, tokenizer));
}

Out parse_json_value(Token& token, Tokenizer& tokenizer)
{
    switch (token.tag) {
        case Token::STRING:
            return parse_json_string(token);
        case Token::LITERAL:
            return parse_json_literal(token);
        case Token::OBJECT_OPEN:
            return parse_json_object(tokenizer);
        case Token::ARRAY_OPEN:
            return parse_json_array(tokenizer);
        case Token::END_OF_FILE:
            throw ParseError::unexpected_eof;
        default:
            throw ParseError::unexpected_token(token);
    }
}

Out parse_json_object(Tokenizer& tokenizer)
{
    auto out = std::make_unique<JSONValue>(JSONValue::OBJECT);
    while (true) {
        Token token = tokenizer.get_next();
        switch (token.tag) {
            case Token::STRING: {
                std::string key(token.value.get());
                Token token_colon = tokenizer.get_next();
                if (token_colon.tag != Token::COLON) { throw ParseError::unexpected_token(token_colon); }
                Token token_value = tokenizer.get_next();
                auto value = parse_json_value(token_value, tokenizer);
                out->object[key] = std::move(value);
                break;
            }
            case Token::COMMA: break; // TODO: Handle invalid single comma in empty object
            case Token::OBJECT_CLOSE: return out;
            case Token::END_OF_FILE: throw ParseError::unexpected_eof;
            default: throw ParseError::unexpected_token(token);
        }
    }
}

static Out parse_json_array(Tokenizer& tokenizer)
{
    auto out = std::make_unique<JSONValue>(JSONValue::ARRAY);
    while (true) {
        Token token = tokenizer.get_next();
        switch (token.tag) {
            case Token::COMMA: break;
            case Token::ARRAY_CLOSE: return out;
            case Token::END_OF_FILE: throw ParseError::unexpected_eof;
            default:
                auto value = parse_json_value(token, tokenizer);
                out->array.push_back(std::move(value));
                break;
        }
    }
}

Out parse_json_string(Token& token)
{
    if (token.tag != Token::STRING) { throw ParseError::unexpected_token(token); }
    JSONValue::StringType value(token.value.get());
    return std::make_unique<JSONValue>(value);
}

Out parse_json_literal(Token& token)
{
    switch (*token.value.get()) {
        case 't':
            if (strcmp(token.value.get(), "true") == 0) {
                return std::make_unique<JSONValue>(true);
            } else {
                throw ParseError::unexpected_token(token);
            }
            break;
        case 'f':
            if (strcmp(token.value.get(), "false") == 0) {
                return std::make_unique<JSONValue>(false);
            } else {
                throw ParseError::unexpected_token(token);
            }
            break;
        case 'n':
            if(strcmp(token.value.get(), "null") == 0) {
                return std::make_unique<JSONValue>();
            } else {
                throw ParseError::unexpected_token(token);
            }
            break;
        default:
            double number = strtod(token.value.get(), nullptr); // TODO: Handle endptr and out-of-range errors
            return std::make_unique<JSONValue>(number);
    }
}

}
