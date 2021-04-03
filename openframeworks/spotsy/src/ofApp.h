#pragma once

#include "ofMain.h"
#include "ofxXmlPoco.h"
#include "ofxXmlSettings.h"
//#include "ofxSpout.h"
#include "ofxSyphon.h"
#include "ofxCrypto.h"
#include "ofxLibwebsockets.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
	
		// websocket methods
		void onConnect(ofxLibwebsockets::Event& args);
		void onOpen(ofxLibwebsockets::Event& args);
		void onClose(ofxLibwebsockets::Event& args);
		void onIdle(ofxLibwebsockets::Event& args);
		void onMessage(ofxLibwebsockets::Event& args);
		void onBroadcast(ofxLibwebsockets::Event& args);

		void sendWsVideo();
		void generateUniqueId();

		//ofxSpout::Receiver receiver;
        ofxSyphonClient receiver;
        ofxSyphonServerDirectory receiverDir;
        void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);

        ofxLibwebsockets::Client client;
		ofxLibwebsockets::ClientOptions clientOptions;

		ofTexture texture;
		int width = 640;
		int height = 480;
		int videoQuality; // 5 best to 1 worst, default 3 medium
		string uniqueId = "test";
		ofFile file;
		ofxXmlSettings settings;
		ofBuffer videoBuffer;
		int timestamp;
		ofFbo fbo;
		ofPixels pixels;
	


};
