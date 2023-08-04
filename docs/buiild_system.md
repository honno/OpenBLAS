**This page is made by someone who is not the developer and should not be considered as an official documentation of the build system. For getting the full picture, it is best to read the Makefiles and understand them yourself.**

### Makefile dep graph

```
Makefile                                                        
|                                                               
|-----  Makefile.system 
|       |
|       |=====  Makefile.prebuild # This is triggered (not included) by Makefile.system 
|       |       |                 # and runs before any of the code is built.
|       |       |
|       |       -----  (Makefile.conf) [ either this or Makefile_kernel.conf is generated ] 
|       |       |                            { Makefile.system#L243 }
|       |       -----  (Makefile_kernel.conf) [ either this or Makefile.conf is generated ]
|       |
|       |-----  Makefile.rule
|       |
|       |-----  Makefile.$(ARCH)
|
|~~~~~ exports/
|
|~~~~~ test/
|
|~~~~~ utest/
|
|~~~~~ ctest/
|
|~~~~~ cpp_thread_test/
|
|~~~~~ kernel/
|
|~~~~~ ${SUBDIRS}
|
|~~~~~ ${BLASDIRS}
|
|~~~~~ ${NETLIB_LAPACK_DIR}{,/timing,/testing/{EIG,LIN}}
|
|~~~~~ relapack/
```

### Important Variables

Most of the tunable variables are found in [Makefile.rule](https://github.com/xianyi/OpenBLAS/blob/develop/Makefile.rule), along with their detailed descriptions.<br/>
Most of the variables are detected automatically in [Makefile.prebuild](https://github.com/xianyi/OpenBLAS/blob/develop/Makefile.prebuild), if they are not set in the environment.

#### CPU related
```
ARCH         - Target architecture (eg. x86_64)
TARGET       - Target CPU architecture, in case of DYNAMIC_ARCH=1 means library will not be usable on less capable CPUs
TARGET_CORE  - TARGET_CORE will override TARGET
DYNAMIC_ARCH - For building library for multiple ARCHs (does not lose any optimizations, but increases library size)
```

#### Toolchain related
```
CC                 - TARGET C compiler used for compilation (can be cross-toolchains)
FC                 - TARGET Fortran compiler used for compilation (can be cross-toolchains, set NOFORTRAN=1 if used cross-toolchain has no fortran compiler)
AR, AS, LD, RANLIB - TARGET toolchain helpers used for compilation (can be cross-toolchains)

HOSTCC             - compiler of build machine, needed to create proper config files for target architecture
HOST_CFLAGS        - flags for build machine compiler
```

#### Library related
```
BINARY          - 32/64 bit library

BUILD_SHARED    - Create shared library
BUILD_STATIC    - Create static library

QUAD_PRECISION  - Support for IEEE quad precision [ Experimental and largely unimplemented, don't use in production ]
EXPRECISION     - Obsolete option to use float80 of SSE on BSD-like systems
INTERFACE64     - Build with 64bit integer representations [ incompatible with standard API ]

BUILD_SINGLE    - ??
BUILD_DOUBLE    - ??
BUILD_COMPLEX   - ??
BUILD_COMPLEX16 - ??
USE_THREAD      - Use a threading backend (default to pthread)
USE_OPENMP      - Use OpenMP threading backend
```


