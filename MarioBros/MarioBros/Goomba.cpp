#include "Goomba.h"
#include "Object.h"
#include "Info.h"
#include "Animation.h"
#include "Sprite.h"
#include "Collision.h"

Goomba::Goomba():Object(){}
Goomba::Goomba(float _x, float _y,float width,float height,bool _isBright,int _ID,Sprite* _sprite,Game* _game):Object(_x,_y,width,height)
{
	isBright=_isBright;
	sprite	=_sprite;
	ID		=_ID;
	isAlive	=true;
	game	=_game;
	
	xOld	=x;
	yOld	=y;
	oldRect	=rect;

	vx		=-80;
	vy		=0;
	ay		=G;
	state	=ES_ACTIVING;
	isNeedDelete=false;
	collisionList=new list<Object*>;

	ani=new Animation(6,1,0.2,sprite);
}
Goomba::~Goomba()
{
	if(collisionList!=NULL)
		delete collisionList;
	collisionList=NULL;
}
void Goomba::Update(list<Object*>* staObjs,list<Object*>* dynObjs,Input* input, float TPF)
{
	switch(state)
	{
	case ES_ACTIVING:
		Move(TPF,staObjs);
		break;
	case ES_IDLE:
		waitIdle+=TPF;
		if(waitIdle>1)
			ChangeState(ES_DIED);
		break;
	case ES_FALL:
		x+=vx*TPF;
		vy+=ay*TPF;
		y+=vy*TPF+0.5*ay*TPF*TPF;
		ResetRect();
		if(y>HEIGHT)
			ChangeState(ES_DIED);
	}
	UpdateAnimation(TPF);
}
void Goomba::Render(Camera* camera)
{
	if(state!=ES_FALL)
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,ani->rect,0.5);
	else sprite->Draw(x-camera->x,y+TILE_SIZE,ZOOM,-ZOOM,ani->rect,0.5);
}
void Goomba::Move(float TPF,list<Object*>* staObjs)
{
	Object* obj;
	//Theo phương x
	x+=vx*TPF;
	ResetRect();
	list<Object*>::iterator i;
	for(i=staObjs->begin();i!=staObjs->end();i++)
	{
		obj=*i;
		if(obj->ID>=20&&obj->ID<=29)				 //Nếu là thùng, đá , gạch...
			if(Collision::RectVsRect(rect,obj->rect)==true
				&&obj->GetVar("state")!=TS_BREAKING&&obj->GetVar("state")!=TS_BREAKED)
			{
				vx=-vx;
				x=xOld;
			}
	}
	//Theo phương y
	ay=G;
	vy+=ay*TPF;
	y+=vy*TPF+1.0/2*ay*TPF*TPF;
	oldRect=rect;
	ResetRect();
	collisionList->clear();
	for(i=staObjs->begin();i!=staObjs->end();i++)
	{
		obj=*i;
		if(obj->ID>=20&&obj->ID<=28)				//Nếu là thùng, đá , gạch...
			if(Collision::RectVsRect(rect,obj->rect)==true
				&&obj->GetVar("state")!=TS_BREAKING&&obj->GetVar("state")!=TS_BREAKED)
				collisionList->push_back(obj);
	}
	
	//Xét va chạm để không bị rơi
	if(collisionList->size()>0)
	{
		Object* obj;
		list<Object*>::iterator i;
		for(i=collisionList->begin();i!=collisionList->end();i++)
		{
			obj=*i;
			if(oldRect.bottom<obj->rect.bottom)
				if(obj->y-height<y)
				{
					y=obj->y-height;
					vy=0;
				}
		}
		ResetRect();
	}
	xOld=x;
	yOld=y;
}
void Goomba::UpdateAnimation(float TPF)
{
	switch(state)
	{
	case ES_ACTIVING:
		if(isBright==true)
			ani->Update(0,1,TPF);
		else
			ani->Update(3,4,TPF);
		break;
	case ES_IDLE:
		if(isBright==true)
			ani->SetIndex(2);
		else ani->SetIndex(5);
		break;
	case ES_FALL:
		if(isBright==true)
			ani->SetIndex(0);
		else ani->SetIndex(3);
		break;
	}
}
void Goomba::ChangeState(char _state)
{
	state=_state;
	switch(state)
	{
	case ES_IDLE:
		waitIdle=0;
		break;
	case ES_DIED:
		game->score+=100;
		isNeedDelete=true;
		break;
	case ES_FALL:		
		vy=-400;
		ay=G;
		if(Huong==1)
			vx=200;
		else if(Huong==2)
			vx=-200;
		break;
	}
}
void Goomba::SetVar(char* varName,int val)
{
	if(strcmp(varName,"state")==0)
		ChangeState(val);
}
int Goomba::GetVar(char* varName)
{
	if(strcmp(varName,"isNeedDelete")==0)
		return isNeedDelete;
	if(strcmp(varName,"state")==0)
		return state;
	return -1;
}
void Goomba::ResetWhenReload(Sprite* _sprite)
{
	collisionList= new list<Object*>;
	sprite=_sprite;
	ani=new Animation(6,1,0.2,sprite); 
}
void Goomba::Save(fstream *fs)
{
	fs->write(reinterpret_cast<char*>(&ID),sizeof(ID));	//int ID, để khi load biết được đang load Obj nào
	int sizeObj=sizeof(*this);
	fs->write(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->write(reinterpret_cast<char*>(this),sizeObj);
}
void Goomba::Load(fstream *fs)
{
	int sizeObj;
	fs->read(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->read(reinterpret_cast<char*>(this),sizeObj);
}