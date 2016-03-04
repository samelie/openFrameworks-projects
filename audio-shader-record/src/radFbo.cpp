#include "radFbo.h"

radFbo::radFbo(){
}

//--------------------------------------------------------------
void radFbo::setup(int _w, int _h, ofVideoPlayer _player) {
    fbo.allocate(_w,_h);
 		fbo.begin();
    ofClear(0, 0, 0, 0);
    fbo.end();

   	w = _w;
   	h = _h;

    player = _player;

    hasSatShader = false;
}

//--------------------------------------------------------------
void radFbo::update(float peram1, float peram2 , float peram3 ) {
		player.update();

		fbo.begin();
    player.draw(0, 0, w, h);
    fbo.end();

    if(hasSatShader){
    	fbo.begin();
	    saturationShader.begin();
	    saturationShader.setUniform1f("screenWidth",  ofGetWidth());
	    saturationShader.setUniform1f("screenHeight",  ofGetHeight());
	    saturationShader.setUniform1f("uSaturation", peram1);
	    saturationShader.setUniformTexture("tex0", fbo.getTextureReference(0), 1);
	    player.draw(0, 0, w, h);
	    saturationShader.end();
	    fbo.end();
    }
}

//--------------------------------------------------------------
void radFbo::addSaturation() {
		saturationShader.load("vert/basic.vert", "frag/saturation.frag");
		hasSatShader = true;
}



//--------------------------------------------------------------
void radFbo::draw() {
  
}

