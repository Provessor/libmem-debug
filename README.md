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

The provided functions `print_mem_debug()`, `fprint_mem_debug(FILE
*stream)` and `log_mem_debug(const char *fn, int8_t append)` will
print the current state of memory to `stderr`, `stream` or file with
name given by `fn` respectively.

## Enable/Disable
The macro `LIBMEM_DEBUG_DISABLE` is used to disable all functionality
of libmem-debug without the need to remove it from all includes. It
removes any logging for memory management and makes the provided
functions do nothing (this includes `print_mem_debug()`,
`fprint_mem_debug(FILE *)` and `log_mem_debug(const char *, int8_t)`).

There also exists a `LIBMEM_DEBUG_ENABLE` macro which can be used to
force-enable functionality even when `LIBMEM_DEBUG_DISABLE` is
defined.

All functionality is enabled by default.

### Tips
The easiest method of controlling functionality is to just define
`LIBMEM_DEBUG_DISABLE` in production builds (append
`-DLIBMEM_DEBUG_DISABLE` to your compiler flags).

## Compilation
To include the library when compiling append `-lmem-debug` to the
compilation command. If it hasn't been installed to the system
`-L/path/to/build` must be appended before `-lmem-debug`. This should
look something like: 
``` c
c89 -o main main.c -L. -lmem-debug
```
An example of this can be found in the Makefile under tests/
