#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // oF setup
    ofSetBackgroundColor(0);
    ofSetVerticalSync(false);
    
    // 大きい雨粒
    large_scene.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    // 小さい雨粒
    small_scenes.first.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    small_scenes.second.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    small_scenes.second.begin();
    ofClear(0);
    small_scenes.second.end();
    
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
    screen_size = vec2(ofGetWidth(), ofGetHeight());
    float time = ofGetElapsedTimef();
    
    // 大きい雨粒
    if (ofGetFrameNum() % 10 == 0) {
        // 新しい大きい雨粒の生成
        auto large_drop = new LargeDrop();
        large_drop->pos = vec2(ofRandom(screen_size.x), ofRandom(screen_size.y * 0.7));
        
        float r = ofRandom(10, 40);
        large_drop->r = r;
        large_drop->momentum.y = 1+((r-10)*0.1)+ofRandom(2);
        large_drop->spread = vec2(1.5);
        large_drops.push_back(large_drop);
    }
    
    for (auto& r : large_drops) {
        
        // positionの更新
        bool moved = r->momentum.y > 0;
        if (moved && !r->killed) {
            r->pos += r->momentum * time_scale;
            // 画面外判定
            if (r->pos.y > screen_size.y + r->r) r->killed = true;
        }
    }
    
    // 大きい粒のレンダリング
    ofPushMatrix();
    large_scene.begin();
    ofClearAlpha();
    ofClear(0, 0, 0, 0);
    
    for (const auto& r : large_drops) {
        
        // debug
        ofSetColor(255, 0, 0);
        ofDrawCircle(r->pos, r->r);
    }
    
    large_scene.end();
    ofPopMatrix();
    
    // 小さい雨粒のレンダリング
    ofPushMatrix();
    ofDisableAlphaBlending();
    small_scenes.first.begin();
    ofClear(0);
    small_scenes.second.draw(vec2(0), screen_size.x, screen_size.y);
    
    if (ofNoise(time*10.) > 0.3) {
        vec2 move_pos = vec2(ofRandom(screen_size.x), ofRandom(screen_size.y));
        ofTranslate(move_pos);
        ofRotateZRad(ofRandom(M_PI_2));
        
        ofVboMesh small_drop;
        small_drop.enableColors();
        small_drop.addVertex(vec3(0));
        small_drop.addColor(ofFloatColor(0.5,0.5,0.));
        for (int i = 0; i < 11; ++i) {
            vec2 p = vec2(cos((i%10)*PI*.2), sin((i%10)*PI*.2)) * (ofNoise(vec3(move_pos, (i%10)*0.2))*.6 + .4);
            small_drop.addColor(ofFloatColor((p.x + 1.)*.5, (p.y + 1.)*.5, 0.));
            small_drop.addVertex(vec3(p, .0) * 3.);
        }
        
        small_drop.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        ofSetColor(255);
        small_drop.draw();
    }
    
    
    small_scenes.first.end();
    ofPopMatrix();
    
    swap(small_scenes.first, small_scenes.second);
    
//    small_scenes.swap();
    
    // 雨粒フィルターのレンダリング
    ofPushMatrix();
    texcoord_scene.begin();
    texcoord_shader.begin();
    ofClear(0);
    ofScale(2.0);
    ofSetColor(255);
    texcoord_shader.setUniform2f("u_resolution", screen_size);
    texcoord_shader.setUniformTexture("small_drops", small_scenes.first, 0);
    ofDrawPlane(0, 0, screen_size.x, screen_size.y);
    
    texcoord_shader.end();
    texcoord_scene.end();
    ofPopMatrix();
    ofFbo front_small_scene;
    
    // 雨粒フィルターを通す前の画像
    ofPushMatrix();
    main_scene.begin();
    ofClear(0);
    // debug
    test_image.draw(vec2(0));
    
    main_scene.end();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw(){
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
    large_scene.draw(vec2(0), screen_size.x, screen_size.y);
    
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
