#include "ofMain.h"

class radFbo {

		public:

    void setup(int _w, int _h, ofVideoPlayer _player);
    void update(float peram1=0, float peram2=0, float peram3=0);
    void draw();

    void addSaturation();

    int w;
    int h;

    ofFbo fbo;
    ofVideoPlayer player;
    ofShader saturationShader;
    bool hasSatShader;

    radFbo();
};