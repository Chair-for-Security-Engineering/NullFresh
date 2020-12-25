/*
* -----------------------------------------------------------------
* COMPANY : Ruhr University Bochum
* AUTHOR  : Amir Moradi (amir.moradi@rub.de)
* DOCUMENT: https://doi.org/10.46586/tches.v2021.i1.305-342
* -----------------------------------------------------------------
*
* Copyright (c) 2020, Amir Moradi, Aein Rezaei Shahmirzadi
*
* All rights reserved.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Please see LICENSE and README for license and further instructions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <omp.h>

#define	MaxNumberOfTerms 1000000000L

unsigned char   *HW;
unsigned int    *NumberOfIndexesInHW;
unsigned long  **IndexesInHW;

#define ONEL ((unsigned __int64)1)

void FillANFTables(unsigned int NumberOfCells)
{
	unsigned int	i, Bit;
	//unsigned int	NumberOfCells=256;
	unsigned char	NumberOfBits = log((double)NumberOfCells) / log((double)2);

	HW = (unsigned char *)malloc(NumberOfCells * sizeof(unsigned char));

	NumberOfIndexesInHW = (unsigned int *)malloc((NumberOfBits + 1) * sizeof(unsigned int));
	IndexesInHW = (unsigned long **)malloc((NumberOfBits + 1) * sizeof(unsigned long *));

	for (Bit = 0; Bit < (NumberOfBits + 1); Bit++)
	{
		IndexesInHW[Bit] = (unsigned long *)malloc((NumberOfCells / 2) * sizeof(unsigned long));
		NumberOfIndexesInHW[Bit] = 0;
	}

	for (i = 0; i < NumberOfCells; i++)
	{
		HW[i] = 0;

		for (Bit = 0; Bit < NumberOfBits; Bit++)
			HW[i] += ((i & (1L << Bit)) >> Bit);

		IndexesInHW[HW[i]][NumberOfIndexesInHW[HW[i]]] = i;
		NumberOfIndexesInHW[HW[i]]++;
	}
}


struct FunctionStruct
{
	unsigned int		NumberOfTerms;
	unsigned __int64   *Term;
};

void DefineFunction(FunctionStruct *F, unsigned short NumberOfTerms)
{
	F->Term = (unsigned __int64 *)malloc(NumberOfTerms * sizeof(unsigned __int64));
}

void FreeFunction(FunctionStruct *F)
{
	free(F->Term);
}

unsigned char EvalFunction(FunctionStruct *Function, unsigned __int64 Input)
{
	unsigned char	Res = 0;
	unsigned int	i;

	for (i = 0; i < Function->NumberOfTerms; i++)
		Res ^= ((Function->Term[i] & Input) == Function->Term[i]);

	return (Res);
}

void AddTermtoFunction(FunctionStruct *Function, unsigned __int64 Symbol)
{
	Function->Term[Function->NumberOfTerms] = Symbol;
	Function->NumberOfTerms++;
}

void MakeANF(unsigned char* Table, unsigned int NumberOfElements, FunctionStruct* Functions, unsigned char OutputBit, unsigned char Invert)
{
	unsigned char   NumberOfBits = log((double)NumberOfElements) / log((double)2);
	unsigned char	HWIndex;
	unsigned int	i;

	Functions->NumberOfTerms = 0;

	for (HWIndex = 0; HWIndex < (NumberOfBits + 1); HWIndex++)
		for (i = 0; i < NumberOfIndexesInHW[HWIndex]; i++)
			if (IndexesInHW[HWIndex][i] < NumberOfElements)
				if (EvalFunction(Functions, IndexesInHW[HWIndex][i]) != (((Table[IndexesInHW[HWIndex][i]] >> OutputBit) & 1) ^ Invert))
					AddTermtoFunction(Functions, IndexesInHW[HWIndex][i]);
}

unsigned char*	Term2Index;
unsigned int*	Index2Term;

void FillANFTermTables()
{
	unsigned int	i;
	unsigned char	c;

	Term2Index = (unsigned char*)malloc((1 << 16) * sizeof(unsigned char));
	Index2Term = (unsigned int*)malloc(16 * sizeof(unsigned int));

	// input order: (MSB) a1, a2, b1, b2, c1, c2, d1, d2, e1, e2, f1, f2, g1, g2, h1, h2 (LBS)

	c = 0;
	for (i = 1; i < (1 << 16); i <<= 1)  // only linear terms
	{
		Term2Index[i] = c;
		Index2Term[c] = i;
		c++;
	}
}

unsigned char EvalFunctionSpecial(uint64_t Func, unsigned int Input)
{
	unsigned char	Res = 0;
	unsigned int	i;

	for (i = 0; Func; Func >>= 1)
	{
		if (Func & 1)
			Res ^= ((Index2Term[i] & Input) == Index2Term[i]);
		i++;
	}

	return (Res);
}

uint64_t MakeANFSpecial(unsigned char* Table, unsigned char NumberOfBits)
{
	unsigned int	NumberOfElements = (1 << NumberOfBits);
	unsigned char	HWIndex;
	unsigned char	i;
	uint64_t		Res = 0;
	uint64_t		ONE = 1;

	for (HWIndex = 1; HWIndex <= 1; HWIndex++)
		for (i = 0; i < NumberOfIndexesInHW[HWIndex]; i++)
			if (IndexesInHW[HWIndex][i] < NumberOfElements)
			{
				if (EvalFunctionSpecial(Res, IndexesInHW[HWIndex][i]) != Table[IndexesInHW[HWIndex][i]])
					Res |= (ONE << Term2Index[IndexesInHW[HWIndex][i]]);
			}
			else
				break;

	return Res;
}


unsigned char AlgDegree(FunctionStruct Function)
{
	unsigned int		i;
	unsigned char		Degree;

	Degree = 1;
	for (i = 0; i < Function.NumberOfTerms; i++)
		if (HW[Function.Term[i]] > Degree)
			Degree = HW[Function.Term[i]];

	return(Degree);
}

void SPrintFunction(FunctionStruct Function, char* ResStr, char InputStr[][5])
{
	unsigned int		i, j;
	unsigned __int64	ONE;

	ResStr[0] = 0;

	for (i = 0; i < Function.NumberOfTerms; i++)
	{
		if (i)
			strcat(ResStr, " + ");

		if (Function.Term[i])
		{
			ONE = 1;

			for (j = 0; j < 64; j++)
			{
				if (Function.Term[i] & ONE)
					sprintf(ResStr, "%s%s", ResStr, InputStr[j]);

				ONE <<= 1;
			}
		}
		else
			strcat(ResStr, "1");
	}
}

//---------------------------------------------------------------------------

void FillF8to1(unsigned char** F8to1[4], int NumberOfF8to1[4], unsigned char* DesiredFunc)
{
	unsigned int	i, j, t;
	char			index;
	unsigned char	MadeFunc[256];

	for (index = 0; index < 4; index++)
		if (F8to1[index] == NULL)
			F8to1[index] = (unsigned char**)calloc(256, sizeof(unsigned char*)); // to fill every cell with NULL

	index = 0;
	NumberOfF8to1[index] = 256;
	for (i = 0;i < 256;i++)
	{
		if (F8to1[index][i] == NULL)
			F8to1[index][i] = (unsigned char*)malloc(256 * sizeof(unsigned char));

		memcpy(MadeFunc, DesiredFunc, 256 * sizeof(unsigned char));

		for (j = 0;j < 256;j++)
		{
			for (t = 1;t < 256;t <<= 1) // adding just linear terms
				if (i & j & t)
					MadeFunc[j] ^= 1;

			F8to1[index][i][j] = MadeFunc[j] << index;
		}
	}

	//------------------

	index = 1;
	NumberOfF8to1[index] = 256;
	for (i = 0; i < 256; i++)
	{
		if (F8to1[index][i] == NULL)
			F8to1[index][i] = (unsigned char*)malloc(256 * sizeof(unsigned char));

		memcpy(MadeFunc, DesiredFunc, 256 * sizeof(unsigned char));

		for (j = 0; j < 256; j++)
		{
			for (t = 1; t < 256; t <<= 1) // adding just linear terms
				if (i & j & t)
					MadeFunc[j] ^= 1;

			F8to1[index][i][j] = MadeFunc[j] << index;
		}
	}

	//------------------

	index = 2;
	NumberOfF8to1[index] = 256;
	for (i = 0; i < 256; i++)
	{
		if (F8to1[index][i] == NULL)
			F8to1[index][i] = (unsigned char*)malloc(256 * sizeof(unsigned char));

		memcpy(MadeFunc, DesiredFunc, 256 * sizeof(unsigned char));

		for (j = 0; j < 256; j++)
		{
			for (t = 1; t < 256; t <<= 1) // adding just linear terms
				if (i & j & t)
					MadeFunc[j] ^= 1;

			F8to1[index][i][j] = MadeFunc[j] << (index - 2);
		}
	}

	//------------------

	index = 3;
	NumberOfF8to1[index] = 256;
	for (i = 0; i < 256; i++)
	{
		if (F8to1[index][i] == NULL)
			F8to1[index][i] = (unsigned char*)malloc(256 * sizeof(unsigned char));

		memcpy(MadeFunc, DesiredFunc, 256 * sizeof(unsigned char));

		for (j = 0; j < 256; j++)
		{
			for (t = 1; t < 256; t <<= 1) // adding just linear terms
				if (i & j & t)
					MadeFunc[j] ^= 1;

			F8to1[index][i][j] = MadeFunc[j] << (index - 2);
		}
	}
}

struct ToupleStruct
{
	unsigned short	i[2];
	uint64_t		ANF;
};

void FillTableIndexes(unsigned short** &InputTableIndex, unsigned short* &OutputTableIndex, unsigned char* &Unmasking, unsigned short* &UnmaskedInputTable,
	char InputStr[4][8][5], unsigned char Types[8])
{
	char			index;
	unsigned short  a, a1, a2;
	unsigned short	b, b1, b2;
	unsigned short	c, c1, c2;
	unsigned short	d, d1, d2;
	unsigned short	e, e1, e2;
	unsigned short	f, f1, f2;
	unsigned short	g, g1, g2;
	unsigned short	h, h1, h2;
	unsigned short	abcdefgh;
	unsigned short	abcdefgh1;
	unsigned short	abcdefgh2;
	unsigned short	Masked_InputIndex;
	unsigned short	OutIndex;
	unsigned short	in[8][2];
	unsigned short	i, j, k;
	FILE*			F;
	char			TempStr[100];
	unsigned int	TempInt;

	if (InputTableIndex == NULL)
		InputTableIndex = (unsigned short**)calloc(4, sizeof(unsigned short*));
	for (index = 0; index < 4; index++)
		if (InputTableIndex[index] == NULL)
			InputTableIndex[index] = (unsigned short*)malloc((1 << 16) * sizeof(unsigned short));

	if (OutputTableIndex == NULL)
		OutputTableIndex = (unsigned short*)malloc((1 << 16) * sizeof(unsigned short));

	if (UnmaskedInputTable == NULL)
		UnmaskedInputTable = (unsigned short*)malloc((1 << 16) * sizeof(unsigned short));

	Masked_InputIndex = 0;
	for (abcdefgh = 0; abcdefgh < 256; abcdefgh++)
	{
		a = (abcdefgh & 1) ? 1 : 0;
		b = (abcdefgh & 2) ? 1 : 0;
		c = (abcdefgh & 4) ? 1 : 0;
		d = (abcdefgh & 8) ? 1 : 0;
		e = (abcdefgh & 16) ? 1 : 0;
		f = (abcdefgh & 32) ? 1 : 0;
		g = (abcdefgh & 64) ? 1 : 0;
		h = (abcdefgh & 128) ? 1 : 0;

		for (abcdefgh1 = 0; abcdefgh1 < 256; abcdefgh1++)
		{
			abcdefgh2 = abcdefgh ^ abcdefgh1;

			a1 = (abcdefgh1 & 1) ? 1 : 0;
			b1 = (abcdefgh1 & 2) ? 1 : 0;
			c1 = (abcdefgh1 & 4) ? 1 : 0;
			d1 = (abcdefgh1 & 8) ? 1 : 0;
			e1 = (abcdefgh1 & 16) ? 1 : 0;
			f1 = (abcdefgh1 & 32) ? 1 : 0;
			g1 = (abcdefgh1 & 64) ? 1 : 0;
			h1 = (abcdefgh1 & 128) ? 1 : 0;

			a2 = (abcdefgh2 & 1) ? 1 : 0;
			b2 = (abcdefgh2 & 2) ? 1 : 0;
			c2 = (abcdefgh2 & 4) ? 1 : 0;
			d2 = (abcdefgh2 & 8) ? 1 : 0;
			e2 = (abcdefgh2 & 16) ? 1 : 0;
			f2 = (abcdefgh2 & 32) ? 1 : 0;
			g2 = (abcdefgh2 & 64) ? 1 : 0;
			h2 = (abcdefgh2 & 128) ? 1 : 0;

			in[0][0] = a1;
			in[0][1] = a2;
			in[1][0] = b1;
			in[1][1] = b2;
			in[2][0] = c1;
			in[2][1] = c2;
			in[3][0] = d1;
			in[3][1] = d2;

			in[4][0] = e1;
			in[4][1] = e2;
			in[5][0] = f1;
			in[5][1] = f2;
			in[6][0] = g1;
			in[6][1] = g2;
			in[7][0] = h1;
			in[7][1] = h2;

			for (j = 0;j < 4;j++)
				InputTableIndex[j][Masked_InputIndex] = 0;

			for (j = 0;j < 8;j++)
			{
				if (Types[j] == 0)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][1] << j;
				}
				else if (Types[j] == 1)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][1] << j;
				}
				else if (Types[j] == 2)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][0] << j;
				}
			}

			OutIndex = (a1 << 15) | (a2 << 14) | (b1 << 13) | (b2 << 12) | (c1 << 11) | (c2 << 10) | (d1 << 9) | (d2 << 8) |
				       (e1 << 7)  | (e2 << 6)  | (f1 << 5)  | (f2 << 4)  | (g1 << 3)  | (g2 << 2)  | (h1 << 1) | (h2 << 0);
			OutputTableIndex[Masked_InputIndex] = OutIndex;

			UnmaskedInputTable[Masked_InputIndex] = (h << 7) | (g << 6) | (f << 5) | (e << 4) | (d << 3) | (c << 2) | (b << 1) | (a << 0);
			Masked_InputIndex++;
		}
	}

	char MyInputStr[4][8][5] = {
		{ "a1", "b1", "c1", "d ", "e ", "f ", "g ", "h " },
		{ "a1", "b2", "c2", "d ", "e ", "f ", "g ", "h " },
		{ "a2", "b1", "c2", "d ", "e ", "f ", "g ", "h " },
		{ "a2", "b2", "c1", "d ", "e ", "f ", "g ", "h " } };

	for (j = 0;j < 4;j++)
		for (k = 0;k < 8;k++)
		{
			strcpy(InputStr[j][k], MyInputStr[j][k]);
			InputStr[j][k][1] = MyInputStr[j][Types[k]][1];
		}

	if (Unmasking == NULL)
		Unmasking = (unsigned char*)malloc(256 * sizeof(unsigned char));

	for (j = 0; j < 256; j++)
	{
		Unmasking[j] = 0;
		for (k = 0; k < 8; k++)
			Unmasking[j] ^= (j & (1 << k)) ? 1 : 0;
	}
}

struct LinkListStruct
{
	LinkListStruct*	Before;
	uint64_t		ANF;
	unsigned int	NumberOfTouples;
	unsigned int*	TouplesIndex;
	LinkListStruct*	Next;
};

const unsigned short	MaxSizeOfEachShortList = 10;

struct LinkListResStruct
{
	unsigned char	Found;
	LinkListStruct*	OldAddress;
	LinkListStruct*	Address;
	unsigned int	ShortListIndex;
	unsigned short	PlaceInShortListIndex;
};


LinkListResStruct FindInLinkList(LinkListStruct** ShortListFTouple2InANF, unsigned int NumberOfShortListFTouple2InANF, uint64_t ANF)
{
	unsigned int		up;
	unsigned int		middle;
	unsigned int		down;
	unsigned short		i;
	LinkListResStruct	Res;

	up = 0;
	down = NumberOfShortListFTouple2InANF - 1;

	while (up < (down - 1))
	{
		middle = floor((up + down) / 2);
		if (ShortListFTouple2InANF[middle]->ANF > ANF)
			down = middle;
		else
			up = middle;
	}

	i = 0;
	Res.OldAddress = NULL;
	Res.Address = ShortListFTouple2InANF[up];
	while ((Res.Address != ShortListFTouple2InANF[down]) && (Res.Address->ANF < ANF))
	{
		Res.OldAddress = Res.Address;
		Res.Address = Res.Address->Next;
		i++;
	}

	if (Res.Address->ANF == ANF)
		Res.Found = 1;
	else
	{
		Res.Found = 0;
		Res.ShortListIndex = up;
		Res.PlaceInShortListIndex = i;
	}

	return(Res);
}


void AddToLinkList(LinkListStruct** ShortListFTouple2InANF, unsigned short* SizeOfShortListFTouple2InANF, unsigned int &NumberOfShortListFTouple2InANF,
	uint64_t ANF, unsigned int ToupleIndex)
{
	LinkListResStruct	Res;
	LinkListStruct*		NewCell = NULL;
	unsigned int*		Buff;
	int					i;

	Res = FindInLinkList(ShortListFTouple2InANF, NumberOfShortListFTouple2InANF, ANF);

	if (!Res.Found)
	{
		NewCell = (LinkListStruct*)malloc(sizeof(LinkListStruct));
		NewCell->ANF = ANF;
		NewCell->TouplesIndex = NULL;
		NewCell->NumberOfTouples = 0;

		NewCell->Before = Res.OldAddress;
		NewCell->Next = Res.OldAddress->Next;
		Res.OldAddress->Next = NewCell;
		NewCell->Next->Before = NewCell;

		Res.Address = NewCell;

		SizeOfShortListFTouple2InANF[Res.ShortListIndex]++;
		if ((SizeOfShortListFTouple2InANF[Res.ShortListIndex] + 1) > MaxSizeOfEachShortList)
		{
			if ((Res.ShortListIndex > 0) &&
				(SizeOfShortListFTouple2InANF[Res.ShortListIndex - 1] < MaxSizeOfEachShortList))
			{
				SizeOfShortListFTouple2InANF[Res.ShortListIndex - 1]++;
				ShortListFTouple2InANF[Res.ShortListIndex] = ShortListFTouple2InANF[Res.ShortListIndex]->Next;
				SizeOfShortListFTouple2InANF[Res.ShortListIndex]--;
			}
			else if ((Res.ShortListIndex < (NumberOfShortListFTouple2InANF - 2)) &&
				(SizeOfShortListFTouple2InANF[Res.ShortListIndex + 1] < MaxSizeOfEachShortList))
			{
				SizeOfShortListFTouple2InANF[Res.ShortListIndex]--;
				ShortListFTouple2InANF[Res.ShortListIndex + 1] = ShortListFTouple2InANF[Res.ShortListIndex + 1]->Before;
				SizeOfShortListFTouple2InANF[Res.ShortListIndex + 1]++;
			}
			else
			{
				for (i = NumberOfShortListFTouple2InANF - 1;i > Res.ShortListIndex;i--)
				{
					ShortListFTouple2InANF[i + 1] = ShortListFTouple2InANF[i];
					SizeOfShortListFTouple2InANF[i + 1] = SizeOfShortListFTouple2InANF[i];
				}

				ShortListFTouple2InANF[Res.ShortListIndex + 1] = NewCell;
				SizeOfShortListFTouple2InANF[Res.ShortListIndex + 1] = SizeOfShortListFTouple2InANF[Res.ShortListIndex] - Res.PlaceInShortListIndex;
				SizeOfShortListFTouple2InANF[Res.ShortListIndex] = Res.PlaceInShortListIndex;
				NumberOfShortListFTouple2InANF++;
			}
		}
	}

	Buff = (unsigned int*)malloc((Res.Address->NumberOfTouples + 1) * sizeof(unsigned int));
	memcpy(Buff, Res.Address->TouplesIndex, Res.Address->NumberOfTouples * sizeof(unsigned int));
	free(Res.Address->TouplesIndex);
	Res.Address->TouplesIndex = Buff;

	Res.Address->TouplesIndex[Res.Address->NumberOfTouples] = ToupleIndex;
	Res.Address->NumberOfTouples++;
}

void MakeLargeTables(unsigned char** F8to1[4], int NumberOfF8to1[4], unsigned char Types[8],
	unsigned short** InputTableIndex, unsigned short* OutputTableIndex, unsigned char* Unmasking,
	ToupleStruct* &FTuples1, ToupleStruct* &FTuples2,
	unsigned int &NumberOfFTuples1, LinkListStruct** &ShortListFTouple1InANF, unsigned short* &SizeOfShortListFTouple1InANF, unsigned int &NumberOfShortListFTouple1InANF,
	unsigned int &NumberOfFTuples2,	LinkListStruct** &ShortListFTouple2InANF, unsigned short* &SizeOfShortListFTouple2InANF, unsigned int &NumberOfShortListFTouple2InANF)
{
	char			index;
	char			index2;
	unsigned int	Masked_InputIndex;
	short 			i0;
	short 			i1;
	short 			i_last;
	unsigned short	i[3] = { 0 };
	unsigned char	Masked_Output;
	unsigned char	Unmasked_Output;
	unsigned short	UniformityCounter[2];
	unsigned short	DistributionOld[4];
	unsigned short	Distribution[4];
	unsigned char**	MadeTable;
	uint64_t		MadeANF;
	unsigned short  j;
	char			k;
	unsigned int*	TempBuff;
	unsigned short  Counter;
	int				ThreadNum;

	MadeTable = (unsigned char**)malloc(omp_get_max_threads() * sizeof(unsigned char*));
	for (j = 0;j < omp_get_max_threads();j++)
		MadeTable[j] = (unsigned char*)malloc((1 << 16) * sizeof(unsigned char));

	if (FTuples1 == NULL)
		FTuples1 = (ToupleStruct*)malloc(70000 * sizeof(ToupleStruct)); // should be enough, it is at most 256 * 256

	if (ShortListFTouple1InANF == NULL)
		ShortListFTouple1InANF = (LinkListStruct**)malloc(ceil(70000 / MaxSizeOfEachShortList) * sizeof(LinkListStruct*)); // should be enough, it is at most 256 * 256
	memset(ShortListFTouple1InANF, 0, ceil(70000 / MaxSizeOfEachShortList) * sizeof(LinkListStruct*));

	if (SizeOfShortListFTouple1InANF == NULL)
		SizeOfShortListFTouple1InANF = (unsigned short*)malloc(ceil(70000 / MaxSizeOfEachShortList) * sizeof(unsigned short)); // should be enough, it is at most 256 * 256
	memset(SizeOfShortListFTouple1InANF, 0, ceil(70000 / MaxSizeOfEachShortList) * sizeof(unsigned short));

	NumberOfShortListFTouple1InANF = 2;
	SizeOfShortListFTouple1InANF[0] = 2;
	SizeOfShortListFTouple1InANF[1] = 0;

	ShortListFTouple1InANF[0] = (LinkListStruct*)malloc(sizeof(LinkListStruct));
	ShortListFTouple1InANF[1] = (LinkListStruct*)malloc(sizeof(LinkListStruct));

	ShortListFTouple1InANF[0]->Before = NULL;
	ShortListFTouple1InANF[0]->ANF = 0;
	ShortListFTouple1InANF[0]->NumberOfTouples = 0;
	ShortListFTouple1InANF[0]->TouplesIndex = NULL;
	ShortListFTouple1InANF[0]->Next = ShortListFTouple1InANF[1];

	ShortListFTouple1InANF[1]->Before = ShortListFTouple1InANF[0];
	ShortListFTouple1InANF[1]->ANF = -1;
	ShortListFTouple1InANF[1]->NumberOfTouples = 0;
	ShortListFTouple1InANF[1]->TouplesIndex = NULL;
	ShortListFTouple1InANF[1]->Next = NULL;

	Counter = 0;
	NumberOfFTuples1 = 0;
	#pragma omp parallel for schedule(guided) private(i, ThreadNum, index, MadeANF, UniformityCounter, DistributionOld, Distribution, Masked_InputIndex, Masked_Output, Unmasked_Output, j)
	for (i_last = 0; i_last < NumberOfF8to1[1]; i_last++)
	{
		ThreadNum = omp_get_thread_num();
		i[0] = 0;
		i[1] = i_last;

		while (i[1] == i_last)
		{
			UniformityCounter[0] = 0;
			UniformityCounter[1] = 0;
			memset(Distribution, 0, 4 * sizeof(unsigned short));

			for (Masked_InputIndex = 0; Masked_InputIndex < (1 << 16); Masked_InputIndex++)
			{
				Masked_Output = 0;
				for (index = 0; index < 2; index++)
					Masked_Output |= F8to1[index][i[index]][InputTableIndex[index][Masked_InputIndex]];

				Unmasked_Output = Unmasking[Masked_Output];
				MadeTable[ThreadNum][OutputTableIndex[Masked_InputIndex]] = Unmasked_Output;
				UniformityCounter[Unmasked_Output]++;

				Distribution[Masked_Output]++;

				if ((Masked_InputIndex & 0xFF) == 0xFF) // every 256
				{
					if (UniformityCounter[0] != UniformityCounter[1])
						break;

					UniformityCounter[0] = 0;
					UniformityCounter[1] = 0;

					//---------

					if (Masked_InputIndex == 0xFF) // the first one
						memcpy(DistributionOld, Distribution, 4 * sizeof(unsigned short));
					else
					{
						for (j = 0; j < 4; j++)
							if (Distribution[j] != DistributionOld[j])
								break;

						if (j < 4)
							break;
					}

					memset(Distribution, 0, 4 * sizeof(unsigned short));
				}
			}

			if (Masked_InputIndex == (1 << 16))
			{
				#pragma omp critical (found1)
				{
					FTuples1[NumberOfFTuples1].i[0] = i[0];
					FTuples1[NumberOfFTuples1].i[1] = i[1];
					MadeANF = MakeANFSpecial(MadeTable[ThreadNum], 16);
					FTuples1[NumberOfFTuples1].ANF = MadeANF;

					AddToLinkList(ShortListFTouple1InANF, SizeOfShortListFTouple1InANF, NumberOfShortListFTouple1InANF,
						MadeANF, NumberOfFTuples1);

					NumberOfFTuples1++;
				}
			}

			j = 0;
			do
			{
				if ((++i[j] >= NumberOfF8to1[j]) & (j < 2))
					i[j] = 0;
			} while (i[j++] == 0);
		}

		#pragma omp critical (print)
		{
			Counter++;
			if ((Counter & 0xF) == 0xF)
				printf("%d%d%d%d%d%d%d%d Touple1 %d / %d %d\n",
					Types[0], Types[1], Types[2], Types[3],
					Types[4], Types[5], Types[6], Types[7],
					Counter, NumberOfF8to1[1], NumberOfFTuples1);
		}
	}

	//-------------------------------

	if (!NumberOfFTuples1)
		goto terminate;

	if (FTuples2 == NULL)
		FTuples2 = (ToupleStruct*)malloc(70000 * sizeof(ToupleStruct)); // should be enough, it is at most 256 * 256

	if (ShortListFTouple2InANF == NULL)
		ShortListFTouple2InANF = (LinkListStruct**)malloc(ceil(70000 /MaxSizeOfEachShortList) * sizeof(LinkListStruct*)); // should be enough, it is at most 256 * 256
	memset(ShortListFTouple2InANF, 0, ceil(70000 / MaxSizeOfEachShortList) * sizeof(LinkListStruct*));

	if (SizeOfShortListFTouple2InANF == NULL)
		SizeOfShortListFTouple2InANF = (unsigned short*)malloc(ceil(70000 / MaxSizeOfEachShortList) * sizeof(unsigned short)); // should be enough, it is at most 256 * 256
	memset(SizeOfShortListFTouple2InANF, 0, ceil(70000 / MaxSizeOfEachShortList) * sizeof(unsigned short));

	NumberOfShortListFTouple2InANF = 2;
	SizeOfShortListFTouple2InANF[0] = 2;
	SizeOfShortListFTouple2InANF[1] = 0;

	ShortListFTouple2InANF[0] = (LinkListStruct*)malloc(sizeof(LinkListStruct));
	ShortListFTouple2InANF[1] = (LinkListStruct*)malloc(sizeof(LinkListStruct));

	ShortListFTouple2InANF[0]->Before = NULL;
	ShortListFTouple2InANF[0]->ANF = 0;
	ShortListFTouple2InANF[0]->NumberOfTouples = 0;
	ShortListFTouple2InANF[0]->TouplesIndex = NULL;
	ShortListFTouple2InANF[0]->Next = ShortListFTouple2InANF[1];

	ShortListFTouple2InANF[1]->Before = ShortListFTouple2InANF[0];
	ShortListFTouple2InANF[1]->ANF = -1;
	ShortListFTouple2InANF[1]->NumberOfTouples = 0;
	ShortListFTouple2InANF[1]->TouplesIndex = NULL;
	ShortListFTouple2InANF[1]->Next = NULL;

	NumberOfFTuples2 = 0;
	Counter = 0;
	#pragma omp parallel for schedule(guided) private(i, ThreadNum, index, MadeANF, UniformityCounter, DistributionOld, Distribution, Masked_InputIndex, Masked_Output, Unmasked_Output, j)
	for (i_last = 0; i_last < NumberOfF8to1[3]; i_last++)
	{
		ThreadNum = omp_get_thread_num();
		i[0] = 0;
		i[1] = i_last;

		while (i[1] == i_last)
		{
			UniformityCounter[0] = 0;
			UniformityCounter[1] = 0;
			memset(Distribution, 0, 4 * sizeof(unsigned short));

			for (Masked_InputIndex = 0; Masked_InputIndex < (1 << 16); Masked_InputIndex++)
			{
				Masked_Output = 0;
				for (index = 0; index < 2; index++)
					Masked_Output |= F8to1[index + 2][i[index]][InputTableIndex[index + 2][Masked_InputIndex]];

				Unmasked_Output = Unmasking[Masked_Output];
				MadeTable[ThreadNum][OutputTableIndex[Masked_InputIndex]] = Unmasked_Output;
				UniformityCounter[Unmasked_Output]++;

				Distribution[Masked_Output]++;

				if ((Masked_InputIndex & 0xFF) == 0xFF) // every 256
				{
					if (UniformityCounter[0] != UniformityCounter[1])
						break;

					UniformityCounter[0] = 0;
					UniformityCounter[1] = 0;

					//---------

					if (Masked_InputIndex == 0xFF) // the first one
						memcpy(DistributionOld, Distribution, 4 * sizeof(unsigned short));
					else
					{
						for (j = 0; j < 4; j++)
							if (Distribution[j] != DistributionOld[j])
								break;

						if (j < 4)
							break;
					}

					memset(Distribution, 0, 4 * sizeof(unsigned short));
				}
			}

			if (Masked_InputIndex == (1 << 16))
			{
				#pragma omp critical (found2)
				{
					FTuples2[NumberOfFTuples2].i[0] = i[0];
					FTuples2[NumberOfFTuples2].i[1] = i[1];
					MadeANF = MakeANFSpecial(MadeTable[ThreadNum], 16);
					FTuples2[NumberOfFTuples2].ANF = MadeANF;

					AddToLinkList(ShortListFTouple2InANF, SizeOfShortListFTouple2InANF, NumberOfShortListFTouple2InANF,
						MadeANF, NumberOfFTuples2);

					NumberOfFTuples2++;
				}
			}

			j = 0;
			do
			{
				if ((++i[j] >= NumberOfF8to1[j + 2]) & (j < 2))
					i[j] = 0;
			} while (i[j++] == 0);
		}

		#pragma omp critical (print)
		{
			Counter++;
			if ((Counter & 0xF) == 0xF)
				printf("%d%d%d%d%d%d%d%d Touple2 %d / %d %d\n",
					Types[0], Types[1], Types[2], Types[3],
					Types[4], Types[5], Types[6], Types[7],
					Counter, NumberOfF8to1[3], NumberOfFTuples2);
		}
	}

	terminate:
	for (j = 0;j < omp_get_max_threads();j++)
		free(MadeTable[j]);
	free(MadeTable);
}

void FindCombination(unsigned char** F8to1[4], unsigned char* TargetFunc, unsigned char Types[8], char* FileName, unsigned short MaxNumberOfSolutionsPerANF,
	unsigned short** InputTableIndex, unsigned short* OutputTableIndex, unsigned char* Unmasking, unsigned short* UnmaskedInputTable,
	char InputStr[4][8][5],	ToupleStruct* FTuples1, ToupleStruct* FTuples2,
	unsigned int NumberOfFTuples1, unsigned int NumberOfFTuples2,
	LinkListStruct** ShortListFTouple1InANF, unsigned int NumberOfShortListFTouple1InANF,
	LinkListStruct** ShortListFTouple2InANF, unsigned int NumberOfShortListFTouple2InANF,
	unsigned char Invert)
{
	unsigned short	abcdefgh;
	unsigned short	abcdefgh1;
	unsigned char*	MaskedTargetFunc;
	unsigned int	Masked_InputIndex;
	uint64_t		ANFTargetMaskedFuc;
	int				index1;
	int				index2;
	int				i1;
	int				i2;
	uint64_t		ANFRest;

	unsigned char	Masked_Output;
	unsigned char	Masked_Output1;
	unsigned char	Masked_Output2;
	unsigned short	Unmasked_Input;
	unsigned char	Unmasked_Output;
	unsigned short	UniformityCounter[4];
	unsigned char*	MadeTable1;
	unsigned char*	MadeTable2;
	unsigned int	NumberOfFound;
	unsigned int	j;
	char			k;
	char			TempStr[500];
	char			index;
	unsigned short	NumberOfSolutionsPerANF;
	unsigned short	NumberOfChecked;
	FILE*			F;
	FunctionStruct	ANFFunc;
	LinkListResStruct	Res;
	LinkListStruct*		Address;
	LinkListStruct*		TempAddress;

	MaskedTargetFunc = (unsigned char*)malloc((1 << 16) * sizeof(unsigned char));
	MadeTable1 = (unsigned char*)malloc((1 << 16) * sizeof(unsigned char));
	MadeTable2 = (unsigned char*)malloc((1 << 16) * sizeof(unsigned char));

	Masked_InputIndex = 0;
	for (abcdefgh = 0; abcdefgh < 256; abcdefgh++)
		for (abcdefgh1 = 0; abcdefgh1 < 256; abcdefgh1++)
		{
			MaskedTargetFunc[OutputTableIndex[Masked_InputIndex]] = TargetFunc[abcdefgh];
			Masked_InputIndex++;
		}

	ANFTargetMaskedFuc = MakeANFSpecial(MaskedTargetFunc, 16);
	DefineFunction(&ANFFunc, 500);

	NumberOfChecked = 0;
	NumberOfFound = 0;
	if (NumberOfFTuples1)
	{
		Address = ShortListFTouple1InANF[0];
		while (Address != ShortListFTouple1InANF[NumberOfShortListFTouple1InANF - 1])
		{
			NumberOfChecked++;
			if ((NumberOfChecked & 0xff) == 0xff)
				printf("Checked %d\n", NumberOfChecked);

			ANFRest = ANFTargetMaskedFuc ^ Address->ANF;
			Res = FindInLinkList(ShortListFTouple2InANF, NumberOfShortListFTouple2InANF, ANFRest);

			if (Res.Found)
			{
				NumberOfSolutionsPerANF = 0;

				for (index1 = 0; index1 < Address->NumberOfTouples; index1++)
				{
					i1 = Address->TouplesIndex[index1];
					for (index2 = 0; index2 < Res.Address->NumberOfTouples; index2++)
					{
						i2 = Res.Address->TouplesIndex[index2];
						memset(UniformityCounter,   0, 4 * sizeof(unsigned short));

						for (Masked_InputIndex = 0; Masked_InputIndex < (1 << 16); Masked_InputIndex++)
						{
							Masked_Output1 = 0;
							Masked_Output2 = 0;
							for (index = 0; index < 2; index++)
							{
								Masked_Output1 |= F8to1[index + 0][FTuples1[i1].i[index]][InputTableIndex[index + 0][Masked_InputIndex]];
								Masked_Output2 |= F8to1[index + 2][FTuples2[i2].i[index]][InputTableIndex[index + 2][Masked_InputIndex]];
							}

							MadeTable1[Masked_InputIndex] = Masked_Output1;
							MadeTable2[Masked_InputIndex] = Masked_Output2;

							Masked_Output = (Unmasking[Masked_Output2] << 1) | Unmasking[Masked_Output1];
							Unmasked_Output = Unmasking[Masked_Output];
							Unmasked_Input = UnmaskedInputTable[Masked_InputIndex];

							if (TargetFunc[Unmasked_Input] != Unmasked_Output)
							{
								printf("error\n");
								_getch();
								break;
							}

							if (++UniformityCounter[Masked_Output] > 128)
								break;

							if ((Masked_InputIndex & 0xFF) == 0xFF) // every 256
								memset(UniformityCounter,   0, 4 * sizeof(unsigned short));
						}

						if (Masked_InputIndex == (1 << 16))
						{
							NumberOfFound++;
							if ((NumberOfFound & 0xFF) == 0xFF)
								printf("%d%d%d%d%d%d%d%d found %d\n",
									Types[0], Types[1], Types[2], Types[3],
									Types[4], Types[5], Types[6], Types[7], NumberOfFound);

							F = fopen(FileName, "at");
							fprintf(F, "%d%d%d%d%d%d%d%d, ",
								Types[0], Types[1], Types[2], Types[3],
								Types[4], Types[5], Types[6], Types[7]);

							fprintf(F, "%" PRIu64 ", ", Address->ANF);
							for (index = 0; index < 2; index++)
							{
								MakeANF(F8to1[index + 0][FTuples1[i1].i[index]], 256, &ANFFunc, index, index ? 0 : Invert);
								SPrintFunction(ANFFunc, TempStr, InputStr[index]);
								fprintf(F, "%d, %s, ", ANFFunc.NumberOfTerms, TempStr);
							}

							for (j = 0; j < (1 << 16); j++)
								fprintf(F, "%x", MadeTable1[j]);
							fprintf(F, ", ");

							fprintf(F, "%" PRIu64 ", ", Res.Address->ANF);
							for (index = 0; index < 2; index++)
							{
								MakeANF(F8to1[index + 2][FTuples2[i2].i[index]], 256, &ANFFunc, index, 0);
								SPrintFunction(ANFFunc, TempStr, InputStr[2 + index]);
								fprintf(F, "%d, %s, ", ANFFunc.NumberOfTerms, TempStr);
							}

							for (j = 0; j < (1 << 16); j++)
								fprintf(F, "%x", MadeTable2[j]);
							fprintf(F, ",\n");
							fclose(F);

							//----------------------------

							NumberOfSolutionsPerANF++;
							if (MaxNumberOfSolutionsPerANF && (NumberOfSolutionsPerANF >= MaxNumberOfSolutionsPerANF))
							{
								index1 = Address->NumberOfTouples;
								index2 = Res.Address->NumberOfTouples;
							}
						}
						else
						{
							index1 = Address->NumberOfTouples;
							index2 = Res.Address->NumberOfTouples;
						}
					}
				}
			}

			Address = Address->Next;
		}
	}

	printf("%d%d%d%d%d%d%d%d found %d\n",
		Types[0], Types[1], Types[2], Types[3],
		Types[4], Types[5], Types[6], Types[7], NumberOfFound);

	//-----------------------------

	FreeFunction(&ANFFunc);
	free(MaskedTargetFunc);
	free(MadeTable1);
	free(MadeTable2);

	Address = ShortListFTouple1InANF[0];
	while (Address)
	{
		TempAddress = Address;
		Address = TempAddress->Next;
		free(TempAddress->TouplesIndex);
		free(TempAddress);
	}

	if (NumberOfFTuples1)
	{
		Address = ShortListFTouple2InANF[0];
		while (Address)
		{
			TempAddress = Address;
			Address = TempAddress->Next;
			free(TempAddress->TouplesIndex);
			free(TempAddress);
		}
	}
}

int main()
{
	unsigned short**	InputTableIndex = NULL;
	unsigned short*		OutputTableIndex = NULL;
	unsigned char*		Unmasking = NULL;
	unsigned short*		UnmaskedInputTable = NULL;

	unsigned char**		F8to1[4] = { NULL };
	int					NumberOfF8to1[4] = { 0 };
	ToupleStruct*		FTouple1 = NULL;
	ToupleStruct*		FTouple2 = NULL;
	unsigned int		NumberOfFTouples1;
	unsigned int		NumberOfFTouples2;
	LinkListStruct**	ShortListFTouple1InANF = NULL;
	unsigned short*		SizeOfShortListFTouple1InANF = NULL;
	unsigned int		NumberOfShortListFTouple1InANF;
	LinkListStruct**	ShortListFTouple2InANF = NULL;
	unsigned short*		SizeOfShortListFTouple2InANF = NULL;
	unsigned int		NumberOfShortListFTouple2InANF;
	char				InputStr[4][8][5];
	FILE*				F;

	unsigned char		a, b, c, d;
	unsigned short		abcdefgh;
	unsigned char		TargetFunc[256];
	FunctionStruct		ANFTargetFunc;
	unsigned char		Invert;
	unsigned char		Types[8];
	unsigned char		in[8];
	unsigned short		i, j, k, l, m;
	char				FunctionName[100];
	char				MainFileName[100];
	char				FileName[100];
	unsigned char		OutputBit;
	__int64				TempTerm;
	unsigned char		TermIndexes[10];
	unsigned char		NumberOfTermIndexes;

	// GF16_mul
	unsigned char Func[256] = { 0, 10, 15,  5,  6, 12,  9,  3, 11,  1,  4, 14, 13,  7,  2,  8,  0, 15,  5, 10, 11,  4, 14,  1, 13,  2,  8,  7,  6,  9,  3, 12,  0,  5, 10, 15, 13,  8,  7,  2,  6,  3, 12,  9, 11, 14,  1,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,  7,  9, 15,  1,  8,  6,  5, 11,  2, 12, 10,  4, 13,  3,  0, 11, 13,  6,  2,  9, 15,  4,  3,  8, 14,  5,  1, 10, 12,  7,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,  0,  4,  8, 12,  9, 13,  1,  5, 14, 10,  6,  2,  7,  3, 15, 11,  0,  9, 14,  7, 10,  3,  4, 13, 15,  6,  1,  8,  5, 12, 11,  2,  0, 12,  4,  8,  7, 11,  3, 15,  9,  5, 13,  1, 14,  2, 10,  6,  0,  6, 11, 13,  1,  7, 10, 12,  2,  4,  9, 15,  3,  5,  8, 14,  0,  3,  1,  2, 12, 15, 13, 14,  4,  7,  5,  6,  8, 11,  9, 10,  0, 13,  6, 11,  3, 14,  5,  8,  1, 12,  7, 10,  2, 15,  4,  9,  0,  8, 12,  4, 14,  6,  2, 10,  7, 15, 11,  3,  9,  1,  5, 13,  0,  2,  3,  1,  8, 10, 11,  9, 12, 14, 15, 13,  4,  6,  7,  5,  0,  7,  9, 14,  5,  2, 12, 11, 10, 13,  3,  4, 15,  8,  6,  1 };
	strcpy(FunctionName, "GF16_mul");

	const char		NumberOfCPUs = 22;
	const char		MultipleTypes = 0;
	const short		MaxNumberOfSolutionsPerANF = 1;

	//--------------------------------------------------------------

	FillANFTables(1 << 16);
	FillANFTermTables();
	DefineFunction(&ANFTargetFunc, 500);

	omp_set_num_threads(NumberOfCPUs);

	for (OutputBit = 0; OutputBit <= 3; OutputBit++)
	{
		for (abcdefgh = 0; abcdefgh < 256; abcdefgh++)
		{
			TargetFunc[abcdefgh] = (Func[abcdefgh] >> OutputBit) & 1;
		}

		Invert = TargetFunc[0];
		for (abcdefgh = 0; abcdefgh < 256; abcdefgh++)
			TargetFunc[abcdefgh] ^= Invert;

		FillF8to1(F8to1, NumberOfF8to1, TargetFunc);
		MakeANF(TargetFunc, 256, &ANFTargetFunc, 1, 0);

		//---------------------------------------

		sprintf(MainFileName, "%s_y%d", FunctionName, OutputBit);
		sprintf(FileName, "%s.csv", MainFileName);
		F = fopen(FileName, "wt");
		fclose(F);

		for (Types[0] = 0; Types[0] <= (MultipleTypes ? 2 : 0); Types[0]++)
			for (Types[1] = 0; Types[1] <= (MultipleTypes ? 2 : 0); Types[1]++)
				for (Types[2] = 0; Types[2] <= (MultipleTypes ? 2 : 0); Types[2]++)
					for (Types[3] = 0; Types[3] <= (MultipleTypes ? 2 : 0); Types[3]++)
						for (Types[4] = (MultipleTypes ? 0 : 1); Types[4] <= (MultipleTypes ? 2 : 1); Types[4]++)
							for (Types[5] = (MultipleTypes ? 0 : 1); Types[5] <= (MultipleTypes ? 2 : 1); Types[5]++)
								for (Types[6] = (MultipleTypes ? 0 : 1); Types[6] <= (MultipleTypes ? 2 : 1); Types[6]++)
									for (Types[7] = (MultipleTypes ? 0 : 1); Types[7] <= (MultipleTypes ? 2 : 1); Types[7]++)
									{
										for (i = 0;i < ANFTargetFunc.NumberOfTerms;i++)
										{
											TempTerm = ANFTargetFunc.Term[i];
											NumberOfTermIndexes = 0;
											for (j = 0;TempTerm; TempTerm >>= 1)
											{
												if (TempTerm & 1)
													TermIndexes[NumberOfTermIndexes++] = j;
												j++;
											}

											for (j = 0;j < NumberOfTermIndexes - 1;j++)
											{
												for (k = j + 1;k < NumberOfTermIndexes;k++)
													if (Types[TermIndexes[j]] == Types[TermIndexes[k]])
														break;

												if (k < NumberOfTermIndexes)
													break;
											}

											if (j < NumberOfTermIndexes - 1)
												break;
										}

										if (i == ANFTargetFunc.NumberOfTerms)
										{
											FillTableIndexes(InputTableIndex, OutputTableIndex, Unmasking, UnmaskedInputTable,
												InputStr, Types);

											MakeLargeTables(F8to1, NumberOfF8to1, Types,
												InputTableIndex, OutputTableIndex, Unmasking, FTouple1, FTouple2,
												NumberOfFTouples1, ShortListFTouple1InANF, SizeOfShortListFTouple1InANF, NumberOfShortListFTouple1InANF,
												NumberOfFTouples2, ShortListFTouple2InANF, SizeOfShortListFTouple2InANF, NumberOfShortListFTouple2InANF);

											FindCombination(F8to1, TargetFunc, Types, FileName, MaxNumberOfSolutionsPerANF,
												InputTableIndex, OutputTableIndex, Unmasking, UnmaskedInputTable, InputStr,
												FTouple1, FTouple2, NumberOfFTouples1, NumberOfFTouples2,
												ShortListFTouple1InANF, NumberOfShortListFTouple1InANF,
												ShortListFTouple2InANF, NumberOfShortListFTouple2InANF, Invert);
										}
									}
	}

	printf("done");
	_getch();

	return 0;
}
