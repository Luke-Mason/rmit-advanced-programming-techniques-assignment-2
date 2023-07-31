/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Luke Mason
 * Student Number   : s3630120
 * Course Code      : 1076
 * Program Code     : BP096
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **
 
 **
 * SearchStockId goes through the system nodes and searches for a match in id 
 * and returns the node count position
 **/
int searchStockId(struct ppd_system * system, char *id)
{
	struct ppd_node * current;
	int i;
	current = system->item_list->head;
	for(i = 1; i<= system->item_list->count; i++)
	 {
		if(strcmp(id,current->data->id) == 0)
		{
			return i;
		}
		if(system->item_list->count != i) {current = current->next;}
	 }
	return 0;
}
BOOLEAN display_items(struct ppd_system * system)/*Have not done the space for name*/
{
	struct ppd_stock * stock;
	struct ppd_node * current;
	char lines[20][5+40+255+5+4+2];
	char line[5+40+255+5+4+2];
	char * subString;
	char * subString2;
	int i, j, name_length = 30;
	char temp[311];
	char temp2[311];
	char temp3[311];
	/*struct ppd_node * previous;
	BOOLEAN moved = FALSE;
	current = system->item_list->head;
	previous = NULL;
	for(i = 1; i < system->item_list->count; i++) 
	{
		if(strcmp(current->data->name,current->next->data->name) > 0)
		{
			moved = TRUE;
			if(previous != NULL)
			{
				previous->next = current->next;
			}
			current->next = current->next->next;
			previous->next->next = current;
		}
		if(moved)
		{
			previous = previous->next;
		}
		if(!moved == TRUE)
		{
			previous = current;
			current = current->next;
		}
	}*/
	current = system->item_list->head;
	
	for(i = 0; i< system->item_list->count; i++)
	{
		if(name_length < strlen(current->data->name))
		{
			name_length = strlen(current->data->name);
		}
		printf("before\n");
		if(system->item_list->count-1 != i) {current = current->next;}
		printf("after\n");
	}
	for(i = 0; i< system->item_list->count; i++)
	{
		stock = current->data;
		sprintf(line,"%s | %-*s| %d        | $ %d.%.2d\n",stock->id,name_length, stock->name, stock->on_hand,stock->price.dollars, stock->price.cents);
		strcpy(lines[i],line);
		if(system->item_list->count-1 != i) {current = current->next;}
	}
	
	for(i = 0; i< system->item_list->count; i++)/*Sorting the String array in order by name*/
	{
		memcpy(temp2,lines[i],sizeof(lines[i]));
		strtok(temp2, "|");
		subString = strtok(NULL,"|");
		if(i != 0)
		{
			for(j=i-1; j> -1; j--)
			{
				memcpy(temp3,lines[j],sizeof(lines[j]));
				strtok(temp3, "|");
				subString2 = strtok(NULL,"|");
				if(strcmp(subString2,subString)>0)
				{
					memcpy(temp,lines[j],sizeof(lines[j]));
					memcpy(lines[j],lines[i],sizeof(lines[i]));
					memcpy(lines[i],temp,sizeof(temp));
				}
				else
				{
					subString2 = subString;
					j=0;
				}
			}
		}
		else
		{
			subString2 = subString;
		}
	}
	printf("Items Menu\n\n\n");
	printf("%s    | %-*s| %s | %s","ID",name_length,"Name","Available","Price\n");
	printf("------------------------------------------------------------------------------\n");
	 for(i = 0;i< system->item_list->count; i++)/*Printing out array*/
	 {
		printf("%s",lines[i]);
	 }
	 printf("\n\n");
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
	char id[7];
	char cent[6];
	int cents, itemPosition, i;
	double total, given, givenTotal, extra;
	char *end;
	struct ppd_node * current;
	OTHER other;
	BOOLEAN exit;
	
	printf("\n\nPurchase Item\n");
	printf("---------------\n");
	do
	{
		printf("Please enter the id of the item you wish to purchase(*Case Sensitive*): ");
		exit = TRUE;
		other = getInput(id, sizeof(id), stdin);
		if(other == EXIT)
		{
			printf("\nExiting...\n\n");
			return FALSE;
		}
		if(other == False)
		{
			printf("ERROR: Invalid ID, try again\n");
			exit = FALSE;
			continue;
		}
		itemPosition = searchStockId(system,id);
		if(itemPosition==0)
		{
			printf("ERROR: This item \"%s\" does not exist in the system\n",id);
			exit = FALSE;
			continue;
		}
	}
	while(!exit);
	current = system->item_list->head;
	for(i = 1; i < itemPosition; i++) 
	{
		current = current->next;
	}
	extra = current->data->price.cents;
	total = current->data->price.dollars + (extra/100);
	givenTotal = total;
	printf("You have selected %s, %s. This will cost you $%.2f\n",current->data->name,current->data->desc,total);
	printf("Please hand over the money - type in the value of each note/coin in cents.(e.g 100)\n");
	printf("Press enter on an empty line to cancel this purchase: ");
	do
	{
		if(!exit)
		{
			printf("You still need to give us $%.2f: ",total);
		}
		exit = FALSE;
		other = getInput(cent, sizeof(cent), stdin);
		if(other == EXIT)
		{
			printf("\nExiting...\n\n");
			return FALSE;
		}
		if(other ==False)
		{
			printf("ERROR: Too much data entered, try again \n\n\n");
			continue;
		}
		if (strcmp(cent, "\n") == 0)
		{
			printf("Purchase Cancelled, returning to main menu ... \n\n");
			givenTotal = givenTotal - total;
			if(givenTotal != 0){printf("returning $%.2f as change ...\n\n",givenTotal);}
			return FALSE;
		}
		cents = strtol(cent,&end,10);
		given = ((double)cents/100);
/*Checks if the 'given' money is a correct denomination of money*/
		switch(cents)
		{
			case 5: total = total - 0.05;break;
			case 10: total = total - 0.10;break;
			case 20: total = total - 0.20;break;
			case 50: total = total - 0.50;break;
			case 100: total = total - 1.00;break;
			case 200: total = total - 2.00;break;
			case 500: total = total - 5.00;break;
			case 1000: total = total - 10.00;break;	
			default: printf("Error: $%.2f is not a valid denomination of money.\n",given);
			exit = FALSE;
			continue;
		}
/*checking if amount owing is negative or paid exact*/	
		if(total < 0)
		{
			total = (total * -1);
			printf("Thank you. Here is your %s, and your change of $%.2f\n",current->data->name,total);
			printf("please come back soon\n");
			exit = TRUE;
			current->data->on_hand--;
		}
		else if(total == 0)
		{
			printf("Thank you. Here is your %s\n",current->data->name);
			printf("please come back soon\n");
			exit = TRUE;
			current->data->on_hand--;
		}
	}
	while(!exit);

    return TRUE;
}
BOOLEAN save_system(struct ppd_system * system)
{
    const char * filename;
	struct ppd_node * current;
	struct ppd_stock * stock;
	FILE *fp;
	int i;
	filename = system->stock_file_name;
	current = system->item_list->head;
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("Error: Couldn't open the file!\n");
		return FALSE;
	}
	for(i = 0; i < system->item_list->count; i++)/*Saves all node information by printing it directly into file*/
	{
		stock = current->data;
		fprintf(fp, "%s|%s|%s|%d.%02d|%d\n", stock->id, stock->name, stock->desc, stock->price.dollars, stock->price.cents, stock->on_hand);
		if(system->item_list->count-1 != i) {current = current->next;}
	}
	system_free(system);
	fclose(fp);
    return TRUE;
}
BOOLEAN add_item(struct ppd_system * system)/*Get seg fault when ".45" is entered in price*/
{
    char itemName[NAMELEN +2];
	char description[DESCLEN +2];
	char id[IDLEN +2];
	char price[7];
	char * end;
	char * stringCheck;
	char line[80];
	struct ppd_stock * data;
	int count;
	float money;
	int dollars = 1;
	int cents = 0;
	OTHER other;
	BOOLEAN out = TRUE;
	count = system->item_list->count;
	sprintf(line,"This new meal item will have the Item id of I%04d\n",count+1);
	printf("%s",line);
	line[strlen(line)-1]='\0';
	strcpy(id, &line[strlen(line)-5]);
	do
	{
		out = TRUE;
		printf("Enter in the item name: ");
		other = getInput(itemName, sizeof(itemName), stdin);
		if(other == EXIT)
		{
			printf("\nExiting...\n\n");
			return FALSE;
		}
		if(other == False)
		{
			out = FALSE;
			printf("Error: Invalid Input, Try again\n");
			continue;
		}
		if(strlen(itemName) < 1)
		{
			printf("Error: no name given\n");
			out = FALSE;
		}
	}
	while(!out);

	do
	{
		out = TRUE;
		printf("Enter in the item description: ");
		other = getInput(description, sizeof(description), stdin);
		if(other == EXIT)
		{
			printf("\nExiting...\n\n");
			return FALSE;
		}
		if(other == False)
		{
			out = FALSE;
			printf("Error: Invalid Input, Try again\n");
			continue;
		}
		if(strlen(description) < 1)
		{
			printf("Error: no description given\n");
			out = FALSE;
		}
	}
	while(!out);
	do
	{
		out = TRUE;
		end = NULL;
		printf("Enter in the price of this item(eg 8.00): ");
		other = getInput(price, sizeof(price), stdin);
		if(other == EXIT)
		{
			printf("\nExiting...\n\n");
			return FALSE;
		}
		if(other == False)
		{
			out = FALSE;
			printf("Error: Invalid Input, Try again\n");
			continue;
		}
/*Error checking for price of item*/
		money = strtol(price,&end,10);
		if(money > 99.95)
		{
			printf("ERROR: Cost of the item exceeds to maximum price of $99.95\n");
			out = FALSE;
			printf("Try again\n");
			continue;
		}
		stringCheck = strtok(price,".");
		dollars = strtol(stringCheck,&end,10);
		if(strlen(end) != (size_t)0)
		{
			printf("Error: price in dollars contains non-numeric data\n");
			out = FALSE;
			printf("Try again\n");
			continue;
		}
		if(strlen(stringCheck) < 1)
		{
			printf("ERROR: Price in dollars is not set for the Item\n");
			out = FALSE;
			printf("Try again\n");
			continue;
		}
		stringCheck = strtok(NULL,".");
		cents = strtol(stringCheck,&end,10);
		if(strlen(end) != (size_t)0)
		{
			printf("Error: price in dollars contains non-numeric data\n");
			out = FALSE;
			printf("Try again\n");
			continue;
		}
		if(strlen(stringCheck) < 2 || strlen(stringCheck) > 2)
		{
			printf("ERROR: Price in cents is not valid for the Item\n");
			out = FALSE;
			printf("Try again\n");
			continue;
		}
		if(cents%5 != 0)
		{
			printf("ERROR: The cents cost of the item is not valid\n");
			out = FALSE;
			printf("Try again\n");	
			continue;
		}
	}
	while(!out);\
	/*Getting to the end of the linked list to add a new node*/
	data = addToList(system->item_list);
	if(data == NULL)
	{
		return FALSE;
	}
	/*printf("dollars - %d\n",dollars);
	printf("cents - %d\n",cents);
	printf("Cost - %f\n",money);
	printf("stock level - %d\n",DEFAULT_STOCK_LEVEL);
	printf("description - %s\n", description);
	printf("Item Name - %s\n", itemName);
	printf("ID - %s\n", id);*/

/*Assigning data to the node data*/
	data->price.dollars = dollars;
	data->price.cents = cents;
	data->on_hand = DEFAULT_STOCK_LEVEL;
	strcpy(data->desc, description);
	strcpy(data->name, itemName);
	strcpy(data->id, id);
	printf("\nThis item \"%s - %s\" has now been added to the menu\n\n",itemName, description);
	return TRUE;
}
BOOLEAN remove_item(struct ppd_system * system)
{
	struct ppd_node * current;
	struct ppd_node * previous;
	OTHER other;
	char id[7];
	char desc[255];
	BOOLEAN out;
	int itemPosition, i;
	do
	{
		out = TRUE;
		printf("Enter the item id of the item to remove from the menu: ");
		other = getInput(id, sizeof(id), stdin);
		if(other == EXIT)
		{
			printf("\nExiting...\n\n");
			return FALSE;
		}
		if(other == False)
		{
			printf("ERROR: Invalid ID, try again\n");
			out = FALSE;
			continue;
		}
		itemPosition = searchStockId(system,id);
		if(itemPosition==0)
		{
			printf("ERROR: This item \"%s\" does not exist in the system\n",id);
			out = FALSE;
			continue;
		}
	}
	while(!out);
	previous = NULL;
	current = system->item_list->head;
	for(i = 1; i < itemPosition; i++) 
	{
		previous = current;
		current = current->next;
	}
	strcpy(id, current->data->id);
	strcpy(desc, current->data->desc);
	if(itemPosition == 1)
	{
		system->item_list->head = current->next;
		current->next = NULL;
	}
	else
	{
		previous->next = current ->next;
		current->next = NULL;
	}
	printf("\n\"%s - %s\" has been removed from the system.\n\n",id,desc);
	system->item_list->count--;
	free(current->data);
	free(current);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
	struct ppd_node * current;
    int i;
	current = system->item_list->head;
	for(i = 0; i< system->item_list->count; i++)
	{
		current->data->on_hand = DEFAULT_STOCK_LEVEL;
		if(system->item_list->count-1 != i) {current = current->next;}
	}
	printf("\nAll stock has been reset to the default level of %d\n\n",DEFAULT_STOCK_LEVEL);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;
}
