The Project
============

Semantikin is the codename of a compiler for a subset of the C programming
language. My goal with this project is to develop a fully working compiler, which
is able to perform simple optimizations, like common subexpression elimination,
constant folding, etc. and produce assembly code for at least x86-64 and ARM.

In the next sections I describe in some details the structure of the compiler
and the programming language supported. If you have any feedback feel free to
get in touch!


The Compiler
------------

### Command-line options


### Front-end

### The Lexer

### The Parser

### The Semantic Analyzer

### The Intermediate Representation

### Optimization Passes

### Instruction Selection

### Register Allocation




The Language
------------

The language supported by Semantikin is a subset of the C programming language.
The syntax and semantic tries to follow as close a possible that of the C language.
In the future I intend to expand the language with more constructs, like structures,
but this will be added only after I get a fully retargetable optimizing compiler.


Keywords
------------

| int      	| float    	| string     	| void    	|         	|           	|
|----------	|----------	|------------	|---------	|---------	|-----------	|
| if       	| else     	| elseif     	| return  	| while   	|           	|
| printInt 	| printStr 	| printFloat 	| readInt 	| readStr 	| readFloat 	|


Comments
------------

The only comment style supported is the C-like double backslash, which makes
all following text in the same line to be ignored by the parser.


Identifiers
------------
Identifiers can start with a digit or an alphanumeric letter and can consist of
an arbitrarily long sequence consisting of such chars. The regular expression
used to recognize identifiers is this: [0-9a-zA-Z]*


Types
------------

These are the available data types: int, float and string. Arrays and matrix are
supported. Currently there is no support for user-defined data types.

The system does not have direct support for boolean expressions, however the
language and compiler implicitly use the literal 1 (or a non-empty string) as
true, and 0 (or a empty string) as false.

Function Declaration, Function Call and Variable Declaration
------------

Function and variable declaration follows the syntax of the C language. Currently
the only calling convention supported is that of the C/Linux/x86.

Scalar variables can have an initialization value, arrays cannot.

Examples:

```c++

// First function
void fun1(int n) {
	int a = n, b = n + 1;
	int c = fun1(a + b);
}

// Second function
int fun2(int a) {
	return fun1(a);
}
```



Operators
------------

These are all available operators. Operators closer to the top have lower
precedence than those closer to the bottom.

| %= |    |   | right |
|----|----|---|-------|
| *= | /= |   | right |
| += | -= |   | right |
| =  |    |   | right |
| &#124;&#124; |    |   | left  |
| && |    |   | left  |
| &#124;  |    |   | left  |
| ^  |    |   | left  |
| &  |    |   | left  |
| == | != |   | left  |
| >  | >= |   | left  |
| <  | <= |   | left  |
| +  | -  |   | left  |
| *  | /  | % | left  |
| &  |    |   | right |
| ~  |    |   | right |
| !  |    |   | right |

All operands are well defined for numerical types, however bit manipulation
operands and the modular arithmetic operand are only valid for integer operands.
That means that you can, for instance, use integers in boolean expressions.
For instance:

```cpp
void noeffect() {
	int a = 1 || 0;		// Evaluate to true
	int b = 1 && 0;		// Evaluate to false
}
```

The following operators support string operands under certain circumstances:

| Operator | Semantic                                  |
|----------|-------------------------------------------|
| +        | Concatenate strings. The programmer must ensure that the target has sufficient space to hold the resulting string. |
| ==       | Compare two strings. 		 |
| !=       | Check whether two strings are different. 		 |



What is Not Supported
------------

1. It is not valid to use nested assignments. e.g., a = b = c;
2. It is not valid to use bit operators with float or string operands;
3. Structures are not supported.
4. Global variables are not supported.
5. Pointers are not supported.
6. There is no preprocessor.

TODO
===============

1. Rename the fields chd1, chd2, chd3 of the IR::Instruction class for something more meaningful, e.g., target, left and right.


References
===============


Change Log
===============
