#pragma once
#include"stdafx.h"
#include "Object.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
}

void GameObject::draw()
{
    if (m_textureNum == -1)//hint
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertex.size(), &m_vertex[0], GL_STATIC_DRAW);

        int positionAttrib = glGetAttribLocation(framework->shaderProgramID, "vPos");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(positionAttrib);

        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
        model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
        model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
        //model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

        model_matrix = glm::translate(model_matrix, (move_pos));

        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

        model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
        model_matrix = parents_matrix * model_matrix;

        unsigned int transLocation = glGetUniformLocation(framework->shaderProgramID, "modelTransform");
        glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

        int vColorLocation = glGetUniformLocation(framework->shaderProgramID, "objectColor");

        GLint textureNum = glGetUniformLocation(framework->shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
        glUniform1i(textureNum, 0);

        glLineWidth(1.0f);
        glUniform3f(vColorLocation, color.r, color.g, color.b);
        glDrawArrays(GL_LINE_STRIP, 0, m_vertex.size());
    }
    else if (m_textureNum == 9 || m_textureNum == 10 || m_textureNum == 11 || m_textureNum == 12) //playercheck 9, 플레이어가 별인 경우, 버섯인 경우
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertex.size(), &m_vertex[0], GL_STATIC_DRAW);

        int positionAttrib = glGetAttribLocation(framework->shaderProgramID, "vPos");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(positionAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_normal.size(), &m_normal[0], GL_STATIC_DRAW);

        int normalAttrib = glGetAttribLocation(framework->shaderProgramID, "vNormal");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_vt.size(), &m_vt[0], GL_STATIC_DRAW);

        int textureAttrib = glGetAttribLocation(framework->shaderProgramID, "vTexCoord");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(textureAttrib);

        float cameraYaw{};
        glm::vec3 pos{};

        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
        model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
        model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);

        if (id == 1 || id == 2) { //해당 플레이어일 경우
            // 카메라 방향에서 Y축 회전 각도 계산
            glm::vec3 cameraDirection = glm::normalize(framework->camera.cameraAt); // 카메라의 시선 방향 벡터
            cameraYaw = glm::degrees(atan2(cameraDirection.x, cameraDirection.z)); // Y축 회전 각도

            //model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
            pos = framework->camerapos;
        }
        else { //상대방 플레이어일 경우
            // 카메라 방향에서 Y축 회전 각도 계산
            glm::vec3 cameraDirection = glm::normalize(At); // 카메라의 시선 방향 벡터
            cameraYaw = glm::degrees(atan2(cameraDirection.x, cameraDirection.z)); // Y축 회전 각도
            if (sock_check) pos = P2_pos;
        }   
        
        if (m_textureNum == 10) pos.y -= 0.3f;//P2 주인공이 별인 경우 위치 살짝 내리기      
        if (m_textureNum == 11) pos.y -= 0.17f;//P2 주인공 위치 버섯인 경우 위치 살짝 내리기   


        model_matrix = glm::translate(model_matrix, (pos));

        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
        model_matrix = glm::rotate(model_matrix, glm::radians(cameraYaw), AXIS_Y);

        model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
        model_matrix = parents_matrix * model_matrix;

        unsigned int transLocation = glGetUniformLocation(framework->shaderProgramID, "modelTransform");
        glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

        unsigned int objColorLocation = glGetUniformLocation(framework->shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색

        glUniform3f(objColorLocation, color.r, color.g, color.b);


        GLint textureNum = glGetUniformLocation(framework->shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
        glUniform1i(textureNum, 1);

        glBindTexture(GL_TEXTURE_2D, framework->texture[m_textureNum]);
        glDrawArrays(GL_TRIANGLES, 0, m_vertex.size());
    }
    else { //m_textureNum 0~8
        glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertex.size(), &m_vertex[0], GL_STATIC_DRAW);

        int positionAttrib = glGetAttribLocation(framework->shaderProgramID, "vPos");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(positionAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_normal.size(), &m_normal[0], GL_STATIC_DRAW);

        int normalAttrib = glGetAttribLocation(framework->shaderProgramID, "vNormal");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_vt.size(), &m_vt[0], GL_STATIC_DRAW);

        int textureAttrib = glGetAttribLocation(framework->shaderProgramID, "vTexCoord");
        //어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
        glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
        glEnableVertexAttribArray(textureAttrib);


        model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
        model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
        model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
        //model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

        model_matrix = glm::translate(model_matrix, (move_pos));

        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
        model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

        model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
        model_matrix = parents_matrix * model_matrix;

        unsigned int transLocation = glGetUniformLocation(framework->shaderProgramID, "modelTransform");
        glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));


        unsigned int objColorLocation = glGetUniformLocation(framework->shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
        glUniform3f(objColorLocation, color.r, color.g, color.b);

        GLint textureNum = glGetUniformLocation(framework->shaderProgramID, "TextureN"); //요부분은 framework에서 하는걸로
        glUniform1i(textureNum, 1);

        glBindTexture(GL_TEXTURE_2D, framework->texture[m_textureNum]);
        glDrawArrays(GL_TRIANGLES, 0, m_vertex.size());
    }

}

