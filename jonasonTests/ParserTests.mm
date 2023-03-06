//
//  ParserTests.m
//  jonasonTests
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#import <XCTest/XCTest.h>

#include <jonason/jonason.h>
#include <string>

@interface ParserTests : XCTestCase

@end

@implementation ParserTests

- (void) test_json_true {
    std::string string(R"( true )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::BOOLEAN);
    XCTAssertEqual(json.boolean, true);
}

- (void) test_json_false {
    std::string string(R"( false )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::BOOLEAN);
    XCTAssertEqual(json.boolean, false);
}

- (void) test_json_null {
    std::string string(R"( null )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::JSON_NULL);
    XCTAssert(json.is_null());
}

- (void) test_json_number {
    std::string string(R"( 3.1416 )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::NUMBER);
    XCTAssertEqual(json.number, 3.1416);
}

- (void) test_object_empty {
    std::string string(R"( {  } )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::OBJECT);
    XCTAssertEqual(json["hello"].tag, jonason::JSONValue::JSON_NULL);
}

- (void) test_object_with_single_string {
    std::string string(R"( { "hello": "world" } )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::OBJECT);
    XCTAssertEqual(json["hello"].tag, jonason::JSONValue::STRING);
    XCTAssertEqual(json["hello"].string, std::string("world"));
}

- (void) test_object_with_nested_object {
    std::string string(R"( { "foo": { "bar": 17 } } )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::OBJECT);
    XCTAssertEqual(json["foo"].tag, jonason::JSONValue::OBJECT);
    XCTAssertEqual(json["foo"]["bar"].tag, jonason::JSONValue::NUMBER);
    XCTAssertEqual(json["foo"]["bar"].number, 17);
}

- (void) test_object_with_nested_array {
    std::string string(R"( { "foo": [ "bar", 17 ] } )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::OBJECT);
    XCTAssertEqual(json["foo"].tag, jonason::JSONValue::ARRAY);
    XCTAssertEqual(json["foo"][0].tag, jonason::JSONValue::STRING);
    XCTAssertEqual(json["foo"][0].string, std::string("bar"));
    XCTAssertEqual(json["foo"][1].tag, jonason::JSONValue::NUMBER);
    XCTAssertEqual(json["foo"][1].number, 17);
}

- (void) test_array_empty {
    std::string string(R"( [ ] )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::ARRAY);
    XCTAssertEqual(json[0].tag, jonason::JSONValue::JSON_NULL);
}

- (void) test_array_with_single_string {
    std::string string(R"( [ "hello" ] )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::ARRAY);
    XCTAssertEqual(json[0].tag, jonason::JSONValue::STRING);
    XCTAssertEqual(json[0].string, std::string("hello"));
}

- (void) test_array_with_nested_array {
    std::string string(R"( [ ["foo", true] ] )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::ARRAY);
    XCTAssertEqual(json.array.size(), 1);
    XCTAssertEqual(json[0].tag, jonason::JSONValue::ARRAY);
    XCTAssertEqual(json[0].array.size(), 2);
    XCTAssertEqual(json[0][0].tag, jonason::JSONValue::STRING);
    XCTAssertEqual(json[0][0].string, std::string("foo"));
    XCTAssertEqual(json[0][1].tag, jonason::JSONValue::BOOLEAN);
    XCTAssertEqual(json[0][1].boolean, true);
}

- (void) test_array_with_nested_object {
    std::string string(R"( [ { "foo": true } ] )");
    jonason::JSONValue json;
    jonason::parse(string, json);

    XCTAssertEqual(json.tag, jonason::JSONValue::ARRAY);
    XCTAssertEqual(json.array.size(), 1);
    XCTAssertEqual(json[0].tag, jonason::JSONValue::OBJECT);
    XCTAssertEqual(json[0]["foo"].tag, jonason::JSONValue::BOOLEAN);
    XCTAssertEqual(json[0]["foo"].boolean, true);
}

- (void) testUnexpectedEOFError {
    try {
        std::string string(R"( { "hello" : "world" )");
        jonason::JSONValue json;
        jonason::parse(string, json);
        XCTFail();
    } catch (const jonason::ParseError& error) {
        XCTAssertEqual(error.code, jonason::ParseError::UNEXPECTED_EOF);
    }
}

- (void) testUnexpectedTokenError {
    try {
        std::string string(R"( { "hello" ] "world" } )");
        jonason::JSONValue json;
        jonason::parse(string, json);
        XCTFail();
    } catch (const jonason::ParseError& error) {
        XCTAssertEqual(error.code, jonason::ParseError::UNEXPECTED_TOKEN);
    }
}

- (void) testUnexpectedLiteralError {
    try {
        std::string string(R"( { hello: "world" } )");
        jonason::JSONValue json;
        jonason::parse(string, json);
        XCTFail();
    } catch (const jonason::ParseError& error) {
        XCTAssertEqual(error.code, jonason::ParseError::UNEXPECTED_TOKEN);
    }
}

@end
