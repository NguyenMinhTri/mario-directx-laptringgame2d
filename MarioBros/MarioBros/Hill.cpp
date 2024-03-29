#include "Hill.h"
#include "Object.h"
#include "Info.h"
#include "Animation.h"
#include "Sprite.h"

Hill::Hill():Object(){}
Hill::Hill(float _x, float _y,float width,float height,int _ID,Sprite* _sprite):Object(_x,_y,width,height)
{
	sprite	=_sprite;
	ID	=_ID;	
	ani=new Animation(4,5,0,sprite);
}
void Hill::Render(Camera *camera)
{
	switch(ID)
	{
	case 16:						//Hill 1
		sprite->Draw(x+TILE_SIZE-camera->x,y,ZOOM,ZOOM,ani->GetRect(3),1.0);				//Đỉnh núi
		sprite->Draw(x-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(0),1.0);				//Chân núi bên trái
		sprite->Draw(x+TILE_SIZE-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(1),1.0);	//Giữa núi
		sprite->Draw(x+2*TILE_SIZE-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(2),1.0);	//Chân núi bên phải
		break;
	case 17:						//Hill 2
		sprite->Draw(x+2*TILE_SIZE-camera->x,y,ZOOM,ZOOM,ani->GetRect(3),1.0);
		sprite->Draw(x+TILE_SIZE-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(0),1.0);
		sprite->Draw(x+2*TILE_SIZE-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(1),1.0);
		sprite->Draw(x+3*TILE_SIZE-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(2),1.0);
		sprite->Draw(x-camera->x,y+2*TILE_SIZE,ZOOM,ZOOM,ani->GetRect(0),1.0);
		sprite->Draw(x+TILE_SIZE-camera->x,y+2*TILE_SIZE,ZOOM,ZOOM,ani->GetRect(1),1.0);
		sprite->Draw(x+2*TILE_SIZE-camera->x,y+2*TILE_SIZE,ZOOM,ZOOM,ani->GetRect(7),1.0);
		sprite->Draw(x+3*TILE_SIZE-camera->x,y+2*TILE_SIZE,ZOOM,ZOOM,ani->GetRect(1),1.0);
		sprite->Draw(x+4*TILE_SIZE-camera->x,y+2*TILE_SIZE,ZOOM,ZOOM,ani->GetRect(2),1.0);
		break;
	}
}
void Hill::ResetWhenReload(Sprite* _sprite)
{
	sprite=_sprite;
	ani=new Animation(4,5,0,sprite);	
}
void Hill::Save(fstream *fs)
{
	fs->write(reinterpret_cast<char*>(&ID),sizeof(ID));	//int ID, để khi load biết được đang load Obj nào
	int sizeObj=sizeof(*this);
	fs->write(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->write(reinterpret_cast<char*>(this),sizeObj);
}
void Hill::Load(fstream *fs)
{
	int sizeObj;
	fs->read(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->read(reinterpret_cast<char*>(this),sizeObj);
}