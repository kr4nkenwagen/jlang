#Variables

## Declaration

Variables can be declared as single line. You can declare variables as ```const``` or ```var```.

```
var x = 1, y = 2, x = 3;
```


```
const x = 1, y = 2, z = 3;
```
## String

```
const str = "Hello World";
```

## number
```
const int = 1;
const float = 1.0;
```

## Booleans
```
const boolean = true;
```

## Array
```
var array = ["Hello World", 8];
array[2] = "I forgot to add this.";
```
arrays can be declared as empty array.

```
const array = [];
```
# Types

jLang supports 6 data types.

| Type | Description |
|------|-------------|
| Int  | Signed integer number. |
| Float| 
| Bool | Takes ```true``` and ```false``` constants. |
|String| Takes text enclosed in ```"``` or ```'```. |
| Array| Takes values in ```[0..]```.|
| Null | Takes ```Null``` constant. |

# Termination

Statements can be terminated with newline and with ';'.

# String operations

```
const str = "Hello ", ing = "World";
const string = str + ing;

```

Variable string will have value "Hello World".


```
const str = "Hello World";
const string = str + 3;

```

Variable string will have value "Hello WorldHel".

```
const str = "Hello  WorldHel";
const string = str - 3;

```

Variable string will have value "Hello World",

```
const str = "Hello World";
const string = str * 2;

```

Variable string will have value "Hello WorldHello World";

```
const str = "Hello World";
const string = str / 2
```

Variable string will have value "Hello ". 

```
const str = "Hello World";
const string = str .. 1;
```

Variable string will have value "Hello World1".

```
const str = "Hello World";
const string = str % 2
```
Variable string will have value "d".

```
const str = "Hello World";
const string = str:5;
```
Variable string will have value "Hello". This returns start of string up to the supplied index.

```
const str = "Hello World";
const string = str:^5;
```

Variable string will have value "World". This returns from supplied index to end of string.
```
const str = "Hello World";
const string = str:"d";
```
Variable string will have value "Worl". This returns string from start to first instance of the supplied string.

```
const str = "Hello World";
const string = str:^"W"
```
Variable string will have value "orld". This will return from the last instance of supplied string to the end of string.
