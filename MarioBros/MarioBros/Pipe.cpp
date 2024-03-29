#include "Pipe.h"
#include "Object.h"
#include "Info.h"
#include "Animation.h"
#include "Sprite.h"

Pipe::Pipe():Object(){}
Pipe::Pipe(float _x, float _y,float width,float height,int _ID,Sprite* _sprite):Object(_x,_y,width,height)
{
	sprite	=_sprite;
	ID	=_ID;	
	ani=new Animation(2,2,0,sprite);
}
void Pipe::Render(Camera *camera)
{
	switch(ID)
	{
	case 20:						//Pipe 1
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,ani->GetRect(0),0.3);				//Đỉnh ống
		sprite->Draw(x-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(2),0.3);				//Chân ống
		break;
	case 21:						//Pipe 2
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,ani->GetRect(0),0.3);				//Đỉnh ống
		sprite->Draw(x-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(2),0.3);				//Chân ống
		sprite->Draw(x-camera->x,y+TILE_SIZE*2,ZOOM,ZOOM,ani->GetRect(2),0.3);
		break;
	case 22:
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,ani->GetRect(0),0.3);				//Đỉnh ống
		sprite->Draw(x-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(2),0.3);				//Chân ống
		sprite->Draw(x-camera->x,y+TILE_SIZE*2,ZOOM,ZOOM,ani->GetRect(2),0.3);
		sprite->Draw(x-camera->x,y+TILE_SIZE*3,ZOOM,ZOOM,ani->GetRect(2),0.3);
		break;
	case 23:
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,D3DXVECTOR2(40,40),-PI/2,D3DCOLOR_XRGB(255,255,255),ani->GetRect(0),0.3);
		sprite->Draw(x+TILE_SIZE-camera->x,y,ZOOM,ZOOM,D3DXVECTOR2(40,40),-PI/2,D3DCOLOR_XRGB(255,255,255),ani->GetRect(2),0.3);
		break;
	case 28:
		sprite->Draw(x-camera->x,y,ZOOM,ZOOM,ani->GetRect(1),0.3);
		sprite->Draw(x-camera->x,y+TILE_SIZE,ZOOM,ZOOM,ani->GetRect(3),0.3);
	}
}
void Pipe::ResetWhenReload(Sprite* _sprite)
{
	sprite=_sprite;
	ani=new Animation(2,2,0,sprite);
}
void Pipe::Save(fstream *fs)
{
	fs->write(reinterpret_cast<char*>(&ID),sizeof(ID));	//int ID, để khi load biết được đang load Obj nào
	int sizeObj=sizeof(*this);
	fs->write(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->write(reinterpret_cast<char*>(this),sizeObj);
}
void Pipe::Load(fstream *fs)
{
	int sizeObj;
	fs->read(reinterpret_cast<char*>(&sizeObj),sizeof(int));			//int sizeof(obj)
	fs->read(reinterpret_cast<char*>(this),sizeObj);
}