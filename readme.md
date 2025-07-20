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

#Termination

Statements can be terminated with newline and with ';'.

#String operations

```
var str = "Hello ", ing = "World";
var string = str + str;

```

Variable string will have value "Hello World".


```
var str = "Hello World";
var string = str + 3;

```

Variable string will have value "Hello WorldHel".

```
var str = "Hello  WorldHel";
var string = str - 3;

```

Variable string will have value "Hello World",

```
var str = "Hello World";
var string = str * 2;

```

Variable string will have value "Hello WorldHello World";

```
var str = "Hello World";
var string = str / 2
```

Variable string will have value "Hello ". 

```
var str = "Hello World";
var string = str .. 1;
```

Variable string will have value "Hello World1".
