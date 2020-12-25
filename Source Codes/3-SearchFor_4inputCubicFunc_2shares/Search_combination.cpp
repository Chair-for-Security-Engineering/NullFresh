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

const char			path[500] = "Midori_y";
const unsigned char	NumberOfOutputBits = 4;
const unsigned char Bits[NumberOfOutputBits] = { 0,1,2,3 };

const unsigned char ReduceTables  = 1;
const unsigned char	ReorderTabes  = 1;
const unsigned char CheckCouples  = 1;
const unsigned char	SameType      = 0;
const unsigned char NumberOfCPUs  = 24;

const long long		MaxCouplesPossible = 10000000000L;

//-----------------------------------------------------

unsigned char Unmasking[256];

void FillUnmaskingTable()
{
	short j, k;

	for (j = 0;j < 256;j++)
	{
		Unmasking[j] = 0;
		for (k = 0;k < 8;k++)
			Unmasking[j] ^= (j & (1 << k)) ? 1 : 0;
	}
}

unsigned char CheckUniformity(unsigned char* TablesAddress[NumberOfOutputBits], unsigned char NumberOfTables)
{
	unsigned short	NumberOfUniformityCounters[NumberOfOutputBits - 1];
	unsigned short	Masked_InputIndex;
	unsigned short	Masked_Output;
	unsigned short	j;
	unsigned short	k;
	unsigned char	ExpectedCounterValue[NumberOfOutputBits - 1];
	unsigned char	UniformityCounter[NumberOfOutputBits - 1][1 << (2 * NumberOfOutputBits)];

	for (j = 0;j < NumberOfTables - 1;j++)
	{
		ExpectedCounterValue[j] = 16 / (1 << (j + 2));
		NumberOfUniformityCounters[j] = (1 << (2 * (j + 2)));
		memset(UniformityCounter[j], 0, NumberOfUniformityCounters[j]);
	}

	for (Masked_InputIndex = 0;Masked_InputIndex < 256;Masked_InputIndex++)
	{
		Masked_Output = 0;
		for (j = 0;j < NumberOfTables;j++)
		{
			Masked_Output <<= 2;
			Masked_Output |= TablesAddress[j][Masked_InputIndex];

			if (j)
				UniformityCounter[j - 1][Masked_Output]++;
		}

		if ((Masked_InputIndex & 0xf) == 0xf) // every 16
		{
			for (j = 0;j < NumberOfTables - 1;j++)
			{
				for (k = 0;k < NumberOfUniformityCounters[j];k++)
					if ((UniformityCounter[j][k] != 0) && (UniformityCounter[j][k] != ExpectedCounterValue[j]))
						return(j + 1);

				memset(UniformityCounter[j], 0, NumberOfUniformityCounters[j]);
			}
		}
	}

	return(0); // is uniform
}

struct UniqueTablestruct
{
	unsigned char*	Table;
	unsigned int	Type;
	unsigned int	StartIndex;
	unsigned int	Count;
};


