# A (tiny) clock

A tiny program that permits to print the current in different formats.

# Formats supported

* binary, with -b option. Time 21:41:22 prints out as 1001:01001:10110
* hexadecimal, with -x option. Time 21:42:03 prints out as 9:2a:3
* octal, with -o option. Time 23 21:43:10 prints out as 11:53:12

# Usage

Compile with:

```
make CFLAGS="-Wall -Werror -pedantic -std=c99" clock
```

Run with: 

```
./clock
```

You can also use it with `watch`:


```
watch -n 1 ./clock -b
```

# Licence
This software is distributed under the MIT Licence.

