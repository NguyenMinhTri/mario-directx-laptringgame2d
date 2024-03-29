#pragma once

#include "d3dx9.h"

class Collision
{
public:
	static int RectVsCircle(RECT, float xO, float yO, float R);
	static int RectVsCircle(RECT, float xO1, float yO1,float xO2,float yO2, float R,int differential);
	static int PointInLine(float xM,float yM, float xA, float yA, float xB, float yB);
	static float Distance(float xA, float yA,float xB, float yB);
	//Lấy điểm đối xứng với điểm trên trục tọa độ
	static float Symmetrical(float a, float mirror);
	//Lấy đối xứng A qua M
	static void SymmetricalPoint(float &xA,float &yA,float xM,float yM);
	//Lấy đối xứng M qua đường thẳng Ax, có hệ số góc là a
	static void SymmetricalLine(float &xM,float &yM,float xA, float yA,float a);
	static bool RectVsRect(RECT r1, RECT r2);
	static bool RectVsRect(RECT r1Old, RECT r1, RECT r2Old,RECT r2,int differential);
};

float MiddleNumber(float a,float b,float c);