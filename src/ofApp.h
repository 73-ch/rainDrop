#pragma once

#include "ofMain.h"

using namespace glm;

#define MIN_R 10.0
#define MAX_R 40.0
#define COLLISION_R 0.65
#define COLLISION_INCREASE 0.01
#define COLLISION_BOOST 1.0
#define COLLISION_BOOST_SPEED 0.05
#define DROP_RATE 50

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
        vec2 spread = vec2(0);
        vec2 momentum = vec2(0);
        float last_spawn = 0.;
        float next_spawn = 0.;
        LargeDrop* parent;
        float size;
        bool isNew;
        bool killed;
        float shrink;
    };
    float time_scale = 0.9;
    float fall_speed = 1.0;
    float trail_rate = 1.0;
    float last_time;
    
    
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
