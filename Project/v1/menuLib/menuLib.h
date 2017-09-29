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

//struct that defines menu item
typedef struct menuItemNode
{
	char* text;
	struct menuItemNode* next;
	struct menuItemNode* parentMenu;
} menuItemNode_t;

void MENU_init();
void MENU_addMenuItem(char* text, menuItemNode_t *parent, menuItemNode_t *menu);
void MENU_removeMenuItem(menuItemNode_t* item);
void MENU_printMenuItem(menuItemNode_t* item);
void MENU_printMenu(menuItemNode_t* firstItem);
void MENU_moveUp();
void MENU_moveDown();



#endif /* MENULIB_H_ */