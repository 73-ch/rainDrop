#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // oF setup
    ofSetBackgroundColor(0);
    ofSetVerticalSync(false);
//    ofSetFrameRate(60);
    
    // 大きい雨粒
    large_scene.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    rain_shader.load("rain.vert", "rain.frag");
    // 雨粒フィルターを通す前のシーン
    main_scene.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    // 最後にテクスチャを参照してレンダリングする部分
    refer_texture_shader.load("default.vert", "final_refer_texture.frag");

    // debug
    test_image.load("test_img.jpg");

}

//--------------------------------------------------------------
void ofApp::update(){
    // debug
    rain_shader.load("rain.vert", "rain.frag");
    
    screen_size = vec2(ofGetWidth(), ofGetHeight());
    float time = ofGetElapsedTimef();
    
    // 大きい雨粒
    if (ofGetFrameNum() % 1 == 0 && large_drops.size() <= 4000) {
        // 新しい大きい雨粒の生成
        auto large_drop = new LargeDrop();
        large_drop->pos = vec2(ofRandom(screen_size.x), ofRandom(screen_size.y * 0.7));
        
        float r = MIN_R + pow(ofRandom(1.0),3.0) * (MAX_R - MIN_R);
        large_drop->r = r;
        large_drop->momentum.y = 1+((r-MIN_R)*0.1)+ofRandom(2);
        large_drop->spread = vec2(1.5);
        large_drop->spawn_time = time;
        large_drops.push_back(large_drop);
    }
    
    
    // sort
    
    vector<LargeDrop*> new_trails;
    vector<LargeDrop*> new_drops;

    for (int i = 0; i < large_drops.size(); ++i) {
        auto drop = large_drops[i];
        // update gravity
        if (ofRandom(1.0) <= (drop->r - (MIN_R * fall_speed)) * (.1 / (MAX_R - MIN_R) * time_scale)) {
            drop->momentum.y += ofRandom((drop->r/MAX_R)*4);
        }
        
        // clean small drop
        if (drop->r <= MIN_R && ofRandom(1.0) <= time_scale * 0.05) {
            drop->shrink += 0.01;
        }
        
        // update shrinkage
        drop->r -= drop->shrink * time_scale;
        if (drop->r <= 0) drop->killed = true;
        
        if (raining) {
            // update trail
            drop->last_spawn += drop->momentum.y * time_scale + trail_rate;
            
            if (drop->last_spawn > drop->next_spawn && large_drops.size() <= 4000) {
                auto trail_drop = new LargeDrop();
                trail_drop->pos = drop->pos + vec2(ofRandom(-drop->r, drop->r)*0.1, -drop->r*0.01);
                
                trail_drop->r = drop->r * ofRandom(0.2, 0.5);
                trail_drop->spread.y = drop->momentum.y * 0.1;
                trail_drop->parent = drop;
                trail_drop->spawn_time = time;
                
                new_drops.push_back(trail_drop);
                
                drop->r *= pow(0.97, time_scale);
                drop->last_spawn = 0.0;
                drop->next_spawn = ofRandom(MIN_R, MAX_R) - (drop->momentum.y * 2. * trail_rate) + (MAX_R - drop->r);
            }
        }
        
        
        drop->spread *= vec2(pow(0.4, time_scale), pow(0.7, time_scale));
        
        // positionの更新
        bool moved = drop->momentum.y > 0;
        if (moved && !drop->killed) {
            drop->pos += drop->momentum * time_scale;
            // 画面外判定
            if (drop->pos.y > screen_size.y + drop->r) drop->killed = true;
        }
        
        // collision
        bool check_collision = (moved || drop->isNew) && !drop->killed;
        drop->isNew = false;
        
        if (check_collision) {
            for (int j = i; j < large_drops.size() && j < i + 70; j++) {
                auto drop2 = large_drops[j];
                if (drop != drop2 && (drop->r > drop2->r) && drop->parent != drop2 && drop2->parent != drop && !drop2->killed) {
                    vec2 dist = drop2->pos - drop->pos;
                    float d = sqrt(dot(dist, dist)); // ここ自分で置き換えたので要注意
                    if (d < (drop->r + drop2->r) * (COLLISION_R + (drop->momentum.y * COLLISION_INCREASE * time_scale))) {
                        
                        const float a1 = PI * (drop->r * drop->r);
                        const float a2 = PI * (drop2->r * drop2->r);
                        float target_r = std::min(sqrt((a1 + (a2 * .8))/ PI), MAX_R);
                        drop->r = target_r;
                        drop->momentum.x = dist.x*.1;
                        drop->spread = vec2(0);
                        drop2->killed = true;
                        drop->momentum.y = std::max(drop2->momentum.y, (float)std::min({MAX_R,drop->momentum.y + (target_r*COLLISION_BOOST_SPEED) + COLLISION_BOOST}));
                    }
                }
            }
        }
        
        // slowdown momentum
        drop->momentum.y -= std::max(1.0, (MIN_R * 0.5) - drop->momentum.y) * .1 * time_scale;
        if (drop->momentum.y < 0) drop->momentum.y = 0;
        drop->momentum.x *= pow(0.7, time_scale);
        
        if (!drop->killed) {
            new_drops.push_back(drop);
        }
    }
    
    large_drops.clear();
    swap(large_drops, new_drops);
    
    sort(large_drops.begin(), large_drops.end(), [&](const LargeDrop* a, const LargeDrop* b){
        return (a->pos.y*screen_size.y+a->pos.x) < (b->pos.y*screen_size.y+b->pos.x);
    });
    
    // 大きい粒のレンダリング
    large_scene.begin();
    ofClearAlpha();
    ofClear(0, 0, 0, 0);
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    rain_shader.begin();
    for (const auto& r : large_drops) {
        if (!r->killed) {
            ofPushMatrix();
            ofSetColor(255);
            vec2 d = vec2(r->r) * vec2(1.,1.5) * (r->spread + vec2(1));
            vec2 trans = r->pos - d;
            vec2 p = d*2.;
            ofTranslate(trans);
            rain_shader.setUniform2f("u_resolution", p);
            rain_shader.setUniform2f("screen_size", screen_size);
            rain_shader.setUniform3f("u_color", vec3(1.0));
            ofDrawRectangle(vec2(0), p.x, p.y);
            
            ofPopMatrix();
        }
    }
    rain_shader.end();
    
    ofDisableAlphaBlending();
    large_scene.end();
    
    // 雨粒フィルターを通す前の画像
    ofPushMatrix();
    main_scene.begin();
    ofClear(0);
    // debug
    test_image.draw(vec2(0));
    
    main_scene.end();
    ofPopMatrix();
    
    last_time = time;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // 最後にテクスチャを参照してレンダリングする部分
    ofClear(0);
    ofPushMatrix();
    refer_texture_shader.begin();
    ofSetColor(255, 255,255,255);
    refer_texture_shader.setUniformTexture("reference_texture", main_scene, 0);
    refer_texture_shader.setUniformTexture("large_drop_texcoord_texture", large_scene, 2);
    refer_texture_shader.setUniform2f("u_resolution", screen_size);
    ofScale(2.0);
    ofDrawPlane(0, 0, screen_size.x, screen_size.y);
    
    refer_texture_shader.end();
    ofPopMatrix();
    
    // debug
//    ofSetColor(255);
//    large_scene.draw(vec2(0), screen_size.x, screen_size.y);
    
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
