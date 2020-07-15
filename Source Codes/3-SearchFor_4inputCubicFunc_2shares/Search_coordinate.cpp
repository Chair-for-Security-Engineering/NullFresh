
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

void MakeANF(unsigned char* Table, unsigned int NumberOfElements, FunctionStruct* Functions, unsigned char NumberOfOutputBits, unsigned char Invert)
{
	unsigned char   NumberOfBits = log((double)NumberOfElements) / log((double)2);
	unsigned char	Bit, HWIndex;
	unsigned int	i;

	for (Bit = 0; Bit < NumberOfOutputBits; Bit++)
	{
		Functions[Bit].NumberOfTerms = 0;

		for (HWIndex = 0; HWIndex < (NumberOfBits + 1); HWIndex++)
			for (i = 0; i < NumberOfIndexesInHW[HWIndex]; i++)
				if (IndexesInHW[HWIndex][i] < NumberOfElements)
					if (EvalFunction(&Functions[Bit], IndexesInHW[HWIndex][i]) != (((Table[IndexesInHW[HWIndex][i]] ^ Invert) >> Bit) & 1))
						AddTermtoFunction(&Functions[Bit], IndexesInHW[HWIndex][i]);
	}
}

unsigned char* Term2Index;
unsigned char* Index2Term;

void FillANFTermTables()
{
	unsigned short i;
	unsigned char  c;

	Term2Index = (unsigned char*)malloc(256 * sizeof(unsigned char));
	Index2Term = (unsigned char*)malloc(256 * sizeof(unsigned char));

	// input order: (MSB) a1, a2, b1, b2, c1, c2, d1, d2  (LBS)

	c = 0;
	for (i = 1; i < 128; i <<= 1)  // linear terms without a1
	{
		Term2Index[i] = c;
		Index2Term[c] = i;
		c++;
	}

	for (i = 3; i < 128; i++)  // quadretic terms without a1
	{
		if (HW[i] == 2)
		{
			Term2Index[i] = c;
			Index2Term[c] = i;
			c++;
		}
	}

	for (i = 128; i < 256; i++)  // linear and quadretic terms with a1
	{
		if (HW[i] <= 2)
		{
			Term2Index[i] = c;
			Index2Term[c] = i;
			c++;
		}
	}
}

unsigned char EvalFunctionSpecial(uint64_t Func, unsigned char Input)
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

	for (HWIndex = 1; HWIndex <= 2; HWIndex++)
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

int FillF4to1(unsigned char** F4to1[4], FunctionStruct ANFDesiredTerms)
{
	int				NumberOfF4to1;
	unsigned int	i, j, t;
	char			index;
	FunctionStruct	ANFFunc;
	unsigned char	DesiredDegree;

	DesiredDegree = AlgDegree(ANFDesiredTerms);
	DefineFunction(&ANFFunc, 500);

	for (index = 0; index < 4; index++)
		if (F4to1[index] == NULL)
			F4to1[index] = (unsigned char**)calloc((1 << 16), sizeof(unsigned char*)); // to fill every cell with NULL

	NumberOfF4to1 = 0;
	for (index = 0; index < 4; index++)
		if (F4to1[index][NumberOfF4to1] == NULL)
			F4to1[index][NumberOfF4to1] = (unsigned char*)malloc(16 * sizeof(unsigned char));

	if (DesiredDegree != 3) // adding 0 function
	{
		for (j = 0; j < 16; j++)
			for (index = 0; index < 4; index++)
				F4to1[index][NumberOfF4to1][j] = 0;

		NumberOfF4to1++;
		for (index = 0; index < 4; index++)
			if (F4to1[index][NumberOfF4to1] == NULL)
				F4to1[index][NumberOfF4to1] = (unsigned char*)malloc(16 * sizeof(unsigned char));
	}

	for (i = 2; i < (1 << 16); i += 2)
	{
		t = i;
		for (j = 0; j < 16; j++)
		{
			for (index = 0; index < 4; index++)
				F4to1[index][NumberOfF4to1][j] = (t & 1) << index;
			t >>= 1;
		}

		MakeANF(F4to1[0][NumberOfF4to1], 16, &ANFFunc, 1, 0);

		if (AlgDegree(ANFFunc) == DesiredDegree)
		{
			for (j = 0; j < ANFDesiredTerms.NumberOfTerms; j++)
			{
				if (HW[ANFDesiredTerms.Term[j]] == DesiredDegree)
				{
					for (t = 0; t < ANFFunc.NumberOfTerms; t++)
						if (ANFDesiredTerms.Term[j] == ANFFunc.Term[t])
							break;

					if (t == ANFFunc.NumberOfTerms)
						break;
				}
			}

			if (j == ANFDesiredTerms.NumberOfTerms)  // ANFFunc contains all terms with DesiredDegree in ANFDesiredTerms
			{
				for (j = 0; j < ANFFunc.NumberOfTerms; j++)
				{
					if (HW[ANFFunc.Term[j]] == DesiredDegree)
					{
						for (t = 0; t < ANFDesiredTerms.NumberOfTerms; t++)
							if (ANFFunc.Term[j] == ANFDesiredTerms.Term[t])
								break;

						if (t == ANFDesiredTerms.NumberOfTerms)
							break;
					}
				}

				if (j == ANFFunc.NumberOfTerms) // ANFFunc does not contains any term with DesiredDegree which is not in ANFDesiredTerms
				{
					NumberOfF4to1++;
					for (index = 0; index < 4; index++)
						if (F4to1[index][NumberOfF4to1] == NULL)
							F4to1[index][NumberOfF4to1] = (unsigned char*)malloc(16 * sizeof(unsigned char));
				}
			}
		}
	}

	FreeFunction(&ANFFunc);
	return(NumberOfF4to1);
}