void GameObject::Move(int movetype)
{
    if (movetype == 1) {
        move_pos.z += direction * speed;
        if (move_pos.z > 11.0 || move_pos.z < -11.0)
        {
            direction *= -1;
            rotateAngle[1] += 180 * direction;
        }
    }
    else
    {
        move_pos.x += direction * speed;
        if (move_pos.x > 11.0 || move_pos.x < -11.0)
        {
            direction *= -1;
            rotateAngle[1] += 180 * direction;
        }
    }
}
void GameObject::ability()
{
    if (m_ability == 0) {  //다 보여주기
        item_navi = 1;
        item_where = 1;
        item_Key_where = 1;
        크리에이티브모드 = true;
    }
    else if (m_ability == 1) {  //네비게이션
        item_navi = 1;
        thread timerThread(std::bind(&GameObject::startTimer1, this));
        timerThread.detach();
    }
    else if (m_ability == 2) {  //아이템 힌트
        item_where = 1;
        thread timerThread(std::bind(&GameObject::startTimer1, this));
        timerThread.detach();
    }
    else if (m_ability == 5) {  //열쇠 힌트
        item_Key_where = 1;
    }
}

void GameObject::startTimer1()
{
    this_thread::sleep_for(std::chrono::seconds(10));
    m_ability = -1;
    item_navi = 0;
    framework->hintIndex = -1;
}

void GameObject::startTimer2()
{
    this_thread::sleep_for(std::chrono::seconds(10));
    m_ability = -1;
    item_where = 0;
    framework->hintIndex = -1;
}


