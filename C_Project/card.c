#include <stdio.h>
#include "card.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS


int isalphabetic(char string[])
{
	int counter = 0;
	size_t length = strlen(string);
	while (counter < length)
	{
		if (!(isalpha(string[counter]) || string[counter] == ' '))
			return 0;
		counter++;
	}
	return 1;
}

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t NameValidation;
	printf("Please Enter your Name: ");
	gets(cardData->cardHolderName);
	size_t NameLength = strlen(cardData->cardHolderName);
	if ((NameLength <= 24 && NameLength >= 20) && isalphabetic(cardData->cardHolderName))
	{
		NameValidation = CARD_OK;
		return NameValidation;
	}
	else
	{
		NameValidation = WRONG_NAME;
		return NameValidation;
	}

}

int checkDate(char date[5])
{
	int currentYear = 22;
	if (date[2] != '/')
		return 0;
	else //verifying the Month Part to be less than or equal 12 month
	{
		char monthSubstr[2];
		char yearSubstr[2];
		for (int i = 0; i < 2; i++)
		{
			monthSubstr[i] = date[i];
			yearSubstr[i] = date[i + 3];
		}
		int month = atoi(monthSubstr);
		int year = atoi(yearSubstr);
		if ((month <= 12 && month > 0) && year >= currentYear)
			return 1;
		else
			return 0;	
	}

}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	EN_cardError_t DateValidation;
	printf("Please Enter your Expiration Date: ");
	gets(cardData->cardExpirationDate);
	printf("\n");
	size_t DateLength = strlen(cardData->cardExpirationDate);

	if (DateLength == 5)
	{
		if (checkDate(cardData->cardExpirationDate))
		{
			DateValidation = CARD_OK;
			return DateValidation;
		}
		else
		{
			DateValidation = WRONG_EXP_DATE;
			return DateValidation;
		}
	}
	else
	{
		DateValidation = WRONG_EXP_DATE;
		return DateValidation;
	}

}

int isNumeric(char string[])
{
	int counter = 0;
	size_t length = strlen(string);
	while (counter < length)
	{
		if (!isdigit(string[counter]))
			return 0;
		counter++;
	}
	return 1;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	EN_cardError_t PanValidation;
	printf("Please Enter your PAN: ");
	gets(cardData->primaryAccountNumber);
	printf("\n");
	size_t NameLength = strlen(cardData->primaryAccountNumber);
	if ((NameLength <= 19 && NameLength >= 16) && isNumeric(cardData->primaryAccountNumber))
	{
		PanValidation = CARD_OK;
		return PanValidation;
	}
	else
	{
		PanValidation = WRONG_PAN;
		return PanValidation;
	}

}
