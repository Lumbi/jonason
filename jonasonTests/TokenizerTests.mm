//
//  Tests.cpp
//  jonasonTests
//
//  Created by Gabriel Lumbi on 2023-03-02.
//

#import <XCTest/XCTest.h>

#include <jonason/jonason.h>

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

@interface TokenizerTestCase : XCTestCase

@end

@implementation TokenizerTestCase

- (void) testStringLiteral {
    std::vector<jonason::Token> tokens;
    std::string json(R"(  "te st"  )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 3);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(1).tag, jonason::Token::LITERAL);
    XCTAssertEqual(std::string(tokens.at(1).value), std::string("te st"));
    XCTAssertEqual(tokens.at(2).tag, jonason::Token::DOUBLE_QUOTE);
}

- (void) testNullLiteral {
    std::vector<jonason::Token> tokens;
    std::string json(R"( null  )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 1);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::LITERAL);
    XCTAssertEqual(std::string(tokens.at(0).value), std::string("null"));
}

- (void) testTrueLiteral {
    std::vector<jonason::Token> tokens;
    std::string json(R"(  true  )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 1);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::LITERAL);
    XCTAssertEqual(std::string(tokens.at(0).value), std::string("true"));
}

- (void) testFalseLiteral {
    std::vector<jonason::Token> tokens;
    std::string json(R"(  false  )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 1);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::LITERAL);
    XCTAssertEqual(std::string(tokens.at(0).value), std::string("false"));
}

- (void) testObject {

    std::vector<jonason::Token> tokens;
    std::string json(R"({ "hello": "world" })");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 9);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::OBJECT_OPEN);
    XCTAssertEqual(tokens.at(1).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(2).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(3).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(4).tag, jonason::Token::COLON);
    XCTAssertEqual(tokens.at(5).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(6).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(7).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(8).tag, jonason::Token::OBJECT_CLOSE);
}

- (void) testArray {
    std::vector<jonason::Token> tokens;
    std::string json(R"( [ 1, 2, 3 ] )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 7);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::ARRAY_OPEN);
    XCTAssertEqual(tokens.at(1).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(2).tag, jonason::Token::COMMA);
    XCTAssertEqual(tokens.at(3).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(4).tag, jonason::Token::COMMA);
    XCTAssertEqual(tokens.at(5).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(6).tag, jonason::Token::ARRAY_CLOSE);
}

- (void) test_array_with_literal {
    std::vector<jonason::Token> tokens;
    std::string json(R"( [ true ] )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 3);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::ARRAY_OPEN);
    XCTAssertEqual(tokens.at(1).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(2).tag, jonason::Token::ARRAY_CLOSE);
}

- (void) test_nested_array {
    std::vector<jonason::Token> tokens;
    std::string json(R"( [ [ true] ] )");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 5);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::ARRAY_OPEN);
    XCTAssertEqual(tokens.at(1).tag, jonason::Token::ARRAY_OPEN);
    XCTAssertEqual(tokens.at(2).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(3).tag, jonason::Token::ARRAY_CLOSE);
    XCTAssertEqual(tokens.at(4).tag, jonason::Token::ARRAY_CLOSE);
}

- (void) testObjectWithNumber {
    std::vector<jonason::Token> tokens;
    std::string json(R"({"hello":1234})");
    std::istringstream istream(json);
    jonason::tokenize(istream, tokens);

    XCTAssertEqual(tokens.size(), 7);
    XCTAssertEqual(tokens.at(0).tag, jonason::Token::OBJECT_OPEN);
    XCTAssertEqual(tokens.at(1).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(2).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(3).tag, jonason::Token::DOUBLE_QUOTE);
    XCTAssertEqual(tokens.at(4).tag, jonason::Token::COLON);
    XCTAssertEqual(tokens.at(5).tag, jonason::Token::LITERAL);
    XCTAssertEqual(tokens.at(6).tag, jonason::Token::OBJECT_CLOSE);
}

@end
