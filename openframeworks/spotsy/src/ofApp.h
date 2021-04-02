#pragma once

#include "ofMain.h"
#include "ofxSpout.h"
#include "ofxXmlSettings.h"
#include "ofxHTTP.h"
#include "ofxJSONElement.h"
#include "ofxCrypto.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
	
		ofxSpout::Receiver receiver;
		ofTexture texture;

		ofxHTTP::SimpleWebSocketServer wsServer;
		ofxHTTP::SimpleWebSocketServerSettings wsSettings;
		int wsPort;
		int width = 640;
		int height = 480;
		int videoQuality; // 5 best to 1 worst, default 3 medium
		string uniqueId = "test";
		ofFile file;
		ofBuffer videoBuffer;
		int timestamp;
		ofFbo fbo;
		ofPixels pixels;

		void onWebSocketOpenEvent(ofxHTTP::WebSocketEventArgs& evt);
		void onWebSocketCloseEvent(ofxHTTP::WebSocketCloseEventArgs& evt);
		void onWebSocketFrameReceivedEvent(ofxHTTP::WebSocketFrameEventArgs& evt);
		void onWebSocketFrameSentEvent(ofxHTTP::WebSocketFrameEventArgs& evt);
		void onWebSocketErrorEvent(ofxHTTP::WebSocketErrorEventArgs& evt);
		
		void sendWsVideo();
		void generateUniqueId();

};
