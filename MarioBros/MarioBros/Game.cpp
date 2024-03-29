//Framework ----------------------
#include "d3d9.h"
#include "d3dx9.h"
#include <tchar.h>
#include "time.h"
#include "Game.h"
#include "Graphic.h"
#include "Input.h"
#include "Sprite.h"
#include "Animation.h"
#include "TimeManager.h"
#include "stdio.h"
//--------------------------------

#include "QuadTree.h"
#include "Info.h"
#include "Utilities.h"
#include "Object.h"
#include "Mario.h"
#include "CheckPoint.h"
#include "Score.h"

#include <list>
using namespace std;
Game::Game(HWND _hWnd, HINSTANCE _hInstance)
{
	//Framework ----------------------
	hWnd=_hWnd;
	hInstance=_hInstance;
	input=new Input(hWnd,hInstance,SCREEN_WIDTH,SCREEN_HEIGHT);
	g=new Graphic(hWnd);
	a=new Audio(hWnd); 
	timeManager=new TimeManager();
	D3DXCreateFont(g->device,30,0,FW_BOLD,0,false,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,TEXT("Arial"),&fontArial);
	srand((unsigned)(time(NULL)));
	//--------------------------------
	
	for(int i=0;i<10;i++) sprites[i]=NULL;
	sprites[S_CASTLE]	=new Sprite(g,"Image\\Castle.bmp");
	sprites[S_SCENE]	=new Sprite(g,"Image\\Scene.bmp");
	sprites[S_PIPE]		=new Sprite(g,"Image\\Pipe.bmp");
	sprites[S_BASIC]	=new Sprite(g,"Image\\Basic.png");
	sprites[S_SMARIO]	=new Sprite(g,"Image\\SmallMario.png");
	sprites[S_BMARIO]	=new Sprite(g,"Image\\BigMario.png");
	sprites[S_GOOMBA]	=new Sprite(g,"Image\\Goomba.bmp");
	sprites[S_KOOPA]	=new Sprite(g,"Image\\Koopa.bmp");
	sprites[S_ITEM]		=new Sprite(g,"Image\\Item.png");
	sprites[S_INFO]		=new Sprite(g,"Image\\Info.png");
	sprites[S_NUMBER]	=new Sprite(g,"Image\\Number.bmp");
	sprites[S_PIRHANA]	=new Sprite(g,"Image\\Pirhana.png");
	sprites[S_FLAG]		=new Sprite(g,"Image\\Flag.png");
	sprites[S_BULLET]	=new Sprite(g,"Image\\FireBullet.PNG");
	sprites[S_EXPLOSION]=new Sprite(g,"Image\\Explosion.PNG");

	camera=new Camera(0,0,800,600);
	
	quadTree	=NULL;
	cloneMario	=new Mario();
	checkPoint	=new CheckPoint();
	isBright	=true;
	score=coin	=0;
	numLife		=3;
	state		=GS_MENU;
	//state       =GS_WIN;
	stage		=1;
	isUseCheckPoint	=false;
	timeGame=300;					
	wait1Sec=0;
	//waitbullet=0;

	sBackground=a->LoadSound("Sounds\\Background.wav"); 
	sGameOver=a->LoadSound("Sounds\\GameOver.wav"); 
	sWinState=a->LoadSound("Sounds\\WinStage.wav"); 
	sFireBall=a->LoadSound("Sounds\\fireball.wav"); 
	
	sJump=a->LoadSound("Sounds\\Jump.wav"); 
	sCoin1=a->LoadSound("Sounds\\Coin.wav"); 
	sPowerUp=a->LoadSound("Sounds\\PowerUp.wav"); 
	sDeath=a->LoadSound("Sounds\\Death.wav"); 
	sMip=a->LoadSound("Sounds\\mip.wav"); 
	sBonusRise=a->LoadSound("Sounds\\BonusRise.wav"); 
	sBrickBreaked=a->LoadSound("Sounds\\BrickBreaked.wav"); 
	sKick=a->LoadSound("Sounds\\kick.wav"); 
	sCoin2=a->LoadSound("Sounds\\Coin.wav"); 
	sCoin3=a->LoadSound("Sounds\\Coin.wav"); 
	sCoin4=a->LoadSound("Sounds\\Coin.wav"); 
	sPipeWarp=a->LoadSound("Sounds\\PipeWarp.wav");
	sBump=a->LoadSound("Sounds\\bump.wav");
	sLife=a->LoadSound("Sounds\\Life.wav");

	//_mario		=new Mario();
	//------K
	menu	    = g->CreateSurface("Image\\Menu.jpg");
	//itemMenu= g->CreateSurface("Image\\ItemMenu.png");
	about       = g->CreateSurface("Image\\About.jpg");
	gameOver    = g->CreateSurface("Image\\GameOver.jpg");
	gameWin     = g->CreateSurface("Image\\GameWin.jpg");
	gameConfirm = g->CreateSurface("Image\\GameConfirm.jpg");
	mariomenu   = new Sprite(g,"Image\\SmallMario.png");
	posymenu    = 320;
	posymenuC   = 150; 
	//-------
}
void Game::GameRun()
{
	timeManager->LimitFPS(60);
	TPF=timeManager->TPF;
	input->GetMouse();	
	input->GetKey();
	Update();
	Render();
}
void Game::ProcessInput()
{
	switch(state)
	{
	case GS_MENU:
	
	    //When player enter hot key.
		if(input->KeyDown(DIK_N))
		{
			//Test.mm	World1-1.mm
			ChangeStage(1);
			ChangeState(GS_PLAYING);
		}
		else if(input->KeyDown(DIK_L))
			ChangeState(GS_LOAD);
		else if(input->KeyDown(DIK_Q))
			PostQuitMessage(0);
		else if (input->KeyDown(DIK_A))
		    ChangeState(GS_ABOUT);

		//-----K
		else if(input->KeyPress(DIK_DOWN))
		{
			posymenu+=50;
			if(posymenu>470)
				posymenu=320;
			a->PlaySound(sFireBall);
		}
		else if(input->KeyPress(DIK_UP))
		{
			posymenu-=50;
			if(posymenu<320)
				posymenu=470;
			a->PlaySound(sFireBall);
		}
		else if(input->KeyPress(DIK_RETURN))
		{
			a->PlaySound(sJump);
			if(posymenu==320)						//New game
			{
				//Test.mm	World1-1.mm
				ChangeStage(1);
				ChangeState(GS_PLAYING);
				score=0;							
				numLife=3;
			}
			else if(posymenu==370)
			{
				ChangeState(GS_LOAD);
			}
			/*else if(posymenu==400)
			{
				ChangeState(GS_SAVE);
			}*/
			else if(posymenu==420)
			{
				ChangeState(GS_ABOUT);
			}
			else if(posymenu==470)
			{
				PostQuitMessage(0);
			}
		}
		//------

		break;
	case GS_PLAYING:
		//waitbullet+=TPF;
		if(input->KeyDown(DIK_ESCAPE))
		{
			//ChangeState(GS_SAVE);
            SaveGame("Save\\Save.sm",this);
            if(quadTree!=NULL) delete quadTree;
            quadTree=NULL;
			ChangeState(GS_CONFIRM);
			a->StopSound(sBackground);
		}
		break;
	case GS_WIN: case GS_GAMEOVER:
		if(input->KeyDown(DIK_RETURN)||input->KeyDown(DIK_ESCAPE))
			ChangeState(GS_MENU);
		break;
	case GS_ABOUT:
		if(input->KeyDown(DIK_ESCAPE))
			ChangeState(GS_MENU);
		break;
	case GS_CONFIRM:
	    if (input->KeyPress(DIK_RIGHT) || input->KeyPress(DIK_LEFT))
	    {
	        //posymenuC += 230;
	        if (posymenuC == 150)
	        {
	            posymenuC = 380;
	        }else if (posymenuC == 380)
	        {
	            posymenuC = 150;
	        }
	        a->PlaySound(sFireBall);
	    }/*
	    else if (input->KeyPress(DIK_LEFT))
	    {
            posymenuC -=230;
            if (posymenuC < 150)
            {
                posymenuC = 150;
            }
            a->PlaySound(sFireBall);
	    }*/
	    else if(input->KeyPress(DIK_RETURN))
	    {
            if (posymenuC == 150)
            {
                ChangeState(GS_MENU);
                a->StopSound(sBackground);
            } 
            else if (posymenuC == 380)
            {
                ChangeState(GS_LOAD);
            }
            	    
	    }
	    break;
	}
}
void Game::Update()
{
	Object* obj;
	Score* sco;
	list<Object*>::iterator i;
	list<Score*>::iterator iScore;
	ProcessInput();
	switch(state)
	{
	case GS_PLAYING:
		if(numLife==0)
			ChangeState(GS_GAMEOVER);
		wait1Sec+=TPF;
		if(wait1Sec>1)
		{
			wait1Sec-=1;
			timeGame--;
		}

		//Xóa những object cần xóa
		for(i=staticObjs.begin();i!=staticObjs.end();i++)
		{
			obj=*i;
			if(obj->GetVar("isNeedDelete")==1)
				quadTree->DeleteObj(obj,true);
		}
		for(i=dynamicObjs.begin();i!=dynamicObjs.end();i++)
		{
			obj=*i;
			if(obj->GetVar("isNeedDelete")==1)
				quadTree->DeleteObj(obj,false);
		}
		
		//Làm rỗng danh sách
		staticObjs.clear();
		dynamicObjs.clear();

		quadTree->GetObjectsFromCamera(camera->rect,&staticObjs,&dynamicObjs);

		//Cập nhật. Chỉ cập nhật những đối tượng ở trước Camera một khoảng  và sau camera một khoảng 
		camera->Update(TPF,quadTree);
		for(i=staticObjs.begin();i!=staticObjs.end();i++)
		{
			obj=*i;
			if(obj->x>camera->x-10&&obj->x<camera->x+WIDTH+10)
				obj->Update(&staticObjs,&dynamicObjs,input,TPF);
		}
		for(i=dynamicObjs.begin();i!=dynamicObjs.end();i++)
		{
			obj=*i;
			if(obj->x>camera->x-WIDTH&&obj->x<camera->x+WIDTH+100)
				obj->Update(&staticObjs,&dynamicObjs,input,TPF);
			//if(obj->ID==40)
			//{
			//	Mario* mario=(Mario*)(*i);
			//	_mario=mario;
			//}
		}
		lScoreRemove.clear();
		for(iScore=lScore.begin();iScore!=lScore.end();iScore++)
		{
			sco=*iScore;
			sco->Update(TPF);
			if(sco->isNeedDelete==true)
			{
				delete sco;
				lScoreRemove.push_back(sco);
			}
		}
		for(iScore=lScoreRemove.begin();iScore!=lScoreRemove.end();iScore++)
		{
			lScore.remove((*iScore));
		}
		quadTree->Update(dynamicObjs);
		break;
	case GS_NEXT_STAGE:				//Khi đổi màn
		ChangeStage(stage+1);
		break;
	case GS_SUB_STAGE:				//Chui hầm (màn phụ).
		ChangeSubStage(stage);
		a->PlaySound(sPipeWarp);
		break;
	case GS_REPLAY: 				//Chết, chơi lại
		ChangeStage(stage);
		break;
	case GS_RETURN_STAGE:
		Object* oTemp;
		ChangeStage(stage);
		quadTree->GetObjectsAllNode(&staticObjs,&dynamicObjs);
		for(i=staticObjs.begin();i!=staticObjs.end();i++)
		{
			obj=*i;
			if(obj->ID==53)					//Vị trí lên.
			{
				oTemp=obj;
				
			}
		}
		for(i=dynamicObjs.begin();i!=dynamicObjs.end();i++)
		{
			obj=*i;
			if(obj->ID==40)
			{				
				obj->x=oTemp->x;
				obj->y=oTemp->y-TILE_SIZE*2;
				obj->ResetRect();	
			}
		}
		quadTree->Update(dynamicObjs);		
	}
}
void Game::Render()
{
	//Framework ----------------------
	g->device->BeginScene();
	g->spriteDX->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
	g->device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT); 
	//--------------------------------

	//Vẽ nằm hết ở đây
	Object* obj;
	list<Object*>::iterator i;
	list<Score*>::iterator iScore;
	switch(state)
	{
	case GS_MENU:
		g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
		Menu();
		break;
	case GS_PLAYING:
		if(isBright==true)
			g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(59,185,255),1.0,0);
		else g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);

		RECT rInfo;
		rInfo.left=0;rInfo.right=132; rInfo.top=0; rInfo.bottom=16;
		sprites[S_INFO]->Draw(50,20,1,1,rInfo,0.1);
		rInfo.top=16; rInfo.bottom=34;
		sprites[S_INFO]->Draw(500,20,1,1,rInfo,0.1);
		DrawNumber(sprites[S_NUMBER],numLife,150,20);
		DrawNumber(sprites[S_NUMBER],score,150,40);
		DrawNumber(sprites[S_NUMBER],coin,550,20);
		DrawNumber(sprites[S_NUMBER],timeGame,700,40);

		for(i=staticObjs.begin();i!=staticObjs.end();i++)
		{
			obj=*i;
			if(obj->x>camera->x-800&&obj->x<camera->x+WIDTH+10)
				obj->Render(camera);
		}
		for(i=dynamicObjs.begin();i!=dynamicObjs.end();i++)
		{
			obj=*i;
			if(obj->x>camera->x-800&&obj->x<camera->x+WIDTH+10)
				obj->Render(camera);
		}
		for(iScore=lScore.begin();iScore!=lScore.end();iScore++)
			(*iScore)->Render(camera);
		break;
	case GS_WIN:
		g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
		/*g->DrawTextAdvanced(L"You Won !",100,300,800,200,D3DXCOLOR(1,0,0,1),fontArial);
		g->DrawTextAdvanced(L"Press Enter or ESC to return Menu !",100,400,800,200,D3DXCOLOR(1,0,0,1),fontArial);*/
		g->device->StretchRect(gameWin, NULL, g->backBuffer, NULL, D3DTEXF_NONE);
		break;
	case GS_GAMEOVER:
		a->PlaySound(sGameOver); 
		a->StopSound(sBackground);
		g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
		g->device->StretchRect(gameOver, NULL, g->backBuffer, NULL, D3DTEXF_NONE);
		break;
	case GS_ABOUT:
		AboutGame();
		break;
	case GS_CONFIRM:
	    g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
	    GameConfirm();
	    break;
	    
	}
	
	//Framework ----------------------
	g->spriteDX->End();
	g->device->EndScene();
	g->device->Present(NULL,NULL,NULL,NULL);
	//--------------------------------
}

