//
//  JSONValue.cpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#include "JSONValue.hpp"

#include <utility>
#include <cassert>

namespace jonason {

JSONValue::JSONValue(decltype(OBJECT) tag): tag(tag) {
    switch (tag) {
        case OBJECT: new(&object) ObjectType(); break;
        case ARRAY: new(&array) ArrayType(); break;
        case STRING: new(&string) StringType(); break;
        case NUMBER: number = 0; break;
        case BOOLEAN: boolean = false; break;
        case JSON_NULL: boolean = false; break;
    }
};

JSONValue::JSONValue(JSONValue&& other)
{
    tag = other.tag;
    switch (tag) {
        case OBJECT: new(&object) ObjectType(std::move(other.object)); break;
        case ARRAY: new(&array) ArrayType(std::move(other.array)); break;
        case STRING: new(&string) StringType(std::move(other.string)); break;
        case NUMBER: number = other.number; break;
        case BOOLEAN: boolean = other.boolean; break;
        case JSON_NULL: boolean = false; break;
    }
}

JSONValue& JSONValue::operator=(JSONValue&& other)
{
    if (this == &other) { return *this; }
    this->~JSONValue();
    new(this) JSONValue(std::move(other));
    return *this;
}

JSONValue::~JSONValue()
{
    switch (tag) {
        case OBJECT:
            object.~ObjectType();
            break;
        case ARRAY:
            array.~ArrayType();
            break;
        case STRING:
            string.~StringType();
            break;
        default:
            break;
    }
}

// Object

const JSONValue& JSONValue::operator[](const std::string& key) const
{
    assert(tag == OBJECT);
    auto found = object.find(key);
    if (found != object.end() && found->second) return *found->second;
    return JSONValue::null;
}

// Array

const JSONValue& JSONValue::operator[](std::size_t index) const
{
    assert(tag == ARRAY);
    if (index < array.size() && array[index]) return *array[index];
    return JSONValue::null;
}

}
