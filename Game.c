#include "Game.h"
#include "UI.h"
#include <stdlib.h>
#include <stdbool.h>
//#include <Windows.h>


typedef struct Node Node;
typedef struct Node* PNode;


static void _Pause();

static void _InitializeSnake(struct Snake *pSnake)
{
	//初始化蛇
	//把蛇的身体位置确定下来，运行的时候再画出

	char* tmp = "█";
	int width = strlen(tmp);   //width==2;


	pSnake->pBody = (Node*)malloc(sizeof(Node));

	pSnake->direction = RIGHT;
	//蛇的初始化位置坐标给的默认值为什么会报错，发生中断？？？
	
	pSnake->pBody->position.x = 6*width;;
	pSnake->pBody->position.y = 10;


	PNode p1 = (PNode)malloc(sizeof(Node));
	p1->position.x = 5.5*width;
	p1->position.y = 10;

	PNode p2 = (PNode)malloc(sizeof(Node));
	p2->position.x = 5*width;
	p2->position.y = 10;

	pSnake->pBody->pNext = p1;
	p1->pNext = p2;
	p2->pNext = NULL;


}

static int _IsOverlapSnake( struct Snake *pSnake, int x, int y)
{
    


}

static void _GenerateNewFood(struct Game *pGame, struct Snake *pSnake)
{
	//修改Game中的 foodPosition 位置
	//位置应该合法： 1. 在游戏区域内   2.不能与蛇的身体坐标冲突

	/*
	p->foodPosition.x=
	p->foodPosition.y=*/

	struct Game* p = pGame;
lable:
	
	p->foodPosition.x = rand() % p->width;
	p->foodPosition.y = rand() % p->height;
	
	PNode ptr = pSnake->pBody;
	while (ptr)  //遍历排除蛇身重复
	{
		;
		if (p->foodPosition.x == ptr->position.x && p->foodPosition.y== ptr->position.y)
		{
			goto lable;
		}
		ptr=ptr->pNext;
	}

}


/////////////////游戏初始化//////////////////////////////////////
struct Game * CreateGame()
{
	struct Game *pGame = (struct Game *)malloc(sizeof(struct Game));
	// TODO: 异常处理
	pGame->width = 28;
	pGame->height = 27;
	pGame->score = 0;
	pGame->scorePerFood = 10;

	// 关于蛇的初始化和放置食物部分，感兴趣的同学可以做扩展练习
	_InitializeSnake(&pGame->snake);
	_GenerateNewFood(pGame,&pGame->snake);

	return pGame;
}

static void _DisplaySnake(const struct UI *pUI, const struct Snake *pSnake)
{
	
	PNode cur = NULL;
	cur = pSnake->pBody;
	while (cur){

		UIDisplaySnakeBlockAtXY(pUI, cur->position.x, cur->position.y);
		cur = cur->pNext;
	}
	
	
	
}



static struct Position _GetNextPosition(const struct Snake *pSnake)
{
	//蛇下一步移动到达位置的坐标
	struct Position nextPosition = { 0 };

	enum Direction direct = pSnake->direction;
	if (direct == RIGHT){
	
		nextPosition.x = pSnake->pBody->position.x+1;
		nextPosition.y = pSnake->pBody->position.y;

	}
	if (direct == LEFT){
		//拿到蛇最后一个节点坐标然后-1;

		PNode p = pSnake->pBody;
		while (p != NULL){
		
			p = p->pNext;
		}

		nextPosition.x = p->position.x-1;
		nextPosition.y = p->position.y;

	}	
	if (direct == UP){
		//拿到蛇的头结点,然后y-1;x不变

		nextPosition.x = pSnake->pBody->position.x ;
		nextPosition.y = pSnake->pBody->position.y-1;

	}
	if (direct == DOWN){
		//拿到蛇的头结点,然后y+1;x不变

		nextPosition.x = pSnake->pBody->position.x;
		nextPosition.y = pSnake->pBody->position.y +1;

	}


	return nextPosition;
}

static bool _IsWillEatFood(struct Position foodPosition, struct Position nextPosition)
{
	return (foodPosition.x == nextPosition.x && foodPosition.y == nextPosition.y);

}

static void _GrowAndMoveAndDisplay(const struct UI *pUI,
struct Snake *pSnake,
struct Position nextPosition)
{
}

static void _UpdateScoreAndDisplay(const struct UI *pUI, struct Game *pGame)
{
}


//删除首元节点
static void _RemoveHead(const struct UI *pUI, struct Snake *pSnake)
{
	PNode m = pSnake->pBody;
	pSnake->pBody = pSnake->pBody->pNext;
    free (m);
}


