#pragma once

#include "ofMain.h"
#include "ofxXmlPoco.h"
#include "ofxXmlSettings.h"
//#include "ofxSpout.h"
#include "ofxSyphon.h"
#include "ofxCrypto.h"
#include "ofEvents.h"
#include "ofxSocketIO.h"
#include "ofxSocketIOData.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
	
		void sendWsVideo();
		void generateUniqueId();
        
        // SPOUT
		//ofxSpout::Receiver receiver;
        // SYPHON
        ofxSyphonClient receiver;
        ofxSyphonServerDirectory receiverDir;
        void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);

        ofxSocketIO socketIO;
        bool isConnected;
        void onConnection();
        void bindEvents();
        string host;
        string status = "...";
        void gotEvent(std::string& name);
        ofEvent<ofxSocketIOData&> serverEvent;
        void onServerEvent(ofxSocketIOData& data);
        ofEvent<ofxSocketIOData&> pingEvent;
        void onPingEvent(ofxSocketIOData& data);
        ofEvent<ofxSocketIOData&> nspingEvent;
        void onNSPingEvent(ofxSocketIOData& data);
        ofEvent<ofxSocketIOData&> arrayEvent;
        void onArrayEvent(ofxSocketIOData& data);
    
        ofTexture texture;
		int width, height, fps, videoQuality;
		string uniqueId = "spotsy";
		ofFile file;
		ofxXmlSettings settings;
		ofBuffer videoBuffer;
		int timestamp;
		ofFbo fbo;
		ofPixels pixels;
	


};
