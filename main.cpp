#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<iomanip>
#include "cmd_console_tools.h"
using namespace std;

#define PAGES 32
#define MEMORY_BLOCKS 4
#define INST_OF_PAGE 10

void display();
void waver(int cur);
void LRU(int cur, int block[MEMORY_BLOCKS]);
void show_block(int block[MEMORY_BLOCKS]);

int page_fault = 0, option = 0, index = 0;

void press_enter()
{
	char ch = _getch();
	while (ch != '\r')
		ch = _getch();
}

void show_inst(int cur)
{
	for (int i = 0; i < 3; i++)
	{
		if (index % 10 == 0)
			cct_showch(40, 34 + index % 10, ' ', COLOR_BLACK, COLOR_WHITE, 22);
		else
			cct_showch(40, 24 + index % 10, ' ', COLOR_BLACK, COLOR_WHITE, 22);
		cct_showstr(40, 25 + index % 10, "current instruction->");
		cct_gotoxy(62, 25 + index % 10);
		cct_setcolor(COLOR_PINK, COLOR_BLACK);
		cout << cur;
		cct_setcolor();
		cout << "  ";
		Sleep(10*option);
		cct_gotoxy(62, 25 + index % 10);
		cct_setcolor(COLOR_WHITE, COLOR_BLACK);
		cout << cur;
		cct_setcolor();
		cout << "  ";
		Sleep(10 * option);
	}
	index++;
}

bool one_step(int cur, int block[MEMORY_BLOCKS])
{
	if (option)
		press_enter();
	show_inst(cur);
	waver(cur);
	LRU(cur, block);
	show_block(block);
	cct_gotoxy(0, 35);
	cct_setcolor();
	cout << "page fault rate: " << (page_fault + 0.0) / index << endl;
	if (index == PAGES * INST_OF_PAGE)
		return true;
	return false;
}

void LRU(int cur, int block[MEMORY_BLOCKS])
{
	
	int i = 0;
	for (; i < MEMORY_BLOCKS; i++)
	{
		if (block[i] == cur/INST_OF_PAGE)
		{
			//将block[i]删除
			for (int j = i; j < MEMORY_BLOCKS - 1; j++)
				block[j] = block[j + 1];
			break;
		}
	}
	if (i == 4)
		page_fault++;
	//整体后移
	for (int i = MEMORY_BLOCKS - 1; i > 0; i--)
		block[i] = block[i - 1];
	block[0] = cur / INST_OF_PAGE;
}

void show_block(int block[MEMORY_BLOCKS])
{
	for (int i = 0; i < MEMORY_BLOCKS; i++)
	{
		if (block[i] != -1)
		{
			cct_gotoxy(7 + 11 * (i % 2), 28 + i / 2 * 4);
			cct_setcolor(COLOR_RED, COLOR_BLACK);
			cout << block[i];
			cct_setcolor(COLOR_WHITE, COLOR_BLACK);
			cout << "  ";
		}
	}
}

void display()
{
	//32个页面(3*3)
	cct_setconsoleborder(85, 45);
	cct_showstr(35, 0, "Pages", COLOR_PINK, COLOR_BLACK);
	for (int i = 0; i < PAGES / 2; i++)
	{
		cct_gotoxy(4 + 5 * i, 1);
		cct_setcolor();
		cout << i;
		if (i < 10)
		{
			cct_gotoxy(0, i + 2);
			cout << setw(2) << right << i;
		}
		for (int j = 0; j < INST_OF_PAGE; j++)
			cct_showch(3 + 5 * i, 2 + j, ' ', COLOR_WHITE, COLOR_BLACK, 3);
	}
	for (int i = 0; i < PAGES / 2; i++)
	{
		cct_gotoxy(4 + 5 * i, 13);
		cct_setcolor();
		cout << right << i + 16;
		if (i < 10)
		{
			cct_gotoxy(0, i + 14);
			cout << setw(2) << right << i;
		}
		for (int j = 0; j < INST_OF_PAGE; j++)
			cct_showch(3 + 5 * i, 14 + j, ' ', COLOR_WHITE, COLOR_BLACK, 3);
	}
	cct_showstr(8, 25, "Memory Blocks", COLOR_PINK, COLOR_BLACK);
	cct_setcolor(COLOR_WHITE, COLOR_BLACK);
	//4个内存块(5*5)
	for (int i = 0; i < MEMORY_BLOCKS; i++)
	{
		for (int j = 0; j < 3; j++)
			cct_showch(5 + 11 * (i % 2), 27 + j + i / 2 * 4, ' ', COLOR_WHITE, COLOR_BLACK, 8);
	}
	cout << endl;
}

void waver(int cur)
{
	int page = cur / INST_OF_PAGE, inst = cur % INST_OF_PAGE;
	if (page < PAGES / 2)
	{
		for (int i = 0; i < 5; i++)
		{
			cct_showch(3 + 5 * page, 2 + inst, ' ', COLOR_WHITE, COLOR_BLACK, 3);
			Sleep(10*option);
			cct_showch(3 + 5 * page, 2 + inst, ' ', COLOR_RED, COLOR_BLACK, 3);
			Sleep(10 * option);
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			cct_showch(3 + 5 * (page - PAGES / 2), 14 + inst, ' ', COLOR_WHITE, COLOR_BLACK, 3);
			Sleep(10 * option);
			cct_showch(3 + 5 * (page - PAGES / 2), 14 + inst, ' ', COLOR_RED, COLOR_BLACK, 3);
			Sleep(10 * option);
		}
	}
}

int main()
{
	cout << "Please enter the display speed: (0 for auto ,1 for step by step) ";
	while (1)
	{
		cin >> option;
		if (!cin.good() || option > 1 || option < 0)
		{
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else
			break;
	}
	cct_setcursor(CURSOR_INVISIBLE);
	display();
	if (option)
		cct_showstr(0, 0, "press enter to make a move", COLOR_BLACK, COLOR_WHITE, 0);
	cct_setcolor();
	int block[MEMORY_BLOCKS] = { -1,-1,-1,-1 };
	srand((unsigned int)time(NULL));
	int index = 0, cur = rand() % (PAGES * INST_OF_PAGE);
	one_step(cur, block);
	cur++;
	one_step(cur, block);
	while (1)
	{
		cur = rand() % (cur - 1);
		if (one_step(cur, block))
			break;
		cur++;
		if (one_step(cur, block))
			break;
		cur = rand() % (319 - cur) + cur + 1;
		if (one_step(cur, block))
			break;
		cur++;
		if (one_step(cur, block))
			break;
	}
	return 0;
}