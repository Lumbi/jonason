//
//  Parser.hpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <string>

#include "JSONValue.hpp"
#include "Tokenizer.hpp"

struct JSONValue;

namespace jonason {

struct ParseError: public std::exception {
    enum Code {
        UNEXPECTED_EOF = 1,
        UNEXPECTED_TOKEN = 2,
        UNKNOWN = 999
    };

    Code code;
    std::string message;

    explicit ParseError(Code code, const std::string& message) : code(code), message(message) {};

    explicit operator bool() { return code; }

    static const ParseError unexpected_eof;
    static ParseError unexpected_token(const Token& token);
    static const ParseError unknown_error;
};


void parse(const std::string&, JSONValue& out);
void parse(std::istream& istream, JSONValue& out);

}

#endif /* Parser_hpp */
