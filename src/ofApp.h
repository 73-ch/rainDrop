#pragma once

#include "ofMain.h"

using namespace glm;

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // common
    vec2 screen_size;
    
    struct LargeDrop {
        float r;
        vec2 pos;
        vec2 spread;
        vec2 momentum;
        float lastSpawn;
        float nextSpawn;
        LargeDrop* parent;
        float size;
        bool isNew;
        bool killed;
        float shrink;
    };
    float time_scale = 1.0;
    
    vector<LargeDrop*> large_drops;
    ofFbo large_scene;
    
    pair<ofFbo, ofFbo> small_scenes;
    
    ofFbo texcoord_scene;
    ofShader texcoord_shader;
    
    ofFbo main_scene;
    

    ofEasyCam cam;
    ofShader refer_texture_shader;
    
    // debug
    ofImage test_image;
};
