//
//  JSONValue.hpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#ifndef JSONValue_hpp
#define JSONValue_hpp

#include <unordered_map>
#include <vector>
#include <string>

namespace jonason {

struct JSONValue {
    enum { OBJECT, ARRAY, STRING, NUMBER, BOOLEAN, JSON_NULL } tag;

    using ObjectType = std::unordered_map<std::string, JSONValue>;
    using ArrayType = std::vector<JSONValue>;
    using StringType = std::string;
    using NumberType = double;
    using BooleanType = bool;
    using KeyType = std::string;

    union {
        ObjectType object;
        ArrayType array;
        StringType string;
        NumberType number;
        BooleanType boolean;
    };

    explicit JSONValue(): tag(JSON_NULL), boolean(false) {};
    explicit JSONValue(decltype(OBJECT) tag): tag(tag) {
        switch (tag) {
            case OBJECT: new(&object) ObjectType(); break;
            case ARRAY: new(&array) ArrayType(); break;
            case STRING: new(&string) StringType(); break;
            case NUMBER: number = 0; break;
            case BOOLEAN: boolean = false; break;
            case JSON_NULL: boolean = false; break;
        }
    };

    explicit JSONValue(const ObjectType& value): tag(OBJECT), object(value) {};
    explicit JSONValue(const ArrayType& value): tag(ARRAY), array(value) {};
    explicit JSONValue(const StringType& value): tag(STRING), string(value) {};
    explicit JSONValue(NumberType value): tag(NUMBER), number(value) {};
    explicit JSONValue(BooleanType value): tag(BOOLEAN), boolean(value) {};

    JSONValue(const JSONValue&);
    JSONValue& operator=(JSONValue);

    ~JSONValue() {};

    // Object

    const JSONValue& operator[](const KeyType&) const;
    void set(KeyType, JSONValue&&);

    // Array

    const JSONValue& operator[](std::size_t) const;

    // Null

    static const JSONValue null;
    bool is_null() const { return tag == JSON_NULL; }
};

inline const JSONValue JSONValue::null = JSONValue();

}

#endif /* JSONValue_hpp */
