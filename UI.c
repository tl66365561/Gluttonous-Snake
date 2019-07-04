#define _CRT_SECURE_NO_WARNINGS

#include "UI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// 移动光标到x，y处，注意，这里是相对整个屏幕的，而不是游戏区域的
static void _SetPos(int x, int y);

// 显示墙
static void _DisplayWall(const struct UI *pUI);

// 显示右侧信息
static void _DisplayDesc(const struct UI *pUI);



// 重置光标到屏幕下方，主要是为了显示的美观
static void _ResetCursorPosition(const struct UI *pUI);


//界面————规定界面属性
struct UI * UIInitialize(int width, int height)
{
	const int left = 2;
	const int top = 2;
	const int descWidth = 50;

	struct UI *pUI = (struct UI *)malloc(sizeof(struct UI));
	pUI->gameWidth = width;
	pUI->gameHeight = height;
	pUI->marginLeft = left;
	pUI->marginTop = top;
	pUI->windowHeight = top + height + 2 + 3;
	pUI->foodBlock = "█";
	pUI->snakeBlock = "█";
	pUI->wallBlock = "█";
	pUI->blockWidth = strlen(pUI->wallBlock);
	pUI->windowWidth = left + (width + 2) * pUI->blockWidth + descWidth;

	char modeCommand[1024];
	sprintf(modeCommand, "mode con cols=%d lines=%d", pUI->windowWidth, pUI->windowHeight);
	system(modeCommand);

	return pUI;
}



//界面————显示向导
void UIDisplayWizard(const struct UI *pUI)
{
	int i;
	const char *messages[3] = {
		"欢迎来到贪吃蛇小游戏",
		"用↑.↓.←.→分别控制蛇的移动， F1为加速，F2为减速。",
		"加速将能得到更高的分数。"
	};

	i = 0;
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2);
	printf("%s\n", messages[i]);
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2 + 2);
	system("pause");
	system("cls");

	i = 1;
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2);
	printf("%s\n", messages[i]);

	i = 2;
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2 + 2);
	printf("%s\n", messages[i]);
	_SetPos(pUI->windowWidth / 2 - strlen(messages[i]) / 2, pUI->windowHeight / 2 + 4);
	system("pause");
	system("cls");
}


//界面————显示游戏窗口
void UIDisplayGameWindow(const struct UI *pUI, int score, int scorePerFood)
{
	_DisplayWall(pUI);
	UIDisplayScore(pUI, score, scorePerFood);
	_DisplayDesc(pUI);

	_ResetCursorPosition(pUI);
}

//放置食物
void UIDisplayFoodAtXY(const struct UI *pUI, int x, int y)
{
	_CoordinatePosAtXY(pUI, x, y);
	printf(pUI->foodBlock);

	_ResetCursorPosition(pUI);
}


//显示蛇的显示块
void UIDisplaySnakeBlockAtXY(const struct UI *pUI, int x, int y)
{
	_CoordinatePosAtXY(pUI, x, y);
	printf(pUI->snakeBlock);

	_ResetCursorPosition(pUI);
}

//删除指定位置的显示块
void UICleanBlockAtXY(const struct UI *pUI, int x, int y)
{
	_CoordinatePosAtXY(pUI, x, y);
	int i;

	for (i = 0; i < pUI->blockWidth; i++) {
		printf(" ");
	}

	_ResetCursorPosition(pUI);
}

//右侧界面————显示分数
void UIDisplayScore(const struct UI *pUI, int score, int scorePerFood)
{
	int blockWidth = strlen(pUI->wallBlock);
	int left = pUI->marginLeft + (pUI->gameWidth + 2) * blockWidth + 2;
	_SetPos(left, pUI->marginTop + 8);
	printf("得分: %3d，每个食物得分: %3d", score, scorePerFood);

	_ResetCursorPosition(pUI);
}

void UIShowMessage(const struct UI *pUI, const char *message)
{
	// 左填充宽度 + (1(左边界个数）+游戏区域的个数/2) * 每个字符的宽度 - 字符串的宽度 / 2
	_SetPos(
		pUI->marginLeft + (
		(1 + pUI->gameWidth / 2) * pUI->blockWidth)
		- strlen(message) / 2,
		pUI->marginTop + 1 + pUI->gameHeight / 2);
	printf("%s\n", message);

	_ResetCursorPosition(pUI);
}

void UIDeinitialize(struct UI *pUI)
{
	free(pUI);
}


//显示墙
static void _DisplayWall(const struct UI *pUI)
{
	int left = pUI->marginLeft;
	int top = pUI->marginTop;
	int width = pUI->gameWidth;
	int height = pUI->gameHeight;
	int blockWidth = pUI->blockWidth;
	int i;

	// 上
	_SetPos(left, top);
	for (i = 0; i < width + 2; i++) {
		printf("%s", pUI->wallBlock);
	}

	// 下
	_SetPos(left, top + 1 + height);     //这块为什么不用 top+(1+height）*blockWidth???
	for (i = 0; i < width + 2; i++) {
		printf("%s", pUI->wallBlock);
	}

	// 左
	for (i = 0; i < height + 2; i++) {
		_SetPos(left, top + i);
		printf("%s", pUI->wallBlock);
	}

	// 右
	for (i = 0; i < height + 2; i++) {
		_SetPos(left + (1 + width) * blockWidth, top + i);
		printf("%s", pUI->wallBlock);
	}
}


//界面————显示右侧描述信息
static void _DisplayDesc(const struct UI *pUI)
{
	int left = pUI->marginLeft + (pUI->gameWidth + 2) * pUI->blockWidth + 2;
	const char *messages[] = {
		"不能穿墙，不能咬到自己。",
		"用 ↑ ↓ ← → 分别控制蛇的移动。",
		"F1 为加速，F2 为减速。",
		"ESC 退出游戏。 SPACE 暂停游戏。",
		"版权 @tonglin"
	};
	int i;

	for (i = 0; i < sizeof(messages) / sizeof(char *); i++) {
		_SetPos(left, pUI->marginTop + 10 + i);
		printf("%s\n", messages[i]);
	}
}


//定位光标
static void _SetPos(int x, int y)
{
	COORD position = { x, y };
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, position);
}

static void _CoordinatePosAtXY(const struct UI *pUI, int x, int y)
{
	_SetPos(pUI->marginLeft + (1 + x) * pUI->blockWidth,
		pUI->marginTop + 1 + y);
}
//为了美观，光标定位到左下角
static void _ResetCursorPosition(const struct UI *pUI)
{
	_SetPos(0, pUI->windowHeight - 1);
}
