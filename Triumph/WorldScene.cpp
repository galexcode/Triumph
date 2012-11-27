#include "WorldScene.h"
#include "GLUtil.h"

int WorldScene::init() {
	m_engine = GameEngine::getInstance();
	m_engine->registerScene(this);

	m_meshGlobeZoom = 0;

	m_cam.m_eye = Vector3(0, 0, -3000.0f);

	return INIT_SUCCESS;
}

void WorldScene::initGlobe()
{
    float radius = 1000.0f;
    m_texGlobe = Texture::CreateFromFile("earth_hires.bmp");
    
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
    m_meshGlobe = glGenLists(1);
    glNewList(m_meshGlobe, GL_COMPILE);
    gluSphere(sphere, radius, 200, 200);
    glEndList();
    gluDeleteQuadric(sphere);
    
    m_globe = new GameObject();
	m_globe->rotate(Vector3::right, PI / 2);
    m_globe->m_boundingRadius = radius;
}

void WorldScene::initSkybox()
{
    m_texSkybox[TOP_ID] = Texture::CreateFromFile("skyboxhigh_top.bmp");
    m_texSkybox[BOTTOM_ID] = Texture::CreateFromFile("skyboxhigh_bottom.bmp");
    m_texSkybox[LEFT_ID] = Texture::CreateFromFile("skyboxhigh_left.bmp");
    m_texSkybox[RIGHT_ID] = Texture::CreateFromFile("skyboxhigh_right.bmp");
    m_texSkybox[FRONT_ID] = Texture::CreateFromFile("skyboxhigh_front.bmp");
    m_texSkybox[BACK_ID] = Texture::CreateFromFile("skyboxhigh_back.bmp");
}

int WorldScene::load() {
	initSkybox();
	initGlobe();
	return 0;
}

void WorldScene::mouseButtonEvent(int button, int state)
{
	switch (button)
	{
		case INPUTBTN_MOUSELEFT:
		{
            if (!m_engine->m_input->m_fMouseLeft)
            {
                m_dragGlobeStart = Vector3::zero;
            }
		}
	}
}

void WorldScene::mouseWheelEvent(int dir)
{
	m_meshGlobeZoom += dir;
    if (m_meshGlobeZoom < 1)
        m_meshGlobeZoom = 1;
    Vector3 eyeNorm = m_cam.m_eye;
    eyeNorm.normalize();
	m_cam.m_eye += (dir) * eyeNorm * -10;
    m_dragGlobeStart = Vector3::zero;
}

void WorldScene::updateCamera()
{
    //glTranslatef(0, 0, -3000 + m_meshGlobeZoom * 10);
	
    // camera is based on a 3d system, so make sure the 3d
    // perspective is set before the lookat transform is done
    m_engine->set3D();
    
	gluLookAt(m_cam.m_eye.m_x, m_cam.m_eye.m_y, m_cam.m_eye.m_z,
		m_cam.m_target.m_x, m_cam.m_target.m_y, m_cam.m_target.m_z,
		m_cam.m_up.m_x, m_cam.m_up.m_y, m_cam.m_up.m_z);
}

void WorldScene::draw(float dTime) {
    glEnable(GL_TEXTURE_2D);
    
    glPushMatrix();
    glTranslatef(m_globe->m_position.m_x, m_globe->m_position.m_y, m_globe->m_position.m_z);
    glMultMatrixf(m_globe->m_rotation.getMatrix().v);
    glBindTexture(GL_TEXTURE_2D, m_texGlobe->gid());
    glCallList(m_meshGlobe);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    GLUtil::DrawSkybox(m_cam.m_eye, m_texSkybox);
}

// the camera rotation parts of this function
// require that 3d mode and the camera be fully
// prepared for the scene
void WorldScene::update(float dTime)
{
    updateCamera();
    
    float angle = 0;
    Vector3 vec;
    Vector3 dir = m_cam.m_target - m_cam.m_eye;
    if (m_engine->m_input->m_fLeft || m_engine->m_input->m_fRight)
    {
        if (m_engine->m_input->m_fLeft)
        {
            angle = -0.02f; // / m_meshGlobeZoom;
            vec = Vector3::up;
        }
        if (m_engine->m_input->m_fRight)
        {
            angle += 0.02f; // / m_meshGlobeZoom;
            vec = Vector3::up;
        }
        m_cam.m_eye = Quaternion::CreateFromAxis(vec, angle) * m_cam.m_eye;
    }
    if (m_engine->m_input->m_fUp || m_engine->m_input->m_fDown)
    {
        if (m_engine->m_input->m_fUp)
        {
            angle = -0.02f; // / m_meshGlobeZoom;
            vec = dir.cross(m_cam.m_up);
        }
        if (m_engine->m_input->m_fDown)
        {
            angle += 0.02f; // / m_meshGlobeZoom;
            vec = dir.cross(m_cam.m_up);
        }
        m_cam.m_eye = Quaternion::CreateFromAxis(vec, angle) * m_cam.m_eye;
    }
    
    if (m_engine->m_input->m_fMouseLeft)
	{
        // this seems pretty sloppy, but it will allow us to
        // rotate the camera about the mouse drag of the globe
        // todo: clean up, perhaps optimize
        Ray3 ray = GLUtil::GetMouseRay();
        Sphere globeBounds(m_globe->m_position, m_globe->m_boundingRadius);
        Vector3 hit;
        if (ray.intersects(globeBounds, &hit))
        {
            if (m_dragGlobeStart.equals(Vector3::zero))
            {
                m_dragGlobeStart = hit;
            }
            else
            {
                float angle = m_dragGlobeStart.angleBetween(hit);
                if (angle != 0)
                {
                    Quaternion q = Quaternion::CreateFromAxis(hit.cross(m_dragGlobeStart),
                                                              angle);
                    m_cam.m_eye = q * m_cam.m_eye;
                    
                    updateCamera();
                    ray = GLUtil::GetMouseRay();
                    ray.intersects(globeBounds, &hit);
                    m_dragGlobeStart = hit;
                }
            }
        }
	}

}

void WorldScene::clean() {
    glDeleteLists(m_meshGlobe, 1);
    
    delete m_texGlobe;
    delete m_texSkybox[TOP_ID];
    delete m_texSkybox[BOTTOM_ID];
    delete m_texSkybox[LEFT_ID];
    delete m_texSkybox[RIGHT_ID];
    delete m_texSkybox[FRONT_ID];
    delete m_texSkybox[BACK_ID];
    
    delete m_globe;
}