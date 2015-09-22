The Project
===============


Compiler Command-line Options
===============



The Language
===============


Keywords
------------

| int      	| float    	| string     	| void    	|         	|           	|
|----------	|----------	|------------	|---------	|---------	|-----------	|
| if       	| else     	| elseif     	| return  	| while   	|           	|
| printInt 	| printStr 	| printFloat 	| readInt 	| readStr 	| readFloat 	|


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

Function Declaration and Function Call
------------

Function declaration and calling follows the same syntax and calling convention
of the C programming language and the Linux operating system.


Variable Declaration
------------




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



What is Not Supported
------------

1. It's not valid to use nested assignments. e.g., a = b = c;
