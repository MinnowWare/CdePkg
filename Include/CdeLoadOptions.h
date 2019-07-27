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
@file
	CdeLoadOptionsDxe.h
@brief this file provides the FILE_GUID to command line / Filename + LoadOptions mapping
@todo
*/
    {/*FILE_GUID, EfiCallerIdGuid*/ {0xCDE000FF, 0x0A15, 0x41D7, { 0x9A, 0x7D, 0x71, 0x11, 0x33, 0x8C, 0xBD, 0x8B }}, "CdeWelcomeDxe /frq 32 /count 40"},
    {/*FILE_GUID, EfiCallerIdGuid*/ {0xCDE000FF, 0xE038, 0x435A, { 0x8B, 0x68, 0x34, 0xD7, 0xDF, 0xE1, 0xEF, 0x54 }}, "CdeWelcomePei /frq 8  /count 10" },
    {/*FILE_GUID, EfiCallerIdGuid*/{ 0xCDE000FF, 0xFBBA, 0x4AA6,{ 0xB1, 0xC2, 0x97, 0x01, 0x03, 0x89, 0x05, 0xB2 } },"clockDxe /frq321 3 /countABC 63" },/*strncmp()*/
    {/*FILE_GUID, EfiCallerIdGuid*/{ 0xCDE000FF, 0xA2C0, 0x4C76,{ 0x82, 0xD5, 0xF0, 0x20, 0x6E, 0xA5, 0x32, 0x89 } },"clockPei /frq123 1 /countABC 21" },/*strncmp()*/
    {/*FILE_GUID, EfiCallerIdGuid*/  {0xCDE00007, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }},"UnknownDriver" },
	/* add more drivername/commandline pairs here...*/