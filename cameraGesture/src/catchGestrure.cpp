#include "catchGestrure.h"



catchGestrure::catchGestrure()
{
}


catchGestrure::~catchGestrure()
{
	waitForThread(true);
	clear();

	ofRemoveListener(blobTracker->blobAdded, this, &catchGestrure::blobAdded);
	ofRemoveListener(blobTracker->blobMoved, this, &catchGestrure::blobMoved);
	ofRemoveListener(blobTracker->blobDeleted, this, &catchGestrure::blobDeleted);
}

void catchGestrure::setup()
{
	//摄像头初始化！
	{
		vidGrabber = new ofVideoGrabber();

		vidGrabber->listDevices();

		vidGrabber->setDeviceID(0);
		vidGrabber->setVerbose(true);
		vidGrabber->initGrabber(IMG_WIDTH, IMG_HEIGHT);
	}
	
	//仿射矩阵
	{
		//仿射源矩阵
		srcPts[0] = ofVec2f(0, IMG_HEIGHT);
		srcPts[1] = ofVec2f(IMG_WIDTH, IMG_HEIGHT);
		srcPts[2] = ofVec2f(IMG_WIDTH, 0);
		srcPts[3] = ofVec2f(0, 0);

		//仿射目标矩阵
		dstPts[0] = ofVec2f(0, IMG_HEIGHT);
		dstPts[1] = ofVec2f(IMG_WIDTH, IMG_HEIGHT);
		dstPts[2] = ofVec2f(IMG_WIDTH, 0);
		dstPts[3] = ofVec2f(0, 0);

		affine.setup("affine.xml");

		setDstPts(affine.getBoxPts());
	}

	//纹理内存申请
	{
		affinedImg = new ofxCvColorImage();
		affinedImg->allocate(IMG_WIDTH, IMG_HEIGHT);
		colorImg = new ofxCvColorImage();
		colorImg->allocate(IMG_WIDTH, IMG_HEIGHT);

		grayImage = new ofxCvGrayscaleImage();
		grayImage->allocate(IMG_WIDTH, IMG_HEIGHT);

		blockGrayImage = new ofxCvGrayscaleImage();
		blockGrayImage->allocate(IMG_WIDTH, IMG_HEIGHT);
	}

	{
		blobTracker = new ofxBlobTracker();

		offset.set(10, 10);

		bRefreshBack = true;

		threold = 100;

		ofAddListener(blobTracker->blobAdded, this, &catchGestrure::blobAdded);
		ofAddListener(blobTracker->blobMoved, this, &catchGestrure::blobMoved);
		ofAddListener(blobTracker->blobDeleted, this, &catchGestrure::blobDeleted);
	}

	{
		loadSetting();
	}

	{
		isUntouching = false;
	}

	start();
}

void catchGestrure::update()
{
	auto ite = blobs.begin();
	while (ite != blobs.end())
	{
		auto & movers = ite->second;

		blobsClear(movers);

		if (movers.size() > 5)
		{
			float xOffset = movers.back().blob.centroid.x - movers.front().blob.centroid.x;
			float yOffset = movers.back().blob.centroid.y - movers.front().blob.centroid.y;

			xOffset = -xOffset;						//左右映像
			if ( abs(xOffset) > distThreshold)
			{
				unTouchTimer = ofGetElapsedTimef();
				isUntouching = true;
				if (xOffset < 0.0f)
				{
					cout << "left" << endl;
					ofSendMessage(msg_left_move);
				}
				else
				{
					cout << "right" << endl;
					ofSendMessage(msg_right_move);
				}

				movers.clear();
			}
			if (abs(yOffset) > distThreshold)
			{
				unTouchTimer = ofGetElapsedTimef();
				isUntouching = true;
				if (yOffset < 0.0f)
				{
					cout << "up" << endl;
					ofSendMessage(msg_up_move);
				}
				else
				{
					cout << "down" << endl;
					ofSendMessage(msg_down_move);
				}

				movers.clear();
			}
		}

		ite++;
	}


	if (isUntouching)
	{
		if (ofGetElapsedTimef() - unTouchTimer > unTouchTimeLen)
		{
			isUntouching = false;
		}
	}
}

void catchGestrure::draw()
{
	ofPushMatrix();
	lock();
	ofTranslate(offset.x, offset.y);
	
	colorImg->draw(0, 0, IMG_WIDTH, IMG_HEIGHT);
	affine.drawBox();

	ofTranslate(IMG_WIDTH + 10,0);
	affinedImg->draw(0, 0, IMG_WIDTH, IMG_HEIGHT);
	blobTracker->draw();

	ofTranslate(IMG_WIDTH + 10.0f,0.0);
	grayImage->draw(0, 0, IMG_WIDTH, IMG_HEIGHT);
	unlock();

	ofPopMatrix();

	ofPushStyle();
	ofSetColor(255, 0, 0);
	stringstream sss;
	sss << blobTracker->size() << endl;
	sss << "Press key left or right to set 'threold' value" << endl;
	sss << "threold:"<< threold << endl;
	sss << "Press key up or down to set 'dist threshold' value" << endl;
	sss << "move disDthreshold:" << distThreshold << endl;
	sss << "Press key 's' to save" << endl;
	ofDrawBitmapString(sss.str(), 500, 500);
	ofPopStyle();
}

