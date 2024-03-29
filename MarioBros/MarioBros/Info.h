#pragma once
#ifndef _INFO
#define _INFO

#define PI			3.141592654
#define TILE_SIZE	40
#define ZOOM		2.5
#define G			2000
#define WIDTH		800
#define HEIGHT		600

enum E_SPRITE
{
	S_SCENE,
	S_BASIC,
	S_ITEM,
	S_CASTLE,
	S_PIPE,
	S_SMARIO,
	S_BMARIO,
	S_GOOMBA,
	S_KOOPA,
	S_INFO,
	S_NUMBER,
	S_PIRHANA,
	S_FLAG,
	S_BULLET,
	S_EXPLOSION
};
enum ITEM_STATE
{
	IS_IDLE,
	IS_MOVING,
	IS_ACTIVING,
	IS_ATE,
	IS_FREE,
	IS_READY
};
enum TERRAIN_STATE
{
	TS_IDLE,
	TS_IDLE_2,
	TS_MOVEUP,
	TS_BREAKING,
	TS_BREAKED,
	TS_ACTIVING
};
enum ENEMY_STATE
{
	ES_ACTIVING,
	ES_IDLE,			//Khi Goomba hoặc Koopa bị dậm
	ES_MOVE_SHELL_LEFT,		//Khi Koopa di chuyển trong mai rùa.
	ES_MOVE_SHELL_RIGHT,
	ES_FALL,				//Khi Goomba hoặc Koopa bị rơi (chuẩn bị chết)
	ES_DIED
};
enum GAME_STATE
{
	GS_MENU,
	GS_PLAYING,
	GS_LOAD,
	GS_SAVE,
	GS_WIN,
	GS_GAMEOVER,
	GS_NEXT_STAGE,
	GS_SUB_STAGE,
	GS_REPLAY,
	GS_RETURN_STAGE,
	GS_ABOUT,
	GS_CONFIRM
};
enum MARIO_STATE
{
	MS_NORMAL,
	MS_SIT,
	MS_PULL_FLAG,
	MS_AUTO_TO_CASTLE,				//Khi kéo xong cờ.
	MS_DIEING,
	MS_DIED,
	MS_AUTO_RIGHT_PIPE,				//Khi chui vào ống ngang
	MS_EAT_FLOWER,			//H
	MS_ZOOM_IN,
	MS_ZOOM_OUT
};
enum BULLET_STATE
{
	BS_IDLE,
	BS_ACTIVING,
	BS_BLOW,
	BS_BLOWED
};
#endif