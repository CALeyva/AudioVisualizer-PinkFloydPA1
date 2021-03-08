#include "ofApp.h"
#include <string>

//--------------------------------------------------------------
void ofApp::setup(){
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    windowScale = 1.0f;
    widthScaled = windowWidth * windowScale;
    heightScaled = windowHeight * windowScale;

    ofHideCursor(); //Hides cursor
    sound.loadSound("beat.wav"); //Loads a sound file (in bin/data/)
    songPlayingID = 1;
    sound.setLoop(true); // Makes the song loop indefinitely
    sound.setVolume(1.00); // Sets the song volume

    /* Sets the Background Color */
    if(mode == '1'){ 
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
    ofEnableSmoothing(); //smoothens color and shapes
}

//--------------------------------------------------------------
void ofApp::update(){
        /* The update method is called muliple times per second
    It's in charge of updating variables and the logic of our app */

    /* Updates if scale changed */
    if (scaleDirFixed) {
        scaleDirFixed = false;
    }

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
                    case 'z': case 'x': case 'c': case 'v': case '[': case ']':
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
        // UPDATE
        ofDrawBitmapString("Press 'p' to enter or exit starting menu", ofGetWidth()/2 - 150, (ofGetHeight()/2) - 75);
        ofDrawBitmapString("Press 'a' to pause song and visuals", ofGetWidth()/2 - 150, (ofGetHeight()/2) - 50);
        ofDrawBitmapString("Press 'z', 'x', 'c' or 'v' to change songs", ofGetWidth()/2 - 150, (ofGetHeight()/2) - 25);
        ofDrawBitmapString("Press '1', '2', '3' or '4' to change visual modes", ofGetWidth()/2 - 150, ofGetHeight()/2);
        ofDrawBitmapString("Press '=' or '-' to raise or lower volume", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 25);
        ofDrawBitmapString("Press 'r' to start/stop recording with the beat!", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 50);
        ofDrawBitmapString("Press 't' after recording for AUTO-PILOT MODE!", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 75);
    } else {
        ofDrawBitmapString(songTitles[songPlayingID], (ofGetWidth()/2) - (songTitles[songPlayingID].length() * 3), ofGetHeight() - 25);
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
        ofDrawBitmapString("Vertical Rectangle Visualizer", 0, 15);
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        
        // Sets bar widths across the entire window
        for (int i = 0.00; i < ofGetWidth(); i+=(ofGetWidth()/64.00)) {
            ofDrawRectangle(i, ofGetHeight() - 100.00, ofGetWidth()/64.00,  amplitudes[i/(ofGetWidth()/64.00)]);
        }

    /* Creates the cursor shapes */
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

     /* Creates the cursor shapes */
       int numCircles = 10;
        for (int r=0; r<numCircles; r++) {
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
    ofDrawBitmapString("Horizontal Rectangle Visualizer", 0, 15);
    ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
    
     /* Creates the cursor shapes */
    int numRects = 10;
    for (int r=0; r<numRects; r++) {
        ofSetColor(ofRandom(50, 255));
        float width = ofRandom(5, 20);
        float height = ofRandom(5, 20);
        float xOffset1 = ofRandom(-40, 40);
        float yOffset1 = ofRandom(-40, 40);
        ofDrawRectangle(ofGetMouseX()+xOffset1, ofGetMouseY()+yOffset1, width, height);
      } 

    
     /* Creates the rectangles rotated 90 degress */
    ofPushMatrix(); //push the current coordinate position
    ofRotateDeg(90);
    for (int i = 0.00; i < ofGetWidth(); i+=(ofGetWidth()/64.00)) {
        ofDrawRectangle(i, 0.00, ofGetWidth()/64.00,  amplitudes[i/(ofGetWidth()/64.00)]);
    }
    ofPopMatrix(); //recall the pushed coordinate position
}

void ofApp::drawMode4(vector<float> amplitudes){
    // circle moving around screen
    float x = ofMap( ofNoise( ofGetElapsedTimef()/2.0, -1000), 0, 1, 0, ofGetWidth());
    float y = ofMap( ofNoise( ofGetElapsedTimef()/2.0, 1000), 0, 1, 0, ofGetHeight());
    ofNoFill();
    ofDrawCircle(x,y,3);

    ofSetColor(ofRandom(255),ofRandom(255),ofRandom(255)); // This resets the color of the "brush" to white
    ofDrawBitmapString("Hexagon Visualizer", 0, 15);
    ofSetSphereResolution(6); //gives cirlce hexagon shape

    /* Creates the hexagon following the cursor with the 64 amplitudes*/
    ofNoFill();
    for (int i = 0; i < 64; i++){
        ofDrawSphere(ofGetMouseX(),ofGetMouseY(),amplitudes[i]);
    }

}

void ofApp::drawMode5(vector<float> amplitudes){
    ofDrawBitmapString("Pink Floyd Edition", 0, 15);

    /* Creacion de los triangulos multicolor rotando en el background */
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 1.80);
    ofNoFill();
    ofSetLineWidth(2);
    for (int i = 0; i < 30; i++){
        ofSetColor(ofRandom(255),ofRandom(255), ofRandom(255));
        ofRotateDeg(ofGetElapsedTimef());
        ofScale(0.9);
        ofDrawTriangle(500,500,1000,1000,1500,500);
    }
    ofPopMatrix();
    
    ofSetLineWidth(10);

    /* Vectores para los valores de RGB */
    vector<int> r = {255,128,0,0,255,255,255};
    vector<int> g = {255,0,0,128,255,165,0};
    vector<int> b = {255,128,255,0,0,0,0};

    /* Vectores de las coordenadas de los waves */
    double translateAllLeftX = -85;
    double translateAllUpY = 0;
    vector<double> translateX = {-100, (ofGetWindowWidth()/1.4)+ translateAllLeftX, (ofGetWindowWidth()/1.44)+ translateAllLeftX, (ofGetWindowWidth()/1.47)+ translateAllLeftX, (ofGetWindowWidth()/1.51)+ translateAllLeftX, (ofGetWindowWidth()/1.55)+ translateAllLeftX, (ofGetWindowWidth()/1.59)+ translateAllLeftX};
    vector<double> translateY = {-100, (ofGetWindowHeight()/3.0)+translateAllUpY, (ofGetWindowHeight()/3.2)+translateAllUpY, (ofGetWindowHeight()/3.4)+translateAllUpY, (ofGetWindowHeight()/3.65)+translateAllUpY, (ofGetWindowHeight()/3.95)+translateAllUpY, (ofGetWindowHeight()/4.3)+translateAllUpY};
  //  vector<int> side = {1, 1, 1, 1, 1, 1, 1};
   // vector<double> amplitudeChange = {1.0, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25};

    /* For-Loop para dibujar los curves/waves con su translation, rotation y rgb */
    for (int i = 0; i < 7; i++) {
        ofPushMatrix();
        ofSetColor(r[i],g[i],b[i]);
        ofTranslate(translateX[i],(translateY[i] + ofGetWindowHeight()/4)); //+ translateAllLeftX
        ofRotateDeg(31); // * side[i]
        int spacing = 50;
        ofBeginShape();
        for(int x=0; x < (ofGetWidth()/2) + 175; x += spacing) {
            ofCurveVertex(x, amplitudes[0] * 0.3f * sin(x*0.01f + ofGetFrameNum() * 0.02f)); //amplitudeChange[i] *
        }
        ofEndShape(false);
        ofPopMatrix();
    }

 /* Coordenadas/Info para la creacion del triangulo del medio */
    ofSetColor(256);
    ofNoFill();
    ofSetLineWidth(10);
    ofPoint p1;
    p1.x = (windowWidth/2.00)*windowScale; // multiplied by windowScale so triangle sides resize accordingly to the changes in screen 
    p1.y = (windowHeight/3.00)*windowScale;
    ofPoint p2;
    p2.x = (windowWidth/3.00)*windowScale;
    p2.y = (windowHeight/1.50)*windowScale;
    ofPoint p3;
    p3.x = (windowWidth/1.50)*windowScale;
    p3.y = (windowHeight/1.50)*windowScale;
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
            if (songPlayingID < 3) { // If Pink Floyd is not playing, you're doing it wrong...
                setMusic('v');
            }
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
            if(mode == '5'){ //only in mode 5, preset songs change to pink floyd
                sound.load("great-gig.wav");
                sound.play();
                songPlayingID = 4;
                break;
            }
            sound.load("rock-song.wav");
            sound.play();
            songPlayingID = 0;
            break;
        case 'x':
            if(mode == '5'){
                sound.load("brain-damage.wav");
                sound.play();
                songPlayingID = 5;
                break;
            }
            sound.load("beat.wav");
            sound.play();
            songPlayingID = 1;
            break;
        case 'c':
            if(mode == '5'){
                sound.load("eclipse.wav");
                sound.play();
                songPlayingID = 6;
                break;
            }
            sound.load("geesebeat.wav");
            sound.play();
            songPlayingID = 2;
            break;
        case 'v':
            if(mode == '5'){
                sound.load("money.wav");
                sound.play();
                songPlayingID = 7;
                break;
            }
            sound.load("pigeon-coo.wav");
            sound.play();
            songPlayingID = 3;
            break;
        case '[':
            if (songPlayingID != 0) {
                songPlayingID -= 1;
            } else {
                songPlayingID = playlist.size() - 1;
            }
            sound.load(playlist[songPlayingID]);
            sound.play();
            break;
        case ']':
            if (songPlayingID != playlist.size() - 1) {
                songPlayingID += 1;
            } else {
                songPlayingID = 0;
            }
            sound.load(playlist[songPlayingID]);
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
        case 'z': case 'x': case 'c': case 'v': case '[': case ']':
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

/* Ajusts screen height and width accordingly */
    if (!scaleDirFixed) {
        int diffW = abs(widthScaled - w);
        int diffH = abs(heightScaled - h);
        if (diffW > diffH)
            scaleDir = SCALE_DIR_HORIZONTAL;
        else
            scaleDir = SCALE_DIR_VERTICAL;
            scaleDirFixed = true;
    }
    
    if (scaleDir == SCALE_DIR_HORIZONTAL) {
        ratio = float(windowHeight) / float(windowWidth);
        h = w * ratio;
        windowScale = float(w) / float(windowWidth);
    }
    else if (scaleDir == SCALE_DIR_VERTICAL) {
        ratio = float(windowWidth) / float(windowHeight);
        w = h * ratio;
        windowScale = float(h) / float(windowHeight);
    }
    widthScaled = windowWidth * windowScale;
    heightScaled = windowHeight * windowScale;
    ofSetWindowShape(widthScaled, heightScaled);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}