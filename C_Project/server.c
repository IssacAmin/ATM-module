#include <stdio.h>
#include <string.h>
#include "server.h"
#include <stdlib.h>

//accounts Database
ST_accountsDB_t accountsDB[255] =
{
	{.balance = 2000.0,.state = RUNNING,.primaryAccountNumber = "8989374615436851"},
	{.balance = 100000.0,.state = BLOCKED, .primaryAccountNumber = "5807007076043875"},
	{.balance = 500.0,.state = RUNNING,.primaryAccountNumber =  "5255616295993335"},
	{.balance = 5000.0, .state = RUNNING, .primaryAccountNumber = "6761341744968995"},
	{.balance = 0 ,.state = BLOCKED, .primaryAccountNumber = "5584210620099729"},
};
int accoutsIndex = 0;



EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_serverError_t validator;
	int found = 0;
	//search for a match
	for (accoutsIndex = 0; accoutsIndex < 255; accoutsIndex++)
	{
		if (isValidAccount(transData->cardHolderData, accountsDB[accoutsIndex]) == SERVER_OK)
		{
			if (isBlockedAccount(&accountsDB[accoutsIndex]) == SERVER_OK)
			{
				if (isAmountAvailable(&transData->terminalData) == SERVER_OK)
				{
					if (saveTransaction(transData) == SERVER_OK)
					{
						accountsDB[accoutsIndex].balance = accountsDB[accoutsIndex].balance - transData->terminalData.transAmount;
						printf("Your new balance is: %f\n", accountsDB[accoutsIndex].balance);
						validator = APPROVED;
						return validator;
					}
					else
					{
						validator = INTERNAL_SERVER_ERROR;
						return validator;
					}
				}
				else
				{
					validator = DECLINED_INSUFFECIENT_FUND;
					return validator;
				}
			}
			else
			{
				validator = DECLINED_STOLEN_CARD;
				return validator;
			}
		}
		else
			continue;
	}
		validator = FRAUD_CARD;
		return validator;
		
	
}


EN_serverError_t isValidAccount(ST_cardData_t cardData, ST_accountsDB_t accountRefrence)
{
	EN_serverError_t validator;
	if (strcmp(cardData.primaryAccountNumber , accountRefrence.primaryAccountNumber) == 0)
	{
		validator = SERVER_OK;
		return validator;
	}
	else
	{
		validator = ACCOUNT_NOT_FOUND;
		return validator;
	}

}


EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	EN_serverError_t validator;
	if (accountRefrence->state == BLOCKED)
	{
		validator = BLOCKED_ACCOUNT;
		return validator;
	}
	else
	{
		validator = SERVER_OK;
		return validator;
	}
		
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	EN_serverError_t validator;
	if (termData->transAmount > accountsDB[accoutsIndex].balance)
	{
		validator = LOW_BALANCE;
		return validator;
	}
	else
	{
		validator = SERVER_OK;
		return validator;
	}
}


EN_serverError_t saveTransaction(ST_transaction_t* transData) 
{
	EN_serverError_t validator; 
	FILE* DB = fopen("transactionsDB.txt", "a");
	if (DB == NULL)
	{
		validator = SAVING_FAILED;
		return validator;
	}
	else
	{
			FILE* sequence = fopen("transactionsDB.txt", "r");
			char c;
			int count = 1;		
			if (sequence == NULL)
			{
				printf("Could not open file ");
			}
			else
			{
				for (c = getc(sequence); c != EOF; c = getc(sequence))
				{
					if (c == '\n')
						count = count + 1;
				}
				transData->transactionSequenceNumber = count;
				fclose(sequence);
			}
			fprintf(DB, "%s, %s, %s, %f, %f, %s, %d, %d\n",
				transData->cardHolderData.cardHolderName,
				transData->cardHolderData.primaryAccountNumber,
				transData->cardHolderData.cardExpirationDate,
				transData->terminalData.transAmount,
				transData->terminalData.maxTransAmount,
				transData->terminalData.transactionDate,
				(int)transData->transState,
				transData->transactionSequenceNumber);
			validator = SERVER_OK;
			return validator;
		
	}
	fclose(DB);
}


EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)

{
	EN_serverError_t validator;
	FILE* DB = fopen("transactionsDB.txt", "r");


	if (DB == NULL)
	{
		validator = INTERNAL_SERVER_ERROR;
		return validator;
	}
	else
	{
		int counter = 0;
		int transaction = 0;
		char buffer[1024];
		while (fgets(buffer, 1024, DB))
		{
			counter = 0;
			char* value = strtok(buffer, ",");
			
			
			while (value) 
			{
				switch (counter)
				{
				case 0:
					strcpy(transData->cardHolderData.cardHolderName, value);
					break;
				case 1:
					strcpy(transData->cardHolderData.primaryAccountNumber , value);
					break;
				case 2:
					strcpy(transData->cardHolderData.cardExpirationDate, value);
					break;
				case 3:
					transData->terminalData.transAmount = atof(value);
					break;
				case 4:
					transData->terminalData.maxTransAmount = atof(value);
					break;
				case 5:
					strcpy(transData->terminalData.transactionDate, value);
				case 6:
					transData->transState = atoi(value);
				case 7:
					transData->transactionSequenceNumber = atoi(value);
					if (transactionSequenceNumber == transData->transactionSequenceNumber)
					{
						validator = SERVER_OK;
						printf("%s %s %s %f %f %s %d %d\n",
							transData->cardHolderData.cardHolderName,
							transData->cardHolderData.primaryAccountNumber,
							transData->cardHolderData.cardExpirationDate,
							transData->terminalData.transAmount,
							transData->terminalData.maxTransAmount,
							transData->terminalData.transactionDate,
							transData->transState,
							transData->transactionSequenceNumber);
						return validator;
					}
					break;
				default:
					break;
				}
				value = strtok(NULL, ",");
				counter++;
			}
			
			transaction++;
		}		
		fclose(DB);
		validator = TRANSACTION_NOT_FOUND;
		return validator;	
	}	
}




