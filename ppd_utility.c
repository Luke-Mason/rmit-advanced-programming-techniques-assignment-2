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

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/
void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}
/**
 * Gets the input of user and does error checks and returns result
 * Implemented so that repeated code is not made
 **/
OTHER getInput(char * save, int size, FILE * stream)
{
	if(fgets(save, size, stream) == NULL)
	{
		return FALSE;
	}
	if(save[strlen(save)-1] != '\n')
	{
		read_rest_of_line();
		return FALSE;
	}
	if (strcmp(save, "\n") == 0)
	{
		save = "\n";
		return EXIT;
	}
	save[strlen(save)-1]='\0';
	return TRUE;
}
/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
	system->item_list = init_item_list();
	system->item_list->count = 0;
	if(system->item_list == NULL)
	{
		return FALSE;
	}
    return TRUE;
}

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system * system, const char * filename)
{
	FILE * fp;
	int cents2, dollars2, quantity2;
	double money2;
	char stockLine[MAX_LENGTH+2];
	struct ppd_stock * data;
	char *money;
	char *dollars;
	char *cents;
	char *end;
	char *quantity;
	char *stringCheck;
	fp = fopen(filename, "r");
	if(fp==NULL)
	{
		printf("ERROR: File %s could not be read\n",filename);
		return FALSE;
	}
	system->stock_file_name = filename;
/*grabbing each line in the file and allocating memory towards a node to store the grabbed information in*/
	while(getInput(stockLine,MAX_LENGTH+2,fp)==True)
	{
		/*
		void push(struct ppd_node *nodePointer, nodePointer->data) 
		{
			ppd_node * current = head;
			while (current->next != NULL) 
			{
				current = current->next;
			}
			current->next = malloc(sizeof());
			current->next->data = nodePointer->data;
			current->next->next = NULL;
		}
		*/
		data = addToList(system->item_list);
		if(data == NULL)
		{
			return FALSE;
		}
/*ERROR checking for each line of file*/
		stringCheck = strtok(stockLine,"|");
		if(strlen(stringCheck) != IDLEN)
		{
			printf("ERROR: ID string length is not valid for stock item %d\n"
			,system->item_list->count+1);
			return FALSE;
		}
		if(findId(system->item_list, stringCheck)==TRUE)
		{
			strcpy(data->id, stringCheck);	
		}
		else
		{
			printf("ERROR: Repeated id - %s \n",stringCheck);
			return FALSE;
		}
		stringCheck = strtok(NULL,"|");
		if(strlen(stringCheck) > NAMELEN || strlen(stringCheck) < 1)
		{
			printf("ERROR: Name string length is not valid for stock item %d\n"
			,system->item_list->count+1);
			return FALSE;
		}
		strcpy(data->name,stringCheck);

		stringCheck = strtok(NULL,"|");
		if(strlen(stringCheck) > DESCLEN || strlen(stringCheck) < 1)
		{
			printf("ERROR: Description string length is not valid for stock item %d\n"
			,system->item_list->count+1);
			return FALSE;
		}
		strcpy(data->desc, stringCheck);
		
		stringCheck = strtok(NULL,"|");
		if(strlen(stringCheck) > 5 || strlen(stringCheck) < 4)
		{
			printf("ERROR: Money is not valid for stock item %d\n",system->item_list->count+1);
			return FALSE;
		}
		money = stringCheck;

		stringCheck = strtok(NULL,"|");
		if(strlen(stringCheck) < 1)
		{
			printf("ERROR: Quantity is not set for stock item %d\n",system->item_list->count+1);
			return FALSE;
		}
		quantity = stringCheck;

		money2 = strtol(money, &end, 10);

		if(money2 > 99.95)
		{
			printf("ERROR: Cost of item %s exceeds to maximum price of $99.95\n",data->id);
			return FALSE;
		}
		stringCheck = strtok(money,".");
		if(strlen(stringCheck) < 1)
		{
			printf("ERROR: Price in dollars is not set for stock item %d\n",system->item_list->count+1);
			return FALSE;
		}
		dollars = stringCheck;

		stringCheck = strtok(NULL,".");
		if(strlen(stringCheck) < 2)
		{
			printf("ERROR: Price in cents is not valid for stock item %d\n",system->item_list->count+1);
			return FALSE;
		}
		cents = stringCheck;
	
		dollars2 = strtol(dollars, &end, 10);
		if(strlen(end) != (size_t)0)
		{
			printf("Error: price in dollars %s contains non-numeric data\n",data->id);
			return FALSE;
		}
		cents2 = strtol(cents, &end, 10);
		if(strlen(end) != (size_t)0)
		{
			printf("Error: price in Cents of item %s contains non-numeric data\n",data->id);
			return FALSE;
		}
		if(cents2%5 != 0)
		{
			printf("ERROR: The cents cost of item %s is not valid\n",data->id);
			return FALSE;	
		}
		quantity2 = strtol(quantity, &end, 10);
		if(strlen(end) != (size_t)0)
		{
			printf("Error: Quantity of item %s contains non-numeric data\n",data->id);
			return FALSE;
		}
		if(quantity2 > 99)
		{
			printf("ERROR: There is too much quantity of item %s to hold in the vending machine\n"
			,data->id);
			return FALSE;
		}
/*Assigning data values to node*/
		data->price.dollars = dollars2;

		data->price.cents = cents2;

		data->on_hand = quantity2;

		
	}

	fclose(fp);
    return TRUE;
}

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system * system, const char * filename)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    return FALSE;

}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	struct ppd_node * current;
	struct ppd_node *next;
	current = system->item_list->head;
	
	while (current != NULL)
    {
      free (current->data);
      next = current->next;
      free (current);
	current = next;
    }
	free(system->item_list);
}

/*valgrind --leak-check=full --track-origins=yes --tool=memcheck ./ppd stock.dat*/
