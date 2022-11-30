#include <stdio.h>
#include <string.h>
#include "terminal.h"
#include <stdlib.h>
#include "card.h"
#define _CRT_SECURE_NO_WARNINGS

int checkTerminalDate(char date[10])
{
	if (date[2] != '/' && date[5] != '/')
		return 0;	
	else //verifying the day, month, year Part to be less than or equal 12 month.
	{
		char daySubstr[2];
		char monthSubstr[2];
		char yearSubstr[4];
		for (int i = 0; i < 2; i++)
		{
			daySubstr[i] = date[i];
			monthSubstr[i] = date[i + 3];
		}
		for (int j = 0; j < 4; j++)
		{
			yearSubstr[j] = date[j + 6];
		}
		int month = atoi(monthSubstr);
		int year = atoi(yearSubstr);
		int day = atoi(daySubstr);
		if ((month <= 12 && month > 0) && (day <= 31 && day > 0) )
			return 1;
		else
			return 0;
	}
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	EN_terminalError_t DateValidation;
	printf("Please Enter your Transaction Date: ");
	gets(termData->transactionDate);
	printf("\n");
	size_t DateLength = strlen(termData->transactionDate);
	if (DateLength == 10)
	{
		if (checkTerminalDate(termData->transactionDate))
		{
			DateValidation = TERMINAL_OK;
			return DateValidation;
		}
		else
		{
			DateValidation = WRONG_DATE;
			return DateValidation;
		}
	}
	else
	{
		DateValidation = WRONG_DATE;
		return DateValidation;
	}
}


EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	//get month and year as integer values.
	EN_terminalError_t validator;
	char expMonthSubstr[2];
	char expYearSubstr[2];
	char transMonthSubstr[2];
	char transYearSubstr[4];

	for (int i = 0; i < 2; i++)
	{
		expMonthSubstr[i] = cardData.cardExpirationDate;
		expYearSubstr[i] = cardData.cardExpirationDate[i + 3];
		transMonthSubstr[i] = termData.transactionDate[i + 3];
	}
	for (int i = 0; i < 4; i++)
	{
		transYearSubstr[i] = termData.transactionDate[i + 6];
	}
	int expMonth = atoi(expMonthSubstr);
	int expYear = 2000 + atoi(expYearSubstr);
	int transMonth = atoi(transMonthSubstr);
	int transYear = atoi(transYearSubstr);
	//compare the values.
	if (transYear <= expYear)
	{
		if (transYear < expYear)
		{
			validator = TERMINAL_OK;
			return validator;
		}
		else if(transMonth > expMonth)
		{
			validator = EXPIRED_CARD;
			return validator;
		}
		else
		{
			validator = TERMINAL_OK;
			return validator;
		}
	}
	else
	{
		validator = EXPIRED_CARD;
		return validator;
	}

}


EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	//luhn algorithm verification.
	EN_terminalError_t validator;
	int length = strlen(cardData->primaryAccountNumber);
	int even = 1;
	int sum = 0;
	for (int i = 0; i < length; i++)
	{
		if (even == 1)
		{
			int tmp = ((int)cardData->primaryAccountNumber[i] - 48) * 2;
			if (tmp >= 10)
			{
				tmp -= 10;
				tmp += 1;
			}
			sum += tmp;
		}
		else
		{
			int tmp = ((int)cardData->primaryAccountNumber[i] - 48);
			sum += tmp;
		}
		even = !even;
	}
	if (sum % 10 == 0)
	{
		validator = TERMINAL_OK;
		return validator;
	}
	else
	{
		validator = INVALID_CARD;
		return validator;
	}
}



EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t validator;
	printf("Please Enter the transaction Amount: ");
	scanf_s("%f", &termData->transAmount);
	if (termData->transAmount <= 0 || NULL)
	{
		validator = INVALID_AMOUNT;
		int gc;
		do {
			gc = getchar();
			if (gc == EOF) exit(EXIT_FAILURE);
		} while (gc != '\n');
		return validator;
	}
	else
	{
		validator = TERMINAL_OK;
		return validator;
	}
	
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t validator;
	printf("Please Enter the MAX Amount: ");
	scanf_s("%f", &termData->maxTransAmount);
	if (termData->maxTransAmount <= 0 || NULL)
	{
		validator = INVALID_MAX_AMOUNT;
		int gc;
		do {
			gc = getchar();
			if (gc == EOF) exit(EXIT_FAILURE);
		} while (gc != '\n');
		return validator;
	}
	else
	{
		validator = TERMINAL_OK;
		return validator;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	EN_terminalError_t validator;
	if(termData->transAmount <= termData->maxTransAmount)
	{ 
		validator = TERMINAL_OK;
		return validator;
	}
	else
	{
		validator = EXCEED_MAX_AMOUNT;
		return validator;
	}
}