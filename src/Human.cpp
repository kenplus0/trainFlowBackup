//
//  Human.cpp
//  trainStream
//
//  Created by Kento Morita on 2015/02/24.
//
//

#include "Human.h"

Human::Human(float _radius, float _pSpaceMultiply) {
    // setting default status
    radius = _radius;
    pSpace = _radius * _pSpaceMultiply;
    location = ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    velocity = ofVec2f(ofRandom(1, 5), ofRandom(1, 5));
    rectEnabled = false;
    isInTrain = false;
    doorOpen = true;
    pSpaceOn = true;
}

void Human::draw() {
    // draw human. if in train, draw it blue
    if (!isInTrain) {
        ofSetColor(0);
    } else {
        ofSetColor(0, 255, 0);
    }
    ofCircle(location.x, location.y, radius);
    
    // draw personal space if normal mode
    if (pSpaceOn) {
        ofSetColor(100, 50);
        ofCircle(location.x, location.y, pSpace);
    }
}

void Human::draw(float _x, float _y) {
    // change location and radius
    location = ofVec2f(_x, _y);
    
    // draw human itself
    ofSetColor(255, 0, 0);
    ofCircle(_x, _y, radius);
    
    // draw personal space
    ofSetColor(100, 50);
    ofCircle(_x, _y, pSpace);
}

void Human::update() {
    velocity += acceleration;
    location += velocity;
    acceleration *= 0.001;
    // velocity goes down faster if outside the train
    if (!isInTrain) {
        velocity *= 0.96;
    } else {
        velocity *= 0.99;
    }
    
    // check edges
    if (location.x > ofGetWidth() - radius) {
        location.x = ofGetWidth() - radius;
        velocity.x *= -1;
    } else if (location.x < radius) {
        location.x = radius;
        velocity.x *= -1;
    }
    if (location.y > ofGetHeight() - radius) {
        location.y = ofGetHeight() - radius;
        velocity.y *= -1;
    } else if (location.y < radius) {
        location.y = radius;
        velocity.y *= -1;
    }
    
    // draw trainish stuff
    if (rectEnabled) {
        // set door parameters
        ofSetColor(0, 0, 255, 150);
        x1Door = rectStart.x+rectWidth*0.4;
        y1Door = rectStart.y+rectHeight*0.98;
        door1Width = rectWidth*0.2;
        door1Height = rectHeight*0.02;
        ofRect(x1Door, y1Door, door1Width, door1Height);
        
        // true if between first door
        x1Between = location.x > x1Door && location.x < x1Door+door1Width;
        y1Between = location.y > y1Door && location.y < y1Door+door1Height;
        
        // if going outside, isintrain is false. else true
        // only when the door is open
        if (doorOpen) {
            if (x1Between) {
                if (velocity.y > 0) {
                    if (location.y > y1Door
                        && location.y < y1Door+door1Height+radius*5) {
                        isInTrain = false;
                        // dont go side ways
                        velocity.x *= 0.3;
                    }
                } else {
                    if (y1Between) {
                        isInTrain = true;
                        // dont go side ways
                        velocity.x *= 0.3;
                    }
                }
            }
        }
        
        // dont go pass train walls
        trainBorder();
    }
}

// check if this guy is inside personal space;
bool Human::inPspace(ofVec2f thisGuy) {
    // check the distance between the two.
    // if they are closer than the personal space, return true.
    if (pSpace + radius > location.distance(thisGuy)) {
        return true;
    } else {
        return false;
    }
}

// check if this guy is inside radius
bool Human::inRadius(ofVec2f thisGuy) {
    if (radius*2 > location.distance(thisGuy)) {
        return true;
    } else {
        return false;
    }
}

// repel from thisGuy
void Human::repel(ofVec2f thisGuy, float _multiply) {
    velocity += (location - thisGuy).normalize()* _multiply;
}

ofVec2f Human::getLoc() {
    return location;
}

void Human::gather(ofVec2f _here, float _multiply) {
    // if the human is outside the train, go straight to mouse
    if (!isInTrain) {
        velocity += (_here - location).normalize() * _multiply;
    }
    // if the human is inside the train, first get off
    goOutside();
}

