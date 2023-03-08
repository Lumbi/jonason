# `jonason`, your friendly yet unreliable JSON parser

`jonason` is just a practice project to mess around with optimizing in C++. I named it `jonason` because it kinda sounds like JSON and kinda sounds like a name (and the Jason pun has been overused at this point).

Far from compliant, but it can still parse some basic JSON.

Known limitations:
- Error handling is awful, you either get "Unexpected EOF" or "Unexpected token"
- Invalid number literals are just parsed as 0 and overflows are not handled
- An empty array with commas is parsed just fine (should be invalid)
- Probably doesn't support many encodings well

Learnings:
- I started with value semantics for `JSONValue` but quickly switched to pointers when the benchmark was calling the copy-assignment operator millions of times
- Messing around with anonymous unions was "interesting" but made copy-constructor and copy-assignment operator pretty annoying to implement
- Always run tests with address sanitizer on

What I'd like to improve if I could lock myself in a cave and code for a week:
- Safer memory management
- Merge the tokenize and parse steps to reduce the overhead
- Using exceptions added about 20ms overhead on a 115ms benchmark so it would be nice to get rid of it
- Safer `char*` handling to avoid security issues
- Use `JSON_checker` to test conformance to specification
