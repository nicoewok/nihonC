# Changes
This is a list of all syntactical and keyword changes.

## End of line
```;``` has been replaced by the Japanese dot ```。```.

## Keywords

### Type names (WIP)
The most common C type names were converted into  their Katakana counterpart:
| C type  | Katakana        |
|---------|-----------------|
| char    | 文字            |
| int     | 整数            |
| long    | ロング          |
| float   | 浮動小数点       |
| double  | 倍精度浮動小数点 |

### Const
The **const** keyword was changed to **定数**

### Brackets
**{** and **}** have been changed to the Japanese **「** and **」**

### For loops
The syntax of for loops have changed:
```C
[type] [lower bound] から [upper bound] まで [variable name]
```
Example:
```C
イント 0 から 5 まで i
//turns into
for (i = 0; i <= 5; i++)
```

To be more precise you can always use the C for loop as well.


### Assignments
The syntax of assignments have been changed to fit Japanese grammar:
```C
... は ... です。
```
Example:
```C
イント count は 0 です。
//turns into
int count = 0;
```