#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sound.loadSound("beat.wav"); //Loads a sound file (in bin/data/)
    sound.setLoop(true); // Makes the song loop indefinitely
    sound.setVolume(1); // Sets the song volume
    if(mode == '1'){ // Sets the Background Color
        ofSetBackgroundColor(4, 148, 68); // Green colored background 
    }else if(mode == '2'){
        ofSetBackgroundColor(251, 4, 4); // Red colored background
    }else if(mode == '3'){
        ofSetBackgroundColor(255, 245, 56); // Yellow colored background
    }else if(mode == '4'){
        ofSetBackgroundColor(ofRandom(255), ofRandom(255), ofRandom(255)); // Random colored background
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    /* The update method is called muliple times per second
    It's in charge of updating variables and the logic of our app */
    if (!pauseDraw) {
        ofSoundUpdate(); // Updates all sound players
        visualizer.updateAmplitudes(); // Updates Amplitudes for visualizer
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    /* The update method is called muliple times per second
    It's in charge of drawing all figures and text on screen */
    if(!playing){
        ofDrawBitmapString("Press 'p' to play some music!", ofGetWidth()/2 - 50, ofGetHeight()/2);
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
    }
}

void ofApp::drawMode1(vector<float> amplitudes){
        ofFill(); // Drawn Shapes will be filled in with color
        ofSetColor(256); // This resets the color of the "brush" to white
        ofDrawBitmapString("Rectangle Height Visualizer", 0, 15);
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        
        //codigo para que las barras se vean a traves de la pantalla completa
        for (int i = 0; i < ofGetWidth(); i+=(ofGetWidth()/64)) {
            ofDrawRectangle(i, ofGetHeight() - 100, ofGetWidth()/64,  amplitudes[0]);
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
}

void ofApp::drawMode3(vector<float> amplitudes){
    ofFill(); // Drawn Shapes will be filled in with color
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Rectangle Width Visualizer", 0, 15);
    ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));

    ofRotate(90);
    for (int i = 0; i < ofGetWidth(); i+=(ofGetWidth()/64)) {
        ofDrawRectangle(i, 0, ofGetWidth()/64,  amplitudes[0]);
    }
}

void ofApp::drawMode4(vector<float> amplitudes){
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Custom Visualizer", 0, 15);
    // YOUR CODE HERE
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // This method is called automatically when any key is pressed
    switch(key){
        case 'p':
            if(playing && !pauseDraw) {
                sound.stop();
                playing = !playing;
            }else if (!pauseDraw) {
                sound.play();
                playing = !playing;
            }
            break;
        case 'a':
            if(pauseDraw && playing){
                sound.play();
                pauseDraw = !pauseDraw;
            }else if  (!pauseDraw && playing) {
                sound.stop();
                pauseDraw = !pauseDraw;
            }
            break;
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
        case 'z':
            sound.load("rock-song.wav"); //cambiar cancion a personal
            sound.play();
            break;
        case 'x':
            sound.load("beat.wav"); //cambiar cancion a personal
            sound.play();
            break;
        case 'c':
            sound.load("geesebeat.wav"); //cambiar cancion a personal
            sound.play();
            break;
        case 'v':
            sound.load("pigeon-coo.wav"); //esta cancion no suena
            sound.play();
            break;
        case '=': //sube volumen
            if(sound.getVolume() >= 1.00){
                sound.setVolume(1.00);
            }
            else if(sound.getVolume() >= 0.00 && sound.getVolume() < 1.00){
                sound.setVolume(sound.getVolume() + 0.20);
            } else {
                sound.setVolume(0.00);
            }
            break;
        case '-': //baja volumen
            if(sound.getVolume() <= 0.00){
                sound.setVolume(0.00);
            }
            else if(sound.getVolume() > 0.00 && sound.getVolume() <= 1.00){
                sound.setVolume(sound.getVolume() - 0.20);
            } else {
                sound.setVolume(1.00);
            }
            break;
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