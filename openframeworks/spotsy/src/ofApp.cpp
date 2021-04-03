#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowShape(width, height);
	generateUniqueId();
	fbo.allocate(width, height, GL_RGBA);
	pixels.allocate(width, height, OF_IMAGE_COLOR);

	receiver.init();

	wsUrl = "https://" + wsUrl + "/socket.io/";
	client.connect(wsUrl, 80);
	client.addListener(this);
}

void ofApp::update() {
	timestamp = (int) ofGetSystemTimeMillis();
	receiver.receive(texture);

	if (texture.isAllocated()) {
		fbo.begin();
		texture.draw(0, 0, width, height);
		fbo.end();
		fbo.readToPixels(pixels);

		switch (videoQuality) {
		case 5:
			ofSaveImage(pixels, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
			break;
		case 4:
			ofSaveImage(pixels, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
			break;
		case 3:
			ofSaveImage(pixels, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
			break;
		case 2:
			ofSaveImage(pixels, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
			break;
		case 1:
			ofSaveImage(pixels, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
			break;
		}

		sendWsVideo();
	}
}

void ofApp::draw() {
	fbo.draw(0, 0);
	ofDrawBitmapString("Spout: " + receiver.getChannelName(), 20, 20);
	ofDrawBitmapString(client.isConnected() ? "ws client connected" : "ws client disconnected :(", 10, 50);
}

void ofApp::sendWsVideo() {
	string msg = "{\"unique_id\":\"" + uniqueId + "\",\"video\":\"" + ofxCrypto::base64_encode(videoBuffer) + "\",\"timestamp\":\"" + ofToString(timestamp) + "\"}";
	client.send(msg);
}

void ofApp::generateUniqueId() {
	// a randomly generated id
	file.open(ofToDataPath("unique_id.txt"), ofFile::ReadWrite, false);
	ofBuffer buff;
	if (file) { // use existing file if it's there
		buff = file.readToBuffer();
		uniqueId = buff.getText();
	}
	else { // otherwise make a new one
		uniqueId += "_" + ofGetTimestampString("%y%m%d%H%M%S%i");
		ofStringReplace(uniqueId, "\n", "");
		ofStringReplace(uniqueId, "\r", "");
		buff.set(uniqueId.c_str(), uniqueId.size());
		ofBufferToFile("unique_id.txt", buff);
	}
}

void ofApp::onConnect(ofxLibwebsockets::Event& args) {
	ofLogVerbose() << "on connect";
}

//--------------------------------------------------------------
void ofApp::onOpen(ofxLibwebsockets::Event& args) {
	ofLogVerbose() << "on open";
}

void ofApp::onClose(ofxLibwebsockets::Event& args) {
	ofLogVerbose() << "on close";
}

void ofApp::onIdle(ofxLibwebsockets::Event& args) {
	ofLogVerbose() << "on idle";
}

void ofApp::onMessage(ofxLibwebsockets::Event& args) {
	cout << "got message " << args.message << endl;
}

void ofApp::onBroadcast(ofxLibwebsockets::Event& args) {
	cout << "got broadcast " << args.message << endl;
}