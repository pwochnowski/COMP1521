// ADT for Bit-strings
// COMP1521 17s2 Week02 Lab Exercise
// Written by John Shepherd, July 2017
// Modified by ...

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Bits.h"

// assumes that an unsigned int is 32 bits
#define BITS_PER_WORD 32

// A bit-string is an array of unsigned ints (each a 32-bit Word)
// The number of bits (hence Words) is determined at creation time
// Words are indexed from right-to-left
// words[0] contains the most significant bits
// words[nwords-1] contains the least significant bits
// Within each Word, bits are indexed right-to-left
// Bit position 0 in the Word is the least significant bit
// Bit position 31 in the Word is the most significant bit

typedef unsigned int Word;

struct BitsRep {
   int nwords;   // # of Words
   Word *words;  // array of Words
};

Word charToUInt(char);
// make a new empty Bits with space for at least nbits
// rounds up to nearest multiple of BITS_PER_WORD
Bits makeBits(int nbits)
{
   Bits new;
   new = malloc(sizeof(struct BitsRep));
   assert(new != NULL);
   int  nwords;
   if (nbits%BITS_PER_WORD == 0)
      nwords = nbits/BITS_PER_WORD;
   else
      nwords = 1+nbits/BITS_PER_WORD;
   new->nwords = nwords;
   // calloc sets to all 0's
   new->words = calloc(nwords, sizeof(Word));
   assert(new->words != NULL);
   return new;
}

// release space used by Bits
void  freeBits(Bits b)
{
   assert(b != NULL && b->words != NULL);
   free(b->words);
   free(b);
}

// form bit-wise AND of two Bits a,b
// store result in res Bits
void andBits(Bits a, Bits b, Bits res)
{
	for (int j=0; j < a->nwords; j++) {
		res->words[j] = a->words[j] & b->words[j];
	}
}

// form bit-wise OR of two Bits a,b
// store result in res Bits
void orBits(Bits a, Bits b, Bits res)
{
	for (int j=0; j < a->nwords; j++) {
		res->words[j] = a->words[j] | b->words[j];
	}
}

// form bit-wise negation of Bits a,b
// store result in res Bits
void invertBits(Bits a, Bits res)
{
	for (int j=0; j < a->nwords; j++) {
		res->words[j] = ~a->words[j];
	}
}

// left shift Bits
void leftShiftBits(Bits b, int shift, Bits res)
{
	int finalWord, currIndex, currWord;
	Word mask;
	for (currWord = 0; currWord < b->nwords; currWord++) {
		res->words[currWord] = 0;
		for (mask= charToUInt(1)<<(31-(shift%32)), currIndex=shift%32; currIndex < 32; currIndex++, mask>>=1) {
			finalWord = currWord + 
                ((currIndex-shift < 0) ? (currIndex-shift)/BITS_PER_WORD - 1 : (currIndex-shift)/BITS_PER_WORD);
			if (finalWord < 0) continue;
			res->words[finalWord] |= (b->words[currWord] & mask) << shift%32; 
		}
	}
}

// right shift Bits
void rightShiftBits(Bits b, int shift, Bits res)
{
	int finalWord, currIndex, currWord;
	Word mask;
	for (currWord = b->nwords-1; currWord >= 0; currWord--) {
		res->words[currWord] = 0;
		for (mask= charToUInt(1)<<shift%32, currIndex=31-shift%32; currIndex>=0; currIndex--, mask<<=1) {
			finalWord = (currIndex+shift)/BITS_PER_WORD + currWord;
			if (finalWord >= b->nwords) continue;
			res->words[finalWord] |= (b->words[currWord] & mask) >> shift%32; 
		}
	}
}

// copy value from one Bits object to another
void setBitsFromBits(Bits from, Bits to)
{
	for (int j = 0; j < from->nwords; j++) {	
		to->words[j] = from->words[j];
	}
}

int min(int a, int b) {
	return (a < b) ? a : b;
}

// assign a bit-string (sequence of 0's and 1's) to Bits
// if the bit-string is longer than the size of Bits, truncate higher-order bits
void setBitsFromString(Bits b, char *bitseq)
{
	int count = min(strlen(bitseq), b->nwords*BITS_PER_WORD);

	int j=b->nwords;
	for (int i=0; i<count; i++) {
		if (i%32 == 0) b->words[--j]=0;
		b->words[j] = b->words[j] | (charToUInt(bitseq[count-(i+1)]) << i%32);
	}
	while (--j >= 0) b->words[j]=0;
}

//Documentation pending
Word charToUInt(char c) {
	return c-'0';
}

// display a Bits value as sequence of 0's and 1's
void showBits(Bits b)
{
	for (int i = 0; i < b->nwords; i++) {
		Word mask = 1;
		mask <<= 31;
		for (int j = 0; j < 32; j++) {
			printf("%d", (b->words[i] & mask) ? 1 : 0);
			mask >>= 1;
		}
	}
}

