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
The syntax of for-loops have changed:
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

### While loops
The **while** keyword has been replaced by **間**.


### Operators
The syntax of some operators have been changed to fit Japanese grammar:
```C
A は B です //A = B
A と B が等しい //A == B
A は B と異なる //A != B
A は B より大きい //A > B
A は B より小さい //A < B
A は B と等しいかより大きい //A >= B
A は B と等しいかより小さい //A <= B
```
Example:
```C
整数 count は 0 です。
while (count は 5 より小さい) ...
//turns into
int count = 0;
while (count < 5) ...
```

### If-else
**if** has been replace by **もし**.
**else** has been replace by **それ以外**.

### Preprocessor
The preprocessor terms **define** and **include** have been replaced by **定義** and **組込** respectively.