Game::~Game()
{
	if(quadTree!=NULL) delete quadTree;
	for(int i=0;i<10;i++)
		if(sprites[i]!=NULL) delete sprites[i];
	if(checkPoint) delete checkPoint;
	delete sJump;
	delete sCoin1;
	delete sCoin2;
	delete sCoin3;
	delete sCoin4;
	delete sPowerUp;
	delete sDeath;
	delete sMip;
	delete sBonusRise;
	delete sBrickBreaked;
	delete sKick;
	delete sPipeWarp;
	delete sBump;
	delete sLife;

	//Framework ----------------------
	delete g;
	delete input;
	delete timeManager;
	delete a; 
	//--------------------------------
}
void Game::ChangeState(char _state)
{
	state=_state;
	switch(state)
	{
	case GS_SAVE:
		SaveGame("Save\\Save.sm",this);
		if(quadTree!=NULL) delete quadTree;
		quadTree=NULL;
		ChangeState(GS_MENU);
		break;
	case GS_LOAD:
		LoadGame("Save\\Save.sm",this);
		SetIsBright(stage);
		ChangeState(GS_PLAYING);
		break;
	case GS_PLAYING:
		//a->PlaySound(sBackground); 
		camera->x=0;
		staticObjs.clear();
		dynamicObjs.clear();
		break;
	case GS_WIN:
		a->PlaySound(sWinState);
		break;
	}
}

