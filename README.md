<p align="center">
  <img width="100" src="https://github.com/pranavmodx/modlang/blob/logo/resources/logofire_80px_png_trans.png" alt="Fire Mod Logo">
</p>

# ![alt text](https://github.com/pranavmodx/modlang/blob/logo/resources/logofire_40px_png_trans.png)od Programming Language                     
Mod (or ModLang) is a minimal, toy programming language implemented in C++ and STL (Standard Template Library) without any other external dependencies. It is dynamically typed and it's syntax is majorly influenced by C/C++, Python and JavaScript. It is currently an interpreted language and supports imperative - procedural programming.

## Features
- Dynamically typed.
- Simple short syntax.
- Integer, boolean, and string primitive data types.
- Basic arithmetic for integer expressions.
- If-else conditional statement and while loop statement.
- First class functions, higher-order functions, closures and recursion.
- Data structures like arrays, hashmaps, hashsets, stacks, queues and more.
- Built-in utility functions.
- An REPL (Read Evaluate Print Loop) shell for experimenting.

## Documentation
### Table of Contents
- [Mod Programming Language](#mod-programming-language)
	- [Features](#features)
	- [Status](#status)
	- [Usage](#usage)
	- [Mod Language](#mod-language)
		- [First Program](#first-program)
		- [Data Types](#data-types)
		- [Data Structures](#data-structures)
		- [Binary and Unary Operators](#binary-and-unary-operators)
			- [Overloaded Operators](#overloaded-operators)
		- [Control Statements](#control-statements)
			- [Conditional Expressions](#conditional-expressions)
			- [Looping Expressions](#looping-expressions)
		- [Variables and Assignments](#variables-and-assignments)
		- [Functions](#functions)
			- [Closures](#closures)
			- [Recursive Functions](#recursive-functions)
			- [Built-in Functions](#built-in-functions)
	- [References](#references)
	- [Credits](#credits)
	- [Contributing](#contributing)
	- [Support](#support)

## Status
> Working on bytecode Compiler and Virtual Machine.

## Usage
```bash
> make
```
or 
```bash
> g++ -std=c++11 main.cpp src/*.cpp -o mod --debug
```

```bash
> ./mod examples/test.modx
Hello world!
```

- Replace main.cpp with repl.cpp, rppl.cpp or rlpl.cpp for experimenting with interactive shell

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
Type      | Syntax                        	| Example
--------- | -----------------------------	| ---------------------
array     | `[<elems>]`                   	| let x = [1, "str", {key: val}];
hashmap   | `{<key> : <value>}`           	| let x = {5: 25, 6: 36, "name": "mod"};
hashset   | `hashset<> {<key>}`           	| let x = hashset<> {9, "bar", false};
stack     | `stack<> {<elems>}`           	| let x = stack<> {1, "foo", node};
queue     | `queue<> {<elems>}`           	| let x = queue<> {node, -1};
deque     | `deque<> {<elems>}`           	| let x = deque<> {idx, "substr"};
max_heap  | `max_heap<type> {<type_elems>}`	| let x = max_heap<int> {4, 5, 9};
min_heap  | `min_heap<type> {<type_elems>}`	| let x = min_heap<str> {"p", "m", "x"};

Note: max_heap and min_heap needs type specified in order for the priority function to map the elements.

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

### Control Statements

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

### Functions
Mod supports named and anonymous functions.

```
let square = def(x) { x * x };
square(5); // 25

def(x) { x * x; }(5); // 25
```

### Closures
Closures are functions inside functions that reference outer variables.

```
let tripleCall = def(x, func) { 
	func(func(func(x))); 
};

tripleCall(2, square); // 256
```

### Recursive Functions

```
let gcd = def(x, y) { 
	if (y == 0) { 
		return x; 
	}

	return gcd(y, x % y); 
}

gcd(27, 3); // 3

let factorial = def(n) { 
	if (n == 0) { 
		return 1;
	} 
	else { 
		return n * factorial(n - 1) 
	} 
};

factorial(5); // 120
```

### Built-in Functions

- `print(object...)`
	Prints the object(s) to standard output followed by a newline.

- `len(object)`
	Print the length of the object (String, Array and Hashmap).

- `size(object)`
	Print the length of the object (Hashset, Stack, Queue, Deque, Max Heap, Min Heap).

- `push(container_object, object)`
	Pushes an object onto a container object. (All Data Structures except Deque).

- `push_front(container_object, object)`
	Pushes an object onto a container object at the front. (Only deque).

- `push_back(container_object, object)`
	Pushes an object onto a container object at the back. (Only deque).

- `pop(container_object, object)`
	Pops an object from a container object. (All Data Structures except Deque).

- `pop_front(container_object, object)`
	Pops an object from a container object from the front. (Only deque).

- `pop_back(container_object, object)`
	Pops an object from a container object from the back. (Only deque).

- `insert(container_object, object)`
	Inserts an object into a container object. (Hashmap and Hashset only).

- `remove(container_object, object)`
	Removes an object from a container object. (Hashmap and Hashset only).

- `find(container_object, object)`
	Find an object in a container object. (Strings, Arrays, Hashmap and Hashset). Note: Arrays can find only primitive data types (Booleans, Integers, Strings)


# References
1. [Writing an Interpreter in Go](https://interpreterbook.com/) - [Thorsten Ball](https://www.linkedin.com/in/thorsten-ball-3142b652/)
2. [Crafting Interpreters](http://craftinginterpreters.com/) - [Bob Nystrom](https://www.linkedin.com/in/robert-nystrom-3124052/)

# Credits
Special thanks to [Lijo Zechariah](https://www.linkedin.com/in/lijo-zechariah-james-12091999/) for the logo

# Contributing
If you find any bug, please raise an issue or send a pull request if you have fixed it. Feel free to contribute in any way possible.

# Support
If you like Mod and would like to support it's development, consider donating:

[![Buy Me A Coffee](https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png)](https://www.buymeacoffee.com/pranavmodx)

<p align="center">
  <img width="100" src="https://github.com/pranavmodx/modlang/blob/logo/resources/logofire_80px_png_trans.png" alt="Fire mod logo">
</p>
