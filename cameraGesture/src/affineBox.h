/************************************************************************/
/* 项目名称：徐州墙面互动投影                                                                     
/* 文件描述：仿射矩阵类
/* author:hui
/* date:2016/1/3
/* 补充：增加UI调试界面，对代码进行优化
/************************************************************************/
#ifndef AFFINE_BOX_H
#define AFFINE_BOX_H
#include "ofMain.h"
#include "ofxXmlSettings.h"
class affineBox
{
public:
	affineBox(void);
	~affineBox(void);

	struct DragPoint
	{
		bool bOver;
		bool bDrag;
		float radio;
	};

	void setup();
	void setup(string _configFile);
	void update();
	void drawBox();
	ofVec2f* getBoxPts();
	void saveMessage();

	void mouseReleased(int x,int y);
	void mousePressed(int x,int y);
	void mouseMoved(int x,int y);
	void mouseDragged(int x,int y);

	ofVec2f affinePts[4];
	DragPoint affineDragPts[4];
	ofVec2f affinePoints[4];

	string dir;			//数据目录
	string configName;


};
#endif


