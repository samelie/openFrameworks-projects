#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    receiver.setup(PORT);

    // int bufferSize = 512;
    // int ticksPerBuffer = bufferSize/64;
    // int inChannels = 64;
    // sampleRate = 44100;
    // channels = 2;

    videoOne.setLoopState(OF_LOOP_NORMAL);
    videoOne.setVolume(0.0);
    videoTwo.setLoopState(OF_LOOP_NORMAL);
    videoTwo.setVolume(0.0);
    videoThree.setLoopState(OF_LOOP_NORMAL);
    videoThree.setVolume(0.0);
    font.load("type/verdana.ttf", 100, true, false, true, 0.4, 72);
    videoOne.load("f.mp4");
    videoOne.play();
    videoTwo.load("v.mp4");
    videoTwo.play();
    videoThree.load("f.mp4");
    videoThree.play();

    fileName = "testMovie";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("2000k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    //soundStream.setup(this, 0, channels, sampleRate, 256, 4);
    bRecording = false;

    ofEnableAlphaBlending();

    fbo.allocate(V_W, V_H);

    recorderPixels.allocate(V_W, V_H, OF_PIXELS_RGB);

    shader.load("vert/basic.vert", "frag/chroma.frag");

    int sampleRate = 44100;
    int bufferSize = 512;
    int outChannels = 0;
    int inChannels = 2;
    int ticksPerBuffer = bufferSize/2;
    
    soundStream.printDeviceList();
    soundStream.setDeviceID(1);
    
    buffer_1 =  new float[bufferSize];
    buffer_2 =  new float[bufferSize];
    for(int i = 0; i < bufferSize; i++) { buffer_1[i]=buffer_2[i]= 0;}
    audioAnalyzer1.setup(512, 44100);
    soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, ticksPerBuffer);

    videoOneFbo.setup(V_W, V_H, videoOne);
    videoTwoFbo.setup(V_W, V_H, videoTwo);

    videoOneFbo.addSaturation();
    videoTwoFbo.addSaturation();

}

//--------------------------------------------------------------
void ofApp::update() {
    while (receiver.hasWaitingMessages()) {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        if (m.getAddress() == "/video1") {
            videoOne.load(m.getArgAsString(0));
            videoOne.play();
        }
        if (m.getAddress() == "/video2") {
            videoTwo.load(m.getArgAsString(0));
            videoTwo.play();
        }
        if (m.getAddress() == "/video3") {
            videoThree.load(m.getArgAsString(0));
            videoThree.play();
        }
    }

    videoOneFbo.update(audioAnalyzer1.getEnergy() * 1.5);
    videoTwoFbo.update(audioAnalyzer1.getRms() * 1.5);
    videoThree.update();

    videoOne.setVolume(0.0);
    videoTwo.setVolume(0.0);
    videoThree.setVolume(0.0);

    fbo.begin();
    ofClear(0, 0, 0, 0);
    shader.begin();
    shader.setUniform1f("uThreshold", audioAnalyzer1.getRms());
    shader.setUniform1f("uMixRatio", audioAnalyzer1.getEnergy());
    shader.setUniformTexture("tex0", videoOneFbo.fbo.getTextureReference(0), 1);
    shader.setUniformTexture("tex1", videoTwoFbo.fbo.getTextureReference(0), 2);
    videoOne.draw(0, 0, V_W, V_H);
    shader.end();
    fbo.end();


    //maskFbo.attachTexture(fbo.getTexture(),GL_RGB,0);
}

//--------------------------------------------------------------
void ofApp::draw() {
    //videoOne.draw(0,0);
    fbo.readToPixels(fboPixels);
    for (int i = 0; i < V_W * V_H; i++) {
        int pix = i * 4;
        int pixRgb = i * 3;
        int r = fboPixels[pix]; // red pixels
        int g = fboPixels[pix + 1]; // green pixels
        int b = fboPixels[pix + 2]; // blue pixels
        recorderPixels[pixRgb] = r;
        recorderPixels[pixRgb + 1] = g;
        recorderPixels[pixRgb + 2] = b;
    }

    fbo.draw(0, 0, ofGetWidth(), ofGetWidth()*ASPECT);

    if (bRecording) {
        //fbo.readToPixels(recorderPixels);
        //bool success = vidRecorder.addFrame(recorderPixels);
        bool success = vidRecorder.addFrame(recorderPixels);
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }

    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }

    if (vidRecorder.hasAudioError()) {
        ofLogWarning("The video recorder failed to write some audio samples!");
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, & ofApp::recordingComplete);
    vidRecorder.close();
    soundStream.stop();
    soundStream.close();
    
    audioAnalyzer1.exit();
    audioAnalyzer2.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    if (bRecording)
        vidRecorder.addAudioSamples(input, bufferSize, nChannels);

     for (int i = 0; i < bufferSize; i++){
        buffer_1[i]	= input[i*nChannels];
        buffer_2[i] = input[i*nChannels+1];
    }
    audioAnalyzer1.analyze(buffer_1, bufferSize);
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs & args) {
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

    if (key == 'r') {
        bRecording = !bRecording;
        if (bRecording && !vidRecorder.isInitialized()) {
            vidRecorder.setup(fileName + ofGetTimestampString() + fileExt, V_W, V_H, 30, sampleRate, channels);
            vidRecorder.start();
        } else if (!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        } else if (bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
    }
    if (key == 'c') {
        bRecording = false;
        vidRecorder.close();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}