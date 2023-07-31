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

#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    struct ppd_system system;
	BOOLEAN exit = TRUE;
	char input[3];
	int choice;
	char * end;
    /* uncomment this menu structure when you are ready to work on the 
     * menu system with function pointers
    struct menu_item menu[NUM_MENU_ITEMS];
    */
    /* validate command line arguments */
	if(argc == 1)
	{
		printf("ERROR: No arguments given\n");
		return EXIT_FAILURE;
	}
	if(argc > 2)
	{
		printf("ERROR: Too many arguments\n");
		return EXIT_FAILURE;
	}
    /* represents the data structures to manage the system */
    
    /* init the system */
	if(system_init(&system) == FALSE)
	{
		return EXIT_FAILURE;
	}
	
    /* load data */
	/*if(argc==2)
	{
		fp = fopen(argv[2], "r");
		fclose(fp);
	}*/
	if(!(load_stock(&system, argv[1])))
	{
		system_free(&system);
		return EXIT_FAILURE;
	}
    /* test if everything has been initialised correctly */

    /* initialise the menu system */
	/*init_menu(menu);*/
	
    /* loop, asking for options from the menu */
	do
	{
		printf("Main Menu:\n");
		printf("1.Display Items\n");
		printf("2.Purchase Items\n");
		printf("3.Save and Exit\n");
		printf("Administration-Only Menu:\n");
		printf("4.Add Item\n");
		printf("5.Remove Item\n");
		printf("6.Display Coins(Not Implemented)\n");
		printf("7.Reset Stock\n");
		printf("8.Reset Coins(Not Implemented)\n");
		printf("9.Abort Program\n");
		do
		{
			printf("Select your option (1-9): ");
			if(getInput(input, sizeof(choice), stdin)==False)
			{
				printf("\nERROR: That option is invalid, please try again\n\n");
				exit = TRUE;
				continue;
			}
			choice = strtol(input, &end, 10);
			if(choice < 1 || choice > 9)
			{
				printf("ERROR: That option is invalid, please try again\n");
				exit = TRUE;
				continue;
			}
			if(strlen(end) != (size_t)0)
			{
				printf("Error: That option conatins non-numeric data, try again\n");
				exit = FALSE;
				continue;
			}
		}
		while(!exit);
		exit = TRUE;
		/* run option option selected */
		switch(choice)
		{
			case 1: display_items(&system); break;
			case 2: purchase_item(&system); break;
			case 3: if(save_system(&system)==TRUE)
					{exit = FALSE;break;}else{printf("Save Failed!"); break;}
			case 4: add_item(&system); break;
			case 5: remove_item(&system); break;
			case 7: reset_stock(&system); break;
			case 9: printf("\nAborting\n\n");system_free(&system); exit = FALSE; break;
		}
	}
	/* until the user quits */
	while(exit);
    /* make sure you always free all memory and close all files 
     * before you exit the program
     */
	 

    return EXIT_SUCCESS;
}