int main()
{
	FILE*				F;
	char				FilePath[500];
	char*				TempStr;
	UniqueTablestruct  *UniqueTables[NumberOfOutputBits];
	unsigned int		NumberOfUniqueTables[NumberOfOutputBits];
	unsigned int		Type;
	uint64_t			ANF_old;
	uint64_t			ANF;
	char**				Strings[16][NumberOfOutputBits];
	unsigned int		NumberOfTables[NumberOfOutputBits];
	unsigned char*		Flags[NumberOfOutputBits];
	unsigned char		OnPattern[NumberOfOutputBits];
	unsigned char		OffPattern[NumberOfOutputBits];
	unsigned int		NumberOfReducedTables[NumberOfOutputBits];
	unsigned int*		ReducedTablesIndex[NumberOfOutputBits];

	unsigned int		TempSize[NumberOfOutputBits];
	unsigned int		min;
	unsigned int		max;
	unsigned char		TablesOrder[NumberOfOutputBits];
	unsigned char		TablesOrderInv[NumberOfOutputBits];
	char				TempChar;
	int					j;
	unsigned int		k, l;
	char				index;
	char				index2;
	int					i_last;
	unsigned int		i[NumberOfOutputBits + 1];
	int					i0;
	int					i1;
	unsigned char*		TablesAddress[NumberOfOutputBits];
	long long			NumberOfFound;
	int					Counter;
	char				Last;
	char				CheckBit;
	char				Res;
	char**				CouplesPossible[NumberOfOutputBits][NumberOfOutputBits] = { NULL };

	omp_set_num_threads(NumberOfCPUs);

	FillUnmaskingTable();
	TempStr = (char*)malloc(100000 * sizeof(char));

	for (index = 0; index < NumberOfOutputBits; index++)
	{
		printf("getting size of table %d", index);

		sprintf(FilePath, "%s%d.csv", path, Bits[index]);
		F = fopen(FilePath, "rt");
		NumberOfTables[index] = 0;
		Last = '\n';

		while (!feof(F))
		{
			j = fread(TempStr, 1, 100000, F);

			for (k = 0; k < j; k++)
			{
				TempChar = TempStr[k];
				if (((TempChar == '\n') | (TempChar == '\r')) & (Last != '\n') & (Last != '\r'))
					NumberOfTables[index]++;
				Last = TempChar;
			}
		}

		printf(": %d, reading", NumberOfTables[index]);
		fseek(F, 0, SEEK_SET);

		UniqueTables[index] = (UniqueTablestruct*)malloc(NumberOfTables[index] * sizeof(UniqueTablestruct));
		for (k = 0; k < 16; k++)
			Strings[k][index] = (char**)malloc(NumberOfTables[index] * sizeof(char*));

		ANF_old = 0;
		NumberOfUniqueTables[index] = 0;
		for (j = 0; j < NumberOfTables[index]; j++)
		{
			fscanf(F, "%s", TempStr);
			TempStr[strlen(TempStr) - 1] = 0;
			Type = atoi(TempStr);

			fscanf(F, "%s", TempStr);
			TempStr[strlen(TempStr) - 1] = 0;
			sscanf(TempStr, "%" SCNu64 , &ANF);

			for (k = 0; k < 8; k++)
			{
				l = 0;
				do
				{
					TempChar = fgetc(F);
					if (TempChar != '\n')
						TempStr[l++] = TempChar;
				} while (TempChar != ',');

				TempStr[l] = 0;
				Strings[k][index][j] = (char*)malloc((l + 1) * sizeof(char));
				strcpy(Strings[k][index][j], TempStr);
			}

			fscanf(F, "%s", TempStr);
			if (ANF != ANF_old)
			{
				UniqueTables[index][NumberOfUniqueTables[index]].StartIndex = j;
				UniqueTables[index][NumberOfUniqueTables[index]].Count = 1;
				UniqueTables[index][NumberOfUniqueTables[index]].Type = Type;
				UniqueTables[index][NumberOfUniqueTables[index]].Table = (unsigned char*)malloc(256 * sizeof(unsigned char));

				for (k = 0; k < 256; k++)
					UniqueTables[index][NumberOfUniqueTables[index]].Table[k] = Unmasking[(TempStr[k] <= '9') ? (TempStr[k] - '0') : (TempStr[k] - 'a' + 10)];
			}
			else
				UniqueTables[index][NumberOfUniqueTables[index]].Count++;

			fscanf(F, "%s", TempStr); // skip the second ANF

			for (k = 8; k < 16; k++)
			{
				l = 0;
				do
				{
					TempChar = fgetc(F);
					if (TempChar != '\n')
						TempStr[l++] = TempChar;
				} while (TempChar != ',');

				TempStr[l] = 0;
				Strings[k][index][j] = (char*)malloc((l + 1) * sizeof(char));
				strcpy(Strings[k][index][j], TempStr);
			}

			fscanf(F, "%s", TempStr);
			if (ANF != ANF_old)
			{
				for (k = 0; k < 256; k++)
					UniqueTables[index][NumberOfUniqueTables[index]].Table[k] |= Unmasking[(TempStr[k] <= '9') ? (TempStr[k] - '0') : (TempStr[k] - 'a' + 10)] << 1;

				NumberOfUniqueTables[index]++;
			}

			ANF_old = ANF;
		}

		fclose(F);

		printf(", unique: %d\n", NumberOfUniqueTables[index]);
	}

	//====================================================

	memcpy(TempSize, NumberOfUniqueTables, NumberOfOutputBits * sizeof(unsigned int));
	for (index = 0; index < NumberOfOutputBits; index++)
	{
		if (ReorderTabes)
		{
			min = 0;
			for (j = 1; j < NumberOfOutputBits; j++)
				if (TempSize[min] > TempSize[j])
					min = j;

			TablesOrder[index] = min;
			TempSize[min] = -1;
		}
		else
			TablesOrder[index] = index;
	}

	for (index = 0; index < NumberOfOutputBits; index++)
		TablesOrderInv[TablesOrder[index]] = index;

	//====================================================

	for (index = 0; index < NumberOfOutputBits; index++)
	{
		OnPattern[index] = 0;
		for (index2 = 0; index2 <= index; index2++)
			OnPattern[index] |= (1 << index2);

		OffPattern[index] = ~(1 << index);
	}

	for (index = 0; index < NumberOfOutputBits; index++)
		Flags[index] = (unsigned char*)calloc(NumberOfUniqueTables[index], sizeof(unsigned char));

	printf("\n");

	for (index = 0; index < NumberOfOutputBits; index++)
		NumberOfReducedTables[TablesOrder[index]] = NumberOfUniqueTables[TablesOrder[index]];

	for (index = 0; index < NumberOfOutputBits; index++)
	{
		if (ReduceTables)
		{
			printf("Checking table %d: ", TablesOrder[index]);

			NumberOfReducedTables[TablesOrder[index]] = 0;
			for (i[0] = 0; i[0] < NumberOfUniqueTables[TablesOrder[index]]; i[0]++)
				if (Flags[TablesOrder[index]][i[0]] == (OnPattern[index] & OffPattern[index]))
					NumberOfReducedTables[TablesOrder[index]]++;
			printf("%d (", NumberOfReducedTables[TablesOrder[index]]);

			for (index2 = index + 1; index2 < NumberOfOutputBits; index2++)
			{
				if (index2 != index + 1)
					printf(", ");
				printf("with %d ", TablesOrder[index2]);

				if (((long long)NumberOfReducedTables[TablesOrder[index]]) * ((long long)NumberOfReducedTables[TablesOrder[index2]]) < MaxCouplesPossible)
				{
					#pragma omp parallel for schedule(guided) private(i1, TablesAddress, CheckBit)
					for (i0 = 0; i0 < NumberOfUniqueTables[TablesOrder[index]]; i0++)
					{
						if (Flags[TablesOrder[index]][i0] == (OnPattern[index2 - 1] & OffPattern[index]))
						{
							TablesAddress[0] = UniqueTables[TablesOrder[index]][i0].Table;
							CheckBit = 0;

							for (i1 = 0; i1 < NumberOfUniqueTables[TablesOrder[index2]]; i1++)
								if (((!SameType) ||
									(UniqueTables[TablesOrder[index]][i0].Type == UniqueTables[TablesOrder[index2]][i1].Type)) &&
										((((index == 0) & (Flags[TablesOrder[index2]][i1] == 0)) ||
									((index != 0) & (Flags[TablesOrder[index2]][i1] == (OnPattern[index - 1] & OffPattern[index2]))) ||
											((Flags[TablesOrder[index2]][i1] == (OnPattern[index] & OffPattern[index2])) & (!CheckBit)))))
								{
									TablesAddress[1] = UniqueTables[TablesOrder[index2]][i1].Table;

									if (!CheckUniformity(TablesAddress, 2))
									{
										CheckBit = 1;
										#pragma omp atomic
										Flags[TablesOrder[index2]][i1] |= (1 << index);
									}
								}

							if (CheckBit)
							{
								#pragma omp atomic
								Flags[TablesOrder[index]][i0] |= (1 << index2);
							}
						}
					}
				}
				else
				{
					for (i0 = 0; i0 < NumberOfUniqueTables[TablesOrder[index]]; i0++)
						Flags[TablesOrder[index]][i0] |= (1 << index2);

					for (i1 = 0; i1 < NumberOfUniqueTables[TablesOrder[index2]]; i1++)
						Flags[TablesOrder[index2]][i1] |= (1 << index);
				}

				if (index2 != NumberOfOutputBits - 1)
				{
					NumberOfReducedTables[TablesOrder[index]] = 0;
					for (i[0] = 0; i[0] < NumberOfUniqueTables[TablesOrder[index]]; i[0]++)
						if (Flags[TablesOrder[index]][i[0]] == (OnPattern[index2] & OffPattern[index]))
							NumberOfReducedTables[TablesOrder[index]]++;
					printf("-> %d", NumberOfReducedTables[TablesOrder[index]]);
				}
			}

			NumberOfReducedTables[TablesOrder[index]] = 0;
			for (i[0] = 0; i[0] < NumberOfUniqueTables[TablesOrder[index]]; i[0]++)
				if (Flags[TablesOrder[index]][i[0]] == (OnPattern[NumberOfOutputBits - 1] & OffPattern[index]))
					NumberOfReducedTables[TablesOrder[index]]++;

			printf(") -> %d\n", NumberOfReducedTables[TablesOrder[index]]);
		}
		else
			NumberOfReducedTables[TablesOrder[index]] = NumberOfUniqueTables[TablesOrder[index]];

		ReducedTablesIndex[TablesOrder[index]] = (unsigned int*)malloc(NumberOfReducedTables[TablesOrder[index]] * sizeof(unsigned int));
		NumberOfReducedTables[TablesOrder[index]] = 0;
		for (i[0] = 0; i[0] < NumberOfUniqueTables[TablesOrder[index]]; i[0]++)
			if ((ReduceTables == 0) ||
				(Flags[TablesOrder[index]][i[0]] == (OnPattern[NumberOfOutputBits - 1] & OffPattern[index])))
				ReducedTablesIndex[TablesOrder[index]][NumberOfReducedTables[TablesOrder[index]]++] = i[0];
	}

	printf("\n");

	//====================================================

	memcpy(TempSize, NumberOfReducedTables, NumberOfOutputBits * sizeof(unsigned int));

	for (index = 0; index < NumberOfOutputBits; index++)
	{
		if (ReorderTabes)
		{
			max = 0;
			for (j = 1; j < NumberOfOutputBits; j++)
				if (TempSize[max] < TempSize[j])
					max = j;

			TablesOrder[index] = max;
			TempSize[max] = 0;
		}
		else
			TablesOrder[index] = index;
	}

	for (index = 0; index < NumberOfOutputBits; index++)
		TablesOrderInv[TablesOrder[index]] = index;

	//====================================================

	for (index2= NumberOfOutputBits-1;index2>=1;index2--)
		for (index = index2 - 1; index >=0; index--)
			if (CheckCouples)
			{
				printf("Checking possible couples (%d,%d) ", TablesOrder[index2], TablesOrder[index]);

				if (((long long)NumberOfReducedTables[TablesOrder[index2]]) * ((long long)NumberOfReducedTables[TablesOrder[index]]) < MaxCouplesPossible)
				{
					if (NumberOfReducedTables[TablesOrder[index2]])
						CouplesPossible[TablesOrder[index2]][TablesOrder[index]] = (char**)malloc(NumberOfReducedTables[TablesOrder[index2]] * sizeof(char*));
					else
						CouplesPossible[TablesOrder[index2]][TablesOrder[index]] = NULL;

					#pragma omp parallel for schedule(guided) private(i0, TablesAddress)
					for (i1 = 0; i1 < NumberOfReducedTables[TablesOrder[index2]]; i1++)
					{
						TablesAddress[0] = UniqueTables[TablesOrder[index2]][ReducedTablesIndex[TablesOrder[index2]][i1]].Table;
						CouplesPossible[TablesOrder[index2]][TablesOrder[index]][i1] = (char*)malloc(NumberOfReducedTables[TablesOrder[index]] * sizeof(char));

						for (i0 = 0; i0 < NumberOfReducedTables[TablesOrder[index]]; i0++)
						{
							TablesAddress[1] = UniqueTables[TablesOrder[index]][ReducedTablesIndex[TablesOrder[index]][i0]].Table;
							CouplesPossible[TablesOrder[index2]][TablesOrder[index]][i1][i0] = CheckUniformity(TablesAddress, 2);
						}
					}

					printf("done\n");
				}
				else
				{
					CouplesPossible[TablesOrder[index2]][TablesOrder[index]] = NULL;
					printf("too large\n");
				}
			}
			else
				CouplesPossible[TablesOrder[index2]][TablesOrder[index]] = NULL;

	//====================================================

	strcpy(FilePath, path);
	for (index = 0; index < NumberOfOutputBits; index++)
		sprintf(FilePath, "%s%d", FilePath, Bits[index]);

	strcat(FilePath, ".csv");
	F = fopen(FilePath, "wt");
	fclose(F);

	NumberOfFound = 0;
	Counter = 0;
	#pragma omp parallel for schedule(guided) private(j, i, index, index2, TablesAddress, Res)
	for (i_last = 0; i_last < NumberOfReducedTables[TablesOrder[NumberOfOutputBits - 1]]; i_last++)
	{
		#pragma omp atomic
		Counter++;

		#pragma omp critical (print)
		{
			printf("%d / %d\n", Counter, NumberOfReducedTables[TablesOrder[NumberOfOutputBits - 1]]);
		}

		for (j = 0; j < NumberOfOutputBits - 1; j++)
			i[j] = 0;
		i[NumberOfOutputBits - 1] = i_last;

		while (i[NumberOfOutputBits - 1] == i_last)
		{
			for (index = NumberOfOutputBits - 1; index >= 0; index--)
			{
				if (SameType && (index < (NumberOfOutputBits - 1)) &&
					(UniqueTables[TablesOrder[index]][ReducedTablesIndex[TablesOrder[index]][i[index]]].Type !=
						UniqueTables[TablesOrder[index + 1]][ReducedTablesIndex[TablesOrder[index + 1]][i[index + 1]]].Type))
					break;

				for (index2 = NumberOfOutputBits - 1; index2 > index; index2--)
					if (CouplesPossible[TablesOrder[index2]][TablesOrder[index]] &&
						CouplesPossible[TablesOrder[index2]][TablesOrder[index]][i[index2]][i[index]])
						break;

				if (index2 > index)
					break;

				TablesAddress[NumberOfOutputBits - 1 - index] = UniqueTables[TablesOrder[index]][ReducedTablesIndex[TablesOrder[index]][i[index]]].Table;
			}

			if (index >= 0)
			{
				for (j = index - 1; j >= 0; j--)
					i[j] = NumberOfReducedTables[TablesOrder[j]];
			}
			else
			{
				Res = CheckUniformity(TablesAddress, NumberOfOutputBits);

				if (Res)
				{
					for (j = NumberOfOutputBits - 2 - Res; j >= 0; j--)
						i[j] = NumberOfReducedTables[TablesOrder[j]];
				}
				else
				{
					#pragma omp critical (found)
					{
						unsigned int small_i[NumberOfOutputBits + 1];

						for (index = 0; index < NumberOfOutputBits + 1; index++)
							small_i[index] = 0;

						F = fopen(FilePath, "at");
						while (!small_i[NumberOfOutputBits])
						{
							NumberOfFound++;
							if ((NumberOfFound & 0xff) == 0xff)
							{
								printf("%d / %d found %I64d ", Counter, NumberOfReducedTables[TablesOrder[NumberOfOutputBits - 1]], NumberOfFound);
								for (index = 0; index < NumberOfOutputBits; index++)
									printf("%d ", i[index]);
								printf("\n");
							}

							for (index = 0; index < NumberOfOutputBits; index++)
							{
								fprintf(F, "%04d, ", UniqueTables[index][ReducedTablesIndex[index][i[TablesOrderInv[index]]]].Type);
								for (k = 0; k < 16; k++)
									fprintf(F, "%s ", Strings[k][index][UniqueTables[index][ReducedTablesIndex[index][i[TablesOrderInv[index]]]].StartIndex + small_i[index]]);
								fprintf(F, ", ");
							}

							fprintf(F, "\n");


							j = 0;
							do
							{
								small_i[j]++;
								if (j < NumberOfOutputBits)
									if (small_i[j] >= UniqueTables[j][ReducedTablesIndex[j][i[TablesOrderInv[j]]]].Count)
										small_i[j] = 0;
							} while (small_i[j++] == 0);
						}
						fclose(F);
					}
				}
			}

			j = 0;
			do
			{
				i[j]++;
				if (j < NumberOfOutputBits)
					if (i[j] >= NumberOfReducedTables[TablesOrder[j]])
						i[j] = 0;
			} while (i[j++] == 0);
		}
	}

	printf("Total found %I64d\n\n", NumberOfFound);

	//--------------------------------------------------

	for (index = 0; index < NumberOfOutputBits; index++)
	{
		for (index2 = 0; index2 < NumberOfOutputBits; index2++)
			if (CouplesPossible[TablesOrder[index]][TablesOrder[index2]])
			{
				for (i0 = 0; i0 < NumberOfReducedTables[TablesOrder[index]]; i0++)
					free(CouplesPossible[TablesOrder[index]][TablesOrder[index2]][i0]);

				free(CouplesPossible[TablesOrder[index]][TablesOrder[index2]]);
			}

		if (ReducedTablesIndex[index])
			free(ReducedTablesIndex[index]);
		if (Flags[index])
			free(Flags[index]);

		for (j = 0; j < NumberOfUniqueTables[index]; j++)
			free(UniqueTables[index][j].Table);
		free(UniqueTables[index]);

		for (j = 0; j < NumberOfTables[index]; j++)
		{
			for (k = 0; k < 16; k++)
				free(Strings[k][index][j]);
		}

		for (k = 0; k < 16; k++)
			free(Strings[k][index]);
	}

	printf("done");
	_getch();

	return 0;
}
