#include "ofApp.h"

/*
 
 let's do this!
 
 About recording a video, I am simply using Quicktime, LadioCast and Soundflowerbed to capture.
 
 */

// pool of characters to generate random string
static const char pool[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789";
int poolSize = sizeof(pool)-1;
char getRandomChar(){
    return pool[rand() % poolSize];
}

//--------------------------------------------------------------
string getYoutubeDlStream(string _link){
    string command = "/usr/local/bin/youtube-dl --get-url --format 18 " + _link;
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    string dlStream = result;
    
    ofLogNotice(command + "---> " + dlStream);
    
    bool success = (ofIsStringInString(dlStream, "http://") || ofIsStringInString(dlStream, "https://"));
    
    return ( success? dlStream : "");
    
    /* reference http://forum.openframeworks.cc/t/picking-a-strategy-for-playing-video-from-youtube-solved/12907/7 */
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetEscapeQuitsApp(false);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSetFrameRate(20);
    ofSetVerticalSync(true);
    quality = OF_IMAGE_QUALITY_HIGH;
    glitchFlag = true;
    playFlag = false;
    endFlag = false;
    seconds = ofGetElapsedTimef();
    gapTime = ofRandom(4);
    
    titleFont.loadFont("DIN-LightItalicAlt.ttf", 48);
    font.loadFont("DIN-LightItalicAlt.ttf", 12);
    
//    gui.setup();
//    gui.add(clipUrl.setup("Copy from Clipboard"));
    
    /*
     
     HOW ABOUT USING A RANDOM URL INSTEAD?
     
     ALGORITHM OUT OF CONTROL
     
     AND HOW CAN I GET THE FAN? USING TWITTER?
     
     SOMETIMES I SHOULD DO SPECULATIVE APPROACH
     
     */
    
    /*
     https://www.youtube.com/watch?v=iIprJoPTJoI good example 
     */
    
//    for(int i=0; i<11; i++){
//        randomString += getRandomChar();
//    }
//    URL = "https://www.youtube.com/watch?v=" + randomString;
//    cout << URL << endl;
//    glitchVideo.loadMovie(getYoutubeDlStream(URL));
    URL = "https://www.youtube.com/watch?v=vbeCFcfHxhQ";
    glitchVideo.loadMovie(getYoutubeDlStream(URL));
    randomString = "";
    glitchVideo.setFrame(20);
    //glitchVideo.play();
    
    /* resizing and repositioning window size */
    ofSetWindowShape(640, 360);
    ofSetWindowPosition(350, 200);
    
    img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    
    /* artist: algorithm
     this algorithm starts to go mad, and keep doing this uploading.
     claiming that he made this.
     */
}

//--------------------------------------------------------------
void ofApp::reset(){
    img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    glitchVideo.update();
    glitchVideo.draw(0, 0, ofGetWidth(), ofGetHeight());
    //ofSetDataPathRoot("./ilLegal.app/Contents/Resources/");
    string compressedFilename = "compressed.jpg";
    
    img.saveImage(compressedFilename, quality);
    ofBuffer file = ofBufferFromFile(compressedFilename);
    int fileSize = file.size();
    
    char*buffer = file.getBinaryBuffer();
    int whichByte = (int) ofRandom(fileSize);
    
    int whichBit = ofRandom(5);
    
    char bitMask = 50 << whichBit;
    buffer[whichByte] |= bitMask;
    
    ofBufferToFile(compressedFilename, file);
    img.loadImage(compressedFilename);
    
    // how about resetting by counting seconds? just give this a try
    float coin = ofRandom(100);
    if(coin > 98 && glitchVideo.getCurrentFrame()>20){
        //rewind
        glitchVideo.setFrame(glitchVideo.getCurrentFrame()-20);
    }else if(coin < 3 && glitchVideo.getTotalNumFrames()-glitchVideo.getCurrentFrame()>20){
        glitchVideo.setFrame(glitchVideo.getCurrentFrame()+20);
    }
    
    if(ofGetElapsedTimef()-seconds>=gapTime){
        reset();
        seconds=ofGetElapsedTimef();
        gapTime = ofRandom(4);
    }
    
    cout << glitchVideo.getCurrentFrame() << endl;
    
    /* TOKYO 2020 Olympic LOGO problem 
     -> maybe I should make diagram */
}

//--------------------------------------------------------------
void ofApp::draw(){
    youtubePlay();
    
    if(playFlag==false){
        ofSetColor(0);
        ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        ofSetColor(255);
        noiseAmount = ofRandom(0.99, 1.0);
        titleFont.setLetterSpacing(noiseAmount);
        titleFont.drawString("(IL)LEGALIZE", 145-noiseAmount*12, 190);
    }
    cout << "current time = " << ofGetElapsedTimef() - finishingTime << endl;
    
    if(endFlag==true){
        ofSetColor(0);
        ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        ofSetColor(255);
        noiseAmount = ofRandom(0.98, 1.0);
        titleFont.setLetterSpacing(noiseAmount);
        font.drawString("A Film by", 100, 140);
        font.drawString("(IL)LEGAL.app", 245, 140);
        font.drawString("Sound Mix by", 100, 170);
        font.drawString("(IL)LEGAL.app", 245, 170);
        font.drawString("Special thanks to ", 100, 200);
        font.drawString(URL, 245, 200);
    }
    
    //ofSaveFrame();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if(key == 's'){
        glitchVideo.play();
    }else if(key == 'g'){
        glitchFlag = false;
    }else if(key == 'v'){
        clipboardContent = ofxClipboard::paste();
    }else if(key == 'b'){
        glitchVideo.setFrame(glitchVideo.getCurrentFrame()-20);
    }else if(key == 'f'){
        glitchVideo.setFrame(glitchVideo.getCurrentFrame()+20);
    }else if(key == 'p'){
        playFlag=true;
        //URL = "https://www.youtube.com/watch?v=qtfp_qI4tSk";
        glitchVideo.loadMovie(getYoutubeDlStream(URL));
        glitchVideo.play();
        finishingTime = ofGetElapsedTimef();
    }else if(key == 's'){
        glitchVideo.stop();
        endFlag=true;
    }else if(key == 'r'){
        for(int i=0; i<11; i++){
            randomString += getRandomChar();
        }
        URL = "https://www.youtube.com/watch?v=" + randomString;
        cout << URL << endl;
        glitchVideo.loadMovie(getYoutubeDlStream(URL));
        glitchVideo.setFrame(20);
        glitchVideo.play();
        finishingTime = ofGetElapsedTimef();
        URL = "";
        randomString = "";
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'g'){
        glitchFlag = true;
    }else if(key == 'p'){
    }
}

//--------------------------------------------------------------
void ofApp::youtubePlay(){
    /* simply playing a video */
    glitchVideo.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255, 255, 255);
    
    if(glitchFlag == true){
        img.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    // how about mixing frames to unreadable and no sound noise?
    // once the movie is finished (if it's less than 1min, force finishing?), how about indicating URL?
    // and uploading images BACK TO Youtube again? automatically?
    
    /* when finishing movie */
    if(ofToString(glitchVideo.getCurrentFrame())==ofToString(glitchVideo.getTotalNumFrames())){
        glitchVideo.stop();
        endFlag = true;
    }else if(finishingTime>1 && ofGetElapsedTimef() - finishingTime>300){
        //stop and then show the IDENTITY: project name & url
        glitchVideo.stop();
        endFlag = true;
    }
    
    // if it can make 1min or 6sec or gif selector, perect? or too much?
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
