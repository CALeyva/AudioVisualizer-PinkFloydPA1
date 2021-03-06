#include "ofApp.h"
#include <string>

//--------------------------------------------------------------
void ofApp::setup(){
    sound.loadSound("beat.wav"); //Loads a sound file (in bin/data/)
    sound.setLoop(true); // Makes the song loop indefinitely
    sound.setVolume(1.00); // Sets the song volume
    if(mode == '1'){ // Sets the Background Color
        ofSetBackgroundColor(4, 148, 68); // Green colored background 
    }else if(mode == '2'){
        ofSetBackgroundColor(251, 4, 4); // Red colored background
    }else if(mode == '3'){
        ofSetBackgroundColor(255, 245, 56); // Yellow colored background
    }else if(mode == '4'){
        ofSetBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255)); // Random colored background
    }else if(mode == '5'){
        ofSetBackgroundColor(0,0, 0); // Random colored background
    }
    ofSetRectMode(OF_RECTMODE_CENTER); //check
    ofEnableSmoothing();
}

//--------------------------------------------------------------
void ofApp::update(){
    /* The update method is called muliple times per second
    It's in charge of updating variables and the logic of our app */
    if (recording || playback) {
        counter++; // Counting frames passed
    }
    if (playback) {
        keyInFrame = true;
        if (counter <= framesRecorded) {
            try { // Reduces lookup from O(N) to O(1)
                playKey = recordings[counter];
                // keyRecorded = playKey; // Fix !showing auto key presses in draw() when playback
            } catch (int e) {
                keyInFrame = false; // There is no recorded key in this frame
            }
            if (keyInFrame) { // If key was recorded in this frame count, press the key
                switch(playKey){
                    case '1': case '2': case '3': case '4': case '5': case 'p': case 'a':
                        setMode(playKey);
                        break;
                    case 'z': case 'x': case 'c': case 'v':
                        setMusic(playKey);
                        break;
                    case '=': case '-':
                        setVolume(playKey);
                        break;
                    case 'r': case 't':
                        recordPlayback(playKey);
                        break;
                }
            }

        } else if (counter >= framesRecorded) { // If in final frame, reset
            playback = false;
            counter = 0;
        }
    }

    if (!pauseDraw) { // If not in pause visuals mode
        ofSoundUpdate(); // Updates all sound players
        visualizer.updateAmplitudes(); // Updates Amplitudes for visualizer
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    /* The update method is called muliple times per second
    It's in charge of drawing all figures and text on screen */

    if (recording) {
        ofDrawBitmapString("Frames recorded: " + to_string(counter) + "  Key pressed: " + keyRecorded, ofGetWidth() - 300, 15);
    } else if (playback) {
        ofDrawBitmapString("AUTO-PILOT MODE - Frames played: " + to_string(counter), ofGetWidth() - 300, 15);
    }

    if(!playing){
        ofSetColor(256);
        ofDrawBitmapString("Press 'p' to enter or exit starting menu", ofGetWidth()/2 - 125, (ofGetHeight()/2) - 75);
        ofDrawBitmapString("Press 'a' to pause song and visuals", ofGetWidth()/2 - 125, (ofGetHeight()/2) - 50);
        ofDrawBitmapString("Press 'z', 'x', 'c' or 'v' to change songs", ofGetWidth()/2 - 125, (ofGetHeight()/2) - 25);
        ofDrawBitmapString("Press '1', '2', '3' or '4' to change visual modes", ofGetWidth()/2 - 125, ofGetHeight()/2);
        ofDrawBitmapString("Press '=' or '-' to raise or lower volume", ofGetWidth()/2 - 125, (ofGetHeight()/2) + 25);
        ofDrawBitmapString("Press 'r' to start/stop recording with the beat!", ofGetWidth()/2 - 125, (ofGetHeight()/2) + 50);
        ofDrawBitmapString("Press 't' after recording for AUTO-PILOT MODE!", ofGetWidth()/2 - 125, (ofGetHeight()/2) + 75);
    }

    vector<float> amplitudes = visualizer.getAmplitudes();
    if(mode == '1'){
        drawMode1(amplitudes);
    }else if(mode == '2'){
        drawMode2(amplitudes);
    }else if(mode == '3'){
        drawMode3(amplitudes);
    }else if(mode == '4'){
        drawMode4(amplitudes);
    }else if(mode == '5'){
        drawMode5(amplitudes);
    }
}

void ofApp::drawMode1(vector<float> amplitudes){
        ofFill(); // Drawn Shapes will be filled in with color
        ofSetColor(256); // This resets the color of the "brush" to white
        ofDrawBitmapString("Rectangle Height Visualizer", 0, 15);
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        
        // Sets bar widths across the entire window
        for (int i = 0.00; i < ofGetWidth(); i+=(ofGetWidth()/64.00)) {
            ofDrawRectangle(i, ofGetHeight() - 100.00, ofGetWidth()/64.00,  amplitudes[i/(ofGetWidth()/64.00)]);
        }

        int numRects = 10;
        for (int r=0; r<numRects; r++) {
            ofSetColor(ofRandom(50, 255));
            float width = ofRandom(5, 20);
            float height = ofRandom(5, 20);
            float xOffset1 = ofRandom(-40, 40);
            float yOffset1 = ofRandom(-40, 40);
            ofDrawRectangle(ofGetMouseX()+xOffset1, ofGetMouseY()+yOffset1, width, height);
       } 
    }

void ofApp::drawMode2(vector<float> amplitudes){
        ofSetLineWidth(5); // Sets the line width
        ofNoFill(); // Only the outline of shapes will be drawn
        ofSetColor(256); // This resets the color of the "brush" to white
        ofDrawBitmapString("Circle Radius Visualizer", 0, 15);
        int bands = amplitudes.size();
        for(int i=0; i< bands; i++){
            ofSetColor((bands - i)*32 %256,18,144); // Color varies between frequencies
            ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, amplitudes[0]/(i +1));
        }

       int numRects = 10;
        for (int r=0; r<numRects; r++) {
            ofSetColor(ofRandom(50, 255));
            float width = ofRandom(5, 20);
            float xOffset1 = ofRandom(-40, 40);
            float yOffset1 = ofRandom(-40, 40);
            ofDrawCircle(ofGetMouseX()+xOffset1,ofGetMouseY()+yOffset1, width);
        } 
}

