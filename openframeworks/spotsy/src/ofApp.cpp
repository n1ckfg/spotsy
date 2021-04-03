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

	//receiver.init();
    receiver.setup();
    receiverDir.setup();
    ofAddListener(receiverDir.events.serverAnnounced, this, &ofApp::serverAnnounced);

    isConnected = false;
    address = settings.getValue("settings:host", "http://echo.websocket.org");
    status = "not connected";

    socketIO.setup(address);
    /*
     * You can also pass a query parameter at connection if needed.
     */
    // std::map<std::string,std::string> query;
    // query["token"] = "hello";
    // socketIO.setup(address, query);

    ofAddListener(socketIO.notifyEvent, this, &ofApp::gotEvent);
    ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);
}

void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg) {
    for(auto& dir : arg.servers) {
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    receiver.set(arg.servers[0].serverName, arg.servers[0].appName);
}


void ofApp::update() {
	timestamp = (int) ofGetSystemTimeMillis();
	
    //receiver.receive(texture);
	
    //if (texture.isAllocated()) {
    if (receiverDir.isValidIndex(0)) {
		fbo.begin();
		receiver.draw(0, 0, width, height);
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
    //ofDrawBitmapString("Spout: " + receiver.getChannelName(), 20, 20);
    ofDrawBitmapString("Syphon: " + receiver.getServerName(), 20, 20);
	//ofDrawBitmapString(client.isConnected() ? "ws client connected :)" : "ws client disconnected :(", 10, 50);
}

void ofApp::sendWsVideo() {
	string msg = "{\"unique_id\":\"" + uniqueId + "\",\"video\":\"" + ofxCrypto::base64_encode(videoBuffer) + "\",\"timestamp\":\"" + ofToString(timestamp) + "\"}";
	//client.send(msg);
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

void ofApp::onConnection () {
  isConnected = true;
  bindEvents();
}

void ofApp::bindEvents () {
  std::string serverEventName = "server-event";
  socketIO.bindEvent(serverEvent, serverEventName);
  ofAddListener(serverEvent, this, &ofApp::onServerEvent);

  std::string pingEventName = "pingy";
  socketIO.bindEvent(pingEvent, pingEventName);
  ofAddListener(pingEvent, this, &ofApp::onPingEvent);

  std::string nspingEventName = "nsping";
  socketIO.bindEvent(nspingEvent, nspingEventName, "nsp");
  ofAddListener(nspingEvent, this, &ofApp::onNSPingEvent);

  std::string arrayEventName = "array-event";
  socketIO.bindEvent(arrayEvent, arrayEventName);
  ofAddListener(arrayEvent, this, &ofApp::onArrayEvent);
}

void ofApp::gotEvent(string& name) {
  status = name;
}

void ofApp::onServerEvent (ofxSocketIOData& data) {
  ofLogNotice("ofxSocketIO", data.getStringValue("stringData"));
  ofLogNotice("ofxSocketIO", ofToString(data.getIntValue("intData")));
  ofLogNotice("ofxSocketIO", ofToString(data.getFloatValue("floatData")));
  ofLogNotice("ofxSocketIO", ofToString(data.getBoolValue("boolData")));

  ofxSocketIOData nested = data.getNestedValue("nested");
  ofLogNotice("ofxSocketIO", ofToString(nested.getStringValue("hello")));
}

void ofApp::onArrayEvent (ofxSocketIOData& data) {
    for (auto line : data.getVector()) {
        ofLogNotice("ofxSocketIO", line->get_string());
    }

  std::string who = "tina";
  std::string you = "[ \"you're simply the best\", \"better than all the rest\", \"better than anyone\", \"anyone I ever met\" ]";

  socketIO.emit(who, you);
}

void ofApp::onPingEvent (ofxSocketIOData& data) {
  ofLogNotice("ofxSocketIO", "pingy");
  std::string pong = "pongy";
  std::string param = "foo";
  socketIO.emit(pong, param);
  /*
   * You can also emit without param
   */
  // socketIO.emit(pong);
}

void ofApp::onNSPingEvent (ofxSocketIOData& data) {
  ofLogNotice("ofxSocketIO", "nsping");
  std::string pong = "nspong";
  std::string param = "foo";
  std::string nsp = "nsp";
  socketIO.emit(pong, param, nsp);
}
