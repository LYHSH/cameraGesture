#include "affineBox.h"

affineBox::affineBox(void)
{
}

affineBox::~affineBox(void)
{
}
void affineBox::setup(){

	configName = "CMT_data/affine.xml";
	setup(configName);
}

void affineBox::setup(string _configFile)
{
	configName = _configFile;
	ofxXmlSettings xml;
	xml.loadFile(configName);
	float px1 = xml.getValue("point:x",0.0,0);
	float py1 = xml.getValue("point:y",0.0,0);
	float px2 = xml.getValue("point:x",320.0,1);
	float py2 = xml.getValue("point:y",0.0  ,1);
	float px3 = xml.getValue("point:x",320.0,2);
	float py3 = xml.getValue("point:y",240.0,2);
	float px4 = xml.getValue("point:x",0.0  ,3);
	float py4 = xml.getValue("point:y",240.0,3);
	int _threshold = xml.getValue("threshold",40,0);
	affinePts[0] = ofVec2f(px1,py1);
	affinePts[1] = ofVec2f(px2,py2);
	affinePts[2] = ofVec2f(px3,py3);
	affinePts[3] = ofVec2f(px4,py4);

	for (int i = 0; i < 4; i++)
	{
		affineDragPts[i].bOver = false;
		affineDragPts[i].bDrag = false;
		affineDragPts[i].radio = 6;
	}
}
void affineBox::update(){

}

//----------------------得到仿射点坐标
ofVec2f* affineBox::getBoxPts(){	
	for (int i=0;i<4;i++)
	{
		affinePoints[i] = affinePts[3-i];
	}
	return affinePoints;
}
//----------------------绘制仿射点及边界线
void affineBox::drawBox(){
	ofPushStyle();
	ofSetColor(0,255,255);
	ofLine(affinePts[0].x,affinePts[0].y,affinePts[1].x,affinePts[1].y);
	ofLine(affinePts[1].x,affinePts[1].y,affinePts[2].x,affinePts[2].y);
	ofLine(affinePts[2].x,affinePts[2].y,affinePts[3].x,affinePts[3].y);
	ofLine(affinePts[3].x,affinePts[3].y,affinePts[0].x,affinePts[0].y);
	for (int i=0;i<4;i++)
	{
		if (affineDragPts[i].bOver)
		{
			ofFill();
		}
		else
		{
			ofNoFill();
		}
		ofCircle(affinePts[i].x,affinePts[i].y,6);
	}
	ofPopStyle();
}
//----------------------保存仿射点信息
void affineBox::saveMessage(){
	ofxXmlSettings xml;
	for (int i=0;i<4;i++)
	{
		xml.setValue("point:x",affinePts[i].x,i);
		xml.setValue("point:y",affinePts[i].y,i);
	}
	xml.saveFile(configName);
	
}

void affineBox::mouseMoved(int x,int y)
{
	for (int i = 0; i < 4; i++)
	{
		if (ofDist(x,y,affinePts[i].x,affinePts[i].y) < affineDragPts[i].radio)
		{
			affineDragPts[i].bOver = true;
		}
		else
		{
			affineDragPts[i].bOver = false;
		}
	}
}

void affineBox::mouseReleased(int x,int y)
{
	for (int i = 0; i < 4; i++)
	{
		affineDragPts[i].bOver = false;
		affineDragPts[i].bDrag = false;
	}
}

void affineBox::mousePressed(int x,int y)
{
	for (int i = 0; i < 4; i++)
	{
		if (affineDragPts[i].bOver)
		{
			affineDragPts[i].bDrag = true;
		}
	}
}


void affineBox::mouseDragged(int x,int y){
	for (int i = 0; i < 4; i++)
	{
		if (affineDragPts[i].bDrag)
		{
			affinePts[i] = ofVec2f(x,y);
		}
	}

}