void ofApp::drawMode3(vector<float> amplitudes){
    ofFill(); // Drawn Shapes will be filled in with color
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Rectangle Width Visualizer", 0, 15);
    ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));

    int numRects = 10;
    for (int r=0; r<numRects; r++) {
        ofSetColor(ofRandom(50, 255));
        float width = ofRandom(5, 20);
        float height = ofRandom(5, 20);
        float xOffset1 = ofRandom(-40, 40);
        float yOffset1 = ofRandom(-40, 40);
        ofDrawRectangle(ofGetMouseX()+xOffset1, ofGetMouseY()+yOffset1, width, height);
      } 

    ofRotate(90);
    for (int i = 0.00; i < ofGetWidth(); i+=(ofGetWidth()/64.00)) {
        ofDrawRectangle(i, 0.00, ofGetWidth()/64.00,  amplitudes[i/(ofGetWidth()/64.00)]);
    }
}

void ofApp::drawMode4(vector<float> amplitudes){
    ofSetColor(ofRandom(255),ofRandom(255),ofRandom(255)); // This resets the color of the "brush" to white
    ofDrawBitmapString("Custom Visualizer", 0, 15);
    ofSetSphereResolution(6);
    
    ofNoFill();
    int bands = amplitudes.size();
    for (int i = 0; i<bands; i++){
        // ofDrawCircle(ofGetMouseX(), ofGetMouseY(), amplitudes[0]/(i +1));
        ofDrawSphere(ofGetMouseX(),ofGetMouseY(),amplitudes[0]/(i +1));
    }
}

void ofApp::drawMode5(vector<float> amplitudes){
    ofDrawBitmapString("Bonus Visualizer: Pink Floyd Edition", 0, 15);

    ofSetLineWidth(1);
    int numRects = 5;
    for (int r=0; r<numRects; r++) {
        ofSetColor(ofRandom(50, 255));
        float xOffset1 = ofRandom(-40, 40);
        float yOffset1 = ofRandom(-40, 40);
        float xOffset2 = ofRandom(-40, 40);
        float yOffset2 = ofRandom(-40, 40);
        float xOffset3 = ofRandom(-40, 40);
        float yOffset3 = ofRandom(-40, 40);
        ofDrawTriangle(ofGetMouseX()+xOffset1, ofGetMouseY()+yOffset1, ofGetMouseX()+xOffset2, ofGetMouseY()+yOffset2, ofGetMouseX()+xOffset3, ofGetMouseY()+yOffset3);
    }
 
    ofRectangle rect1(0,ofGetHeight()/1.50, ofGetWidth()/2,10) ; //white rectangle
    ofRectangle rect2(ofGetWidth()/1.05, ofGetHeight()/3.00,ofGetWidth()/2,15) ; //purple rectangle
    ofRectangle rect3(ofGetWidth()/1.07, ofGetHeight()/3.20,ofGetWidth()/2,15) ; //blue rectangle
    ofRectangle rect4(ofGetWidth()/1.09, ofGetHeight()/3.40,ofGetWidth()/2,15) ; //green rectangle

    ofRectangle rect5(ofGetWidth()/1.11, ofGetHeight()/3.65,ofGetWidth()/2,15) ; //yellow rectangle
    ofRectangle rect6(ofGetWidth()/1.13, ofGetHeight()/3.95,ofGetWidth()/2,15) ; //orange rectangle
    ofRectangle rect7(ofGetWidth()/1.15, ofGetHeight()/4.30,ofGetWidth()/2,15) ; //red rectangle
    vector<ofRectangle> myRects = {rect1,rect2,rect3,rect4,rect5,rect6, rect7};

    vector<int> r = {255,128,0,0,255,255,255};
    vector<int> g = {255,0,0,128,255,165,0};
    vector<int> b = {255,128,255,0,0,0,0};
    
    int i = 1;
    ofFill();
    do {
        ofSetColor(r[i],g[i],b[i]);
        ofPushMatrix();
        ofRotateDeg(15);
        ofDrawRectangle(myRects[i]);
        ofPopMatrix();
        i++;
    }
    while ( i < myRects.size());

    ofSetColor(r[0],g[0],b[0]);
    ofPushMatrix();
    ofRotateDeg(335);
    ofDrawRectangle(myRects[0]);
    ofPopMatrix();

    ofNoFill();
    ofSetLineWidth(15);
    ofPoint p1;
    p1.x = ofGetWidth()/2.00;
    p1.y = ofGetHeight()/3.00 ;
    ofPoint p2;
    p2.x = ofGetWidth()/3.00;
    p2.y = ofGetHeight()/1.50 ;
    ofPoint p3;
    p3.x = ofGetWidth()/1.50;
    p3.y = ofGetHeight()/1.50;

    ofDrawTriangle(p1,p2,p3);
}

