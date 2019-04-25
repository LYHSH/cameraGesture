/************************************************************************/
/* ��Ŀ���ƣ�����ǽ�滥��ͶӰ                                                                     
/* �ļ����������������
/* author:hui
/* date:2016/1/3
/* ���䣺����UI���Խ��棬�Դ�������Ż�
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

	string dir;			//����Ŀ¼
	string configName;


};
#endif


