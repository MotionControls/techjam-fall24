/*
Last updated 10/12/24
Desc:
	The point of this file is to implement fixed point maths without external libs for use with PVSnesLib.
	The idea behind fixed point numbers is to represent a decimal number in two parts.
	One part holds the whole number, the other holds the fraction.
	This is usually done by scaling the original number by a factor of 10.
		ex.
		23.24 = 2324 = 23.24 * 10^2
		-100.00980 = -10000980 = -100.00980 * 10^5
	This can also be done by representing the number via binary.
	The number gets ""moved"" into a bigger container.
		ex.
		(u. char) 23 = 00010111
		(u. short) 23 << 8 = 5888 = 0001011100000000
Sources:
	https://en.wikipedia.org/wiki/Fixed-point_arithmetic
*/

#ifndef FP_MATH_H
#define FP_MATH_H

#define ufx		unsigned short
#define sfx		short

ufx CharToUFX(unsigned char base, char dec){ return ((ufx)base << 8) + dec; }
sfx CharToSFX(char base, char dec){ return ((sfx)base << 8) + dec; }

unsigned char UFXToChar(ufx fp){ return fp >> 8; }
char SFXToChar(sfx fp){ return fp >> 8; }

ufx UFXAdd(ufx fp, unsigned char add){ return (fp + CharToUFX(add, 0)); }
sfx SFXAdd(sfx fp, char add){ return (fp + CharToSFX(add, 0)); }

ufx UFXSub(ufx fp, unsigned char add){ return (fp - CharToUFX(add, 0)); }

ufx UFXMultC(ufx fp, unsigned char mult){ return CharToUFX(UFXToChar(fp) * mult, 0); }	// This sucks but I'm having trouble getting it to work otherwise.
ufx UFXMult(ufx fp1, ufx fp2){ return (ufx)((unsigned int)(fp1 * fp2) >> 8); }
sfx SFXMultC(sfx fp, char mult){ return (sfx)((int)(fp * CharToSFX(mult, 0)) >> 8); }
sfx SFXMult(sfx fp1, sfx fp2){ return (sfx)((int)(fp1 * fp2) >> 8); }

ufx UFXDivC(ufx fp, unsigned char div){ return (fp / CharToUFX(div, 0)) << 8; }
ufx UFXDiv(ufx fp1, ufx fp2){ return (fp1 / fp2) << 8; }
sfx SFXDivC(sfx fp, char div){ return (fp / CharToSFX(div, 0)) << 8; }
sfx SFXDiv(sfx fp1, sfx fp2){ return (fp1 / fp2) << 8; }

#endif