void ofApp::setMode(int key) {
    switch(key) {
        case '1':
            mode = '1';
            ofSetBackgroundColor(4, 148, 68); // Green colored background 
            break;
        case '2':
            mode = '2';
            ofSetBackgroundColor(251, 4, 4); // Red colored background
            break;
        case '3':
            mode = '3';
            ofSetBackgroundColor(255, 245, 56); // Yellow colored background
            break;
        case '4':
            mode = '4';
            ofSetBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255)); // Random colored background
            break;
        case '5':
            mode = '5';
            ofSetBackgroundColor(0, 0, 0); //black background
            break;
        case 'p': // Go to main screen
            if(playing) {
                sound.stop();
                playing = !playing;
            }else if (pauseDraw){
                playing = !playing;
            }else {
                sound.play();
                playing = !playing;
            }
            break;
        case 'a': // Pause visuals
            if(pauseDraw && playing){
                sound.play();
                pauseDraw = !pauseDraw;
            }else if  (!pauseDraw && playing) {
                sound.stop();
                pauseDraw = !pauseDraw;
            }
            break;
    }
}

void ofApp::setMusic(int key) {
    switch(key) {
        case 'z':
            if(mode == '5'){
                sound.load("great-gig.wav");
                sound.play();
                break;
            }
                sound.load("rock-song.wav");
                sound.play();
                break;
        case 'x':
            if(mode == '5'){
                sound.load("brain-damage.wav");
                sound.play();
                break;
            }
                sound.load("beat.wav");
                sound.play();
                break;
        case 'c':
            if(mode == '5'){
                sound.load("eclipse.wav");
                sound.play();
                break;
            }
            sound.load("geesebeat.wav");
            sound.play();
            break;
        case 'v':
            if(mode == '5'){
                sound.load("money.wav");
                sound.play();
                break;
            }
                sound.load("pigeon-coo.wav");
                sound.play();
                break;
    }
}

void ofApp::setVolume(int key) {
    switch(key) {
        case '=': // Increase volume
            if(sound.getVolume() > 0.80){
                sound.setVolume(1.00);
            }
            else if(sound.getVolume() >= 0.00 && sound.getVolume() <= 0.80){
                sound.setVolume(sound.getVolume() + 0.20);
            } else {
                sound.setVolume(0.00);
            }
            break;
        case '-': // Decrease volume
            if(sound.getVolume() < 0.20){
                sound.setVolume(0.00);
            }
            else if(sound.getVolume() >= 0.20 && sound.getVolume() <= 1.00){
                sound.setVolume(sound.getVolume() - 0.20);
            } else {
                sound.setVolume(1.00);
            }
            break;
    }
}

void ofApp::recordPlayback(int key) {
    switch(key) {
        case 'r': // Begins or stops recording keys pressed
            if (!recording) {
                recordings.clear();
                counter = 0; 
                recording = true;
            } else {
                recording = false;
                framesRecorded = counter;
                counter = 0;
            }
            break;
        case 't': // Begins or stops playback
            if (!recording && !playback) {
                playback = true;
            } else if (!recording && playback) {
                playback = false;
                counter = 0;
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // This method is called automatically when any key is pressed
    switch((playback && key != 't') ? 0 : key){ // Disables keyPressed if in AUTO-PILOT MODE, unless escaping
        case '1': case '2': case '3': case '4': case '5': case 'p': case 'a':
            setMode(key);
            break;
        case 'z': case 'x': case 'c': case 'v':
            setMusic(key);
            break;
        case '=': case '-':
            setVolume(key);
            break;
        case 'r': case 't':
            recordPlayback(key);
            break;
    }
    // Saves keys pressed if in record mode
    if (recording && key != 'r' && !playback) {
        recordings[counter] = key;
        keyRecorded = key;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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