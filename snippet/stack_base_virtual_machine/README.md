# 栈式虚拟机的实现

```
int a = 1, b = 2, c = a + b;
print(a);
```

```
.type
1   "int"         ; int

.global
0   null          ; null
1   print         ; print

.static
2   1             ; `int 1

.code
TypeName    1     ; int
Push        2     ; 1
Declara     3     ; `a
Push        2     ; 2
Declara     4     ; `b
Push        3     ; `push a
Push        4     ; `push b
IntAdd            ; ` a + b
Declara     5     ; `c
ClearType   1     ;

BeforeCall  1     ; 
Param       3     ; a
Call        1     ; print a
```
