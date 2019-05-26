# CdePkg
C Development Environment Package for EDK2

## Introduction
**CdePkg**, C Development Environment Package, introduces the usage of *Hosted Environment*,
as specified by [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf),
for both UEFI POST and SHELL drivers.
This is a reference implementation only, using the Microsoft C compiler, linker, library 
manager and IDE that comes with Visual Studio 2017 for x86 platforms.

Since a missing or faulty *Hosted Environment* is a hindrance for developing UEFI SHELL applications,
it's introduction for UEFI POST drivers is a novum. But the wealth of computing power of current UEFI
maschines offer the chance to translate [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf)
compatible source code to run as a UEFI POST driver.

With the growing complexity of firmware due to the requirements for both security and trust and the
need for speed in development, usage of platform independant source code allows:
* reuse of validated source code, as already done in EDK2\CryptoPkg with openSSL
* crossdevelopment of complex firmware code on non-UEFI platforms with superb build and debug capabilities

Since the UEFI "OS" interface (DXE/SHELL/SMM and PEI) can be accessed directly by the compiler
translated sourcecode and UEFI provides an independant set of functions, macros and type definitions,
[ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) and UEFI "OS" specific sourcecode can seamless coexist. That allows a functional [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) prototype
to adjust successive to real world driver requirements in the UEFI environment. One intermediate step for
that proceeding might be a UEFI SHELL application, if the target is a DXE or SMM driver.

In case, external UEFI libraries (created by the EDK build process) are not used in a particular UEFI
driver (and therefore the *Library Constructor* process is not needed), a UEFI driver can be translated
in the VS2017 build environment, that is much faster than EDK build process (the driver's binary is not
placed in the BIOS image, of course). *But the creation of syntactically correct source code, using the
luxurious auto completion and mouse hover actions also for UEFI specific type definitions, enhances the
development process notably.*

## Intention
**CdePkg** is a feasibility study on how to provide a complete *Hosted Environment* 
(according to [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) Specification chapter 5.1.2) including all instrisic functions, 
that the compiler requires to be a full featured C-compiler, in particular the full
set of C-language operators (specifically `/ % << >>`) for the 32 bit code generator, needed in PEI.

Furthermore the questions has to be answered, if UEFI based products can be improved regarding:
* software quality (in terms of number of required updates during a life cycle)
* development time
* feature set (complexity and plenty)
* storage space needed in a flash part (the smaller the better)

if a standardized programming interface as [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) is available, additionally a storage space optimization
strategy is used, as described below, that splits *wrapper libraries* from *worker drivers*.

In the UEFI programming environment not even the smallest piece of code can be cross checked on a
different platform, since function names differs in upper/lower case letters, format specifier for
`Print()`-functions differs from C's `printf()`, a `scanf()` counterpart is absent, wideness
of stringtypes differs from C functions.

Since the [ANSI C](https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf) specification leaves freedom for a particular library implementation in many cases
(return values, flags, structure layout) but the chosen build and debug environment was VS2017 the original
Microsoft C Library functions has to be recreated, fully compatible, bug for bug (except otherwise noted). 
That would provide the most relieable solution for cross development, enable the use of the original
Microsoft header files and prevents from documenting yet another C Library implementation.

A *Hosted Environment* provides that obligatory features: 
* `int main(int argc,char **argv)` is the driver entry point
* `argc` and `argv` are used for parameter passing, in **CdePkg** also for POST drivers
* full blown ANSI C library
* buffered I/O, that means that `ungetc()` works on streams
* `exit()` invokation on driver termination, that implies
    * run `atexit()`-registered functions
    * free allocated memory 
    * remove all tempfiles (not applicable for POST drivers)
    * close open files (not applicable for POST drivers)

The command line parameter / load options for each *CdePkg*Driver shall be able to be adjusted
on a final system w/o recompiling the BIOS and updating the BIOS chip.
This allows e.g. the support engineer to change BIOS behaviour on a (partially)
defect system for repair/debug mode, enabling trace messages, excluding special
(non-compliant)devices from beeing enumerated. 

## Implementation
**CdePkg**'s functionality is composed of three components:
  1. the C Library **CdeLib**
  2. the service driver **CdeServices**
  3. the POST command line reference implementation **CdeLoadOptions**

all in 32 and 64 bit variant for DXE, SMM and PEI each.

**CdeLib** and **CdeServices** are highly optimized for space minimized appearance of 
ANSI C library functions in the UEFI drivers. This is done by implementing all-embracing workerfunctions

* `printf()`-family core
* `scanf()`-family core
* `realloc()` core (`malloc()`, `free()`, `realloc()` and `calloc()`)
* `strcmp()`-family core (all str/wcs/mem n i functions)
* `strcpy()`-family core (all str/wcs/mem n functions)
* `strpbrk()`/`strspn()` -family core (all str/wcs pbrk- and spn- functions)
* `strtok()`-family core (all str/wcs tok-functions)
* CRT0 startup code for DXE, SMM, PEI
* buffered I/O core (`fread()`, `fwrite()` and `ungetc()`)
* locale character tables
* interface functions to access DXE- and PEI-API

in the **CdeServices** driver, that resides once in DXE-, SMM- and PEI-phase each.
The **CdeLib** just provides small wrapper functions that invokes **CdeServices**.

### **CdeLoadOptions** / command line
Each *CdePkg*Driver reports it's EfiCallerIdGuid while running through CRT0 to **CdeLoadOptions**.
**CdeLoadOptions** provides a pointer to the matching "Command Line" from an simple EfiCallerIdGuid/CommandLine table.
This is just a proof of concept. In a real implementation, as mentioned above, the command line can be
changed w/o BIOS update.

## Status
The **CdeLib** and **CdeServices** are derived from it's companion project 
[Torito C Library](https://github.com/JoaquinConoBolillo/torito-C-Library) but
split into *wrapper*/*worker* architecture. (Internally [Torito C Library](https://github.com/JoaquinConoBolillo/torito-C-Library)
was designed from the very beginning for that splitted architecture, but merged library and driver into one executable, to
run on platforms w/o **CdeServices** protocol.)

The functions below are already implemented and tested, every single one of them, except otherwise noted:

[list of available functions](https://github.com/MinnowWare/CdePkg/blob/master/implemented.md)


[Torito C Library](https://github.com/JoaquinConoBolillo/torito-C-Library) has passed extensive
tests to verify Microsoft's C Library compatibility and is also approved in various real world applications.
Therefore the **CdePkg**'s C library will be validated by simple tests only, in an upcoming **CdeValPkg**, for
DXE, SMM and PEI each.

## Known bugs
* SMM support still missing
* PEI support still missing
* CRT0 startup code is still library code
* local character tables still library code
* buffered I/O core still library code

# Revision history

