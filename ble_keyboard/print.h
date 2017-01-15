/* Copyright (C) 2011-2016 by Jacob Alexander
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

// ----- Includes -----

// ----- Defines -----
#define NL "\r\n"



// ----- Functions and Corresponding Function Aliases -----

/* XXX
 * Note that all the variadic functions below, take comma separated string lists, they are purposely not printf style (simplicity)
 */
#define dPrint(c)         { (void)c; }

// Info Messages
#define info_dPrint(...)  {} // Info Msg
#define info_print(str)   {} // Info Msg
#define info_msg(str)     {} // Info Msg

// Warning Messages
#define warn_dPrint(...)  {} // Warning Msg
#define warn_print(str)   {} // Warning Msg
#define warn_msg(str)     {} // Warning Msg

// Error Messages
#define erro_dPrint(...)  {} // Error Msg
#define erro_print(str)   {} // Error Msg
#define erro_msg(str)     {} // Error Msg

// Debug Messages
#define dbug_dPrint(...)  {} // Debug Msg
#define dbug_print(str)   {} // Debug Msg
#define dbug_msg(str)     {} // Debug Msg


// Static String Printing
#if defined(_at90usb162_) || defined(_atmega32u4_) || defined(_at90usb646_) || defined(_at90usb1286_)
#define print(s) _print(PSTR(s))
#else
#define print(s) _print(s)
#endif

void _print( const char *s ){}
void printstrs( char* first, ... ){}


// Printing numbers
#define printHex(hex)   printHex_op(hex, 1)
#define printHex32(hex) printHex32_op(hex, 1)

void printInt8    ( uint8_t  in ){}
void printInt16   ( uint16_t in ){}
void printInt32   ( uint32_t in ){}
void printHex_op  ( uint16_t in, uint8_t op ){}
void printHex32_op( uint32_t in, uint8_t op ){}


// String Functions
#define hexToStr(hex, out) hexToStr_op(hex, out, 1)

void int8ToStr    ( uint8_t  in, char*  out ){}
void int16ToStr   ( uint16_t in, char*  out ){}
void int32ToStr   ( uint32_t in, char*  out ){}
void hexToStr_op  ( uint16_t in, char*  out, uint8_t op ){}
void hex32ToStr_op( uint32_t in, char*  out, uint8_t op ){}
void revsStr      ( char*  in ){}
uint16_t lenStr   ( char*  in ){ return 0; }
int16_t eqStr     ( char*  str1, char* str2 ){ return 0; } // Returns -1 if identical, last character of str1 comparison (0 if str1 is like str2)
int numToInt      ( char* in ){ return 0; } // Returns the int representation of a string

void hex32ToStr16 ( uint32_t in, uint16_t* out, uint8_t op ){} // Used for USB Descriptors

