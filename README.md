# Mod Programming Language
Mod (or ModLang) is a minimal and toy programming language implemented in C++ and STL (Standard Template Library) without any other external dependencies. It is dynamically typed and it's syntax is majorly influenced by C/C++, Python and JavaScript. It is currently an interpreted language and supports imperative - procedural programming.

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
* [Mod Language](#modlang)

## Status
> Working on bytecode Compiler and Virtual Machine.

## Mod Language

### First Program
Writing a "Hello World" program in ModLang is as simple as:
```
print("Hello World");
```

### Data Types
Type      | Syntax                                       
--------- | -------------------
null      | `null`                                      
bool      | `true, false`                                
int       | `0, 100, -1, ...`                            
str       | `"foo", "bar", ...`                          

### Data Structures
Type      | Syntax                        
--------- | -----------------------------
array     | `[<elems>]`                   
hashmap   | `{<key> : <value>}`           
hashset   | `hashset<> {<key>}`           
stack     | `stack<> {<elems>}`           
queue     | `queue<> {<elems>}`           
deque     | `deque<> {<elems>}`           
max_heap  | `max_heap<type> {<type_elems>}`
min_heap  | `min_heap<type> {<type_elems>}`

### Binary and Unary Operators
Operators      | Description
-------------- | -----------
`* / %`        | Multiplication, Division, Modulo
`+ -`          | Addition, Subtraction
`< <= > >=`    | Comparison
`== !=`        | Equality
`-`            | Unary Minus
`!`            | Logical Not
`[]`           | Index

### Overloaded Operators
Operator   | Types           | Action
---------- | --------------- | ------
`+`        | `int + int`     | add ints
`+`        | `str + str`     | concatenate strs
`-`        | `int`           | negate int
`-`        | `int - int`     | subtract ints
`[]`       | `str[int]`      | fetch nth char of str (0-based)
`[]`       | `array[int]`    | fetch nth element of array (0-based)
`[]`       | `hashmap[str]`  | fetch value in hashmap by key str

### Conditional Expressions
Mod supports 'if' and 'else'.

```
if (condition/expression) {
	body;
}
else {
	other body;
}
```

### Looping Expressions
Mod supports 'while'.

```
while (condition/expression) {
	body;
}
```

### Variables and Assignments
Mod uses 'let' keyword to define a variable. Declaration alone is not allowed.

```
let var_name = expression;
```

Once defined you can reassign the variable to any other value/expression.
```
var_name = other_expression;
```