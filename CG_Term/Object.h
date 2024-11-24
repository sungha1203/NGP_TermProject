#pragma once
#include"Framework.h"
class Framework;
class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
public:
    GLint result;
    GLchar errorLog[512];
public:
    vector <glm::vec3> m_vertex;
    vector <glm::vec3> m_normal;
    vector <glm::vec2> m_vt;

    GLuint m_VBOVertex;
    GLuint m_VBONormal;
    GLuint m_VBOTexture;
    GLuint m_VBOColor;
    void SetVBO() {//vbo��ü ����
        glGenBuffers(1, &m_VBOVertex);
        glGenBuffers(1, &m_VBONormal);
        glGenBuffers(1, &m_VBOTexture);
    }
public:
    glm::mat4 model_matrix = glm::mat4(1.0f); //������ķ� �ʱ�ȭ
    glm::mat4 parents_matrix = glm::mat4(1.0f); //������ķ� �ʱ�ȭ
    float rotateAngle[3] = { 0 };
    float revolutionAngle[3] = { 0 };
    float scale[3] = { 2.0f,1.0f,0.5f };
    vec3 color;
    vec3 move_pos;
    void draw();
    int m_textureNum = -1;
public:
    int m_ability; //item��
    bool exist = true;
    int item_navi = 0;
    int item_where = 0;
    int item_Key_where = 0;
    bool ũ������Ƽ���� = false;
    void ability();
    void startTimer1();
    void startTimer2();
    bool m_direction = true;//ghost��
    int m_direcN = 0;
    float speed = 0.0f;
    int direction = 1;
    void Move(int movetype);
    int m_modeN;   //mode��
    int m_IsOpen = 0;//door��
    int m_OpenN = 0;
    Framework* framework;
    void setFramework(Framework* fw) {
        framework = fw;
    }
public:
    vec3 P2_pos; //�ٸ� Ŭ���̾�Ʈ ��ġ ����
    vec3 At; //�ٸ� Ŭ���̾�Ʈ ���� ���� ����
    int id{}; //Ŭ���̾�ƮID


    bool sock_check = false; //�ٸ� Ŭ���̾�Ʈ�� �����ߴ��� ����
    //�÷��̾ �浹ó����
    BoundingBox m_box;
};

class planeObject : public GameObject
{
public:
    planeObject(int num);
    ~planeObject();
};
class doorObject : public GameObject
{
public:
    doorObject(int num);
    ~doorObject();
};
class itemObject : public GameObject
{
public:
    itemObject(int num);
    ~itemObject();
};
class keyObject : public GameObject
{
public:
    keyObject(int num);
    ~keyObject();
};
class bottomObject : public GameObject
{
public:
    bottomObject();
    ~bottomObject();
};
class hintObject : public GameObject
{
public:
    hintObject();
    ~hintObject();
};
class ghostObject : public GameObject
{
public:
    ghostObject(int num);
    void Move(int movetype);
    ~ghostObject();
};
class modeObject : public GameObject
{
public:
    modeObject(int num);
    ~modeObject();
};
class playerCheck : public GameObject
{
public:
    playerCheck(int id);
    ~playerCheck();
};
////////////////////////////////////////////////////////////////
//P2�� ���� ������Ʈ
class OtherPlayer : public GameObject
{
public:
    OtherPlayer(int id);
    ~OtherPlayer();
};
