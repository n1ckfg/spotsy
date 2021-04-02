#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowShape(width, height);
	generateUniqueId();
	fbo.allocate(width, height, GL_RGBA);
	pixels.allocate(width, height, OF_IMAGE_COLOR);

	receiver.init();

	// * websockets *
	// https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/include/ofx/HTTP/WebSocketConnection.h
	// https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/src/WebSocketConnection.cpp
	// events: connect, open, close, idle, message, broadcast
	wsSettings.setPort(wsPort);
	wsServer.setup(wsSettings);
	wsServer.webSocketRoute().registerWebSocketEvents(this);
	wsServer.start();
	cout << "Using websockets." << endl;
}

void ofApp::update() {
	timestamp = (int) ofGetSystemTimeMillis();
	receiver.receive(texture);

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
}

void ofApp::draw() {
	fbo.draw(0, 0);
	ofDrawBitmapString("Channel : " + receiver.getChannelName(), 20, 20);

	sendWsVideo();
}

void ofApp::sendWsVideo() {
	string msg = "{\"unique_id\":\"" + uniqueId + "\",\"video\":\"" + ofxCrypto::base64_encode(videoBuffer) + "\",\"timestamp\":\"" + ofToString(timestamp) + "\"}";
	wsServer.webSocketRoute().broadcast(ofxHTTP::WebSocketFrame(msg));
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

