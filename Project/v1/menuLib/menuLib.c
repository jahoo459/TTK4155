/*
 * menuLib.c
 *
 * Created: 9/29/2017 9:00:21 AM
 *  Author: Jasiek
 */ 

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>
#include "menuLib.h"
#include "..\JoystickLib\JoystickLib.h"

//******************************************************************************************
menuItemNode_t *mainMenu = NULL; //pointer pointing to the first main menu item
menuItemNode_t *currItem = NULL; //current item, at the beginning will point at the first item (mainMenu)

uint8_t menuActiveFlag = 0; //When this flag gets 0 waitForInput function will stop so menu will be inactive

int currentPosition = 0; //Current menu position read from the Joystick

int menuFrameOffset = 10; //Columns from left frame border
//******************************************************************************************
char *mainMenuItems[] = {"New Game", "Highscores", "Joy Calib", "Debugging", "Info"};
// char *difficultyMenuItems[] = {"easy", "medium", "hard"};

void MENU_init()
{
	//create main menu
	for(int i = 0; i < sizeof(mainMenuItems)/sizeof(char*); i++)
	{
		MENU_addMenuItem(mainMenuItems[i], currItem, mainMenu);
	}
	
	MENU_printMenu(mainMenu);
	
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

void MENU_printMenuItem(menuItemNode_t* item, int lineNumber)
{
	if(item != NULL)
	{
		OLED_goto(lineNumber, menuFrameOffset);
		OLED_print_string(item->text);
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
	JOY_direction_t currDir;
	
	while(menuActiveFlag)
	{
		while(JOY_getDirection() == "CENTRE")
		{
			//wait for changing the direction
			if(JOY_getDirection() != "CENTRE")
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
			}
			else
			{
				_delay_ms(100);
			}
		}
	}
}

void MENU_moveUp()
{
	if(currentPosition >0)
	{
		currentPosition = currentPosition - 1;
	}
}

void MENU_moveDown()
{
	if(currentPosition < MAX_MENU_SIZE -1)
	{
		currentPosition = currentPosition + 1;
	}
}

void MENU_moveRight()
{
	//Move to the next menu
}

void MENU_moveLeft()
{
	//Move to the previous Menu
}

void MENU_activate()
{
	menuActiveFlag = 1;
}












