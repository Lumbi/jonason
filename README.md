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
- Just use `std::unique_ptr`
- Using `std::istream::get` to skip whitespaces was surprisingly slow compared to `std::ws`

What I'd like to improve if I could lock myself in a cave and code for a week:
- Merge the tokenize and parse steps to reduce the overhead
- Use `JSON_checker` to test conformance to specification

Parsing performance results
- clang-1400.0.29.202
- 2.9 GHz 6-Core Intel Core i9

| input        | jonason  | NSJSONSerialization |
| -            | -        | -                   |
| canada.json  | ~105 ms  | ~75 ms              |
| citm_ca.json | ~21 ms   | ~10 ms              |

Unfortunately, `jonsason` is not only unreliable, but also abit slow. Regardless, I'm pretty happy with the results.
