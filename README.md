# Compiler-Design-Lab
[Aldrin Jenson](https://github.com/aldrinjenson/compiler-design-lab)

[Binu VP](https://compilerlabktu.blogspot.com/2022/10/ktu-compiler-lab-csl411.html)

## How to run lex program?
`
save with .l extension
`
``` bash
lex filename.l
```

Compile the generated c code:
```
gcc lex.yy.c -o out -ll
```

To Run:
```
./out
```

## How to write regular expressions?
| Symbol | Meaning                  | Example                                    |       |                              |
| ------ | ------------------------ | ------------------------------------------ | ----- | ---------------------------- |
| `.`    | Any single character     | `a.b` matches "acb"                        |       |                              |
| `*`    | Zero or more repetitions | `ab*` matches "a", "ab", "abb", etc.       |       |                              |
| `+`    | One or more repetitions  | `ab+` matches "ab", "abb", etc.            |       |                              |
| `?`    | Zero or one occurrence   | `ab?` matches "a" or "ab"                  |       |                              |
| `[]`   | Character class          | `[aeiou]` matches any vowel                |       |                              |
| `[^]`  | Negated character class  | `[^aeiou]` matches any non-vowel           |       |                              |
| `()`   | Grouping                 | `(ab)+` matches "ab", "abab", etc.         |       |                              |
| `^`    | Start of line            | `^abc` matches lines that start with "abc" |       |                              |
| `$`    | End of line              | `abc$` matches lines ending with "abc"     |       |                              |

### Example usage 
```
%%
[0-9]+        { printf("Number: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*   { printf("Identifier: %s\n", yytext); }
\/\/.*        { printf("Comment: %s\n", yytext); }
.             { printf("Other char: %s\n", yytext); }
%%
```