planeObject::planeObject(int num) :GameObject()
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    m_textureNum = 0;
    if (num == 0) {
        move_pos = { -12.0f,0.0f,-9.4f };
    }
    else if (num == 1) {
        move_pos = { -12.0f,0.0f,-6.46f };
    }
    ReadObj("planeTP.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}
planeObject::~planeObject()
{
}
doorObject::doorObject(int num) :GameObject()
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    scale[0] = 1.0f;
    scale[1] = 0.5f;
    scale[2] = 0.324f;
    m_textureNum = 4;
    if (num == 1) {
        move_pos = { -9.8f,0.0f,-9.08f };
    }
    else if (num == 2) {
        rotateAngle[1] = 180.0f;
        move_pos = { -9.8f,0.0f,-9.728f };
    }
    if (num == 3) {
        move_pos = { -9.96f,0.0f,-6.1f };
    }
    else if (num == 4) {
        rotateAngle[1] = 180.0f;
        move_pos = { -9.96f,0.0f,-6.748f };
    }
    ReadObj("door.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}
doorObject::~doorObject()
{
}
itemObject::itemObject(int num) :GameObject()
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 0.0f;
    m_textureNum = 1;
    if (num == 0) {
        move_pos = { -9.05f,0.1f,-9.4f };
        m_ability = 1;
    }
    else if (num == 1) {
        move_pos = { -0.5f,0.1f,-4.5f };
        m_ability = 5;
    }
    else if (num == 2) {
        move_pos = { -2.5f,0.1f,-0.5f };
        m_ability = 2;
    }
    else if (num == 3) {
        move_pos = { -4.5f,0.1f,-2.5f };
        m_ability = 1;
    }
    else if (num == 4) {
        move_pos = { -8.5f,0.1f,-7.5f };
        m_ability = 3;
    }
    else if (num == 5) {
        move_pos = { -6.5f,0.1f,-2.5f };
        m_ability = 1;
    }
    else if (num == 6) {
        move_pos = { -1.5f,0.1f,3.5f };
        m_ability = 5;
    }
    else if (num == 7) {
        move_pos = { 1.5f,0.1f,6.5f };
        m_ability = 5;
    }
    else if (num == 8) {
        move_pos = { 3.5f,0.1f,8.5f };
        m_ability = 1;
    }
    else if (num == 9) {
        move_pos = { 6.5f,0.1f,6.5f };
        m_ability = 2;
    }
    else if (num == 10) {
        move_pos = { 5.5f,0.1f,4.5f };
        m_ability = 1;
    }
    else if (num == 11) {
        move_pos = { 9.5f,0.1f,5.5f };
        m_ability = 1;
    }
    else if (num == 12) {
        move_pos = { 6.5f,0.1f,-1.2f };
        m_ability = 5;
    }
    else if (num == 13) {
        move_pos = { 4.5f,0.1f,-3.5f };
        m_ability = 2;
    }
    else if (num == 14) {
        move_pos = { 2.0f,0.1f,-9.5f };
        m_ability = 1;
    }
    else if (num == 15) {
        move_pos = { 7.5f,0.1f,-9.5f };
        m_ability = 5;
    }
    else if (num == 16) {
        move_pos = { -9.5f,0.1f,6.5f };
        m_ability = 2;
    }
    else if (num == 17) {
        move_pos = { -8.5f,0.1f,0.5f };
        m_ability = 1;
    }
    else if (num == 18) {
        move_pos = { -4.5f,0.1f,5.5f };
        m_ability = 3;
    }
    else if (num == 19) {// 개발자 힌트 (시작하자마자 뒤)
        move_pos = { -13.8f,0.1f,-9.4f };
        m_ability = 0;
    }
    ReadObj("cube.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}
itemObject::~itemObject()
{
}
keyObject::keyObject(int num) :GameObject()
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    key_num = num;
    m_textureNum = 5;
    if (num == 0) {
        move_pos = { -2.5, 0.1, -9.4 };
    }
    else if (num == 1) {
        move_pos = { 9.5f,0.1f,-8.5f };
    }
    else if (num == 2) {
        move_pos = { -8.5f,0.1f,4.5f };
    }
    ReadObj("key.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}
keyObject::~keyObject()
{
}
bottomObject::bottomObject() :GameObject()
{
    m_textureNum = 3;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;

    move_pos = { 0.0f,-8.0f,0.0f };
    SetVBO();
}
bottomObject::~bottomObject()
{
}
hintObject::hintObject() :GameObject()
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    move_pos.x = 0.0f;
    move_pos.y = 0.0f;
    move_pos.z = 0.0f;
    scale[0] = 1.0f;
    scale[1] = 1.0f;
    scale[2] = 1.0f;
    m_vertex.emplace_back(-10, 0.02, -9.4);
    m_vertex.emplace_back(-2.5, 0.02, -9.4);
    m_vertex.emplace_back(-2.5, 0.02, -3.5);
    m_vertex.emplace_back(-3.5, 0.02, -3.5);
    m_vertex.emplace_back(-3.5, 0.02, -8.5);
    m_vertex.emplace_back(-7.5, 0.02, -8.5);
    m_vertex.emplace_back(-7.5, 0.02, -7.5);
    m_vertex.emplace_back(-4.5, 0.02, -7.5);
    m_vertex.emplace_back(-4.5, 0.02, -6.5);
    m_vertex.emplace_back(-5.5, 0.02, -6.5);
    m_vertex.emplace_back(-5.5, 0.02, -4.5);
    m_vertex.emplace_back(-6.5, 0.02, -4.5);
    m_vertex.emplace_back(-7.5, 0.02, -2.5);
    m_vertex.emplace_back(-6.5, 0.02, -0.5);
    m_vertex.emplace_back(-4.5, 0.02, -0.5);
    m_vertex.emplace_back(-4.5, 0.02, 2.5);
    m_vertex.emplace_back(-5.5, 0.02, 2.5);
    m_vertex.emplace_back(-5.5, 0.02, 3.5);
    m_vertex.emplace_back(-3.5, 0.02, 3.5);
    m_vertex.emplace_back(-1.5, 0.02, 2.5);
    m_vertex.emplace_back(-0.5, 0.02, 2.5);
    m_vertex.emplace_back(2.5, 0.02, 1.15);
    m_vertex.emplace_back(1.9, 0.02, -1.0);
    m_vertex.emplace_back(3.3, 0.02, -4.0);
    m_vertex.emplace_back(2.5, 0.02, -5.0);
    m_vertex.emplace_back(2.5, 0.02, -8.5);
    m_vertex.emplace_back(-1.5, 0.02, -8.5);
    m_vertex.emplace_back(-1.5, 0.02, -9.5);
    m_vertex.emplace_back(6.5, 0.02, -9.5);
    m_vertex.emplace_back(6.5, 0.02, -7.5);
    m_vertex.emplace_back(9.5, 0.02, -7.5);
    m_vertex.emplace_back(9.5, 0.02, 3.5);
    m_vertex.emplace_back(8.5, 0.02, 3.5);
    m_vertex.emplace_back(8.5, 0.02, 2.0);
    m_vertex.emplace_back(6.5, 0.02, 0.0);
    m_vertex.emplace_back(6.5, 0.02, 1.5);
    m_vertex.emplace_back(8.5, 0.02, 5.5);
    m_vertex.emplace_back(5.5, 0.02, 5.5);
    m_vertex.emplace_back(5.5, 0.02, 7.5);
    m_vertex.emplace_back(2.5, 0.02, 7.5);
    m_vertex.emplace_back(2.5, 0.02, 5.5);
    m_vertex.emplace_back(-1.5, 0.02, 5.5);
    m_vertex.emplace_back(-1.5, 0.02, 4.5);
    m_vertex.emplace_back(-6.5, 0.02, 4.5);
    m_vertex.emplace_back(-6.5, 0.02, 1.5);
    m_vertex.emplace_back(-5.5, 0.02, 1.5);
    m_vertex.emplace_back(-5.5, 0.02, 0.5);
    m_vertex.emplace_back(-7.5, 0.02, 0.5);
    m_vertex.emplace_back(-7.5, 0.02, 3.5);
    m_vertex.emplace_back(-9.5, 0.02, 3.5);
    m_vertex.emplace_back(-9.5, 0.02, -3.5);
    m_vertex.emplace_back(-8.5, 0.02, -3.5);
    m_vertex.emplace_back(-8.5, 0.02, -4.5);
    m_vertex.emplace_back(-9.5, 0.02, -4.5);
    m_vertex.emplace_back(-9.5, 0.02, -5.5);
    m_vertex.emplace_back(-7.5, 0.02, -5.5);
    m_vertex.emplace_back(-7.5, 0.02, -6.5);
    m_vertex.emplace_back(-10.0, 0.02, -6.5);
}
hintObject::~hintObject()
{
}
ghostObject::ghostObject(int num) :GameObject()
{
    random_device rd;
    default_random_engine dre(rd());
    uniform_real_distribution<float> urd(0.01, 0.04);
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    m_textureNum = 6;
    for (int i = 0; i < 10; ++i)
    {
        if (num == i) {
            move_pos = { -10.0 + 2 * (i + 1) - 0.5f, 0.2, -11.0 };
        }
    }

    for (int i = 10; i < 20; ++i)
    {
        if (num == i) {
            move_pos = { 11.0f, 0.2, -10.0 + 2 * ((i - 10) + 1) - 0.5 };
            rotateAngle[1] = 90;
        }
    }
    speed = urd(dre);
    ReadObj("Ghost.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}
ghostObject::~ghostObject()
{
}
modeObject::modeObject(int num) :GameObject()
{
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    m_modeN = num;
    scale[0] = 5.0f;
    scale[1] = 5.0f;
    scale[2] = 5.0f;
    if (num == 0) {
        move_pos = { -12.0f,-2.3f,-15.0f };
        m_textureNum = 7;
        rotateAngle[1] = -90.0f;
    }
    else if (num == 1) {
        move_pos = { -12.0f,-2.3f,-15.0f };
        m_textureNum = 8;
        rotateAngle[1] = -90.0f;
    }
    ReadObj("planeMode.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}

modeObject::~modeObject()
{
}

playerCheck::playerCheck(int id)
{
    color.r = 1.0f;
    color.g = 0.5f;
    color.b = 0.3f;
    move_pos = { -12.0f,0.2f,-9.5f }; //플레이어 초기위치
    scale[0] = 0.005;
    scale[1] = 0.005;
    scale[2] = 0.005;
    move_pos = { 0.0f,0.0f,0.0f };
    if(id == 1) m_textureNum = 9;  //P1일 경우
    if(id == 2) m_textureNum = 12; //P2일 경우
    ReadObj("Player_check.obj", m_vertex, m_normal, m_vt);
    SetVBO();
}

playerCheck::~playerCheck()
{
}
////////////////////////////////////////////////////////////////
//P2의 게임 오브젝트
OtherPlayer::OtherPlayer(int id)
{
    color.r = 1.0f;
    color.g = 0.5f;
    color.b = 0.3f;
    move_pos = { -12.0f,0.1f,-9.9f }; //플레이어 초기위치
    //move_pos = { -12.0f,0.2f,-10.728f };
    m_box.Center = XMFLOAT3(-12.0f, 0.1f, -9.9f);
	m_box.Extents = XMFLOAT3(0.05f, 0.05f, 0.05f);;
	if (id == 1) {
		ReadObj("Player2.obj", m_vertex, m_normal, m_vt);
	    m_textureNum = 11;
		scale[0] = 0.003;
		scale[1] = 0.003;
		scale[2] = 0.003;
	}
	if (id == 2) {
		ReadObj("Player1.obj", m_vertex, m_normal, m_vt);
	    m_textureNum = 10;
		scale[0] = 0.007;
		scale[1] = 0.007;
		scale[2] = 0.007;
	}
    SetVBO();
}

OtherPlayer::~OtherPlayer()
{
}