// create a train
void Human::createFreeTrain(float _x, float _y, float _width, float _height) {
    rectEnabled = true;
    rectStart = ofVec2f(_x, _y);
    rectWidth = _width;
    rectHeight = _height;
    // check if in train
    if (location.x >= rectStart.x + radius &&
        location.x <= rectStart.x + rectWidth - radius &&
        location.y >= rectStart.y + radius &&
        location.y <= rectStart.y + rectHeight - radius) {
        isInTrain = true;
    } else {
        isInTrain = false;
    }
}

// create template train
void Human::createTrain(float _x, float _y, float _scale) {
    rectEnabled = true;
    rectStart = ofVec2f(_x, _y);
    rectWidth = 10 * _scale;
    rectHeight = 2.8 * _scale;
    // check if in train
    if (location.x >= rectStart.x + radius &&
        location.x <= rectStart.x + rectWidth - radius &&
        location.y >= rectStart.y + radius &&
        location.y <= rectStart.y + rectHeight - radius) {
        isInTrain = true;
    } else {
        isInTrain = false;
    }
}

void Human::reflecter() {
    // reflect from train body
    // sides
    if (location.y > rectStart.y && location.y < rectStart.y + rectHeight) {
        // left outside
        if (location.x < rectStart.x) {
            // if moving towards inside
            if (velocity.x > 0 && location.x > rectStart.x - radius) {
                location.x = rectStart.x - radius;
                velocity.x = 0;
                acceleration.x = 0;
            }
        }
        // left inside
        if (location.x > rectStart.x) {
            // if moving towards outside
            if (velocity.x < 0 && location.x < rectStart.x + radius) {
                location.x = rectStart.x + radius;
                velocity.x = 0;
                acceleration.x = 0;
            }
        }
        // right outside
        if (location.x > rectStart.x && location.x > rectStart.x + rectWidth) {
            // if moving towards inside
            if (velocity.x < 0 && location.x < rectStart.x+rectWidth + radius) {
                location.x = rectStart.x+rectWidth + radius;
                velocity.x = 0;
                acceleration.x = 0;
            }
        }
        // right inside
        if (location.x > rectStart.x && location.x < rectStart.x + rectWidth) {
            // if moving towards outside
            if (velocity.x > 0 && location.x > rectStart.x+rectWidth - radius) {
                location.x = rectStart.x+rectWidth - radius;
                velocity.x = 0;
                acceleration.x = 0;
            }
        }
    }
    // top & bottom
    if (location.x > rectStart.x && location.x < rectStart.x + rectWidth) {
        // top outside
        if (location.y < rectStart.y) {
            // if moving towards inside
            if (velocity.y > 0 && location.y > rectStart.y - radius) {
                location.y = rectStart.y - radius;
                velocity.y = 0;
                acceleration.y = 0;
            }
        }
        // top inside
        if (location.y > rectStart.y) {
            // if moving towards outside
            if (velocity.y < 0 && location.y < rectStart.y + radius) {
                location.y = rectStart.y + radius;
                velocity.y = 0;
                acceleration.y = 0;
            }
        }
        // bottom outside
        if (location.y > rectStart.y && location.y > rectStart.y + rectHeight) {
            // exclude the door
            if (!(location.x > x1Door && location.x < x1Door + door1Width)) {
                // if moving towards inside
                if (velocity.y < 0 && location.y < rectStart.y+rectHeight + radius) {
                    location.y = rectStart.y+rectHeight + radius;
                    velocity.y = 0;
                    acceleration.y = 0;
                }
            }
        }
        // bottom inside
        if (location.y > rectStart.y && location.y < rectStart.y + rectHeight) {
            // exclude the door
            if (!(location.x > x1Door && location.x < x1Door + door1Width)) {
                // if moving towards outside
                if (velocity.y > 0 && location.y > rectStart.y+rectHeight - radius) {
                    location.y = rectStart.y+rectHeight - radius;
                    velocity.y = 0;
                    acceleration.y = 0;
                }
            }
        }
    }
}

