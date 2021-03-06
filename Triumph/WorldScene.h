//
//  WorldScene.h
//  Triumph
//
//  Created by Justin Labak on 11/8/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__WorldScene__
#define __Triumph__WorldScene__

#include "Scene.h"
#include "GameObject.h"
#include "Geometry.h"
#include "GameCamera.h"
#include "Texture.h"
#include "GameEngine.h"

class WorldScene : public Scene
{
    
public:
	int load();
	int init();
    
	void mouseButtonEvent(int button, int state);
	void mouseWheelEvent(int dir);

	void draw(float dTime);
	void update(float dTime);

	void clean();

private:
	GameEngine *m_engine;

	int m_meshGlobe;
	float m_meshGlobeZoom;
    Texture *m_texGlobe;
    Texture *m_texSkybox[6];
    GameObject *m_globe;
    Vector3 m_dragGlobeStart;
    
	GameCamera m_cam;

    void initGlobe();
    void initSkybox();

    void updateCamera();

};

#endif /* defined(__Triumph__WorldScene__) */
