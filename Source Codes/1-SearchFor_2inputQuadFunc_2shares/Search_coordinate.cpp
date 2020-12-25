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
#include <conio.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define	MaxNumberOfTerms 1000000000L

unsigned char   *HW;
unsigned int    *NumberOfIndexesInHW;
unsigned long  **IndexesInHW;

#define ONEL ((unsigned __int64)1)

void FillANFTables(unsigned char NumberOfInputBits)
{
	unsigned int	NumberOfCells = (1 << NumberOfInputBits);
	unsigned int	i, Bit;

	HW = (unsigned char *)malloc(NumberOfCells * sizeof(unsigned char));

	NumberOfIndexesInHW = (unsigned int *)malloc((NumberOfInputBits + 1) * sizeof(unsigned int));
	IndexesInHW = (unsigned long **)malloc((NumberOfInputBits + 1) * sizeof(unsigned long *));

	for (Bit = 0;Bit <= NumberOfInputBits;Bit++)
	{
		IndexesInHW[Bit] = (unsigned long *)malloc((NumberOfCells / 2) * sizeof(unsigned long));
		NumberOfIndexesInHW[Bit] = 0;
	}

	for (i = 0;i < NumberOfCells;i++)
	{
		HW[i] = 0;

		for (Bit = 0;Bit < NumberOfInputBits;Bit++)
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

	for (i = 0;i < Function->NumberOfTerms;i++)
		Res ^= ((Function->Term[i] & Input) == Function->Term[i]);

	return (Res);
}

void AddTermtoFunction(FunctionStruct *Function, unsigned __int64 Symbol)
{
	unsigned int	i, j;
	unsigned char	Exist = 0;

	for (i = 0;i < Function->NumberOfTerms;i++)
	{
		if (Function->Term[i] == Symbol)
		{
			Exist = 1;

			Function->NumberOfTerms--;

			for (j = i;j < Function->NumberOfTerms;j++)
				Function->Term[j] = Function->Term[j + 1];

			break;
		}
	}

	if (!Exist)
	{
		Function->Term[Function->NumberOfTerms] = Symbol;
		Function->NumberOfTerms++;
	}
}

void MakeANF(unsigned char* Table, unsigned char NumberOfInputBits, FunctionStruct* Functions, unsigned char NumberOfOutputBits)
{
	unsigned int	NumberOfElements = (1 << NumberOfInputBits);
	unsigned char	Bit, HWIndex;
	unsigned int	i;

	for (Bit = 0;Bit < NumberOfOutputBits;Bit++)
	{
		Functions[Bit].NumberOfTerms = 0;

		for (HWIndex = 0;HWIndex <= NumberOfInputBits;HWIndex++)
			for (i = 0;i < NumberOfIndexesInHW[HWIndex];i++)
				if (IndexesInHW[HWIndex][i] < NumberOfElements)
					if (EvalFunction(&Functions[Bit], IndexesInHW[HWIndex][i]) != ((Table[IndexesInHW[HWIndex][i]] & (1L << Bit)) >> Bit))
						AddTermtoFunction(&Functions[Bit], IndexesInHW[HWIndex][i]);
	}
}

unsigned char AlgDegree(FunctionStruct Function)
{
	unsigned int		i;
	unsigned char		Degree;

	Degree = 1;
	for (i = 0;i < Function.NumberOfTerms;i++)
		if (HW[Function.Term[i]] > Degree)
			Degree = HW[Function.Term[i]];

	return(Degree);
}

void SPrintFunction(FunctionStruct Function, char* ResStr, char InputStr[2][50])
{
	unsigned int		i, j;
	unsigned __int64	ONE;

	ResStr[0] = 0;

	for (i = 0;i < Function.NumberOfTerms;i++)
	{
		if (i)
			strcat(ResStr, " + ");

		if (Function.Term[i])
		{
			ONE = 1;

			for (j = 0;j < 64;j++)
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

int FillF2to1(unsigned char** F2to1[2], FunctionStruct ANFDesiredTerms)
{
	int				NumberOfF2to1;
	unsigned int	i, j, t;
	char			index;
	FunctionStruct	ANFFunc;
	unsigned char	DesiredDegree;

	DesiredDegree = AlgDegree(ANFDesiredTerms);
	DefineFunction(&ANFFunc, 500);

	for (index=0;index<2;index++)
		if (F2to1[index] == NULL)
			F2to1[index] = (unsigned char**)calloc((1 << 4), sizeof(unsigned char*)); // to fill every cell with NULL

	NumberOfF2to1 = 0;
	for (index = 0;index < 2;index++)
		if (F2to1[index][NumberOfF2to1] == NULL)
			F2to1[index][NumberOfF2to1] = (unsigned char*)malloc(4 * sizeof(unsigned char));

	if (DesiredDegree != 2) // adding 0 function
	{
		for (j = 0;j < 4;j++)
			for (index = 0;index < 2;index++)
				F2to1[index][NumberOfF2to1][j] = 0;

		NumberOfF2to1++;
		for (index = 0;index < 2;index++)
			if (F2to1[index][NumberOfF2to1] == NULL)
				F2to1[index][NumberOfF2to1] = (unsigned char*)malloc(4 * sizeof(unsigned char));
	}

	for (i = 2;i < (1 << 4);i += 2)
	{
		t = i;
		for (j = 0;j < 4;j++)
		{
			for (index = 0;index < 2;index++)
				F2to1[index][NumberOfF2to1][j] = (t & 1) << index;
			t >>= 1;
		}

		MakeANF(F2to1[0][NumberOfF2to1], 2, &ANFFunc, 1);

		if (((DesiredDegree == 2) & (AlgDegree(ANFFunc) == 2)) |
			((DesiredDegree < 2) & (AlgDegree(ANFFunc) <= DesiredDegree)))
		{
			for (j = 0;j < ANFDesiredTerms.NumberOfTerms;j++)
			{
				for (t = 0;t < ANFFunc.NumberOfTerms;t++)
					if (ANFDesiredTerms.Term[j] == ANFFunc.Term[t])
						break;

				if (t == ANFFunc.NumberOfTerms)
					break;
			}

			if (j == ANFDesiredTerms.NumberOfTerms)  // ANFFunc contains all terms in ANFDesiredTerms
			{
				for (j = 0;j < ANFFunc.NumberOfTerms;j++)
				{
					if (HW[ANFFunc.Term[j]] == DesiredDegree)
					{
						for (t = 0;t < ANFDesiredTerms.NumberOfTerms;t++)
							if (ANFFunc.Term[j] == ANFDesiredTerms.Term[t])
								break;

						if (t == ANFDesiredTerms.NumberOfTerms)
							break;
					}
				}

				if (j == ANFFunc.NumberOfTerms)
				{
					NumberOfF2to1++;
					for (index = 0;index < 2;index++)
						if (F2to1[index][NumberOfF2to1] == NULL)
							F2to1[index][NumberOfF2to1] = (unsigned char*)malloc(4 * sizeof(unsigned char));
				}
			}
		}
	}

	FreeFunction(&ANFFunc);
	return(NumberOfF2to1);
}

void Check(unsigned char** F2to1[2], int NumberOfF2to1, unsigned char* OrigFunc, FILE* F)
{
	unsigned char   a, a1, a2;
	unsigned char	b, b1, b2;
	unsigned char	ab;
	unsigned char	a1b1;
	unsigned char*	InputTable[4];
	unsigned char*	UnmasedInputTable;
	unsigned short	Masked_InputIndex;
	unsigned char	Masked_Output;
	unsigned char	Masked_Output1;
	unsigned char	Masked_Output2;
	unsigned char	Unmasking1[256];
	unsigned char	Unmasking2[256];
	unsigned char	Unmasked_Input;
	unsigned char	Unmasked_Output;
	unsigned char	UniformityCounter[4];
	unsigned char	Distribution1Old[4];
	unsigned char	Distribution1[4];
	unsigned char	Distribution2Old[4];
	unsigned char	Distribution2[4];
	int				i0;
	int				i[5];
	long long		NumberOfFound;
	unsigned short  j;
	char			k;
	FunctionStruct	ANFFunc;
	char			TempStr[500];
	char			index;
	char			InputStr[4][2][50] = {
		{ "a1", "b1"},
		{ "a1", "b2"},
	    { "a2", "b1"},
	    { "a2", "b2"}};

	DefineFunction(&ANFFunc, 500);

	for (index=0;index<4;index++)
		InputTable[index] = (unsigned char*)malloc(16 * sizeof(unsigned char));

	UnmasedInputTable = (unsigned char*)malloc(16 * sizeof(unsigned char));

	for (j = 0;j < 256;j++)
	{
		Unmasking1[j] = 0;
		for (k = 0;k < 8;k++)
			Unmasking1[j] ^= (j & (1 << k)) ? 1 : 0;

		Unmasking2[j] = Unmasking1[j] << 1;
	}

	Masked_InputIndex = 0;
	for (ab = 0;ab < 4;ab++)
	{
		a = (ab & 1) ? 1 : 0;
		b = (ab & 2) ? 1 : 0;

		for (a1b1 = 0;a1b1 < 4;a1b1++)
		{
			a1 = (a1b1 & 1) ? 1 : 0;
			b1 = (a1b1 & 2) ? 1 : 0;

			a2 = a ^ a1;
			b2 = b ^ b1;

			InputTable[0][Masked_InputIndex] = (b1 << 1) | (a1 << 0);
			InputTable[1][Masked_InputIndex] = (b2 << 1) | (a1 << 0);
			InputTable[2][Masked_InputIndex] = (b1 << 1) | (a2 << 0);
			InputTable[3][Masked_InputIndex] = (b2 << 1) | (a2 << 0);

			UnmasedInputTable[Masked_InputIndex] = (b << 1) | (a << 0);
			Masked_InputIndex++;
		}
	}

	omp_set_num_threads(1);

	NumberOfFound = 0;
	#pragma omp parallel for schedule(dynamic) private(i, index, UniformityCounter, Distribution1Old, Distribution1, Distribution2Old, Distribution2, Masked_InputIndex, Masked_Output1, Masked_Output2, Masked_Output, Unmasked_Output, Unmasked_Input, j, TempStr)
	for (i0 = 0;i0 < NumberOfF2to1;i0++)
	{
		printf("%d / %d\n", i0, NumberOfF2to1);

		i[0] = i0;
		for (index = 1;index <= 4 ;index++)
			i[index] = 0;

		while (i[4] == 0)
		{
			memset(UniformityCounter, 0, 4);
			memset(Distribution1, 0, 4);
			memset(Distribution2, 0, 4);

			for (Masked_InputIndex = 0;Masked_InputIndex < 16;Masked_InputIndex++)
			{
				Masked_Output1 = 0;
				Masked_Output2 = 0;
				for (index = 0;index < 2;index++)
				{
					Masked_Output1 |= F2to1[index][i[index]][InputTable[index][Masked_InputIndex]];
					Masked_Output2 |= F2to1[index][i[index + 2]][InputTable[index + 2][Masked_InputIndex]];
				}

				Masked_Output = Unmasking2[Masked_Output2] | Unmasking1[Masked_Output1];
				Unmasked_Output = Unmasking1[Masked_Output];
				Unmasked_Input = UnmasedInputTable[Masked_InputIndex];

				if (OrigFunc[Unmasked_Input] != Unmasked_Output)
					break;

				UniformityCounter[Masked_Output]++;

				Distribution1[Masked_Output1]++;
				Distribution2[Masked_Output2]++;

				if ((Masked_InputIndex & 0x3) == 0x3) // every 4
				{
					for (j = 0;j < 4;j++)
						if ((UniformityCounter[j] != 0) & (UniformityCounter[j] != 2))
							break;

					if (j < 4)
						break;

					memset(UniformityCounter, 0, 4);

					//----

					if (Masked_InputIndex == 0x3) // the first one
					{
						memcpy(Distribution1Old, Distribution1, 4);
						memcpy(Distribution2Old, Distribution2, 4);
					}
					else
					{
						for (j = 0;j < 4;j++)
							if ((Distribution1[j] != Distribution1Old[j]) |
								(Distribution2[j] != Distribution2Old[j]))
								break;

						if (j < 4)
							break;
					}

					memset(Distribution1, 0, 4);
					memset(Distribution2, 0, 4);
				}
			}

			if (Masked_InputIndex == 16)
			{
				#pragma omp critical
				{
					NumberOfFound++;
					printf("%4I64d", NumberOfFound);
					for (index = 0;index < 4;index++)
						printf(" %3d", i[index]);
					printf("\n");

					for (index = 0;index < 4;index++)
					{
						MakeANF(F2to1[0][i[index]], 2, &ANFFunc, 1);
						SPrintFunction(ANFFunc, TempStr, InputStr[index]);
						fprintf(F, "%d, %s, ", ANFFunc.NumberOfTerms, TempStr);
					}
					fprintf(F, "\n");
				}
			}

			j = 1;
			do
			{
				if ((++i[j] == NumberOfF2to1) & (j < 4))
					i[j] = 0;
			} while (i[j++] == 0);
		}
	}

	FreeFunction(&ANFFunc);
}

int main()
{
	FillANFTables(8);

	unsigned char**	F2to1[2] = { NULL };
	int				NumberOfF2to1;

	unsigned char   a, b;
	unsigned char	ab;
	unsigned char	DesiredTerms[4];
	unsigned char	OrigFunc[4];
	FunctionStruct	ANFDesiredTerms;
	FILE*			F;

	DefineFunction(&ANFDesiredTerms, 500);

	for (ab = 0;ab < 4;ab++)
	{
		a = (ab & 1) ? 1 : 0;
		b = (ab & 2) ? 1 : 0;
		OrigFunc[ab] = a&b;
		DesiredTerms[ab] = a&b;
	}

	MakeANF(DesiredTerms, 2, &ANFDesiredTerms, 1);
	F = fopen("Res_AND2bit.csv", "wt");

	NumberOfF2to1 = FillF2to1(F2to1, ANFDesiredTerms);
	Check(F2to1, NumberOfF2to1, OrigFunc, F);

	fclose(F);
	FreeFunction(&ANFDesiredTerms);

	printf("done");
	_getch();

	return 0;
}
