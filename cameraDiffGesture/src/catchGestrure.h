#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "ofxBlobTracker.h"
#include "mycontants.h"
#include "ofxUDPManager.h"
#include "ofxCameraManagerUI.h"

#define IMG_WIDTH 320
#define IMG_HEIGHT 240

class catchGestrure
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
	string getPath()const;
	catchMovement * catcher;
	ofFbo *fbo;
	ofxCvGrayscaleImage *grayImage;		  //灰度图


	ofxBlobTracker * blobTracker;

	void clear();


	bool bRefreshBack;

	int threold;							//画面二极化阈值
	float distThreshold;					//动作距离阈值
	void loadSetting();
	void saveSetting();


	void blobAdded(ofxBlob &_blob);
	void blobMoved(ofxBlob &_blob);
	void blobDeleted(ofxBlob &_blob);

	float const touchTimeLen = 2.0f;
	float const unTouchTimeLen = 0.5f;
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

