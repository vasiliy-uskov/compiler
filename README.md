# Синтаксис языка

```bnf
<программа> ::= <список функций><тело программы>|<тело программы>
<тело программы> ::= main {<список операторов>}

<список функций> ::= <функция><список функций>|<функция>
<функция> ::= 
    <тип> <идентификатор>(<список аргументов при объявлении>) {<список операторов функции>}
    |void <идентификатор>(<список аргументов при объявлении>) {<список операторов>}

<список аргументов при объявлении> ::= <аргумент при объявлении>, <список аргументов при объявлении>|<аргумент при объявлении>
<аргумент при объявлении> ::= <тип> <идентификатор>

<список операторов> ::= <оператор><список операторов>|<оператор>
<список операторов функции> ::= <оператор функции><список операторов функции>|<оператор функции>

<оператор функции> ::=
    <оператор объявления переменной>;
    |<оператор присваивания>;
    |<оператор if функции>
    |<оператор while функции>
    |<вызов функции>;
    |<оператор return>;
    |;
<оператор if функции> ::=
    if (<логическое выражение>) {<список операторов функции>}
    |if (<логическое выражение>) {<список операторов функции>} else {<список операторов функции>}
<оператор while функции> ::= while (<логическое выражение>) {<список операторов функции>}

<оператор> ::=
    <оператор объявления переменной>;
    |<оператор присваивания>;
    |<вызов функции>;
    |<оператор if>
    |<оператор while>
    |;

<вызов функции> := <идентификатор>(<список аргументов при вызове>)
<список аргументов при вызове> ::= <список аргументов при вызове>, <выражение>|<выражение>
<оператор объявления переменной> ::=
    <тип> <идентификатор> = <выражение>
    |<тип> <идентификатор>
<оператор присваивания> ::= <идентификатор> = <выражение>
<оператор if> ::=
    if (<логическое выражение>) {<список операторов>;}
    |if (<логическое выражение>) {<список операторов>;} else {<список операторов>;}
<оператор while> ::= while (<логическое выражение>) {<список операторов>;}
<оператор return> :: = return <выражение>

<выражение> ::= <арифметическое выражение>|<логическое выражение>
<арифметическое выражение> ::=
    <арифметическое выражение> + <арифметическое выражение>
    |<арифметическое выражение> - <арифметическое выражение>
    |-<арифметическое выражение>
    |<арифметическое выражение> * <арифметическое выражение>
    |<арифметическое выражение> / <арифметическое выражение>
    |(<арифметическое выражение>)
    |<вызов функции>
    |<идентификатор>
    |<значение int>
    |<значение float>
<логическое выражение> ::=
    <логическое выражение> || <логическое выражение>
    |<логическое выражение> && <логическое выражение>
    |<арифметическое выражение> < <арифметическое выражение>
    |<арифметическое выражение> <= <арифметическое выражение>
    |<арифметическое выражение> > <арифметическое выражение>
    |<арифметическое выражение> >= <арифметическое выражение>
    |<арифметическое выражение> == <арифметическое выражение>
    |(<логическое выражение>)
    |<вызов функции>
    |<идентификатор>
    |<значение bool>

<идентификатор> ::= <идентификатор><буква>|<идентификатор><цифра>|<маленькая буква>
<буква> ::= A..Z|<маленькая буква>
<цифра> ::= 0..9
<маленькая буква> ::= a..z
<тип> ::= int|float|bool
<значение> ::= <значение int>|<значение float>|<значение bool>
<значение int> := <последовательность цифр>
<значение float> := <последовательность цифр>.<последовательность цифр>
<последовательность цифр> ::= <последовательность цифр><цифра>|<цифра>
<значение bool> := false|true
```