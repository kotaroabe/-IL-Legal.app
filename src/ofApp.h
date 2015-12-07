#pragma once

#include "ofMain.h"
#include "ofxClipboard.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void reset();
        void youtubePlay();
        ofImage img;
        ofImageQualityType quality;
        ofVideoPlayer glitchVideo;
        bool glitchFlag;
        bool playFlag;
        bool endFlag;
        int seconds;
        int finishingTime;
        int gapTime;
        string clipboardContent;
        ofTrueTypeFont titleFont;
        ofTrueTypeFont font;
        float noiseAmount;
        string URL;
        string randomString;
    
        ofxButton clipUrl;
        ofxPanel gui;
    
};
