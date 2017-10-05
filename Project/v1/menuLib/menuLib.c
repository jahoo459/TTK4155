/*
 * menuLib.c
 *
 * Created: 9/29/2017 9:00:21 AM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stddef.h>
#include <util/delay.h>
#include "menuLib.h"
#include "..\JoystickLib\JoystickLib.h"
#include "../oledLib/oledLib.h"

//******************************************************************************************
menuItemNode_t *mainMenu = NULL; //pointer pointing to the first main menu item
menuItemNode_t *difficultyMenu = NULL; //pointer pointing to the first main menu item
menuItemNode_t *currItem = NULL; //current item, at the beginning will point at the first item (mainMenu)

uint8_t menuActiveFlag = 0; //When this flag gets 0 waitForInput function will stop so menu will be inactive

static int currentPosition = 0; //Current menu position read from the Joystick

int menuFrameOffset = 10; //Columns from left frame border
//******************************************************************************************
char *mainMenuItems[] = {"New Game", "Difficulty", "Highscores", "Joy Calib", "Debugging", "Info"};
char *difficultyMenuItems[] = {"easy", "medium", "hard"};

void MENU_init(menuItemNode_t* menu, char *menuItems[], size_t size)
{
	//create main menu
	for(size_t i = 0; i < size; i++)
	{
		MENU_addMenuItem(menuItems[i], currItem, menu);
		printf("Index %d, currItem: %d, Menu: %d\n",i, &currItem, &menu);
	}
}

void MENU_addMenuItem(char* name, menuItemNode_t *parent, menuItemNode_t* currentMenu)
{
	printf("parent: %d, currentMenu: %d\n", &parent, &currentMenu);
	if(mainMenu == NULL) //create first item for this menu
	{
		currentMenu = malloc(sizeof(menuItemNode_t));
		currentMenu->text = name;
		currentMenu->next = NULL;
		currentMenu->parentMenu = currentMenu;
		currItem = currentMenu;
	}
	else //next element
	{
		parent->next = malloc(sizeof(menuItemNode_t));
		parent->next->text = name;
		parent->next->next = NULL;
		parent->next->parentMenu = currentMenu;
		currItem = parent->next; //now current item (last item) points at the just created item
	}
	printf("After parent: %d, currentMenu: %d\n", &parent, &currentMenu);
}

void MENU_printMenuItem(menuItemNode_t* item, int lineNumber)
{
	if(item != NULL)
	{
		OLED_goto(lineNumber, menuFrameOffset);
		OLED_printString(item->text);
		//printf("%s\n", item->text);
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
	int lineNumber = 0;
	while(item2print != NULL)
	{
		MENU_printMenuItem(item2print, lineNumber);
		item2print = item2print->next;
		lineNumber++;
	}
}

void MENU_waitForInput()
{
	OLED_moveArrow(currentPosition);
	JOY_direction_t currDir;
	
	while(menuActiveFlag)
	{
		currDir = JOY_getDirection();
		switch(currDir){
			case LEFT:
			MENU_moveLeft();
			break;
			
			case RIGHT:
			MENU_moveRight();
			break;
			
			case UP:
			MENU_moveUp();
			break;
			
			case DOWN:
			MENU_moveDown();
			break;
		}
		_delay_ms(1200);

	}
}

void MENU_moveUp()
{
	if(currentPosition >0)
	{
		currentPosition = currentPosition - 1;
		printf("Curr Pos Up: %d\n", currentPosition);
		OLED_moveArrow(currentPosition);
	}
}

void MENU_moveDown()
{
	if(currentPosition < MAX_MENU_SIZE -1)
	{
		currentPosition = currentPosition + 1;
		printf("Curr Pos Down: %d\n", currentPosition);
		OLED_moveArrow(currentPosition);
	}
}

void MENU_moveRight()
{
	//MENU_init(mainMenu, mainMenuItems);
	MENU_printMenu(mainMenu);
}

void MENU_moveLeft()
{
	currentPosition = 0;
	//MENU_init(mainMenu, mainMenuItems);
	MENU_printMenu(currItem->parentMenu);
}

void MENU_activate()
{
	menuActiveFlag = 1;
	//mainMenu = malloc(sizeof(menuItemNode_t));
	MENU_init(mainMenu, mainMenuItems, sizeof(mainMenuItems)/sizeof(char*));
	MENU_printMenu(mainMenu);
	MENU_waitForInput();
}












