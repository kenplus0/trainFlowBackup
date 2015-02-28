#include "ofApp.h"
#define NUM_OF_HUMANS 1001

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundAuto(false);
    ofBackground(255);
    radius = 5;
    trainAlpha = 50;
    pSpaceMultiply = 2;
    dPressed = false;
    gPressed = false;
    getOn = false;
    xRect = 0;
    yRect = 0;
    inTrain = 0;
    for (int i=0; i<NUM_OF_HUMANS; i++) {
        Human h = Human(radius, pSpaceMultiply);
        humans.push_back(h);
    }
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void ofApp::update(){
    inTrain = 0;
    for (int i=0; i<humans.size(); i++) {
        if (humans[i].inTrain()) {
            inTrain++;
        }
    }
    mesh.clearVertices();
    for (int i=0; i<humans.size(); i++) {
        mesh.addVertex(ofVec3f(humans[i].getLoc().x, humans[i].getLoc().y, 0));
        mesh.addColor(ofFloatColor(0, 0, 0));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // mesh
    mesh.drawVertices();
    for (int i=0; i<humans.size(); i++) {
        humans[i].update();
    }
    
    // update background with corresponding visualization mode
    if (!showTrack) {
        // normal mode: dont show track
        ofSetColor(255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        trainAlpha = 50;
        // show pspace
        for (int i=0; i<humans.size(); i++) {
            humans[i].setPspace(true);
        }
    } else if (showTrack) {
        // tracking mode: show track
        ofSetColor(255, 10);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        // update normally around the numbers
        ofSetColor(200);
        ofRect(0, 0, 170, 50);
        
        trainAlpha = 2;
        // dont show pspace
        for (int i=0; i<humans.size(); i++) {
            humans[i].setPspace(false);
        }
    }
    
    // show framerate
    ofSetColor(0);
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, 20);
    
    for (int i=0; i<humans.size(); i++) {
        // mouse follower as 0
        if (i == 0) {
            humans[i].draw(mouseX, mouseY);
        } else {
            humans[i].draw();
        }
        humans[i].update();
        
        // check if someone's inside pspace
        for (int j=0; j<humans.size(); j++) {
            // exclude itself
            if (i != j) {
                // check if j is in i's pspace
                // first check if j is in i's radius
                if (humans[i].inRadius(humans[j].getLoc())) {
                    humans[i].separate(humans[j].getLoc());
                } else
                if (humans[i].inPspace(humans[j].getLoc())) {
                    humans[i].repel(humans[j].getLoc(), 0.07);
                }
            }
        }
    }
    
    // if d is pressed
    if (dPressed) {
        ofSetColor(20, 255, 20, trainAlpha);
//        ofRect(xRect, yRect, xRelease, yRelease);
        ofRect(xRect, yRect, 10*30, 2.8*30);
        ofSetColor(0);
        ofDrawBitmapString(ofToString(inTrain)+" humans on train", 10, 40);
    }
    
    // if g is pressed
    if (gPressed) {
        for (int i=0; i<humans.size(); i++) {
            humans[i].gather(ofVec2f(mouseX, mouseY), 0.07);
        }
    }
    
    // if i is pressed
    if (getOn) {
        for (int i=0; i<humans.size(); i++) {
            humans[i].getInside();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'g':
            gPressed = true;
            // open doors
            for (int i=0; i<humans.size(); i++) {
                humans[i].openDoor();
            }
            break;
        case 'd':
            dPressed = false;
            break;
        case 'i':
            getOn = true;
            // open doors
            for (int i=0; i<humans.size(); i++) {
                humans[i].openDoor();
            }
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case '1':
            // dont show track
            showTrack = false;
            break;
        case '2':
            // show track
            showTrack = true;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 'g':
            gPressed = false;
            break;
        case 'i':
            getOn = false;
            // close doors
            for (int i=0; i<humans.size(); i++) {
                humans[i].closeDoor();
            }
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // create train
//    dPressed = true;
    xRelease = mouseX-xRect;
    yRelease = mouseY-yRect;
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    dPressed = false;
    xRect = mouseX;
    yRect = mouseY;
    xRelease = mouseX;
    yRelease = mouseY;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
//    mPressed = false;
    dPressed = true;
    // create field for train when mouse released
    // doesnt reflect right
    for (int i=0; i<humans.size(); i++) {
//        humans[i].createFreeTrain(xRect, yRect, mouseX - xRect, mouseY - yRect);
        humans[i].createTrain(xRect, yRect, 30);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
