#include "ofApp.h"
#include <string>

//--------------------------------------------------------------
void ofApp::setup(){
    // Initializing window information for scaling purposes
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    windowScale = 1.0f;
    widthScaled = windowWidth * windowScale;
    heightScaled = windowHeight * windowScale;

    ofHideCursor(); // Hides default cursor
    sound.loadSound("beat.wav"); // Loads a sound file (in bin/data/)
    songPlayingID = 1; // "beat.wav" song ID
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

    // For recording and AUTO-PILOT Mode; counting frames
    if (recording || playback) {
        counter++; // Add current frame to count
    }
    // If in AUTO-PILOT Mode
    if (playback) {
        // Assume there is a key recorded (see recordings hashmap) in this frame
        keyInFrame = true;
        // If current frame number is less than the total amount of frames recorded
        if (counter <= framesRecorded) {
            try { // Reduces lookup from O(N) to O(1) by assuming there is a key recorded
                playKey = recordings[counter]; // Try and play key possibly recorded
            } catch (int e) { // If an error occured:
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
            playback = false; // Not in AUTO-PILOT Mode anymore
            counter = 0; // Reset counter for future use
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
    // Display recording/recorded information on screen
    if (recording) {
        ofDrawBitmapString("Frames recorded: " + to_string(counter) + "  Key pressed: " + keyRecorded, ofGetWidth() - 300, 15);
    } else if (playback) {
        ofDrawBitmapString("AUTO-PILOT MODE - Frames played: " + to_string(counter), ofGetWidth() - 300, 15);
    }

    if(!playing){ // If in menu
        ofSetColor(256); // Set menu to white
        // Display instructions in middle of the screen
        ofDrawBitmapString("Press 'p' to enter or exit starting menu", ofGetWidth()/2 - 150, (ofGetHeight()/2) - 75);
        ofDrawBitmapString("Press 'a' to pause song and visuals", ofGetWidth()/2 - 150, (ofGetHeight()/2) - 50);
        ofDrawBitmapString("Press 'z', 'x', 'c' or 'v' to select preset songs", ofGetWidth()/2 - 150, (ofGetHeight()/2) - 25);
        ofDrawBitmapString("Press '1', '2', '3', '4' or '5' to change visual modes", ofGetWidth()/2 - 150, ofGetHeight()/2);
        ofDrawBitmapString("Press '=' or '-' to raise or lower volume", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 25);
        ofDrawBitmapString("Press 'r' to start/stop recording with the beat!", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 50);
        ofDrawBitmapString("Press 't' after recording for AUTO-PILOT MODE!", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 75);
        ofDrawBitmapString("Press '[' or ']' to play next or previous song!", ofGetWidth()/2 - 150, (ofGetHeight()/2) + 100);
    } else { // If not in menu, display playing song in center bottom of screen
        ofDrawBitmapString(songTitles[songPlayingID], (ofGetWidth()/2) - (songTitles[songPlayingID].length() * 3), ofGetHeight() - 25);
    }

    // Update amplitude values
    vector<float> amplitudes = visualizer.getAmplitudes();
    // Draw current mode given the amplitudes
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
    for (int i = 0; i < ofGetWidth(); i += (ofGetWidth()/64.0)) {
        ofDrawRectangle(i, ofGetHeight() - 100.0, ofGetWidth()/64.0,  amplitudes[i/(ofGetWidth()/64.0)]);
    }

    /* Creates the cursor shapes */
    int numRects = 10;
    for (int r = 0; r < numRects; r++) {
        ofSetColor(ofRandom(50, 255));
        float width = ofRandom(5, 20);
        float height = ofRandom(5, 20);
        float xOffset1 = ofRandom(-40, 40);
        float yOffset1 = ofRandom(-40, 40);
        ofDrawRectangle(ofGetMouseX() + xOffset1, ofGetMouseY() + yOffset1, width, height);
    } 
}

void ofApp::drawMode2(vector<float> amplitudes){
    ofSetLineWidth(5); // Sets the line width
    ofNoFill(); // Only the outline of shapes will be drawn
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Circle Radius Visualizer", 0, 15);
    int bands = amplitudes.size();
    for(int i=0; i< bands; i++){ // Draw concentric circles with variating radii
        ofSetColor( (bands - i) * 32 % 256, 18, 144); // Color varies between frequencies
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
    for (int r = 0; r < numRects; r++) {
        ofSetColor(ofRandom(50, 255));
        float width = ofRandom(5, 20);
        float height = ofRandom(5, 20);
        float xOffset1 = ofRandom(-40, 40);
        float yOffset1 = ofRandom(-40, 40);
        ofDrawRectangle(ofGetMouseX() + xOffset1, ofGetMouseY() + yOffset1, width, height);
    } 

    /* Creates the rectangles rotated 90 degress */
    ofPushMatrix(); // Push the current coordinate position
    ofRotateDeg(90); // Set to horizontal position
    for (int i = 0; i < ofGetWidth(); i += (ofGetWidth() / 64.0) ) {
        ofDrawRectangle(i, 0.0, ofGetWidth()/64.0,  amplitudes[i/(ofGetWidth()/64.0)]);
    }
    ofPopMatrix(); // Recall the pushed coordinate position
}

void ofApp::drawMode4(vector<float> amplitudes){
    // Display a circle moving around screen because it looks cool and is fun to follow around with cursor :)
    float x = ofMap( ofNoise( ofGetElapsedTimef()/2.0, -1000), 0, 1, 0, ofGetWidth());
    float y = ofMap( ofNoise( ofGetElapsedTimef()/2.0, 1000), 0, 1, 0, ofGetHeight());
    ofNoFill();
    ofDrawCircle(x, y, 3);

    ofSetColor(ofRandom(255),ofRandom(255),ofRandom(255)); // This resets the color of the "brush" to white
    ofDrawBitmapString("Hexagon Visualizer", 0, 15);
    ofSetSphereResolution(6); // Gives circle hexagon shape

    /* Creates a 3D hexagon in the cursor position using the 64 amplitudes*/
    ofNoFill();
    for (int i = 0; i < 64; i++){
        ofDrawSphere(ofGetMouseX(), ofGetMouseY(), amplitudes[i]);
    }
}

void ofApp::drawMode5(vector<float> amplitudes){
    ofDrawBitmapString("Pink Floyd Edition", 0, 15);

    /* Creates mesmerizing, multicolor triangles rotating in the background */
    ofPushMatrix(); // Push the current coordinate position
    ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 1.80);
    ofNoFill();
    ofSetLineWidth(2);
    for (int i = 0; i < 30; i++){ // Draws triangles that change position over time
        ofSetColor(ofRandom(255),ofRandom(255), ofRandom(255));
        ofRotateDeg(ofGetElapsedTimef());
        ofScale(0.9);
        ofDrawTriangle(500,500,1000,1000,1500,500);
    }
    ofPopMatrix(); // Recall the pushed coordinate position
    
    ofSetLineWidth(10);

    /* Vectors for RGB values */
    vector<int> r = {255,128,0,0,255,255,255};
    vector<int> g = {255,0,0,128,255,165,0};
    vector<int> b = {255,128,255,0,0,0,0};

    /* Vectors used for setting the coordinates of the waves */
    double translateAllLeftX = -0.10; // Used to change position of all at once
    double translateAllDownY = -0.20; // Used to change position of all at once
    vector<double> translateX = {ofGetWidth() / 2.1, ofGetWindowWidth() / (1.85 + translateAllLeftX), ofGetWindowWidth() / (1.85 + translateAllLeftX), ofGetWindowWidth() / (1.87 + translateAllLeftX), ofGetWindowWidth() / (1.89 + translateAllLeftX), ofGetWindowWidth() / (1.9 + translateAllLeftX), ofGetWindowWidth() / (1.91 + translateAllLeftX)};
    vector<double> translateY = {ofGetHeight() / 2.00, ofGetWindowHeight() / (1.85 + translateAllDownY), ofGetWindowHeight() / (1.87 + translateAllDownY), ofGetWindowHeight() / (1.90 + translateAllDownY), ofGetWindowHeight() / (1.92 + translateAllDownY), ofGetWindowHeight() / (1.94 + translateAllDownY), ofGetWindowHeight() / (1.96 + translateAllDownY)};
    vector<float> side = {6.80, -1, -1, -1, -1, -1, -1};
    vector<int> amplitudeIndex = {0, 5, 4, 3, 2, 1, 0}; // Gives rainbow a 'bent' look, displays more amplitudes

    ofSetLineWidth(10);
    /* Draw waves according to its translation, rotation and RGB values */
    for (int i = 0; i < 7; i++) {
        ofPushMatrix(); // Push the current coordinate position
        ofSetColor(r[i], g[i], b[i]);
        ofTranslate(translateX[i], translateY[i]);
        ofRotateDeg(31 * side[i]);
        ofScale(windowScale); // For window rescaling
        int spacing = 50; // Essentially the drawing speed
        ofBeginShape();
        for(int x=0; x < (ofGetWidth()/2 + 1000); x += spacing) { // Draw sine-like curves
            ofCurveVertex(x, windowScale * (amplitudes[amplitudeIndex[i]] * 0.3f * sin(x*0.01f + ofGetFrameNum() * 0.02f)));
        }
        ofEndShape(false);
        ofPopMatrix(); // Recall the pushed coordinate position
    }

    /* Draws equilateral triangle in center of screen */
    ofSetColor(256);
    ofNoFill();
    ofSetLineWidth(10);
    ofPoint p1;
    p1.x = (windowWidth/2.00);
    p1.y = (windowHeight/3.00);
    ofPoint p2;
    p2.x = (windowWidth/3.00);
    p2.y = (windowHeight/1.50);
    ofPoint p3;
    p3.x = (windowWidth/1.50);
    p3.y = (windowHeight/1.50);
    ofScale(windowScale); // Scale triangle sides according to windowScale (which responds to changes in window sizes) 
    ofDrawTriangle(p1, p2, p3);
}

void ofApp::setMode(int key) {
    switch(key) {
        case '1': // Vertical Rectangle Mode
            mode = '1';
            ofSetBackgroundColor(4, 148, 68); // Green colored background 
            break;
        case '2': // Circle Radius Mode
            mode = '2';
            ofSetBackgroundColor(251, 4, 4); // Red colored background
            break;
        case '3': // Horizontal Rectangle Mode
            mode = '3';
            ofSetBackgroundColor(255, 245, 56); // Yellow colored background
            break;
        case '4': // Hexagon Mode
            mode = '4';
            ofSetBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255)); // Random colored background
            break;
        case '5': // Pink Floyd Mode
            mode = '5';
            ofSetBackgroundColor(0, 0, 0); //black background
            if (songPlayingID < 3) { // If Pink Floyd is not playing, you're doing it wrong...
                setMusic('v'); // Changing to "Money - Pink Floyd"
            }
            break;
        case 'p': // Go to main menu screen
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
            if(mode == '5'){ // If in mode 5, preset songs change to Pink Floyd Edition
                sound.load("great-gig.wav");
                sound.play();
                songPlayingID = 4; // Sets corresponding song ID
                break;
            }
            sound.load("rock-song.wav");
            sound.play();
            songPlayingID = 0; // Sets corresponding song ID
            break;
        case 'x':
            if(mode == '5'){
                sound.load("brain-damage.wav");
                sound.play();
                songPlayingID = 5; // Sets corresponding song ID
                break;
            }
            sound.load("beat.wav");
            sound.play();
            songPlayingID = 1; // Sets corresponding song ID
            break;
        case 'c':
            if(mode == '5'){
                sound.load("eclipse.wav");
                sound.play();
                songPlayingID = 6; // Sets corresponding song ID
                break;
            }
            sound.load("geesebeat.wav");
            sound.play();
            songPlayingID = 2; // Sets corresponding song ID
            break;
        case 'v':
            if(mode == '5'){
                sound.load("money.wav");
                sound.play();
                songPlayingID = 7; // Sets corresponding song ID
                break;
            }
            sound.load("pigeon-coo.wav");
            sound.play();
            songPlayingID = 3; // Sets corresponding song ID
            break;
        case '[':
            if (songPlayingID != 0) {
                songPlayingID -= 1; // Sets corresponding song ID
            } else {
                songPlayingID = playlist.size() - 1; // Sets corresponding song ID
            }
            sound.load(playlist[songPlayingID]);
            sound.play();
            break;
        case ']':
            if (songPlayingID != playlist.size() - 1) {
                songPlayingID += 1; // Sets corresponding song ID
            } else {
                songPlayingID = 0; // Sets corresponding song ID
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
                sound.setVolume(1.00); // Maximum volume is 1.00
            }
            else if(sound.getVolume() >= 0.00 && sound.getVolume() <= 0.80){
                sound.setVolume(sound.getVolume() + 0.20);
            } else {
                sound.setVolume(0.00);
            }
            break;
        case '-': // Decrease volume
            if(sound.getVolume() < 0.20){
                sound.setVolume(0.00); // Minumum volume is 0
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
    switch((playback && key != 't') ? 0 : key){ // Disables keyPressed if in AUTO-PILOT Mode, unless escaping
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