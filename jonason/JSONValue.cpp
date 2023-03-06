//
//  JSONValue.cpp
//  jonason
//
//  Created by Gabriel Lumbi on 2023-03-03.
//

#include "JSONValue.hpp"

#include <utility>

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

JSONValue::JSONValue(const JSONValue& other)
{
    if (this == &other) { return; }
    tag = other.tag;
    switch (tag) {
        case OBJECT: new(&object) ObjectType(other.object); break;
        case ARRAY: new(&array) ArrayType(other.array); break;
        case STRING: new(&string) StringType(other.string); break;
        case NUMBER: number = other.number; break;
        case BOOLEAN: boolean = other.boolean; break;
        case JSON_NULL: break;
    }
}

JSONValue& JSONValue::operator=(JSONValue other)
{
    using std::swap;
    swap(tag, other.tag);
    switch (tag) {
        case OBJECT: swap(object, other.object); break;
        case ARRAY: swap(array, other.array); break;
        case STRING: swap(string, other.string); break;
        case NUMBER: swap(number, other.number); break;
        case BOOLEAN: swap(boolean, other.boolean); break;
        case JSON_NULL: break;
    }
    return *this;
}

// Object

const JSONValue& JSONValue::operator[](const std::string& key) const
{
    auto found = object.find(key);
    if (found != object.end()) {
        return found->second;
    } else {
        return JSONValue::null;
    }
}

void JSONValue::set(KeyType key, JSONValue&& value)
{
    object[key] = value;
}

// Array

const JSONValue& JSONValue::operator[](std::size_t index) const
{
    if (index < array.size()) {
        return array[index];
    } else {
        return JSONValue::null;
    }
}

}
