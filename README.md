# ASN1DUMP
Print ASN.1 structure and values.

## Introduction
This program prints the structure and the values from a an ASN.1 structure
from a data file. The structure is determined automatically and does not be
provided separately. 

## Usage
Calling the program **asn1dump** without any arguments prints the usage 
instruction and command line arguments

	usage: asn1dump [Options] <filename>
		   Options:
		   -context      : try to show content of context tags
		   -octhex       : hexdump octet strings
		   -dump         : hexdump only
		   -prtoffset    : Print the current offset
		   -offset <pos> : start at byte offset 'pos'

## Installation
- First, you need to checkout, compile and install the ASN.1 library
	from [https://github.com/ankraft/akasn1lib](https://github.com/ankraft/akasn1lib).
- Then check out this repository and make necessary adjustments to the
	Makefile. It should compile on common 32- and 64-bit systems without problems.

## History
This utility program was written in the early 1990's and was used in a couple
of projects and for the development of products.

## License

The MIT License (MIT)

Copyright (c) 1993 - 2016 Andreas Kraft

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.