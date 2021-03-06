/*!
@copyright

	Copyright (c) 2019, MinnowWare. All rights reserved.
	This program and the accompanying materials are licensed and made
	available under the terms and conditions of the BSD License
	which accompanies this distribution.  The full text of the license
	may be found at

	http://opensource.org/licenses/bsd-license.php

	THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
	WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

@file CdeWelcomeDxe.c

@brief C Development Environment (CDE) Welcome sample driver that is controlled by\n
	command line / load options and blinks the D2 LED on the MinnowBoard.

@details The command line parameter are provided by the <em>CdeLoadOptionsDxe</em> driver.\n
			Before <em>main</em> is invoked by the startup code (crt0), <em>CdeLoadOptionsDxe</em>\n
			is queried for the command line that belong to that paricular FILE_GUID/EfiCallerIdGuid.\n\n
			In emulation mode the GPIO access is skipped, but the argc/argv can be observed too.

@todo

@mainpage
	CdeWelcomeDxe sample driver for MinnowBoard

@section intro_sec Introduction
	This sample program exhibits the usage of the Standard C Library usage in UEFI DXE POST driver.\n
	ANSI C requires that the function called at propram startup is named <b><em>int main(int argc, char **argv)</em></b>.\n
	That sample is running on the MinnowBoard, a BayTrail (BYT) open source PC motherboard.\n\n
	<b><em>CdeLib</em></b> is the Standard C Library implementation used in the <b><em>CdePkg</em></b>\n
	
@subsection ref_sec References
	<a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n
	<a href="https://github.com/JoaquinConoBolillo/torito-C-Library/blob/master/implemented.md">functions supported in CdeLib</a>\n
	<a href="https://docs.microsoft.com/en-us/cpp/c-language/c-language-reference?view=vs-2019">Microsoft C Language Reference</a>\n
	<a href="https://minnowboard.org/compare-boards/">MinnowBoard</a>\n
	@image html StdCLibCover.jpg

@subsection Parm_sec Command line parameters
	1. no parameter: blink the D2 LED 10 times at 16Hz
	2. /frq x /count y: blinks the D2 LED y times at a frequency of xHz
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#undef NULL
#include <CDE.h>

#include <uefi.h>

#define BLINKCOUNT 10
#define BLINKFRQ   16

extern EFI_SYSTEM_TABLE* gST;
extern EFI_BOOT_SERVICES* gBS;
extern EFI_GUID gEfiSimpleTextOutProtocolGuid;

#define /*Gpio Dfx0 Pad Cfg           */gpio_dfx0           0x2170

int main(int argc,char **argv)
{
    EFI_STATUS Status = EFI_SUCCESS;
    char buffer[64];
	volatile int *pGPIO_D2_LED_CTRL;        //access 32 bit of GPIO
	unsigned gpiobase;                      // PCI 0/1F/0/4C[31:2]
	clock_t clk;
	int i;
	int blinkcount = BLINKCOUNT;
	int frq = BLINKFRQ;
	
	//__debugbreak();
    
    printf("Welcome, to the jungle...\n");	// console is not available during DXE, so you won't see it :-(
	//
	// get command line parameter "/frq 123" and "/count 13"
	//

	for (i = 0; i < argc; i++) {

		if (0 == strcmp("/frq", argv[i])) {
			frq = atoi(argv[i + 1]);
		}

		if (0 == strcmp("/count", argv[i])) {
			blinkcount = atoi(argv[i + 1]);
		}

	}

    CDEMOFINE((MFNINF(1) "\n######################################################################""\n##### Welcome, to the jungle ##########################################""\n#######################################################################\n"));
    
    sprintf(buffer, "%d %s %s %s %s %s\n", argc, argv[0], argv[1], argv[2], argv[3], argv[4]);
    CDEMOFINE((MFNINF(1) "%s\n", buffer));

	if (8 == sizeof(void*)) {/*don't do that in emulation mode*/
		//
		// get the GPIO base from the LPC device 0/1F/0 register 0x4C
		//
		outpd(0xCF8, 0x80000000 + (0 << 16) + (0x1F << 11) + (0 << 8) + 0x4C);
		gpiobase = ~3 & inpd(0xCFC);

		pGPIO_D2_LED_CTRL = (void*)((size_t)gpiobase + gpio_dfx0 + 8 /* value offset */);

		for (i = blinkcount; i > -1; i--) {

			clk = CLOCKS_PER_SEC / frq + clock();
			while (clk > clock())
				;

			*pGPIO_D2_LED_CTRL ^= 1;
			printf("\b\b\b%3d", i);     // print remaining blinks
		}
		*pGPIO_D2_LED_CTRL = 0;         // LED ON when exit
	}
    printf("\n");                       //new line
    
    return Status;
}
