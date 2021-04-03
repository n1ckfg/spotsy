#include "ofApp.h"

void ofApp::setup() {
	settings.loadFile("settings.xml");
	width = settings.getValue("settings:width", 640);
	height = settings.getValue("settings:height", 480);
	videoQuality = settings.getValue("settings:video_quality", 3);

	ofSetWindowShape(width, height);
	generateUniqueId();
	fbo.allocate(width, height, GL_RGBA);
	pixels.allocate(width, height, OF_IMAGE_COLOR);

	receiver.init();
	clientOptions = ofxLibwebsockets::defaultClientOptions();

	clientOptions.host = settings.getValue("settings:host", "echo.websocket.org");
	clientOptions.port = settings.getValue("settings:port", 80);
	clientOptions.bUseSSL = (bool) settings.getValue("settings:bUseSSL", 0);
	clientOptions.channel = settings.getValue("settings:channel", "/");
	clientOptions.protocol = settings.getValue("settings:protocol", "NULL");
	clientOptions.version = settings.getValue("settings:version", -1); // -1 to use latest
	clientOptions.reconnect = (bool) settings.getValue("settings:reconnect", 0); // docs warn this can crash
	clientOptions.reconnectInterval = settings.getValue("settings:reconnectInterval", 1000);
	clientOptions.ka_time = settings.getValue("settings:ka_time", 0);
	clientOptions.ka_probes = settings.getValue("settings:ka_probes", 0);
	clientOptions.ka_interval = settings.getValue("settings:ka_interval", 0);

	ofSetLogLevel(OF_LOG_VERBOSE);
	client.connect(clientOptions);
	ofSetLogLevel(OF_LOG_ERROR);

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

		//sendWsVideo();
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
	cout << "on connected" << endl;
}

//--------------------------------------------------------------
void ofApp::onOpen(ofxLibwebsockets::Event& args) {
	cout << "on open" << endl;
}

//--------------------------------------------------------------
void ofApp::onClose(ofxLibwebsockets::Event& args) {
	cout << "on close" << endl;
}

//--------------------------------------------------------------
void ofApp::onIdle(ofxLibwebsockets::Event& args) {
	cout << "on idle" << endl;
}

//--------------------------------------------------------------
void ofApp::onMessage(ofxLibwebsockets::Event& args) {
	cout << "got message " << args.message << endl;
}

//--------------------------------------------------------------
void ofApp::onBroadcast(ofxLibwebsockets::Event& args) {
	cout << "got broadcast " << args.message << endl;
}