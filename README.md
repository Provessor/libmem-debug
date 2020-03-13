# libmem-debug
A C89 compliant library for debugging dynamic memory in C

# Building
For a dynamically linked library run
```
$ make dynamic
```
Or for a statically linked library
```
$ make static
```

# Installation
Currently the library must be built dynamically linked for system
installation. To install that: (to `/usr/local` by default)
```
# make install
```

# Usage
## Includes
To enable the dynamic memory tracking
``` c
#include <libmem-debug.h>
```
For a system installation and
``` c
#include "/path/to/libmem-debug.h"
```
For local.

The provided functions `print_mem_debug()`{:.c}, `fprint_mem_debug(FILE
*stream)`{:.c} and `log_mem_debug(const char *fn, int8_t append)`{:.c} will
print the current state of memory to `stderr`{:.c}, `stream`{:.c} or file with
name given by `fn`{:.c} respectively.

More features are planned in the near future.

## Compilation
To include the library when compiling append `-lmem-debug` to the
compilation command. If it hasn't been installed to the system
`-L/path/to/build` must be appended before `-lmem-debug`. This should
look something like: 
``` c
c89 -o main main.c -L. -lmem-debug
```
