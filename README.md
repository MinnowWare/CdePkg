# CdePkg 
C Development Environment Package for EDK2

## Introduction
**CdePkg**, C Development Environment Package, introduces the use of *Hosted Environment*,
as specified by [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf),
for both UEFI POST and SHELL drivers.
This is a reference implementation only, using the Microsoft C compiler, linker, library 
manager and IDE that comes with Visual Studio 2019 for x86 platforms.

A *Hosted Environment* for command line applications is standard, its introduction for drivers is a novum. This also applies for the UEFI environment. But the wealth of computing power of current UEFI machines offers the chance to translate [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf)
compatible sourcecode to run as a UEFI POST driver.

With the growing complexity of firmware due to the requirements for both security and trust and the
need for speed in development, use of platform independent sourcecode allows:
* reuse of validated C sourcecode from the open source community<br>
  (as already done with _openSSL_ in the EDK2\CryptoPkg)
* crossdevelopment of complex firmware code on non-UEFI platforms with superb build and debug capabilities<br>
* use build in static code analysis capabilities of modern C compilers for standard C library functions<br>
  (NOTE: At build time C compiler can verify, validate and warn about parameter passing to `printf()` but not for EDK2's `Print()`)
* allow appraisal of the source code quality by human professionals<br>
  (they will be bemused to see e.g. `AsciiStrnCatS()` because `strncat()` is specified in C)

Since the UEFI "OS" interface (DXE/SHELL/SMM and PEI) can be accessed directly by the compiler
translated sourcecode and UEFI provides an independent set of functions, macros and type definitions,
[ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) and UEFI "OS" specific sourcecode can  coexist seamlessly. This allows a functional [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) prototype to adjust successively to real world driver requirements in the UEFI environment. A UEFI SHELL application might be an intermediate step for this process if the target is a DXE or SMM driver.

In case, external UEFI libraries (created by the EDK build process) are not used in a particular UEFI
driver (and therefore the *Library Constructor* process is not needed), a UEFI driver can be translated
in the VS2017 build environment, which is much faster than the EDK build process (the driver's binary is not
placed in the BIOS image, of course).

*But the creation of syntactically correct sourcecode, using the
luxurious auto completion and mouse hover actions also for UEFI specific type definitions, enhances the
development process notably.*

## Intention
**CdePkg** is a feasibility study on how to provide a complete *Hosted Environment* 
(according to [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) Specification chapter 5.1.2) including all instrisic functions, 
which the compiler requires to be a **full featured C-compiler**, in particular the full
set of C-language operators (specifically `/ % << >>` for 64 bit integers) for the 32 bit code generator, needed in PEI.

Furthermore the questions has to be answered, if UEFI based products can be improved regarding
* software quality (in terms of number of required updates during a life cycle)
* development time
* feature set (complexity and quantity)
* storage space needed in a flash part (the smaller the better)

if a standardized programming interface as [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) is available, in conjunction with a storage space optimization
strategy, as described below, that splits *wrapper libraries* from *worker drivers*.

In the UEFI programming environment not even the smallest piece of code can be cross developed on a
different platform, since function names differ in upper/lower case letters, format specifier for
`Print()`-functions differ from C's `printf()`, a `scanf()` counterpart is absent, wideness
of stringtypes differs from corresponding [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) functions.

Since in many cases the [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) specification allows freedom for a particular library implementation 
(return values, flags, structure layout) but the chosen build and debug environment is VS2017 the original
Microsoft C Library functions had to be recreated, fully compatible, bug for bug (except otherwise noted). 
This would provide the most relieable solution for cross development, enable the use of the original
Microsoft header files and prevent from documenting yet another C Library implementation.

A *Hosted Environment* provides the following obligatory features: 
* [`int main(int argc,char **argv)`](https://docs.microsoft.com/en-us/cpp/c-language/main-function-and-program-execution?view=vs-2019) is the driver entry point<br>
NOTE: The additional `char *envp[]` and wide character version on `main()` is not part of the C specification
* `argc` and `argv` are used for parameter passing, in **CdePkg** also for POST drivers<br>https://docs.microsoft.com/en-us/cpp/c-language/parsing-c-command-line-arguments?view=vs-2019
* full blown ANSI C library
* buffered I/O, that means that [`ungetc()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/ungetc-ungetwc?view=vs-2019) works on streams, beside an improvement in speed
* [`exit()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit?view=vs-2019) invocation on driver termination, that implies
    * run [`atexit()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/atexit?view=vs-2019)-registered functions
    * free allocated memory 
    * remove all tempfiles (not applicable for POST drivers)
    * close open files (not applicable for POST drivers)

The command line parameter / load options for each *CdePkg*Driver shall be able to be adjusted
on a final system without recompiling the BIOS and updating the BIOS chip.
This e.g. allows the support engineer to change BIOS behaviour on a (partially)
defect system for repair/debug mode, enabling trace messages, excluding special
(non-compliant) devices from beeing enumerated. 

## Implementation
**CdePkg**'s functionality is composed of three components:
  1. the C Library **CdeLib**
  2. the service driver **CdeServices**
  3. the POST command line reference implementation **CdeLoadOptions**

all in 32 and 64 bit variants for DXE, SMM and PEI each.

**CdeLib** and **CdeServices** are highly optimized for space minimized appearance of 
ANSI C library functions in the UEFI drivers. This is achieved by implementing all-embracing worker functions

* `printf()`-family core
* `scanf()`-family core
* `realloc()` core (`malloc()`, `free()`, `realloc()` and `calloc()`)
* `strcmp()`-family core (all `str`/`wcs`/`mem` `n` `i` functions)
* `strcpy()`-family core (all `str`/`wcs`/`mem` `n` functions)
* `strpbrk()`/`strspn()` -family core (all `str`/`wcs` `pbrk`- and `spn`- functions)
* `strtok()`-family core (all `str`/`wcs` `tok`-functions)
* CRT0 startup code for DXE, SMM, PEI
* buffered I/O core (`fread()`, `fwrite()` and `ungetc()`)
* locale character tables
* interface functions to access DXE- and PEI-API

in the **CdeServices** driver, that resides once in DXE-, SMM- and PEI-phase each.
The **CdeLib** just provides small wrapper functions that invoke **CdeServices**.

![architectural view](archview.png)

### **CdeLoadOptions** / command line
Each *CdePkg*Driver reports its EfiCallerIdGuid while running through CRT0 to **CdeLoadOptions**.
**CdeLoadOptions** provides a pointer to the matching "Command Line" from an simple EfiCallerIdGuid/CommandLine table
within the file [`CdeLoadOptions.h`](https://github.com/MinnowWare/CdePkg/blob/master/Include/CdeLoadOptions.h), 
compiled into the [**CdeLoadOptions**](https://github.com/MinnowWare/CdePkg/blob/master/CdeLoadOptionsDxe/CdeLoadOptionsDxe.c) driver binary.

**This is just a proof of concept. In a real implementation, as mentioned above, the command line can be
changed without recompilation and BIOS update.**

## Status
The **CdeLib** and **CdeServices** are derived from their companion project 
[Torito C Library](https://github.com/JoaquinConoBolillo/torito-C-Library) but
split into *wrapper*/*worker* architecture. (Internally [Torito C Library](https://github.com/JoaquinConoBolillo/torito-C-Library)
was designed from the very beginning for that splitted architecture, but merged library and driver into one executable, to
run on platforms without **CdeServices** protocol.)

The functions below are already implemented and tested, every single one of them, except otherwise noted:

[List of available functions](https://github.com/MinnowWare/CdePkg/blob/master/implemented.md)


[Torito C Library](https://github.com/JoaquinConoBolillo/torito-C-Library) has passed extensive
tests to verify Microsoft's C Library compatibility and is also approved in various real world applications.
Therefore the **CdePkg**'s C library will be validated by simple tests only, in an upcoming **CdeValidationPkg**, for
DXE, SMM and PEI each.

## todo
* add SMM support
* add PEI support
* move CRT0 startup code to **CdeServices**
* move local character tables to **CdeServices**
* move buffered I/O core to **CdeServices**
* validate functions in DXE, SMM and PEI [List of available functions](https://github.com/MinnowWare/CdePkg/blob/master/implemented.md)
* complete library implementation

[todo reminder](todoreminder.md)

# Revision history

### 20190706
* add PEI support: CdeSrcPkg/0ea26421a591adcb4443e46913da2f429d7f312f

### 20190605
* update CdeSrcPkg/e32a2359acc2c5916581207d5627c1f73a5d1ba6
