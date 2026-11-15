# VPD Lab 2: язык C изнутри

Лабораторная работа № 2: типы и память, операторы, битовые операции, циклы,
измерение времени, `extern`/`static`, препроцессор, стандарты C, ручная сборка, Make и CMake.

Окружение: Omarchy (Arch Linux, x86_64), GCC 13.3.0, GNU Make 4.3, CMake 3.28.3.

## Структура

```
VPD_lab02/
├── CMakeLists.txt      # сборка через CMake (п. 18)
├── Makefile            # сборка через Make (п. 17)
├── main.c              # стадии, progress bar, таймеры (п. 10, 12, 13)
├── device.c / device.h # extern / static (п. 13)
├── timer.c / timer.h   # высокоточный таймер (п. 12)
├── lab1/Makefile       # автоматизация ЛР1 (п. 3)
├── task4/   types.c
├── task5/   arrays.c
├── task6/   chars.c
├── task7/   printf_demo.c, scanf_demo.c
├── task8/   operators.c
├── task9/   register.c
├── task11/  foreach.cpp
├── task13/  auto_register.c
├── task14/  macros.c
└── task15/  standard_probe.c, std_*.c
```

Папка `taskN` соответствует разделу N задания. Разделы 10, 12 и 13.1 выполняются
проектом из корня (`main.c`, `device.*`, `timer.*`), разделы 16-18 тоже в корне.

## Ручная сборка (раздел 16)

```
gcc -E main.c -o main.i
gcc -E -P main.c -o main_no_lines.i
gcc -S main.c -o main.s
gcc -c main.c -o main.o
gcc -c device.c -o device.o
gcc -c timer.c -o timer.o
gcc main.o device.o timer.o -o vpd_lab2
./vpd_lab2
```

Цепочка: `.c` -> (препроцессор) `.i` -> (компилятор) `.s` -> (ассемблер) `.o` -> (компоновщик) исполняемый файл.

## Make (раздел 17)

```
make            # сборка (-O0)
make run        # сборка и запуск
make release    # -O2
make debug      # -O0 -g
make compare    # собрать и запустить -O0 и -O2
make stages     # .i, .s, .o и линковка вручную по этапам
make clean
```

## CMake (раздел 18)

```
cmake -S . -B build
cmake --build build
./build/vpd_lab2
```

Сборка вне исходников: всё генерируется в `build/`, руками файлы там не правятся.

## Программа-«прогрузка»

```
./vpd_lab2 [rounds_per_item] [items_per_stage]
```

По умолчанию 500 раундов на элемент (`DEFAULT_ROUNDS` в `main.c`) и 200000 элементов на стадию
(3 стадии: `for`, `while`, `do...while`).
Подобранное значение для этой машины: `rounds = 500`, чтобы полный запуск на `-O0` занимал 8-15 с.
Время работы зависит от машины, на другой её нужно подбирать заново.

## Эксперименты по разделам

```
# 4: типы и размеры
gcc -Wall -Wextra -Wpedantic task4/types.c -o task4/types && ./task4/types

# 5: неинициализированные массивы и версия с = {0}
gcc -Wall -Wextra -Wpedantic -O0 -DUNINIT task5/arrays.c -o task5/arrays_O0_uninit
gcc -Wall -Wextra -Wpedantic -O2 -DUNINIT task5/arrays.c -o task5/arrays_O2_uninit
gcc -Wall -Wextra -Wpedantic -O0 task5/arrays.c -o task5/arrays_O0

# 6: одинаковые символы в разных типах
gcc -Wall -Wextra -Wpedantic task6/chars.c -o task6/chars && ./task6/chars

# 7: printf / scanf / escape-последовательности
gcc -Wall -Wextra -Wpedantic task7/printf_demo.c -o task7/printf_demo && ./task7/printf_demo
gcc -Wall -Wextra -Wpedantic task7/scanf_demo.c -o task7/scanf_demo && ./task7/scanf_demo

# 8: токены, приоритеты, UB
gcc -Wall -Wextra -Wpedantic task8/operators.c -o task8/operators && ./task8/operators
gcc -Wall -Wextra -Wpedantic -DSHOW_UB task8/operators.c -o task8/operators_ub

# 9: битовый регистр
gcc -Wall -Wextra -Wpedantic task9/register.c -o task9/register && ./task9/register

# 11: foreach (C++)
g++ -Wall -Wextra task11/foreach.cpp -o task11/foreach && ./task11/foreach

# 13.2: auto и register, C17 против C23
gcc -std=c17 -Wall -Wextra -Wpedantic task13/auto_register.c -o task13/auto17
gcc -std=c2x -Wall -Wextra -Wpedantic task13/auto_register.c -o task13/auto23

# 14: const / #define, макросы, условная компиляция
gcc -Wall -Wextra -Wpedantic task14/macros.c -o task14/macros
gcc -Wall -Wextra -Wpedantic -DDEBUG task14/macros.c -o task14/macros_debug

# 15: стандарты C
gcc -Wall -Wextra -Wpedantic task15/standard_probe.c -o task15/probe_default
gcc -std=c90 -Wall -Wextra -Wpedantic task15/standard_probe.c -o task15/c90
gcc -std=c99 -Wall -Wextra -Wpedantic task15/standard_probe.c -o task15/c99
gcc -std=c11 -Wall -Wextra -Wpedantic task15/standard_probe.c -o task15/c11
gcc -std=c17 -Wall -Wextra -Wpedantic task15/standard_probe.c -o task15/c17
gcc -std=c2x -Wall -Wextra -Wpedantic task15/standard_probe.c -o task15/c23
```

На GCC 13.3 флаг `-std=c23` не принимается, поэтому для C23 используется `-std=c2x`
(в GCC 14+ работает `-std=c23`). При `-std=c2x` значение `__STDC_VERSION__` на GCC 13 равно `202000`.

Мини-эксперименты по стандартам (`task15/`):

| возможность | файл | стандарты |
|---|---|---|
| `//` и `for (int i...)` | `std_c90_vs_c99.c` | c90 / c99 |
| `_Static_assert` | `std_c99_vs_c11.c` | c99 / c11 |
| `0b100110000` | `std_binary_literal.c` | c17 / c23 |
| `auto x = 304` | `std_auto.c` | c17 / c23 |

## Git

В репозиторий входят только исходники и файлы сборки. Бинарники, объектные файлы,
`.i`/`.s` и `build/` исключены через `.gitignore`.
