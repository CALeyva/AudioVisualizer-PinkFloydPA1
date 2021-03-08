#pragma once

#include "ofMain.h"
#include "AudioVisualizer.h"
#include <map>

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void drawMode1(vector<float> amplitudes);
		void drawMode2(vector<float> amplitudes);
		void drawMode3(vector<float> amplitudes);
		void drawMode4(vector<float> amplitudes);
		void drawMode5(vector<float> amplitudes); //mode 5 added Bonus (Pink Floyd)

		void setMode(int key);
		void setMusic(int key);
		void setVolume(int key);
		void setPauseMode(int key);
		void recordPlayback(int key);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	private:
		ofSoundPlayer sound;
		AudioVisualizer visualizer;
		
		bool playing = false;
		bool pauseDraw = false;

		long counter = 0;
		bool recording = false;
		bool playback = false;
		long framesRecorded;
		char keyRecorded;
		int playKey;
		bool keyInFrame;
		std::map<long, int> recordings;
		
		char mode = '1';

	/* Initialize variables for resizing screen window */
		enum ScaleDir {
			SCALE_DIR_HORIZONTAL,
			SCALE_DIR_VERTICAL,
		};
		ScaleDir scaleDir;

		int windowWidth, windowHeight; //original window dimensions
		float widthScaled, heightScaled; //scaled window dimensions
		float windowScale; //scale amount
		bool scaleDirFixed; //is direction fixed?
		float ratio; //ratio used in window resizing
};
