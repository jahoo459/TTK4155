/*
 * menuLib.h
 *
 * Created: 9/29/2017 9:00:51 AM
 *  Author: Jasiek
 */ 

/************************************************************************/
/* This is a library for a menu                                                                     */
/************************************************************************/
#ifndef MENULIB_H_
#define MENULIB_H_

#include "..\JoystickLib\JoystickLib.h"

#define MAX_MENU_SIZE 8
//struct that defines menu item
typedef struct menuNode
{
	char* menuName;
	int noChilds;
	struct menuItemNode* children[MAX_MENU_SIZE]; // children[8] maximum 8 elements in menu
	struct menuNode* parentMenu;
} menuNode_t;

typedef struct menuItemNode
{
	char* name;
	struct menuNode* parentMenu;
	struct menuNode* childMenu;
	int hasChildMenu;
	struct menuItemNode_t* next;
	void (*functionPtr)();
}menuItemNode_t;

void MENU_buildMenus();
void MENU_addMenuItem(char* name, menuNode_t* parentMenu, menuNode_t* childMenu, int hasChildMenu);

//void MENU_removeMenuItem(menuItemNode_t* item);
void MENU_printMenuItem(menuItemNode_t* item, int lineNumber);
void MENU_printMenu(menuNode_t* menu, int noElements);

void MENU_moveUp();
void MENU_moveDown();
void MENU_moveRight();
void MENU_moveLeft();
void MENU_waitForInput();
void MENU_clearMemory();
void MENU_reactivate();

void MENU_activate(OP_STATE* state);



#endif /* MENULIB_H_ */