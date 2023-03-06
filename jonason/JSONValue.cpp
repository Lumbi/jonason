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
        case JSON_NULL: boolean = false; break;
    }
}

JSONValue& JSONValue::operator=(const JSONValue& other)
{
    switch (tag) {
        case OBJECT: object.~ObjectType(); break;
        case ARRAY: array.~ArrayType(); break;
        case STRING: string.~StringType(); break;
        default: break;
    }
    tag = other.tag;
    switch (tag) {
        case OBJECT: new(&object) ObjectType(other.object); break;
        case ARRAY: new(&array) ArrayType(other.array); break;
        case STRING: new(&string) StringType(other.string); break;
        case NUMBER: number = other.number; break;
        case BOOLEAN: boolean = other.boolean; break;
        case JSON_NULL: boolean = false; break;
    }
    return *this;
}

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
    switch (tag) {
        case OBJECT: object.~ObjectType(); break;
        case ARRAY: array.~ArrayType(); break;
        case STRING: string.~StringType(); break;
        default: break;
    }
    tag = other.tag;
    switch (tag) {
        case OBJECT: new(&object) ObjectType(std::move(other.object)); break;
        case ARRAY: new(&array) ArrayType(std::move(other.array)); break;
        case STRING: new(&string) StringType(std::move(other.string)); break;
        case NUMBER: number = other.number; break;
        case BOOLEAN: boolean = other.boolean; break;
        case JSON_NULL: boolean = false; break;
    }
    return *this;
}

JSONValue::~JSONValue()
{
    switch (tag) {
        case OBJECT:
            for (auto&& element : object) { delete element.second; element.second = nullptr; } break;
        case ARRAY:
            for (auto&& element : array) { delete element; element = nullptr; } break;
        default:
            break;
    }
}

// Object

const JSONValue& JSONValue::operator[](const std::string& key) const
{
    auto found = object.find(key);
    if (found != object.end() && found->second) return *found->second;
    return JSONValue::null;
}

void JSONValue::set(KeyType key, JSONValue* value)
{
    delete object[key];
    object[key] = value;
}

// Array

const JSONValue& JSONValue::operator[](std::size_t index) const
{
    if (index < array.size() && array[index]) return *array[index];
    return JSONValue::null;
}

}