void catchGestrure::mouseDragged(int x, int y) {
	affine.mouseDragged(x - offset.x, y - offset.y);
	setDstPts(affine.getBoxPts());
}

void catchGestrure::mouseMoved(int x, int y)
{
	affine.mouseMoved(x - offset.x, y - offset.y);
}

void catchGestrure::keyPressed(int key) {
	if (key == 's')
	{
		affine.saveMessage();
		saveSetting();
	}
	if (key == OF_KEY_F3)
	{
		vidGrabber->videoSettings();
	}
	if (OF_KEY_RETURN == key)
	{
		bRefreshBack = true;
	}

	if (OF_KEY_LEFT == key)
	{
		threold--;
		threold = ofClamp(threold, 0, 255);
	}
	else if (OF_KEY_RIGHT == key)
	{
		threold++;
		threold = ofClamp(threold, 0, 255);
	}
	else if (OF_KEY_UP == key)
	{
		distThreshold -= 0.01f;
	}
	else if (OF_KEY_DOWN == key)
	{
		distThreshold += 0.01f;
	}

}

void catchGestrure::mousePressed(int x, int y)
{
	affine.mousePressed(x - offset.x, y - offset.y);
}

void catchGestrure::mouseReleased(int x, int y)
{
	affine.mouseReleased(x - offset.x, y - offset.y);
}

void catchGestrure::setDstPts(ofVec2f* affinePts) {
	for (int i = 0; i < 4; i++)
	{
		srcPts[i] = affinePts[i];
	}
}

void catchGestrure::caThread() {
	ofSleepMillis(50);		// 线程等待时间!

	lock();
	vidGrabber->update();
	//获取源图
	if (vidGrabber->isFrameNew())
	{

		//获取摄像头画面并进行仿射变换
		colorImg->setFromPixels(vidGrabber->getPixels().getPixels(), IMG_WIDTH, IMG_HEIGHT);
		affinedImg->warpIntoMe(*colorImg, srcPts, dstPts);//仿射变换！

		*grayImage = *affinedImg;
		grayImage->threshold(threold);
		*blockGrayImage = *grayImage;
		if (bRefreshBack)
		{
			blobTracker->refreshBack(*blockGrayImage);
			bRefreshBack = false;
		}

		blobTracker->update(*blockGrayImage);
	}
	unlock();
}

void catchGestrure::clear()
{
	if (vidGrabber)
	{
		vidGrabber->close();
		delete vidGrabber;
		vidGrabber = NULL;
	}

	if (colorImg)
	{
		delete colorImg;
		colorImg = NULL;
	}

	if (affinedImg)
	{
		delete affinedImg;
		affinedImg = NULL;
	}

	if (grayImage)
	{
		delete grayImage;
		grayImage = NULL;
	}
}

void catchGestrure::loadSetting()
{
	ofxXmlSettings xml;
	xml.load("catchGesture.xml");

	threold = xml.getValue("threshold", 100);
	distThreshold = xml.getValue("distThreshold", 0.5f);
}

void catchGestrure::saveSetting()
{
	ofxXmlSettings xml;

	xml.setValue("threshold", 100);
	xml.setValue("distThreshold", 0.5f);

	xml.save("catchGesture.xml");
}

void catchGestrure::blobAdded(ofxBlob &_blob) {
//	ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " added" + " x:" + ofToString(_blob.centroid.x) + " y:" + ofToString(_blob.centroid.y));

	BLOB_DATA data;
	data.blob = _blob;
	data.moment = ofGetElapsedTimef();

	blobs[_blob.id].push_back(data);

//	cout << "add ok" << endl;

}

void catchGestrure::blobMoved(ofxBlob &_blob) {
	if (isUntouching)return;
//	ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " moved" + "x: " + ofToString(_blob.centroid.x) + " y:" + ofToString(_blob.centroid.y));

	BLOB_DATA data;
	data.blob = _blob;
	data.moment = ofGetElapsedTimef();

	blobs[_blob.id].push_back(data);

//	cout << "update ok" << endl;
}

void catchGestrure::blobDeleted(ofxBlob &_blob) {
//	ofLog(OF_LOG_NOTICE, "Blob ID " + ofToString(_blob.id) + " deleted");

	blobs.erase(_blob.id);
}

void catchGestrure::blobsClear(list<BLOB_DATA> & datas)
{
	if (datas.size())
	{
		auto newest = datas.back();

		float timer = newest.moment;

		auto ite = datas.begin();
		while (ite != datas.end())
		{
			auto nextblob = *ite;
			if (timer-nextblob.moment > touchTimeLen)
			{
				ite = datas.erase(ite);
			}
			else
			{
				ite++;
			}
		}
	}
}

