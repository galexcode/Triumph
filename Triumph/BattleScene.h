//
//  BattleScene.h
//  Triumph
//
//  Created by Justin Labak on 11/8/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__BattleScene__
#define __Triumph__BattleScene__

#include "Scene.h"
#include "Texture.h"
#include "GameCamera.h"
#include "GameEngine.h"
#include "Geometry.h"
#include "Mesh.h"

class BattleScene : public Scene
{
private:
    GameCamera m_cam;
    GameEngine *m_engine;

    Mesh *m_water;
    
	bool m_fFreeLook;
    bool m_fWireframe;

public:
	int load();
    
    void update(float dTime);
    void draw(float dTime);

	void keyEvent(int key, int state);
    
    void clean();

};

#endif /* defined(__Triumph__BattleScene__) */