struct ToupleStruct
{
	unsigned short	i[4];
	uint64_t		ANF;
};

void FillTableIndexes(unsigned char** &InputTableIndex, unsigned char* &OutputTableIndex, unsigned char* &Unmasking, unsigned char* &UnmaskedInputTable,
	unsigned char Types[4])
{
	char			index;
	unsigned char   a, a1, a2;
	unsigned char	b, b1, b2;
	unsigned char	c, c1, c2;
	unsigned char	d, d1, d2;
	unsigned char	abcd;
	unsigned char	a1b1c1d1;
	unsigned char	a2b2c2d2;
	unsigned char	Masked_InputIndex;
	unsigned char	OutIndex;
	unsigned short	j, k;
	unsigned char	in[4][2];

	if (InputTableIndex == NULL)
		InputTableIndex = (unsigned char**)calloc(8, sizeof(unsigned char*));
	for (index = 0; index < 8; index++)
		if (InputTableIndex[index] == NULL)
			InputTableIndex[index] = (unsigned char*)malloc(256 * sizeof(unsigned char));

	if (OutputTableIndex == NULL)
		OutputTableIndex = (unsigned char*)malloc(256 * sizeof(unsigned char));

	if (UnmaskedInputTable == NULL)
		UnmaskedInputTable = (unsigned char*)malloc(256 * sizeof(unsigned char));

	Masked_InputIndex = 0;
	for (abcd = 0; abcd < 16; abcd++)
	{
		a = (abcd & 1) ? 1 : 0;
		b = (abcd & 2) ? 1 : 0;
		c = (abcd & 4) ? 1 : 0;
		d = (abcd & 8) ? 1 : 0;

		for (a1b1c1d1 = 0; a1b1c1d1 < 16; a1b1c1d1++)
		{
			a2b2c2d2 = abcd ^ a1b1c1d1;

			a1 = (a1b1c1d1 & 1) ? 1 : 0;
			b1 = (a1b1c1d1 & 2) ? 1 : 0;
			c1 = (a1b1c1d1 & 4) ? 1 : 0;
			d1 = (a1b1c1d1 & 8) ? 1 : 0;

			a2 = (a2b2c2d2 & 1) ? 1 : 0;
			b2 = (a2b2c2d2 & 2) ? 1 : 0;
			c2 = (a2b2c2d2 & 4) ? 1 : 0;
			d2 = (a2b2c2d2 & 8) ? 1 : 0;

			in[0][0] = a1;
			in[0][1] = a2;
			in[1][0] = b1;
			in[1][1] = b2;
			in[2][0] = c1;
			in[2][1] = c2;
			in[3][0] = d1;
			in[3][1] = d2;

			for (j = 0;j < 8;j++)
				InputTableIndex[j][Masked_InputIndex] = 0;

			for (j = 0;j < 4;j++)
			{
				if (Types[j] == 0)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[4][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[5][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[6][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[7][Masked_InputIndex] |= in[j][1] << j;
				}
				else if (Types[j] == 1)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[4][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[5][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[6][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[7][Masked_InputIndex] |= in[j][1] << j;
				}
				else if (Types[j] == 2)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[4][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[5][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[6][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[7][Masked_InputIndex] |= in[j][1] << j;
				}
				else if (Types[j] == 3)
				{
					InputTableIndex[0][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[1][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[2][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[3][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[4][Masked_InputIndex] |= in[j][0] << j;
					InputTableIndex[5][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[6][Masked_InputIndex] |= in[j][1] << j;
					InputTableIndex[7][Masked_InputIndex] |= in[j][0] << j;
				}
			}

			OutIndex = (a1 << 7) | (a2 << 6) | (b1 << 5) | (b2 << 4) | (c1 << 3) | (c2 << 2) | (d1 << 1) | (d2 << 0);
			OutputTableIndex[Masked_InputIndex] = OutIndex;

			UnmaskedInputTable[Masked_InputIndex] = (d << 3) | (c << 2) | (b << 1) | (a << 0);
			Masked_InputIndex++;
		}
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

void MakeLargeTables(unsigned char** F4to1[4], int NumberOfF4to1, unsigned char* Types,
	unsigned char** InputTableIndex, unsigned char* OutputTableIndex, unsigned char* Unmasking,
	ToupleStruct* &FTuples1, ToupleStruct* &FTuples2,
	unsigned int &NumberOfFTuples1, LinkListStruct** &ShortListFTouple1InANF, unsigned short* &SizeOfShortListFTouple1InANF, unsigned int &NumberOfShortListFTouple1InANF,
	unsigned int &NumberOfFTuples2,	LinkListStruct** &ShortListFTouple2InANF, unsigned short* &SizeOfShortListFTouple2InANF, unsigned int &NumberOfShortListFTouple2InANF)
{
	char			index;
	char			index2;
	unsigned short	Masked_InputIndex;
	short 			i0;
	short 			i1;
	short 			i_last;
	unsigned short	i[5] = { 0 };
	unsigned char	Masked_Output;
	unsigned char	Unmasked_Output;
	unsigned char	UniformityCounter[2];
	unsigned char	Distribution2Old[4];
	unsigned char	Distribution3Old[8];
	unsigned char	Distribution4Old[16];
	unsigned char	Distribution2[4];
	unsigned char	Distribution3[8];
	unsigned char	Distribution4[16];
	unsigned char**	CouplesPossible[3][4] = { NULL };
	unsigned char	Uniform;
	unsigned char	MadeTable[256];
	uint64_t		MadeANF;
	unsigned short  j;
	char			k;
	unsigned int*	TempBuff;
	unsigned short  Counter;

	for (index = 0;index < 3;index++)
		for (index2 = index + 1;index2 < 4;index2++)
		{
			if (CouplesPossible[index][index2] == NULL)
				CouplesPossible[index][index2] = (unsigned char**)calloc(NumberOfF4to1, sizeof(unsigned char*));

			#pragma omp parallel for schedule(guided) private(i1, Distribution2Old, Distribution2, Masked_InputIndex, Masked_Output, j)
			for (i0 = 0;i0 < NumberOfF4to1;i0++)
			{
				if (CouplesPossible[index][index2][i0] == NULL)
					CouplesPossible[index][index2][i0] = (unsigned char*)malloc(NumberOfF4to1 * sizeof(unsigned char));

				for (i1 = 0;i1 < NumberOfF4to1;i1++)
				{
					memset(Distribution2, 0, 4);

					for (Masked_InputIndex = 0; Masked_InputIndex < 256; Masked_InputIndex++)
					{
						Masked_Output  = F4to1[0][i0][InputTableIndex[index][Masked_InputIndex]];
						Masked_Output |= F4to1[1][i1][InputTableIndex[index2][Masked_InputIndex]];
						Distribution2[Masked_Output]++;

						if ((Masked_InputIndex & 0xF) == 0xF) // every 16
						{
							if (Masked_InputIndex == 0xF) // the first one
								memcpy(Distribution2Old, Distribution2, 4);
							else
							{
								for (j = 0; j < 4; j++)
									if (Distribution2[j] != Distribution2Old[j])
										break;

								if (j < 4)
									break;
							}

							memset(Distribution2, 0, 4);
						}
					}

					CouplesPossible[index][index2][i0][i1] = (Masked_InputIndex == 256);
				}
			}
		}

	//------------------------------------------------------------

	if (FTuples1 == NULL)
		FTuples1 = (ToupleStruct*)malloc(500000000 * sizeof(ToupleStruct)); // hopefully enough, I have seen at most 5,165,056

	if (ShortListFTouple1InANF == NULL)
		ShortListFTouple1InANF = (LinkListStruct**)malloc(ceil(500000000 / MaxSizeOfEachShortList) * sizeof(LinkListStruct*)); // hopefully enough, I have seen at most 5,165,056
	memset(ShortListFTouple1InANF, 0, ceil(500000000 / MaxSizeOfEachShortList) * sizeof(LinkListStruct*));

	if (SizeOfShortListFTouple1InANF == NULL)
		SizeOfShortListFTouple1InANF = (unsigned short*)malloc(ceil(500000000 / MaxSizeOfEachShortList) * sizeof(unsigned short)); // hopefully enough, I have seen at most 5,165,056
	memset(SizeOfShortListFTouple1InANF, 0, ceil(500000000 / MaxSizeOfEachShortList) * sizeof(unsigned short));

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
	#pragma omp parallel for schedule(guided) private(i, index, index2, MadeTable, MadeANF, UniformityCounter, Distribution3Old, Distribution4Old, Distribution3, Distribution4, Masked_InputIndex, Masked_Output, Unmasked_Output, Uniform, j)
	for (i_last = 0; i_last < NumberOfF4to1; i_last++)
	{
		i[0] = 0;
		i[1] = 0;
		i[2] = 0;
		i[3] = i_last;

		while (i[3] == i_last)
		{
			if (!CouplesPossible[2][3][i[2]][i[3]])
			{
				i[0] = NumberOfF4to1;
				i[1] = NumberOfF4to1;
			}
			else if ((!CouplesPossible[1][2][i[1]][i[2]]) || (!CouplesPossible[1][3][i[1]][i[3]]))
			{
				i[0] = NumberOfF4to1;
			}
			else if (CouplesPossible[0][1][i[0]][i[1]] && CouplesPossible[0][2][i[0]][i[2]] && CouplesPossible[0][3][i[0]][i[3]])
			{
				UniformityCounter[0] = 0;
				UniformityCounter[1] = 0;
				Uniform = 1;
				memset(Distribution3, 0, 8);
				memset(Distribution4, 0, 16);

				for (Masked_InputIndex = 0; Masked_InputIndex < 256; Masked_InputIndex++)
				{
					Masked_Output = 0;
					for (index = 0; index < 4; index++)
						Masked_Output |= F4to1[index][i[index]][InputTableIndex[index][Masked_InputIndex]];

					Unmasked_Output = Unmasking[Masked_Output];
					MadeTable[OutputTableIndex[Masked_InputIndex]] = Unmasked_Output;
					UniformityCounter[Unmasked_Output]++;

					Distribution3[Masked_Output >> 1]++;
					Distribution4[Masked_Output >> 0]++;

					if ((Masked_InputIndex & 0xF) == 0xF) // every 16
					{
						Uniform &= (UniformityCounter[0] == UniformityCounter[1]);

						UniformityCounter[0] = 0;
						UniformityCounter[1] = 0;

						//---------

						if (Masked_InputIndex == 0xF) // the first one
						{
							memcpy(Distribution3Old, Distribution3, 8);
							memcpy(Distribution4Old, Distribution4, 16);
						}
						else
						{
							for (j = 0; j < 8; j++)
								if (Distribution3[j] != Distribution3Old[j])
								{
									i[0] = NumberOfF4to1;
									break;
								}

							if (j < 8)
								break;

							for (j = 0; j < 16; j++)
								if (Distribution4[j] != Distribution4Old[j])
									break;

							if (j < 16)
								break;
						}

						memset(Distribution3, 0, 8);
						memset(Distribution4, 0, 16);
					}
				}

				if (Uniform & (Masked_InputIndex == 256))
				{
					#pragma omp critical (found1)
					{
						FTuples1[NumberOfFTuples1].i[0] = i[0];
						FTuples1[NumberOfFTuples1].i[1] = i[1];
						FTuples1[NumberOfFTuples1].i[2] = i[2];
						FTuples1[NumberOfFTuples1].i[3] = i[3];
						MadeANF = MakeANFSpecial(MadeTable, 8);
						FTuples1[NumberOfFTuples1].ANF = MadeANF;

						AddToLinkList(ShortListFTouple1InANF, SizeOfShortListFTouple1InANF, NumberOfShortListFTouple1InANF,
							MadeANF, NumberOfFTuples1);

						NumberOfFTuples1++;
					}
				}
			}

			j = 0;
			do
			{
				if ((++i[j] >= NumberOfF4to1) & (j < 4))
					i[j] = 0;
			} while (i[j++] == 0);
		}

		#pragma omp critical (print)
		{
			Counter++;
			if ((Counter & 0xFF) == 0xFF)
				printf("%d%d%d%d Touple1 %d / %d %d\n", Types[0], Types[1], Types[2], Types[3],
					Counter, NumberOfF4to1, NumberOfFTuples1);
		}
	}

	//-------------------------------

	if (!NumberOfFTuples1)
		return;

	if (FTuples2 == NULL)
		FTuples2 = (ToupleStruct*)malloc(500000000 * sizeof(ToupleStruct)); // hopefully enough, I have seen at most 5,165,056

	if (ShortListFTouple2InANF == NULL)
		ShortListFTouple2InANF = (LinkListStruct**)malloc(ceil(500000000/MaxSizeOfEachShortList) * sizeof(LinkListStruct*)); // hopefully enough, I have seen at most 5,165,056
	memset(ShortListFTouple2InANF, 0, ceil(500000000 / MaxSizeOfEachShortList) * sizeof(LinkListStruct*));

	if (SizeOfShortListFTouple2InANF == NULL)
		SizeOfShortListFTouple2InANF = (unsigned short*)malloc(ceil(500000000 / MaxSizeOfEachShortList) * sizeof(unsigned short)); // hopefully enough, I have seen at most 5,165,056
	memset(SizeOfShortListFTouple2InANF, 0, ceil(500000000 / MaxSizeOfEachShortList) * sizeof(unsigned short));

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

	for (index = 0;index < 3;index++)
		for (index2 = index + 1;index2 < 4;index2++)
		{
			if (CouplesPossible[index][index2] == NULL)
				CouplesPossible[index][index2] = (unsigned char**)calloc(NumberOfF4to1, sizeof(unsigned char*));

			#pragma omp parallel for schedule(guided) private(i1, Distribution2Old, Distribution2, Masked_InputIndex, Masked_Output, j)
			for (i0 = 0;i0 < NumberOfF4to1;i0++)
			{
				if (CouplesPossible[index][index2][i0] == NULL)
					CouplesPossible[index][index2][i0] = (unsigned char*)malloc(NumberOfF4to1 * sizeof(unsigned char));

				for (i1 = 0;i1 < NumberOfF4to1;i1++)
				{
					memset(Distribution2, 0, 4);

					for (Masked_InputIndex = 0; Masked_InputIndex < 256; Masked_InputIndex++)
					{
						Masked_Output = F4to1[0][i0][InputTableIndex[index + 4][Masked_InputIndex]];
						Masked_Output |= F4to1[1][i1][InputTableIndex[index2 + 4][Masked_InputIndex]];
						Distribution2[Masked_Output]++;

						if ((Masked_InputIndex & 0xF) == 0xF) // every 16
						{
							if (Masked_InputIndex == 0xF) // the first one
								memcpy(Distribution2Old, Distribution2, 4);
							else
							{
								for (j = 0; j < 4; j++)
									if (Distribution2[j] != Distribution2Old[j])
										break;

								if (j < 4)
									break;
							}

							memset(Distribution2, 0, 4);
						}
					}

					CouplesPossible[index][index2][i0][i1] = (Masked_InputIndex == 256);
				}
			}
		}

	NumberOfFTuples2 = 0;
	Counter = 0;
	#pragma omp parallel for schedule(guided) private(i, index, MadeTable, MadeANF, UniformityCounter, Distribution3Old, Distribution4Old, Distribution3, Distribution4, Masked_InputIndex, Masked_Output, Unmasked_Output, Uniform, j)
	for (i_last = 0; i_last < NumberOfF4to1; i_last++)
	{
		i[0] = 0;
		i[1] = 0;
		i[2] = 0;
		i[3] = i_last;

		while (i[3] == i_last)
		{
			if (!CouplesPossible[2][3][i[2]][i[3]])
			{
				i[0] = NumberOfF4to1;
				i[1] = NumberOfF4to1;
			}
			else if ((!CouplesPossible[1][2][i[1]][i[2]]) || (!CouplesPossible[1][3][i[1]][i[3]]))
			{
				i[0] = NumberOfF4to1;
			}
			else if (CouplesPossible[0][1][i[0]][i[1]] && CouplesPossible[0][2][i[0]][i[2]] && CouplesPossible[0][3][i[0]][i[3]])
			{
				UniformityCounter[0] = 0;
				UniformityCounter[1] = 0;
				Uniform = 1;
				memset(Distribution3, 0, 8);
				memset(Distribution4, 0, 16);

				for (Masked_InputIndex = 0; Masked_InputIndex < 256; Masked_InputIndex++)
				{
					Masked_Output = 0;
					for (index = 0; index < 4; index++)
						Masked_Output |= F4to1[index][i[index]][InputTableIndex[index + 4][Masked_InputIndex]];

					Unmasked_Output = Unmasking[Masked_Output];
					MadeTable[OutputTableIndex[Masked_InputIndex]] = Unmasked_Output;
					UniformityCounter[Unmasked_Output]++;

					Distribution3[Masked_Output >> 1]++;
					Distribution4[Masked_Output >> 0]++;

					if ((Masked_InputIndex & 0xF) == 0xF) // every 16
					{
						Uniform &= (UniformityCounter[0] == UniformityCounter[1]);

						UniformityCounter[0] = 0;
						UniformityCounter[1] = 0;

						//---------

						if (Masked_InputIndex == 0xF) // the first one
						{
							memcpy(Distribution3Old, Distribution3, 8);
							memcpy(Distribution4Old, Distribution4, 16);
						}
						else
						{
							for (j = 0; j < 8; j++)
								if (Distribution3[j] != Distribution3Old[j])
								{
									i[0] = NumberOfF4to1;
									break;
								}

							if (j < 8)
								break;

							for (j = 0; j < 16; j++)
								if (Distribution4[j] != Distribution4Old[j])
									break;

							if (j < 16)
								break;
						}

						memset(Distribution3, 0, 8);
						memset(Distribution4, 0, 16);
					}
				}

				if (Uniform & (Masked_InputIndex == 256))
				{
					#pragma omp critical (found2)
					{
						FTuples2[NumberOfFTuples2].i[0] = i[0];
						FTuples2[NumberOfFTuples2].i[1] = i[1];
						FTuples2[NumberOfFTuples2].i[2] = i[2];
						FTuples2[NumberOfFTuples2].i[3] = i[3];
						MadeANF = MakeANFSpecial(MadeTable, 8);
						FTuples2[NumberOfFTuples2].ANF = MadeANF;

						AddToLinkList(ShortListFTouple2InANF, SizeOfShortListFTouple2InANF, NumberOfShortListFTouple2InANF,
							MadeANF, NumberOfFTuples2);

						NumberOfFTuples2++;
					}
				}
			}

			j = 0;
			do
			{
				if ((++i[j] >= NumberOfF4to1) & (j < 4))
					i[j] = 0;
			} while (i[j++] == 0);
		}

		#pragma omp critical (print)
		{
			Counter++;
			if ((Counter & 0xFF) == 0xFF)
				printf("%d%d%d%d Touple2 %d / %d %d\n", Types[0], Types[1], Types[2], Types[3],
					Counter, NumberOfF4to1, NumberOfFTuples2);
		}
	}
}

void FindCombination(unsigned char** F4to1[4], int NumberOfF4to1, unsigned char* TargetFunc, unsigned char* Types,
	char* FileName, unsigned int MaxSolutionsPerANF,
	unsigned char** InputTableIndex, unsigned char* OutputTableIndex, unsigned char* Unmasking, unsigned char* UnmaskedInputTable,
	ToupleStruct* FTuples1, ToupleStruct* FTuples2,
	unsigned int NumberOfFTuples1, unsigned int NumberOfFTuples2,
	LinkListStruct** ShortListFTouple1InANF, unsigned int NumberOfShortListFTouple1InANF,
	LinkListStruct** ShortListFTouple2InANF, unsigned int NumberOfShortListFTouple2InANF,
	unsigned char Invert)
{
	unsigned char	abcd;
	unsigned char	a1b1c1d1;
	unsigned char	MaskedTargetFunc[256];
	unsigned short	Masked_InputIndex;
	uint64_t		ANFTargetMaskedFuc;
	int				index1;
	int				index2;
	int				i1;
	int				i2;
	uint64_t		ANFRest;

	unsigned char	Masked_Output;
	unsigned char	Masked_Output1;
	unsigned char	Masked_Output2;
	unsigned char	Unmasked_Input;
	unsigned char	Unmasked_Output;
	unsigned char	UniformityCounter[4];
	unsigned char	MadeTable1[256];
	unsigned char	MadeTable2[256];
	unsigned int	NumberOfFound;
	unsigned int	NumberOfFoundPerANF;
	unsigned short  j;
	char			k;
	char			TempStr[500];
	char			index;
	FILE*			F;
	FunctionStruct	ANFFunc;
	LinkListResStruct	Res;
	LinkListStruct*		Address;
	LinkListStruct*		TempAddress;
	char			InputStr[8][4][5] = {
		{ "a1", "b1", "c1", "d2" },
		{ "a1", "b1", "c2", "d1" },
		{ "a1", "b2", "c1", "d1" },
		{ "a1", "b2", "c2", "d2" },
		{ "a2", "b1", "c1", "d1" },
		{ "a2", "b1", "c2", "d2" },
		{ "a2", "b2", "c1", "d2" },
		{ "a2", "b2", "c2", "d1" } };

	char			LocalInputStr[8][4][5];

	for (j = 0;j < 8;j++)
		for (k = 0;k < 4;k++)
		{
			strcpy(LocalInputStr[j][k], InputStr[j][k]);
			LocalInputStr[j][k][1] = InputStr[j][Types[k]][1];
		}

	Masked_InputIndex = 0;
	for (abcd = 0; abcd < 16; abcd++)
		for (a1b1c1d1 = 0; a1b1c1d1 < 16; a1b1c1d1++)
		{
			MaskedTargetFunc[OutputTableIndex[Masked_InputIndex]] = TargetFunc[abcd];
			Masked_InputIndex++;
		}

	ANFTargetMaskedFuc = MakeANFSpecial(MaskedTargetFunc, 8);
	DefineFunction(&ANFFunc, 500);

	NumberOfFound = 0;
	if (NumberOfFTuples1)
	{
		Address = ShortListFTouple1InANF[0];
		while (Address != ShortListFTouple1InANF[NumberOfShortListFTouple1InANF - 1])
		{
			ANFRest = ANFTargetMaskedFuc ^ Address->ANF;
			Res = FindInLinkList(ShortListFTouple2InANF, NumberOfShortListFTouple2InANF, ANFRest);

			if (Res.Found)
			{
				NumberOfFoundPerANF = 0;
				for (index1 = 0; index1 < Address->NumberOfTouples; index1++)
				{
					i1 = Address->TouplesIndex[index1];
					for (index2 = 0; index2 < Res.Address->NumberOfTouples; index2++)
					{
						i2 = Res.Address->TouplesIndex[index2];
						memset(UniformityCounter, 0, 4);

						for (Masked_InputIndex = 0; Masked_InputIndex < 256; Masked_InputIndex++)
						{
							Masked_Output1 = 0;
							Masked_Output2 = 0;
							for (index = 0; index < 4; index++)
							{
								Masked_Output1 |= F4to1[index][FTuples1[i1].i[index]][InputTableIndex[index + 0][Masked_InputIndex]];
								Masked_Output2 |= F4to1[index][FTuples2[i2].i[index]][InputTableIndex[index + 4][Masked_InputIndex]];
							}

							MadeTable1[Masked_InputIndex] = Masked_Output1;
							MadeTable2[Masked_InputIndex] = Masked_Output2;

							Masked_Output = (Unmasking[Masked_Output2] << 1) | Unmasking[Masked_Output1];
							Unmasked_Output = Unmasking[Masked_Output];
							Unmasked_Input = UnmaskedInputTable[Masked_InputIndex];

							if (TargetFunc[Unmasked_Input] != Unmasked_Output)
							{
								printf("error\n");
								//_getch();
								break;
							}

							UniformityCounter[Masked_Output]++;

							if ((Masked_InputIndex & 0xF) == 0xF) // every 16
							{
								for (j = 0; j < 4; j++)
									if ((UniformityCounter[j] != 0) & (UniformityCounter[j] != 8))
										break;

								if (j < 4)
									break;

								memset(UniformityCounter, 0, 4);
							}
						}

						if (Masked_InputIndex == 256)
						{
							NumberOfFound++;
							if ((NumberOfFound & 0xFF) == 0xFF)
								printf("%d%d%d%d found %d\n", Types[0], Types[1], Types[2], Types[3], NumberOfFound);

							F = fopen(FileName, "at");
							fprintf(F, "%d%d%d%d, ", Types[0], Types[1], Types[2], Types[3]);

							fprintf(F, "%" PRIu64 ", ", Address->ANF);
							for (index = 0; index < 4; index++)
							{
								MakeANF(F4to1[0][FTuples1[i1].i[index]], 16, &ANFFunc, 1, index ? 0 : Invert);
								SPrintFunction(ANFFunc, TempStr, LocalInputStr[index + 0]);
								fprintf(F, "%d, %s, ", ANFFunc.NumberOfTerms, TempStr);
							}

							for (j = 0; j < 256; j++)
								fprintf(F, "%x", MadeTable1[j]);
							fprintf(F, ", ");

							fprintf(F, "%" PRIu64 ", ", Res.Address->ANF);
							for (index = 0; index < 4; index++)
							{
								MakeANF(F4to1[0][FTuples2[i2].i[index]], 16, &ANFFunc, 1, 0);
								SPrintFunction(ANFFunc, TempStr, LocalInputStr[index + 4]);
								fprintf(F, "%d, %s, ", ANFFunc.NumberOfTerms, TempStr);
							}

							for (j = 0; j < 256; j++)
								fprintf(F, "%x", MadeTable2[j]);
							fprintf(F, ",\n");
							fclose(F);

							NumberOfFoundPerANF++;
							if ((MaxSolutionsPerANF) && (NumberOfFoundPerANF >= MaxSolutionsPerANF))
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

	printf("%d%d%d%d found %d\n", Types[0], Types[1], Types[2], Types[3], NumberOfFound);

	//-----------------------------

	FreeFunction(&ANFFunc);

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
	unsigned char**		InputTableIndex = NULL;
	unsigned char*		OutputTableIndex = NULL;
	unsigned char*		Unmasking = NULL;
	unsigned char*		UnmaskedInputTable = NULL;

	unsigned char**		F4to1[4] = { NULL };
	int					NumberOfF4to1;
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
	FILE*				F;

	unsigned char		a, b, c, d;
	unsigned char		abcd;
	unsigned char		TargetFunc[16];
	FunctionStruct		ANFTargetFunc;
	unsigned char		Invert;
	unsigned char		Types[4];
	unsigned char		in[4];
	unsigned char		LocalTargetFunc[16];
	unsigned short		i, j, k;
	char				FunctionName[100];
	char				MainFileName[100];
	char				FileName[100];
	unsigned char		OutputBit;
	__int64				TempTerm;
	unsigned char		TermIndexes[10];
	unsigned char		NumberOfTermIndexes;

	// Midori Sbox
	unsigned char Func[16] = { 0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6 };
	strcpy(FunctionName, "Midori_S");

	// PRINCE Sbox
	//unsigned char Func[16] = { 0xB, 0xF, 0x3, 0x2, 0xA, 0xC, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xE, 0x5, 0xD, 0x4 };
	//strcpy(FunctionName, "PRINCE_S");

	// PRINCE Sbox inv
	//unsigned char Func[16] = { 0xB, 0x7, 0x3, 0x2, 0xF, 0xD, 0x8, 0x9, 0xA, 0x6, 0x4, 0x0, 0x5, 0xE, 0xC, 0x1 };
	//strcpy(FunctionName, "PRINCE_Sinv");

	// PRESENT Sbox
	//unsigned char Func[16] = { 0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2 };
	//strcpy(FunctionName, "PRESENT_S");

	// GF16_inv
	//unsigned char Func[16] = { 0, 1, 3, 2, 14, 13, 8, 10, 15, 6, 7, 12, 4, 9, 5, 11 };
	//strcpy(FunctionName, "GF16_inv");


	const char		NumberOfCPUs = 24;
	const char		MultipleTypes = 1;
	const short		MaxSolutionsPerANF = 1;

	//--------------------------------------------------------------

	FillANFTables(256);
	FillANFTermTables();
	DefineFunction(&ANFTargetFunc, 500);

	omp_set_num_threads(NumberOfCPUs);

	for (OutputBit = 2; OutputBit <= 3; OutputBit++)
	{
		for (abcd = 0; abcd < 16; abcd++)
		{
			a = (abcd & 1) ? 1 : 0;
			b = (abcd & 2) ? 1 : 0;
			c = (abcd & 4) ? 1 : 0;
			d = (abcd & 8) ? 1 : 0;

			TargetFunc[abcd] = ((Func[abcd] >> OutputBit) & 1);
		}

		Invert = TargetFunc[0];
		for (abcd = 0; abcd < 16; abcd++)
			TargetFunc[abcd] ^= Invert;

		//---------------------------------------

		sprintf(MainFileName, "%s_y%d", FunctionName, OutputBit);
		sprintf(FileName, "%s.csv", MainFileName);
		F = fopen(FileName, "wt");
		fclose(F);

		for (Types[0] = 0; Types[0] <= (MultipleTypes ? 3 : 0); Types[0]++)
			for (Types[1] = (MultipleTypes ? 0 : 1); Types[1] <= (MultipleTypes ? 3 : 1); Types[1]++)
				for (Types[2] = (MultipleTypes ? 0 : 2); Types[2] <= (MultipleTypes ? 3 : 2); Types[2]++)
					for (Types[3] = (MultipleTypes ? 0 : 3); Types[3] <= 3; Types[3]++)
					{
						for (abcd = 0; abcd < 16; abcd++)
							LocalTargetFunc[abcd] = TargetFunc[abcd];

						MakeANF(LocalTargetFunc, 16, &ANFTargetFunc, 1, 0);

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
							FillTableIndexes(InputTableIndex, OutputTableIndex, Unmasking, UnmaskedInputTable, Types);
							NumberOfF4to1 = FillF4to1(F4to1, ANFTargetFunc);

							MakeLargeTables(F4to1, NumberOfF4to1, Types,
								InputTableIndex, OutputTableIndex, Unmasking, FTouple1, FTouple2,
								NumberOfFTouples1, ShortListFTouple1InANF, SizeOfShortListFTouple1InANF, NumberOfShortListFTouple1InANF,
								NumberOfFTouples2, ShortListFTouple2InANF, SizeOfShortListFTouple2InANF, NumberOfShortListFTouple2InANF);

							FindCombination(F4to1, NumberOfF4to1, LocalTargetFunc, Types,
								FileName, MaxSolutionsPerANF,
								InputTableIndex, OutputTableIndex, Unmasking, UnmaskedInputTable,
								FTouple1, FTouple2, NumberOfFTouples1, NumberOfFTouples2,
								ShortListFTouple1InANF, NumberOfShortListFTouple1InANF,
								ShortListFTouple2InANF, NumberOfShortListFTouple2InANF, Invert);
						}
					}
	}

	FreeFunction(&ANFTargetFunc);

	printf("done");
	_getch();

	return 0;
}
