#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // oF setup
    ofSetBackgroundColor(0);
    ofSetVerticalSync(false);
    
    // 雨粒フィルターを通す前のシーン
    main_scene.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    // texcoordを作る部分
    texcoord_scene.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    texcoord_shader.load("default.vert", "texcoord.frag");
    
    
    // 最後にテクスチャを参照してレンダリングする部分
    refer_texture_shader.load("default.vert", "final_refer_texture.frag");

    // debug
    test_image.load("test_img.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    vec2 screen_size = vec2(ofGetWidth(), ofGetHeight());
    
    // 雨粒フィルターのレンダリング
    ofPushMatrix();
    texcoord_scene.begin();
    texcoord_shader.begin();
    ofClear(0);
    ofScale(2.0);
    ofSetColor(255);
    texcoord_shader.setUniform2f("u_resolution", screen_size);
    ofDrawPlane(0, 0, screen_size.x, screen_size.y);
    
    texcoord_shader.end();
    texcoord_scene.end();
    ofPopMatrix();
    
    // 雨粒フィルターを通す前の画像
    ofPushMatrix();
    main_scene.begin();
    ofClear(0);
    // debug
    test_image.draw(vec2(0));
    
    main_scene.end();
    ofPopMatrix();
    
    // 最後にテクスチャを参照してレンダリングする部分
    ofClear(0);
    ofPushMatrix();
    refer_texture_shader.begin();
    
    refer_texture_shader.setUniformTexture("reference_texture", main_scene, 0);
    refer_texture_shader.setUniformTexture("texcoord_texture", texcoord_scene, 1);
    refer_texture_shader.setUniform2f("u_resolution", screen_size);
    ofScale(2.0);
    ofDrawPlane(0, 0, screen_size.x, screen_size.y);
    
    refer_texture_shader.end();
    ofPopMatrix();
    
    // debug
    ofSetColor(255);
//    texcoord_scene.draw(vec2(0), screen_size.x, screen_size.y);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
