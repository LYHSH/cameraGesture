#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	catcher.setup();

	{
		ofxXmlSettings xml;
		xml.load("ip.xml");
		udp.Create();

		string ip = xml.getValue("ip", "127.0.0.1");
		int port = xml.getValue("port", 6666);

		udp.Connect(ip.c_str(), port);
		udp.SetNonBlocking(true);
	}
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	catcher.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	catcher.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	catcher.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	catcher.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	catcher.mouseDragged(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	catcher.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	catcher.mouseReleased(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	string message = msg.message;
	string cmd = "";
	if (message == msg_left_move)
	{
		cmd = "left";
	}
	else if (message == msg_right_move)
	{
		cmd = "right";
	}
	else if (message == msg_up_move)
	{
		cmd = "up";
	}
	else if (message == msg_down_move)
	{
		cmd = "down";
	}

	udp.Send(cmd.c_str(), cmd.size());
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
