#include "high_score.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_high_scores(const char* path, High_Score high_scores[], int size)
{
	FILE *fptr = fopen(path,"r");

	if(fptr == NULL)
		return 1;

	int counter = 0;
	char string[61] = {'\0'};

	for(; counter < size; counter++)
	{
		fgets(string, 61, fptr);

		unsigned int i = 0;

		char score_temp[20] = {'\0'};

		for(;i < 61; i++)
		{
			if (string[i] == ' ')
			{
				i++;
				break;
			}

			else score_temp[i] = string[i];
		}

		char *endptr;
		high_scores[counter].score = strtoul(score_temp, &endptr, 10);

		char name_temp[20] = {'\0'};
		unsigned int j = 0;

		for(;i < 61; i++, j++)
		{
			if (string[i] == ' ' && (string[i+1] == '1' || string[i+1] == '2' || string[i+1] == '3' || string[i+1] == '4' || string[i+1] == '5' || string[i] == '6' || string[i] == '7' || string[i] == '8' || string[i] == '9'))
			{
				i++;
				j=0;
				break;
			}

			else name_temp[j] = string[i];
		}
		strcpy(high_scores[counter].name,name_temp);


		char date_temp[20] = {'\0'};

		for(;i < 61; i++, j++)
		{
			if (string[i] == '\n')
				break;

			else date_temp[j] = string[i];
		}

		strcpy(high_scores[counter].date,date_temp);

	}

	fclose(fptr);

	return 0;
}

int save_high_scores(const char* path, High_Score high_scores[], int size)
{
	FILE *fptr = fopen(path,"w");

	if(fptr == NULL)
		return 1;

	int counter = 0;

	for(; counter < size; counter++)
	{
		fprintf(fptr, "%u %s %s\n", high_scores[counter].score, high_scores[counter].name, high_scores[counter].date);
	}

	fclose(fptr);

	return 0;
}

void insert_sort_high_score(High_Score high_scores[], High_Score new_highscore)
{
	unsigned int i = 0;

	for(;i < 10; i++)
	{
		if(high_scores[i].score < new_highscore.score)
		{
			for(;i < 10; i++)
			{
				High_Score temp = high_scores[i];
				high_scores[i] = new_highscore;
				new_highscore = temp;
			}

			return;
		}
	}
}
