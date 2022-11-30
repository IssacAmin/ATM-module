#include <stdio.h>
#include <stdlib.h>
#include "App.h"
#include "card.h"
#include "server.h"
#include "terminal.h"

//global variables
ST_cardData_t cardData;
ST_terminalData_t terminalData;
ST_transaction_t transactionData;

//auxilary function.
void checkCardMSG(EN_cardError_t MSG)
{
	switch (MSG)
	{
	case WRONG_NAME:
		printf("Wrong Name Please Try Again...\n");
		break;
	case WRONG_EXP_DATE:
		printf("Wrong Expiry date Please Try Again...\n");
		break;
	case WRONG_PAN:
		printf("Wrong PAN Please Try Again...\n");
		break;
	default:
		break;
	}
}

void checkTerminalMSG(EN_cardError_t MSG)
{
	switch (MSG)
	{
	case WRONG_DATE:
		printf("Wrong Date Please Try Again...\n");
		break;
	case EXPIRED_CARD:
		printf("Expired CARD... EXITING PROGRAM\n");
		break;
	case INVALID_CARD:
		printf("INVALID PAN... EXITING PROGRAM\n");
		break;
	case INVALID_AMOUNT:
		printf("INVALID AMOUNT...  TRY AGAIN\n");
		break;
	case EXCEED_MAX_AMOUNT:
		printf("EXCEED MAX ACCOUNT... EXITING PROGRAM\n");
		break;
	case INVALID_MAX_AMOUNT:
		printf("INVALID  MAX AMOUNT...  TRY AGAIN\n");
		break;
	default:
		break;
	}
}




void appStart(void)
{
	EN_cardError_t cardMSG;
	EN_terminalError_t terminalMSG;
	EN_transState_t serverMSG;
	EN_serverError_t servermessege;
	int choice;
	char tmp[2] = "1";
	printf("Enter 1 to make a transaction - 2 to get transaction data by sequence Number: ");
	int result = gets(tmp);
	choice =atoi(tmp);
	if (choice == 1)
	{
		do
		{
			cardMSG = getCardHolderName(&cardData);
			checkCardMSG(cardMSG);

		} while (cardMSG != CARD_OK);
		do
		{
			cardMSG = getCardExpiryDate(&cardData);
			checkCardMSG(cardMSG);

		} while (cardMSG != CARD_OK);
		do
		{
			cardMSG = getCardPAN(&cardData);
			checkCardMSG(cardMSG);

		} while (cardMSG != CARD_OK);
		do
		{
			terminalMSG = getTransactionDate(&terminalData);
			checkTerminalMSG(terminalMSG);
		} while (terminalMSG != CARD_OK);

		terminalMSG = isCardExpired(cardData, terminalData);
		if (terminalMSG == EXPIRED_CARD)
		{
			
			checkTerminalMSG(terminalMSG);
		}
		else
		{
			terminalMSG = isValidCardPAN(&cardData);
			if (terminalMSG == INVALID_CARD)
				checkTerminalMSG(terminalMSG);
			else
			{
				do
				{
					terminalMSG = getTransactionAmount(&terminalData);
					checkTerminalMSG(terminalMSG);

				} while (terminalMSG != CARD_OK);
				do
				{
					terminalMSG = setMaxAmount(&terminalData);
					checkTerminalMSG(terminalMSG);

				} while (terminalMSG != CARD_OK);

				terminalMSG = isBelowMaxAmount(&terminalData);
				if (terminalMSG == EXCEED_MAX_AMOUNT)
					checkTerminalMSG(terminalMSG);
				else
				{

					transactionData.cardHolderData = cardData;
					transactionData.terminalData = terminalData;
					serverMSG = recieveTransactionData(&transactionData);
					if (serverMSG != APPROVED)
					{
						if (serverMSG == DECLINED_INSUFFECIENT_FUND)
							printf("Insufficient FUNDS... EXITING PROGRAM\n");
						else if (serverMSG == DECLINED_STOLEN_CARD)
							printf("CARD IS BLOCKED... EXITING PROGRAM\n");
						else if (serverMSG == FRAUD_CARD)
							printf("CARD NOT FOUND... EXITING PROGRAM\n");
						else if (serverMSG == INTERNAL_SERVER_ERROR)
							printf("INTERNAL SERVER ERROR... EXITING PROGRAM\n");
					}
					else if(serverMSG == APPROVED)
						printf("Transaction complete.\n");

				}

			}
		}



	}
	else if (choice == 2)
	{
		int sequence;
		printf("Enter the sequence Number: ");
		int result =scanf("%d", &sequence);
		servermessege = getTransaction(sequence, &transactionData);
		if (servermessege == TRANSACTION_NOT_FOUND)
			printf(" Transaction Not found...\n");
		else if (servermessege == INTERNAL_SERVER_ERROR)
			printf("Internal Server Error...\n");
	}
}
