#include "Star.h"
#include "Object.h"
#include "Info.h"
#include "Animation.h"
#include "Sprite.h"
#include "Collision.h"

Star::Star():Object(){}
Star::Star(float _x, float _y,float width,float height,int _ID,Sprite* _sprite):Object(_x,_y,width,height)
{
	sprite	=_sprite;
	ID	=_ID;	
	ani=new Animation(4,5,0,sprite);

	container	=NULL;
	isCheckedContainer	=false;
	isNeedDelete=false;
	vx=vy=ay	=0;
	oldRect		=rect;
	xOld		=x;
	yOld		=y;
	state		=IS_IDLE;
	collisionList	=new list<Object*>;
}
Star::~Star()
{
	if(collisionList!=NULL)
		delete collisionList;
	collisionList=NULL;
}
void Star::Update(list<Object*>* staObjs,list<Object*>* dynObjs,Input* input, float TPF)
{
	CheckObjects(staObjs);
	if(state!=IS_IDLE)
	{
		Move(TPF,staObjs,dynObjs);
		ani->Update(8,11,TPF);
	}
}
void Star::ChangeState(char _state)
{
	state=_state;
	switch(state)
	{
	case IS_ACTIVING:
		ay=G;
		vy=-800;
		vx=200;
		y=y-TILE_SIZE-3;
		ResetRect();
		container->SetVar("isChanged",true);
		break;
	case IS_ATE:
		isNeedDelete=true;
		break;
	}
}
void Star::CheckObjects(list<Object*>* staObjs)
{
	if(isCheckedContainer) return;

	list<Object*>::iterator i;
	Object* obj;
	for(i=staObjs->begin();i!=staObjs->end();i++)
	{
		obj=*i;
		if(obj->ID==24||obj->ID==25)
			if(Collision::RectVsRect(rect,obj->rect))
				container=obj;
	}
	isCheckedContainer=true;
}
void Star::Move(float TPF,list<Object*>* staObjs,list<Object*>* dynObjs)
{
	Object* obj;
	//Theo phương x
	x+=vx*TPF;
	ResetRect();
	list<Object*>::iterator i;
	for(i=staObjs->begin();i!=staObjs->end();i++)
	{
		obj=*i;
		if(obj->ID>=20&&obj->ID<=29) //Nếu là thùng, đá , gạch...
			if(Collision::RectVsRect(rect,obj->rect)==true)	
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
		{
			if(Collision::RectVsRect(rect,obj->rect)==true)	
			{
				collisionList->push_back(obj);
				if(vy<0)					
				{
					ay=G;
					vy=200;
					y=obj->y+TILE_SIZE+1;
				}
			}
		}
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
					vy=-800;
				}
		}
		ResetRect();
	}
	xOld=x;
	yOld=y;
}
void Star::SetVar(char* varName,int val)
{
	if(strcmp(varName,"state")==0)
	{
		ChangeState(val);
		return;
	}
}
int Star::GetVar(char* varName)
{
	if(strcmp(varName,"state")==0)
		return state;
	if(strcmp(varName,"isNeedDelete")==0)
		return isNeedDelete;
	return -1;
}
void Star::Render(Camera* camera)
{
	if(state!=IS_IDLE)
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,ani->rect,0);
}
void Star::ResetWhenReload(Sprite* _sprite)
{
	isCheckedContainer=false;
	collisionList	=new list<Object*>;
	sprite=_sprite;
	ani=new Animation(4,5,0,sprite);
}
void Star::Save(fstream *fs)
{
	fs->write(reinterpret_cast<char*>(&ID),sizeof(ID));	//int ID, để khi load biết được đang load Obj nào
	int sizeObj=sizeof(*this);
	fs->write(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->write(reinterpret_cast<char*>(this),sizeObj);
}
void Star::Load(fstream *fs)
{
	int sizeObj;
	fs->read(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->read(reinterpret_cast<char*>(this),sizeObj);
}