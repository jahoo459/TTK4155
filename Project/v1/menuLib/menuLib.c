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
menuNode_t *mainMenu = NULL; //pointer pointing to main menu
menuNode_t *difficultyMenu = NULL; //pointer pointing to difficulty menu 
menuNode_t *currentMenu = NULL;	//pointer pointing to currently displayed menu - for navigation
menuItemNode_t *currItem = NULL; //current item, at the beginning will point at the first item (mainMenu)

uint8_t menuActiveFlag = 0; //When this flag gets 0 waitForInput function will stop so menu will be inactive

static int currentPosition = 0; //Current menu position read from the Joystick

int menuFrameOffset = 10; //Columns from left frame border
//******************************************************************************************

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
	
	MENU_addMenuItem("Difficulty", mainMenu, difficultyMenu, 1);
	mainMenu->children[1] = currItem;
	
	MENU_addMenuItem("Highscores", mainMenu, NULL, 0);
	mainMenu->children[2] = currItem;
	
	MENU_addMenuItem("Joy Calib", mainMenu, NULL, 0);
	mainMenu->children[3] = currItem;
	
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
	printf("%s: Number of elements:%d\n", menu->menuName, noElements);
	
	for(int i = 0; i < noElements; i++)
	{
		MENU_printMenuItem(menu->children[i], i);
		printf("%d: %s\n", i, menu->children[i]->name);
		if(menu->children[i]->hasChildMenu)
		{
			printf("*****Child Menu: %s\n", menu->children[i]->childMenu->menuName);
		}
	}
	//currentMenu points to currently printed menu
	currentMenu = menu;
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
	if(currentMenu)
	MENU_printMenu(mainMenu, mainMenu->noChilds);
}

void MENU_moveLeft()
{
	currentPosition = 0;
	//MENU_init(mainMenu, mainMenuItems);
	//MENU_printMenu();
}

void MENU_activate()
{
	menuActiveFlag = 1;
	//mainMenu = malloc(sizeof(menuItemNode_t));
	MENU_buildMenus();
	MENU_printMenu(mainMenu, mainMenu->noChilds);
	MENU_waitForInput();
}












