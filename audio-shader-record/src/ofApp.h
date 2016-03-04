#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxVideoRecorder.h"
#include "ofxAudioAnalyzer.h"

#include "radFbo.h"


#define PORT 12345
#define V_W 480
#define V_H 480
#define ASPECT 0.5625



class ofApp: public ofBaseApp {

    public: void setup();
    void update();
    void draw();
    void exit();


    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    ofSoundStream soundStream;
    ofxAudioAnalyzer audioAnalyzer1, audioAnalyzer2;
    
    float *buffer_1;
    float *buffer_2;
    
    float *spectrum1;
    int spectrumSize;


    ofVideoPlayer videoOne;
    ofVideoPlayer videoTwo;
    ofVideoPlayer videoThree;

    ofxVideoRecorder vidRecorder;
    bool bRecording;
    int sampleRate;
    int channels;
    string fileName;
    string fileExt;

    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs & args);

    ofFbo fbo;

    radFbo videoOneFbo;
    radFbo videoTwoFbo;

    ofShader shader;
    ofShader chromaShader;
    ofShader saturationShader;

    ofPixels fboPixels;
    ofPixels recorderPixels;
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    string mouseButtonState;
};