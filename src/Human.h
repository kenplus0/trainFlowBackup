//
//  Human.h
//  trainStream
//
//  Created by Kento Morita on 2015/02/24.
//
//

#pragma once
#include "ofMain.h"

class Human {
private:
    float radius, pSpace;
    float rectWidth, rectHeight;
    float x1Door, y1Door, door1Width, door1Height;
    bool x1Between, y1Between;
    bool doorOpen;
    bool rectEnabled, isInTrain;
    bool pSpaceOn;
    ofVec2f location, velocity, acceleration, rectStart;
public:
    
    Human(float _radius, float _pSpaceMultiply);
    void draw();
    void draw(float _x, float _y);
    void update();
    bool inPspace(ofVec2f thisGuy);
    bool inRadius(ofVec2f thisGuy);
    void repel(ofVec2f thisGuy, float _multiply);
    ofVec2f getLoc();
    void gather(ofVec2f _here, float _multiply);
    void createFreeTrain(float _x, float _y, float _width, float _height);
    void createTrain(float _x, float _y, float _scale);
    void reflecter();
    void trainBorder();
    bool inTrain();
    void separate(ofVec2f thisGuy);
    void yBorder(float _y);
    void getInside();
    void goOutside();
    void openDoor();
    void closeDoor();
    void setPspace(float _multiply);
    void setPspace(bool _bool);
};