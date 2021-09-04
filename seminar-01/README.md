# Семинар №1  
## Инструменты разработки

---

**Agenda**
  * Компиляторы GCC и CLang
  * От текстового файла до исполняемой программы
  * Объектные файлы, библиотеки и исполняемые файлы
  * Директивы препроцессора и макросы
  * Автоматизация сборки
  * Stylecheckers
  * Summary
  * Extra

---

Если вы напрочь забыли как работать с Linux, то рекомендую предварительно освежить память 
[здесь](https://github.com/MVCionOld/mipt-diht-tp-2021-seminars/tree/master/seminar-01) 
и дополнить их [здесь](https://github.com/victor-yacovlev/mipt-diht-caos/blob/master/practice/linux_basics/intro.md).

---

### Компиляторы GCC и CLang

В стандартную поставку современных UNIX-систем входит один из компиляторов: либо `gcc`, либо `clang`. В случае с Linux, по умолчанию обычно используется `gcc`, а в BSD-системах - `clang`. Далее будет описана работа с компилятором `gcc`, имея ввиду, что работа с `clang` ничем принципиально не отличается: у обоих компиляторов много общего, в том числе опции командной строки.

Кроме того, существует команда `cc`, которая является символической ссылкой на используемый по умолчанию компилятор языка Си (`gcc` или `clang`), и команда `c++`, - символическая ссылка на используемый по умолчанию компилятор для C++.

Рассмотрим простейшую программу на языке C++:
```c++
// файл hello.cpp
#include <iostream>

int main() {
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
```

Создать исполняемый файл из этой программы можно с помощью команды:
```bash
> c++ -o program.jpg hello.cpp
```

Опция компилятора `-o ИМЯ_ФАЙЛА` указывает имя выходного файла, который нужно создать. По умолчанию используется имя `a.out`. Обратите внимание, что файл `program.jpg` является обычным выполняемым файлом!

**Вопрос к аудитории**: как удостовериться, что файл действительно исполняемый, не предпринимая попытки его запустить?

[Выбор языка](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/linux_basics#%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D1%8B-%D0%BD%D0%B0-%D1%81%D0%B8-vs-%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D1%8B-%D0%BD%D0%B0-c)  
[Указание стандартов](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/linux_basics#%D1%83%D0%BA%D0%B0%D0%B7%D0%B0%D0%BD%D0%B8%D0%B5-%D1%81%D1%82%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D1%82%D0%BE%D0%B2)


---

### От текстового файла до исполняемой программы

Вернемся к нашей команде:
```bash
> c++ -o program.jpg hello.cpp
```

Этапы:
  1. Препроцессинг
  2. Компиляция
  3. Ассемблирование
  4. Компоновка (линковка)

**Препроцессинг**  
На данной стадии происходит происходит работа с препроцессорными директивами. Например, препроцессор добавляет хэдеры в код (#include), убирает комментирования, заменяет макросы (#define) их значениями, выбирает нужные куски кода в соответствии с условиями #if, #ifdef и #ifndef.
```bash
> с++ -E -o hello.i hello.cpp
```

Результат выглядит страшно и непонятно, попробуем что-то попроще.


```c
// файл minmax.h

//
// Created by Mikhail Tsion on 04.09.2021.
//

#ifndef MIPT_ATP_CAOS_SEMINARS_MINMAX_H
#define MIPT_ATP_CAOS_SEMINARS_MINMAX_H

extern int max(int lhs, int rhs);

extern int min(int lhs, int rhs);

#endif //MIPT_ATP_CAOS_SEMINARS_MINMAX_H

```
```c
// файл minmax.c

//
// Created by Mikhail Tsion on 04.09.2021.
//

#include "minmax.h"

#define __max(lhs, rhs) ((a) > (b) ? (a) : (b))
#define __min(lhs, rhs) ((a) < (b) ? (a) : (b))

extern int max(int lhs, int rhs) {
  return __max(lhs, rhs);
}

extern int min(int lhs, int rhs) {
  return __min(lhs, rhs);
}
```

```bash
> gсc -E -o minmax.i minmax.c
```

**Компиляция**  
На данной стадии происходит преобразование полученном на прошлом шаге кода без директив в ассемблерный код.
_Ассемблерный код_ — это доступное для понимания человеком представление машинного кода.
```bash
> gcc -S -o minmax.s minmax.c
```

Попались!  

```bash
> gcc -E -o minmax.i minmax_fixed.c
```
```bash
> gcc -S -o minmax.s minmax.i
```

Несколько полезных флагов, которые помогают получить более читаемый ассемблерный код:
```bash
-fno-asynchronous-unwind-tables помогает получить более короткий выхлоп
-fverbose-asm позволит получить более длинный, но более удобный для чтения
```

**Ассемблирование**  
На данной стадии необходимо перевести ассемблерный код в машинный с помощью ассемблера.
Ассемблер преобразовывает ассемблерный код в машинный код, сохраняя его в объектном файле.
_Объектный файл_ — это созданный ассемблером промежуточный файл, хранящий кусок машинного кода. Этот кусок машинного кода, который еще не был связан вместе с другими кусками машинного кода в конечную выполняемую программу, называется объектным кодом.
```bash
> gcc -c -o minmax.o minmax_fixed.c
```
или
```bash
> as minmax.s -o minmax.o
```

**Компоновка**  
На данной стадии происходит связывание всех объектных файлов и статических библиотек в единый исполняемый файл, который мы и сможем запустить в дальнейшем.
```bash
> c++ main.o file1.o ... fileN.o lib1.o ... libN.o -o main
```

Лучше процесс компоновки отдать на откуп компилятору, но если очень хочется потрогать все руками, то поизучайте 
утилиту ```ld```.

**\*Загрузка**  
Формально к сборке проекта никакого отношения не имеет, но надо понимать, что для непосредственно
запуска нашего приложения, программу надо загрузить в память. 

---

### Объектные файлы, библиотеки и исполняемые файлы

**Вопрос к аудитории**: знакомы ли термины "архитектура фон Неймана" и "гарвардская архитектура"?  
**Вопрос к аудитории**: в чем разница между статической и динамической библиотеками?


Собираем объектный файл из исходников:
```bash
> gcc -c minmax_fixed.c -o minmax.o
```

Делаем из объектного файла статическую библиотеку:
```bash
> ar rcs minmax.a minmax.o
```

Делаем из объектного файла динамическую библиотеку:
```bash
> gcc -shared -fPIC minmax.o -o minmax.so
```

Опция ```-fPIC``` предназначена для генерации позиционно-независимого кода - полученный объектный код может быть 
размещен в любой области памяти, поскольку все ссылки на ячейки памяти в нем относительные.

На этом веселье не заканчивается! Давайте проделаем пару маневров (под Linux):
```bash
> hexdump -C minmax.o | head -n 2
```

```bash
> hexdump -C minmax.so | head -n 2
```

[ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

```bash
> hexdump -C minmax.a | head -n 2
```

[Таблицы символов](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/linux_basics#%D0%BF%D1%80%D0%BE%D1%81%D0%BC%D0%BE%D1%82%D1%80-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D1%8B-%D1%81%D0%B8%D0%BC%D0%B2%D0%BE%D0%BB%D0%BE%D0%B2)

```c
// файл py_utils.py

int sum_ints(int a, int b) {
  return a + b;
}

float sum_floats(float a, float b) {
  return a + b;
}

static int mult_ints(int a, int b) {
  return a * b;
}
```

```bash
> gcc -c py_utils.c -o py_utils.o
> objdump -t py_utils.o
```

Использование статических библиотек в ваших С-программах тривиально, достаточно указать желаемую библиотеку для 
компоновщика, ведь статическая библиотека, по сути, архив объектных файлов. Подробное использование динамических
библиотек будет рассмотрено несколько позднее. Сейчас лучше рассмотреть как
динамически подгрузить для использования интерпретатором Python:

```bash
> gcc -shared -fPIC py_utils.o -o lib_py_utils.so
> python3
Python 3.8.2 (default, Jun  8 2021, 11:59:35) 
[Clang 12.0.5 (clang-1205.0.22.11)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> from ctypes import cdll
>>> lib = cdll.LoadLibrary("./lib_py_utils.so")
>>> lib.sum_ints(23, 42)
65
>>> lib.sum_floats(23, 42)
23  # WAT???
```

**Вопрос к аудитории**: почему ```sum_floats``` не возвращает ожидаемый результат?

```bash
>>> import ctypes
>>> lib.sum_floats.restype = ctypes.c_float
>>> lib.sum_floats(23, 42)
1.2611686178923354e-44
>>> lib.sum_floats.argtypes = [ctypes.c_float, ctypes.c_float]
>>> lib.sum_floats(23, 42)
65.0
```

**Вопрос к аудитории**: что произойдет при вызове ```mult_ints```?
```bash
>>> import ctypes
>>> lib.mult_ints(2, 2)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.8/lib/python3.8/ctypes/__init__.py", line 378, in __getattr__
    func = self.__getitem__(name)
  File "/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.8/lib/python3.8/ctypes/__init__.py", line 383, in __getitem__
    func = self._FuncPtr((name_or_ordinal, self))
AttributeError: dlsym(0x143e04150, mult_ints): symbol not found
```

---

### Директивы препроцессора и макросы

Через опции командной строки можно определять константы, которые обрабатывается на стадии препроцессинга текстового файла. Для этого используется опция `-DИМЯ=ЗНАЧЕНИЕ`. В процессе компиляции эти имена эквивалентны конструкциям `#define`.

```
#ifdef PREDEFINED_CONSTANT
static const char * StringConstant = PREDEFINED_CONSTANT;
#else
static const char * StringConstant = "Default value";
#endif
```

Данный код может быть скомпилирован с опцией, которая доопределит `PREDEFINED_CONSTANT`:

```
> cc -DPREDEFINED_CONSTANT='"Hello, World!"'
```

Обратите внимание, что если необходимо использовать символы пробелов или кавычек, то их необходимо либо экранировать, либо заключить аргумент в одинарные кавычки.

Директива препроцессора - это строка кода вида:  ```#ключевое_слово <arg1> ... <argN>```

Список ключевых слов:
 * ```define``` — создание константы или макроса;
 * ```undef``` — удаление константы или макроса;
 * ```include``` — вставка содержимого указанного файла;
 * ```if``` — проверка истинности выражения;
 * ```ifdef``` — проверка существования константы или макроса;
 * ```ifndef``` — проверка несуществования константы или макроса;
 * ```else``` — ветка условной компиляции при ложности выражения ```if```;
 * ```elif``` — проверка истинности другого выражения; краткая форма записи для комбинации ```else``` и ```if```;
 * ```endif``` — конец ветки условной компиляции;
 * ```error``` — вывод сообщения и остановка компиляции;
 * ```warning``` — вывод сообщения без остановки компиляции;
 * ```pragma``` — указание действия, зависящего от реализации, для препроцессора или компилятора, например, c 
   помощью ее можно переопределять поведение, заданное параметром компилятора (в тот момент, когда директива 
   достигается);
 * если ключевое слово не указано, директива игнорируется;
 * если указано несуществующее ключевое слово, выводится сообщение об ошибке и компиляция прерывается (в некоторых 
    компиляторах, таких как g++, компиляция продолжается, просто показывая предупреждение).

Уточним про макросы:
 * Макросы это именно макросы, они ничего не знают про синтаксис С/С++
 * Переменные в макросах это просто куски исходного текста
 * При передаче аргументов в макрос стоит помнить, что значение имеют только запятые ```,``` и скобки ```()```

```c++
// файл macroc_dummy.c

#define my_macro(type, var, value) type var = value;

my_macro((std::vector<pair<int, int>), v, ({{1, 1}, {2, 2}, {3, 3}}))
```
```bash
> gcc -E macros_dummy.c -o macros_dummy.i
> cat macros_dummy.i
# 1 "macros_dummy.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 384 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "macros_dummy.c" 2


(std::vector<pair<int, int>) v = ({{1, 1}, {2, 2}, {3, 3}});
```

```c++
// файл macroc_clever.c

#define my_macro(type, var, value) type var = value;
#define identity(...) __VA_ARGS__

my_macro(identity(std::vector<pair<int, int>), v, identity({{1, 1}, {2, 2}, {3, 3}}))
```
```bash
> gcc -E macroc_clever.c -o macroc_clever.i
> cat macroc_clever.i
# 1 "macros_clever.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 384 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "macros_clever.c" 2



std::vector<pair<int, int> v = {{1, 1}, {2, 2}, {3, 3}};
```

Рассмотрим еще два макроса, которые делают одно и то же:
```c
#define mult_wide(a, b) ((a) * (b))
#define mult_narrow(a, b) (a * b)
```

Примеры полезных макросов. **Используйте их при решении домашек, но не отправляйте код с макросами в ejudge!**
```c

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* #VAR_NAME разворачивается в строковый литерал "VAR_NAME" */
#define print_int(i) printf(#i " = %d\n", (i));

/* Полезный макрос для вывода в поток ошибок */
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define logprintf_impl(fmt, line, ...) eprintf(__FILE__ ":" #line " " fmt, __VA_ARGS__)
#define logprintf_impl_2(line, fmt, ...) logprintf_impl(fmt "%s", line, __VA_ARGS__)
#define logprintf(...) logprintf_impl_2(__LINE__, __VA_ARGS__, "")

#define SWAP(a, b) { __typeof__(a) c = (a); (a) = (b); (b) = (c); }
#define SWAP2(a, b) { char c[sizeof(a)]; memcpy(c, &a, sizeof(a)); \
                      memcpy(&a, &b, sizeof(a)); memcpy(&b, c, sizeof(a)); if (0) { a = b; b = a; } }

/* Способ сделать макрос с переменным числом аргументов
 * И это единственный способ "перегрузить функцию в С" */
#define sum_2(a, b, c) ((a) + (b))
#define sum_3(a, b, c) ((a) + (b) + (c))

#define sum_impl(a, b, c, sum_func, ...) sum_func(a, b, c)

#define sum(...) sum_impl(__VA_ARGS__, sum_3, sum_2)


int main() {
    /* assert(1 > 2); -- это тоже макрос */
    print_int(9 * 9 + 1);

    eprintf("It is in stderr: %d\n", 431);

    int x = 1, y = 2;
    eprintf("(x, y) = (%d, %d)\n", x, y);
    SWAP(x, y);
    eprintf("(x, y) = (%d, %d)\n", x, y);
    SWAP2(x, y);
    eprintf("(x, y) = (%d, %d)\n", x, y);

    print_int(sum(1, 1));
    print_int(sum(1, 1, 1));
    
    eprintf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    
    logprintf("Before exit %s\n", "");
    return 0;
}
```

Объявление числовых констант макросами.

---

### Автоматизация сборки

[Воспоминания про Make из курса ТП](https://github.com/MVCionOld/mipt-diht-tp-2021-seminars/tree/master/seminar-04#make)

Если требуется во время сборки определить какие-либо параметры окружения, то можно вызывать внешние команды, и использовать их вывод в качестве переменных. Для этого используется конструкция `ПЕРЕМЕННАЯ=$(shell КОМАНДА)`. Кроме того, все реализации `make` поддерживают примитивные конструкции для проверки условий `ifeq (A, B) ... endif`.

```bash
# пример сборки с учетом имени операционной системы

# команда uname возвращает имя операционной системы
UNAME=$(shell uname -s)
ifeq ($(UNAME), Darwin)
        OS=APPLE
endif
ifeq ($(UNAME), Linux)
        OS=LINUX
endif

platform_specific.o: platform_specific.c
        $(CC) -c $(CFLAGS) -D__$(OS)__ platform_specific.c
```

```c
// platform_specific.c
#ifdef __APPLE__
// эта часть будет компилироваться только под Mac
#endif
#ifdef __LINUX__
// эта часть будет компилироваться только под Linux
#endif
```


---

### Stylecheckers

Перед публикацией кода, не важно, куда - в репозиторий или при отправке решений, необходимо следовать единому стилю кода, про который есть договоренности с теми, кто этот код будет изучать. Для автоматизации этого процесса используется утилита `clang-format`.

Для языков семейства Си нет единого стандарта, как должен быть отформатирован код. Есть несколько сложившихся в Open Source стандартов, которые задаются опцией `--style=ИМЯ`: `LLVM`, `GNU`, `Google`, `Chromium`, `Microsoft`, `Mozilla`, `WebKit`. Если указать опцию `--style=file`, то стандарт оформления будет использован из локального файла `.clang-format` (имя начинается с символа точки). Файл `.clang-format` может располагаться не только в одном  каталоге с исходным файлом, но и в любом каталоге выше по иерархии, - в этом случае будет использован первый найденный файл.

По умолчанию утилита `clang-format` выводит преобразованный исходный файл на стандартный поток вывода, опция `-i` (inplace) указывает, что необходимо перезаписать исходный файл.

Некоторые текстовые редакторы и среды разработки позволяют интегрировать `clang-format` для переопределения стиля 
форматирования текста.


Среда разработки CLion начиная с версии 2021.2 определяет наличие файла `.clang-format`, и позволяет подключить переопределение стиля кода.


---

### Summary

---

### Extra