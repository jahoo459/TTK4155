/*
 * menuLib.c
 *
 * Created: 9/29/2017 9:00:21 AM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <stddef.h>
#include "menuLib.h"

//******************************************************************************************
menuItemNode_t *mainMenu = NULL; //pointer pointing to the first main menu item
menuItemNode_t *currItem = NULL; //current item, at the beginning will point at the first item (mainMenu)
//******************************************************************************************
char *mainMenuItems[] = {"New Game", "Highscores", "Joystick Calibration"};


void MENU_init()
{
	//create main menu
	for(int i = 0; i < 3/*sizeof(mainMenuItems)/sizeof(char*)*/; i++)
	{
		MENU_addMenuItem(mainMenuItems[i], currItem, mainMenu);
	}
	
}

void MENU_addMenuItem(char* name, menuItemNode_t *parent, menuItemNode_t* currentMenu)
{
	if(mainMenu == NULL) //create first item for this menu
	{
		mainMenu = malloc(sizeof(menuItemNode_t));
		mainMenu->text = name;
		mainMenu->next = NULL;
		mainMenu->parentMenu = mainMenu;
		currItem = mainMenu;
	}
	else //next element
	{
		parent->next = malloc(sizeof(menuItemNode_t));
		parent->next->text = name;
		parent->next->next = NULL;
		parent->next->parentMenu = mainMenu;
		currItem = parent->next; //now current item (last item) points at the just created item
	}
	
}

void MENU_printMenuItem(menuItemNode_t* item)
{
    if(item != NULL)
    {
	    printf("%s Parent: %s \n", item->text, item->parentMenu->text);
    }
    else
    {
        printf("NULL pointer...");
    }
}

void MENU_printMenu(menuItemNode_t* firstItem)
{	
	menuItemNode_t *item2print;
	item2print = firstItem;
	
	while(item2print != NULL)
	{
		MENU_printMenuItem(item2print);
		item2print = item2print->next;
	}
}


void MENU_moveUp()
{
	
}

void MENU_moveDown()
{
	
}








