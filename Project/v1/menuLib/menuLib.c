/*
 * menuLib.c
 *
 * Created: 9/29/2017 9:00:21 AM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stddef.h>

#include "menuLib.h"
#include "..\JoystickLib\JoystickLib.h"
#include "../oledLib/oledLib.h"

//******************************************************************************************
menuNode_t *mainMenu = NULL; //pointer pointing to main menu
menuNode_t *difficultyMenu = NULL; //pointer pointing to difficulty menu 
menuNode_t *currentMenu = NULL;	//pointer pointing to currently displayed menu - for navigation
menuItemNode_t *currItem = NULL; //current item, at the beginning will point at the first item (mainMenu)

uint8_t menuActiveFlag = 0; //When this flag gets 0 waitForInput function will stop so menu will be inactive

static int currentPosition = 0; //Current menu position read from the Joystick

int menuFrameOffset = 10; //Columns from left frame border
//******************************************************************************************

void startGame()
{
	OLED_clear();
	OLED_goto(0,0);
	OLED_printString("START");
	
	OLED_goto(3,0);
	
	for(int i = 14; i > 0; i--)
	{
		OLED_printString("*");
		_delay_ms(500);
	}
	
	OLED_goto(5,0);
	OLED_printString("YUPII :)");
	_delay_ms(3000);
}

void MENU_buildMenus()
{
	//create main menu
	mainMenu = malloc(sizeof(menuNode_t));
	mainMenu->menuName = "MainMenu";
	mainMenu->parentMenu = NULL;
	mainMenu->noChilds = 5;
	
	//create DifficultyMenu
	difficultyMenu = malloc(sizeof(menuNode_t));
	difficultyMenu->menuName = "DifficultyMenu";
	difficultyMenu->parentMenu = mainMenu;
	difficultyMenu->noChilds = 3;
	
	//create menu items for created menus
	MENU_addMenuItem("New Game", mainMenu, NULL, 0);
	mainMenu->children[0] = currItem;
	mainMenu->children[0]->functionPtr = &startGame;
	
	MENU_addMenuItem("Difficulty", mainMenu, difficultyMenu, 1);
	mainMenu->children[1] = currItem;
	
	MENU_addMenuItem("Highscores", mainMenu, NULL, 0);
	mainMenu->children[2] = currItem;
	
	MENU_addMenuItem("Joy Calib", mainMenu, NULL, 0);
	mainMenu->children[3] = currItem;
	mainMenu->children[3]->functionPtr = &JOY_calibrate; //assign function to this item
	
	MENU_addMenuItem("Info", mainMenu, NULL, 0);
	mainMenu->children[4] = currItem;
	
	currItem = NULL;
	
	MENU_addMenuItem("easy", difficultyMenu, NULL, 0);
	difficultyMenu->children[0] = currItem;
	
	MENU_addMenuItem("medium", difficultyMenu, NULL, 0);
	difficultyMenu->children[1] = currItem;
	
	MENU_addMenuItem("hard", difficultyMenu, NULL, 0);
	difficultyMenu->children[2] = currItem;
}

void MENU_addMenuItem(char* name, menuNode_t* parentMenu, menuNode_t* childMenu, int hasChildMenu)
{
	if(currItem != NULL)
	{
		printf("Creating next item...\n");
		currItem = currItem->next;
	}

	currItem = malloc(sizeof(menuItemNode_t)); //first element in this menu
	currItem->parentMenu = parentMenu;
	currItem->name = name;
	
	if(hasChildMenu)
	{
		currItem->childMenu = childMenu;
		currItem->hasChildMenu = 1;
	}
	else
	{
		currItem->childMenu = NULL;
		currItem->hasChildMenu = 0;
	}
}

void MENU_printMenuItem(menuItemNode_t* item, int lineNumber)
{
	if(item != NULL)
	{
		OLED_goto(lineNumber, menuFrameOffset);
		OLED_printString(item->name);
		//printf("%s\n", item->text);
	}
	else
	{
		printf("NULL pointer...");
	}
}

void MENU_printMenu(menuNode_t* menu, int noElements)
{
	//clear the OLED display
	OLED_clear();
	//print arrow
	OLED_moveArrow(0);
	
	for(int i = 0; i < noElements; i++)
	{
		MENU_printMenuItem(menu->children[i], i);
	}
	
	//currentMenu points to currently printed menu
	currentMenu = menu;
	
	currentPosition = 0;
	currItem = currentMenu->children[currentPosition];
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
		_delay_ms(300);

	}
}

void MENU_moveUp()
{
	if(currentPosition > 0)
	{
		currentPosition = currentPosition - 1;
		currItem = currentMenu->children[currentPosition];
		OLED_moveArrow(currentPosition);
	}
}

void MENU_moveDown()
{
	if(currentPosition < currentMenu->noChilds -1)
	{
		currentPosition = currentPosition + 1;
		currItem = currentMenu->children[currentPosition];
		OLED_moveArrow(currentPosition);
	}
	else
	{
		//go back to first item
		currentPosition = 0;
		currItem = currentMenu->children[currentPosition];
		OLED_moveArrow(currentPosition);
	}
}

void MENU_moveRight()
{
	if(currItem->hasChildMenu)
	MENU_printMenu(currItem->childMenu, currItem->childMenu->noChilds);
	
	else if(currItem->functionPtr)
	{
		currItem->functionPtr();
		MENU_reactivate();
	}
}

void MENU_moveLeft()
{
	if(currentMenu->parentMenu != NULL)
	{
		MENU_printMenu(currentMenu->parentMenu, currentMenu->parentMenu->noChilds);
	}
}

void MENU_activate()
{
	menuActiveFlag = 1;
	//mainMenu = malloc(sizeof(menuItemNode_t));
	MENU_buildMenus();
	MENU_printMenu(mainMenu, mainMenu->noChilds);
	MENU_waitForInput();
}

void MENU_reactivate()
{
	MENU_printMenu(currentMenu, currentMenu->noChilds);
	MENU_waitForInput();
}












