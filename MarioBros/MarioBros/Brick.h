#pragma once

#include "Animation.h"
#include "Sprite.h"
#include "Object.h"
#include <list>
using namespace std;

class Brick:public Object
{
private:
	char	state;
	float	yOld;
	float	vy;
	float	ay;
	float	xBreak1;		//Tham số của các mảnh vỡ. 1 là mảnh trên, 2 là mảnh dưới
	float	yBreak1;
	float	xBreak2;
	float	yBreak2;
	float	vyBreak1;
	float	vyBreak2;
	float	vxBreak1;
	float	vxBreak2;
	float	ayBreak;
	float	angle;
	bool	isContainCoin;
public:
	bool	isBright;
	bool	isNeedDelete;
	bool	isChanged;

	Brick();
	Brick(float _x, float _y,float width,float height,bool _isBright,int _ID,Sprite* _sprite);
	void SetVar(char* varName,int val);
	int GetVar(char* varName);
	void Update(list<Object*>* staObjs,list<Object*>* dynObjs,Input* input, float TPF);
	void Render(Camera *camera);
	void ChangeState(char _state);
	void ResetWhenReload(Sprite* _sprite);
	void Save(fstream *fs);
	void Load(fstream *fs);
};