//尾删
static void _RemoveTailAndDisplay(const struct UI *pUI, struct Snake* pSnake)
{
	PNode p = pSnake->pBody;
	//拿到倒数第二个节点,然后next=null;
	for (; p->pNext->pNext != NULL; p = p->pNext){

	}
	PNode m = p->pNext;
	p->pNext = m->pNext;
	free(m);
}

static void _MoveAndDisplay(const struct UI *pUI, struct Snake *pSnake,struct Position nextPosition)
{
	//蛇的移动:在nextPosition位置画出蛇身节点，自己本来的身体尾删： 如果方向是left，那么身体头删；或者在覆盖节点，打印“ ”;
	PNode p = pSnake->pBody;
	while (p){
	
		p = p->pNext;
	}
	
	
	if (pSnake->direction != LEFT){

		//先头插新节点-》尾删（注意尾巴的更新）-》画头-》清尾
		
		PNode Cur=(PNode)malloc(sizeof(PNode));
		Cur->position.x = nextPosition.x;
		Cur->position.y = nextPosition.y;

		//test...
		Cur->pNext = pSnake->pBody;
		pSnake->pBody = Cur;//....


        _RemoveTailAndDisplay(pUI, &pSnake);//尾删
		_SetPos(nextPosition.x, nextPosition.y);
		printf(pUI->snakeBlock);
		UICleanBlockAtXY(pUI, p->position.x, p->position.y);
 
		
	}
	else{
	   //先尾插新节点-》头删（注意头结点的更新）-》画尾-》清头
		PNode Cur = (PNode)malloc(sizeof(PNode));
		Cur->position.x = nextPosition.x;
		Cur->position.y = nextPosition.y;
		PNode p = pSnake->pBody;
		while (p){
			p = p->pNext;
		
		}
		p->pNext = Cur;
		Cur->pNext = p->pNext;



		_RemoveHead(pUI, &pSnake);//删除首元节点
		
		
		_SetPos(nextPosition.x, nextPosition.y);
		printf(pUI->snakeBlock);
		UICleanBlockAtXY(pUI, pSnake->pBody->position.x,pSnake->pBody->position.y);
	
	}





}

static int _IsKilledByWall(const struct Snake *pSnake, int width, int height)
{
}

static int _IsKilledBySelf(const struct Snake *pSnake)
{
}

static void _HandleDirection(struct Snake *pSnake)
{
	if (GetAsyncKeyState(VK_UP)) {
		if (pSnake->direction != DOWN) {
			pSnake->direction = UP;
		}
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		if (pSnake->direction != UP) {
			pSnake->direction = DOWN;
		}
	}
	else if (GetAsyncKeyState(VK_LEFT)) {
		if (pSnake->direction != RIGHT) {
			pSnake->direction = LEFT;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		if (pSnake->direction != LEFT) {
			pSnake->direction = RIGHT;
		}
	}
}


////////////////////////////游戏开始/////////////////////////////////////////////////
void StartGame(struct Game *pGame)
{
	struct UI *pUI = UIInitialize(pGame->width, pGame->height);

	UIDisplayWizard(pUI);
	UIDisplayGameWindow(pUI, pGame->score, pGame->scorePerFood);
	UIDisplayFoodAtXY(pUI, pGame->foodPosition.x, pGame->foodPosition.y);
	_DisplaySnake(pUI, &pGame->snake);

	while (1) {
		// 方向
		_HandleDirection(&pGame->snake);

		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}

		if (GetAsyncKeyState(VK_SPACE)) {
			_Pause();
		}

		struct Position nextPosition = _GetNextPosition(&pGame->snake);
		if (_IsWillEatFood(pGame->foodPosition, nextPosition)) {
			//如果下一步会吃到食物，那么蛇会增长-》更新分数-》选择合适的位置-》放置新食物
			_GrowAndMoveAndDisplay(pUI, &pGame->snake, nextPosition);
			_UpdateScoreAndDisplay(pUI, pGame);
			_GenerateNewFood(pGame,&pGame->snake);
			UIDisplayFoodAtXY(pUI, pGame->foodPosition.x, pGame->foodPosition.y);
		}
		else {
			//蛇的移动
			_MoveAndDisplay(pUI, &pGame->snake, nextPosition);
		}

		if (_IsKilledByWall(&pGame->snake, pGame->width, pGame->height)) {
			break;
		}
		else if (_IsKilledBySelf(&pGame->snake)) {
			break;
		}

		Sleep(300);
	}

	UIShowMessage(pUI, "游戏结束");

	UIDeinitialize(pUI);
}


/////////////////////////////////////////////////////////////
static void _DestroySnake(struct Snake *pSnake)
{
}

void DestroyGame(struct Game *pGame)
{
	struct Node *pNode, *pNext;

	_DestroySnake(&pGame->snake);

	free(pGame);
}

static void _Pause()
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}
