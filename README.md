# Mod Programming Language

Mod (or ModLang) is a minimal and toy programming language implemented in C++ and Standard Template Library (STL) without any other external dependencies. It is dynamically typed and it's syntax is majorly influenced by C/C++, Python and JavaScript. It is currently an interpreted language and supports imperative - procedural programming.

## Features
- Dynamically typed and interpreted. Good for small scale scripting.
- Easy short syntax resembling closely to that of C/C++, Python and JavaScript.
- Integer, boolean, and string primitive data types.
- Basic arithmetic for integer expressions.
- If-else conditional statement and while loop statement.
- First class functions, higher-order functions, closures and recursion.
- Data structures like arrays, hashmaps, hashsets, stacks, queues and more.
- Built-in utility functions.
- An REPL (Read Evaluate Print Loop) shell to try and test out the language.

## Documentation
### Table of Contents
* [Status](#status)
* [Mod Language](#mod-language)

## Status
> Working on a bytecode Compiler with Virtual Machine to improve the speed of the language.

## Mod Language

### First Program
Writing a "Hello World" program in ModLang is as simple as:
```
print("Hello World");
```

### Data Types
`null`, `bool`, `int`, `str`. 

Type      | Syntax                                       
--------- | -------------------
null      | `null`                                      
bool      | `true, false`                                
int       | `0, 100, -1, ...`                            
str       | `"foo", "bar", ...`                          

### Data Structures
`array`, `hashmap`, `hashset`, `stack`, `queue`, `deque`, `min_heap`, `max_heap`.

Type      | Syntax                                       
--------- | -------------------
null      | `null`                                      
bool      | `true, false`                                
int       | `0, 100, -1, ...`                            
str       | `"foo", "bar", ...`

### Binary and unary operators

Operators      | Description
-------------- | -----------
`* / %`        | Multiplication, Division, Modulo
`+ -`          | Addition, Subtraction
`< <= > >=`    | Comparison
`== !=`        | Equality
`-`            | Unary minus
`!`            | Logical not
`[]`           | Subscript

Overloaded operators: 

Operator   | Types           | Action
---------- | --------------- | ------
`+`        | `int + int`     | add ints
`+`        | `str + str`     | concatenate strs
`-`        | `int`           | negate int
`-`        | `int - int`     | subtract ints
`[]`       | `str[int]`      | fetch nth char of str (0-based)
`[]`       | `array[int]`    | fetch nth element of array (0-based)
`[]`       | `hashmap[str]`  | fetch value in hashmap by key str
