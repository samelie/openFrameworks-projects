#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(){
    ofGLWindowSettings settings;
    settings.setGLVersion(3,3); /// < select your GL Version here
    settings.width = 1280;
    settings.height = 720;
    ofCreateWindow(settings); ///< create your window here
    ofRunApp(new ofApp());
}