void Game::ChangeStage(char _stage)
{
	if(isInSubStage==false)		//Nếu chuyển màn thì gán lại thời gian 
		timeGame=300;
	isInSubStage=false;
	stage=_stage;
	switch(stage)
	{
	case 1:
		ReadMap("Map\\World1-1.mm",true,this);
		//ReadMap("Map\\Test.mm",true,this);
		break;
	case 2:
		ReadMap("Map\\World1-2.mm",false,this);
		break;
	case 3:
		ReadMap("Map\\World1-3.mm",true,this);
		break;
	}
	SetIsBright(_stage);
	if(stage<4)
		ChangeState(GS_PLAYING);
	else ChangeState(GS_WIN);
		
}
void Game::ChangeSubStage(char _stage)
{
	isInSubStage=true;
	switch(_stage)
	{
	case 1:
		ReadMap("Map\\World1-1a.mm",false,this);
		isBright=false;
		break;
	case 2:
		ReadMap("Map\\World1-2a.mm",false,this);
		isBright=false;
		break;
	case 3:
		ReadMap("Map\\World1-3a.mm",false,this);
		isBright=false;
		break;
	}
	ChangeState(GS_PLAYING);
}
void Game::SetIsBright(char _stage)
{
	switch(stage)
	{
	case 1:
		isBright=true;
		break;
	case 2:
		isBright=false;
		break;
	case 3:
		isBright=true;
		break;
	}
}
void Game::AddScore(int _score,float _x,float _y)
{
	score+=_score;
	Score* sco=new Score(sprites[S_NUMBER],_x,_y,_score);
	lScore.push_back(sco);
}

//-----K
void Game::Menu()
{	
	g->device->StretchRect(menu,NULL,g->backBuffer,NULL,D3DTEXF_NONE);
	menurect.top=16;
	menurect.left=102;
	menurect.right=120;
	menurect.bottom=32;
	mariomenu->Draw(310,posymenu,1.5,1.5,menurect,0.5);		
}
void Game::AboutGame()
{
	g->device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0,0);
	g->device->StretchRect(about, NULL, g->backBuffer, NULL, D3DTEXF_NONE);
}
void Game::GameConfirm()
{
    g->device->StretchRect(gameConfirm,NULL,g->backBuffer,NULL,D3DTEXF_NONE);
    menurect.top=16;
    menurect.left=102;
    menurect.right=120;
    menurect.bottom=32;
    mariomenu->Draw(posymenuC,270,1.5,1.5,menurect,0.5);
}
//------