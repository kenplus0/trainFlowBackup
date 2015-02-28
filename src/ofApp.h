#pragma once

#include "ofMain.h"
#include "Human.h"

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
		
    float radius, trainAlpha, pSpaceMultiply;
    bool dPressed, gPressed, getOn;
    // visualization mode
    bool showTrack;
    float xRect, yRect, xRelease, yRelease;
    int inTrain;
    vector<Human> humans;
    ofVboMesh mesh;
};
