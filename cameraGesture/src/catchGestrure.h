#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "affineBox.h"
#include "ofxBlobTracker.h"
#include "mycontants.h"
#include "ofxUDPManager.h"

#define IMG_WIDTH 320
#define IMG_HEIGHT 240

class catchGestrure : public ofThread
{
public:
	catchGestrure();
	~catchGestrure();

	void setup();
	void update();
	void draw();

	void mouseDragged(int x, int y);
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);
	void mouseMoved(int x, int y);

	void keyPressed(int key);
private:
	void start() {
		startThread(true, false);   // blocking, verbose
	}

	void stop() {
		stopThread();
	}

	void threadedFunction()
	{
		while (isThreadRunning())
		{
			caThread();
		}
	}

	void caThread();
	ofVideoGrabber* 		vidGrabber;

	ofxCvColorImage         *colorImg;    //源(彩色)图像
	ofxCvColorImage 	*affinedImg;	  //仿射变换后的的图像
	ofxCvGrayscaleImage *grayImage;		  //灰度图
	ofxCvGrayscaleImage * blockGrayImage;	//捕获灰度图


	ofPoint srcPts[4];                 //源仿射数据数组
	ofPoint dstPts[4];                 //目标仿射数据数组
	affineBox affine;
	void setDstPts(ofVec2f* dispts);


	ofxBlobTracker * blobTracker;

	ofVec2f offset;

	void clear();


	bool bRefreshBack;

	int threold;							//画面二极化阈值
	float distThreshold;					//动作距离阈值
	void loadSetting();
	void saveSetting();


	void blobAdded(ofxBlob &_blob);
	void blobMoved(ofxBlob &_blob);
	void blobDeleted(ofxBlob &_blob);

	float const touchTimeLen = 1.0f;
	float const unTouchTimeLen = 1.5f;
	bool isUntouching;
	float unTouchTimer;

	struct BLOB_DATA
	{
		ofxBlob blob;
		float moment;
	}; 

	void blobsClear(list<BLOB_DATA> & datas);

	map<int, list<BLOB_DATA>> blobs;
};

