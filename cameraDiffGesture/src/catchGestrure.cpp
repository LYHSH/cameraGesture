#include "catchGestrure.h"



catchGestrure::catchGestrure()
{
}


catchGestrure::~catchGestrure()
{
	clear();

	ofRemoveListener(blobTracker->blobAdded, this, &catchGestrure::blobAdded);
	ofRemoveListener(blobTracker->blobMoved, this, &catchGestrure::blobMoved);
	ofRemoveListener(blobTracker->blobDeleted, this, &catchGestrure::blobDeleted);
}

string catchGestrure::getPath()const
{
	return "ofxVideoCapture/";
}

void catchGestrure::setup()
{
	{
		catcher = new catchMovement();
		catcher->CMT_setup(IMG_WIDTH, IMG_HEIGHT, 5, getPath() + "CMT_data/affine" + ofToString(0) + ".xml", 0);
		catcher->loadConfig(getPath() + "CMT_data/config" + ofToString(0) + ".xml");
	}
	//ÎÆÀíÄÚ´æÉêÇë
	{

		grayImage = new ofxCvGrayscaleImage();
		grayImage->allocate(IMG_WIDTH, IMG_HEIGHT);

		fbo = new ofFbo();
		fbo->allocate(IMG_WIDTH, IMG_HEIGHT, GL_LUMINANCE);

		fbo->begin();
		ofClear(0, 0, 0, 255);
		fbo->end();


		
	}

	{
		blobTracker = new ofxBlobTracker();
		ofPixels pixel;
		fbo->readToPixels(pixel);
		grayImage->setFromPixels(pixel);
		blobTracker->refreshBack(*grayImage);
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
}

void catchGestrure::update()
{
	*grayImage = catcher->bufferFloat;
	blobTracker->update(*grayImage, threold, 3);


	auto ite = blobs.begin();
	while (ite != blobs.end())
	{
		auto & movers = ite->second;

		blobsClear(movers);

		if (movers.size() > 5)
		{
			//cout << "xxxxx:"<<movers.size() << endl;
			float xOffset = movers.back().blob.centroid.x - movers.front().blob.centroid.x;
			float yOffset = movers.back().blob.centroid.y - movers.front().blob.centroid.y;

			xOffset = -xOffset;						//×óÓÒÓ³Ïñ
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
	//catcher->CMT_draw();
	blobTracker->draw();
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
	catcher->CMT_mouseDragged(x, y);
}

void catchGestrure::mouseMoved(int x, int y)
{
	catcher->CMT_mouseMoved(x, y);
}

void catchGestrure::keyPressed(int key) {
	catcher->CMT_keyPressed(key);

	if (key == 's')
	{
		saveSetting();
		catcher->configSave();
	}
	if (key == OF_KEY_F3)
	{
		
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
	catcher->CMT_mousePressed(x, y);
}

void catchGestrure::mouseReleased(int x, int y)
{
	catcher->CMT_mouseReleased(x, y);
}


void catchGestrure::clear()
{
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
	if (isUntouching)return;
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