// make it stay inside the train
void Human::trainBorder() {
    // if the human is inside the train
    if (isInTrain) {
        // left inside
        if (location.x < rectStart.x + radius){
            location.x = rectStart.x + radius;
            velocity.x = 0;
            acceleration.x = 0;
        }
        // right inside
        if (location.x > rectStart.x+rectWidth - radius) {
            location.x = rectStart.x+rectWidth - radius;
            velocity.x = 0;
            acceleration.x = 0;
        }
        // top inside
        if (location.y < rectStart.y + radius) {
            location.y = rectStart.y + radius;
            velocity.y = 0;
            acceleration.y = 0;
        }
        // bottom inside
        if (doorOpen) {
            // exclude door if door is open
            if (!(x1Between && y1Between)) {
                if (location.y > rectStart.y+rectHeight) {
                    location.y = rectStart.y+rectHeight;
                    velocity.y = 0;
                    acceleration.y = 0;
                }
            }
        } else {
            // include door if door is closed
            if (location.y > rectStart.y+rectHeight - radius) {
                location.y = rectStart.y+rectHeight - radius;
                velocity.y = 0;
                acceleration.y = 0;
            }
        }
    }
    
    // if the human is outside the train
    if (!isInTrain) {
        // sides
        if (location.y > rectStart.y && location.y < rectStart.y + rectHeight*0.8) {
            // left outside
            if (location.x > rectStart.x - radius
                    && location.x < rectStart.x + rectWidth/2) {
                location.x = rectStart.x - radius;
                velocity.x = 0;
                acceleration.x = 0;
            }
            // right outside
            if (location.x >= rectStart.x + rectWidth/2
                    && location.x < rectStart.x + rectWidth + radius) {
                // if moving towards inside
                if (location.x < rectStart.x+rectWidth + radius) {
                    location.x = rectStart.x+rectWidth + radius;
                    velocity.x = 0;
                    acceleration.x = 0;
                }
            }
        }
        // top & bottom
        if (location.x > rectStart.x && location.x < rectStart.x + rectWidth) {
            // top outside
            if (location.y > rectStart.y - radius
                    && location.y < rectStart.y + rectHeight/2) {
                location.y = rectStart.y - radius;
                velocity.y = 0;
                acceleration.y = 0;
            }
            // bottom outside
            if (location.y >= rectStart.y + rectHeight/2
                    && location.y < rectStart.y + rectHeight) {
                if (doorOpen) {
                    // exclude the door if door is open
                    if (!x1Between) {
                        location.y = rectStart.y+rectHeight + radius;
                        velocity.y = 0;
                        acceleration.y = 0;
                    }
                } else {
                    // include the door if door is open
                    location.y = rectStart.y+rectHeight + radius;
                    velocity.y = 0;
                    acceleration.y = 0;
                }
                
            }
        }
    }
}

// return true if inside train
bool Human::inTrain() {
    if (isInTrain) {
        return true;
    } else {
        return false;
    }
}

// separate from this guy by 2radius
void Human::separate(ofVec2f thisGuy) {
    location = thisGuy + (location - thisGuy).normalize()*radius*2;
}

// set y border
void Human::yBorder(float _y) {
    if (location.y < _y) {
        location.y = _y;
    }
}

// make human go inside the train
void Human::getInside() {
    // target that humans will aim to
    ofVec2f doorTarget = ofVec2f(x1Door+door1Width/2, y1Door);
    
    if (!isInTrain) {
        // make it go in front of the train
        if (location.y < y1Door+door1Height) {
            // if human is at left back side of the train, go left
            if (location.x > rectStart.x
                && location.x < rectStart.x+rectWidth/2 ) {
                velocity.x = -1;
            // if human is at right back side of the train, go right
            } else if (location.x < rectStart.x+rectWidth
                       && location.x >= rectStart.x+rectWidth/2) {
                velocity.x = 1;
            } else {
                // go down if neither
                velocity.y = 1;
            }
        } else {
            // go towards the target if human is in front of train
            velocity = (doorTarget-location).normalize();
        }
    }
//    else if (isInTrain) {
//        // go further inside
//        ofVec2f moreInTarget = ofVec2f(x1Door+door1Width/2, y1Door+1);
//        velocity += (location-moreInTarget).normalize() * 0.001;
//    }
}

void Human::goOutside() {
    if (isInTrain) {
        // target that humans will aim to
        ofVec2f doorTarget = ofVec2f(x1Door+door1Width/2, y1Door+radius);
        
        // go towards the target
        velocity = (doorTarget-location).normalize();
    }
}

void Human::openDoor() {
    doorOpen = true;
}

void Human::closeDoor() {
    doorOpen = false;
}

void Human::setPspace(float _multiply) {
    pSpace = radius * _multiply;
}

void Human::setPspace(bool _bool) {
    pSpaceOn = _bool;
}