#pragma once
#include"stdafx.h"
#include "Framework.h"
#define STB_IMAGE_IMPLEMENTATION
#include"LoadImage.h"
#include"Player.h"
#include "network.h"

//C:\Users\User\Desktop\NGP_project\NGP_TermProject\CG_Term\x64\Release

Framework* Framework::instance = nullptr;
Framework::Framework()
{
}

Framework::~Framework()
{
    delete instance;
    instance = nullptr;
}

Framework& Framework::getInstance()
{
    if (instance == nullptr) {
        instance = new Framework();
    }
    return *instance;
}

void Framework::make_vertexShaders()
{
    GLchar* vertexSource;
    vertexSource = filetobuf("vertex_light.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {//result == 0�̸� ������ �ִ�.
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
        return;
    }
}

void Framework::make_fragmentShaders()
{
    GLchar* fragmentSource;
    fragmentSource = filetobuf("fragment_light.glsl");
    int fIdx = 0;
    while (fragmentSource[fIdx] != '\0') {
        if (fragmentSource[fIdx] == '#') {
            fIdx++;
            break;
        }
        fIdx++;
    }
    const char* temp;
    temp = &fragmentSource[fIdx - 1];
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &temp, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        cerr << "ERROR: frag_shader ������ ����\n" << errorLog << endl;
        return;
    }
}

GLvoid Framework::drawScene(GLvoid)
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(instance->shaderProgramID);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    ShowCursor(FALSE);
    instance->light.lightpos = instance->camerapos;

    glViewport(0, 0, instance->g_width, instance->g_height);
    if (instance->Mode == 0) {
        instance->camera.rotate_camera();
        instance->camera.Draw(instance->camerapos, instance->shaderProgramID, perspective_projection);
        instance->m_ppObject[10]->draw(); //mode 0
        instance->light.draw(instance->camerapos, instance->shaderProgramID, FALSE);

        glClear(GL_DEPTH_BUFFER_BIT);
        //glViewport(g_width * 4 / 5, g_height * 4 / 5, g_width / 5, g_height / 5);
    }      //����ȭ��
    else if (instance->Mode == 1) {         //�ΰ��� ȭ��
        instance->light.draw(instance->camerapos, instance->shaderProgramID, instance->light_button);
        instance->camera.rotate_camera();
        instance->camera.InGameDraw(instance->camerapos, instance->shaderProgramID, perspective_projection);
        instance->maze[0].Draw(instance->D_vertex, instance->D_normal, instance->D_vt, instance->shaderProgramID);
        instance->m_ppObject[0]->draw();
        instance->m_ppObject[3]->draw();
        instance->m_ppObject[0]->draw();
        instance->m_ppObject[1]->draw();
        instance->m_ppObject[53]->draw();//playercheck
        instance->m_ppObject[54]->draw();//player
        instance->m_ppObject[55]->draw();//player2 (��������)

        //���� �ٸ� Ŭ�� �����ߴٸ� Player2�� �׷��ش�.
        if (instance->sock_check) instance->m_ppObject[55]->draw();//player2

        for (int i = 3; i < 7; i++)//door
        {
            instance->m_ppObject[i]->draw();
        }
        instance->maze[0].Draw(instance->D_vertex, instance->D_normal, instance->D_vt, instance->shaderProgramID);

        for (int i = 0; i < 20; i++)//������
        {
            if (instance->m_ppObject[i + 32]->exist)
                instance->m_ppObject[i + 32]->draw();
        }
        for (int i = 0; i < 3; ++i) {//Ű
            if (instance->m_ppObject[i + 7]->exist)
                instance->m_ppObject[i + 7]->draw();
        }
        if (instance->hintIndex != -1)
        {
            if (instance->m_ppObject[instance->hintIndex]->item_navi == 1) { //��ã�� ��Ʈ
                instance->m_ppObject[52]->draw();
            }
        }

        for (int i = 0; i < 20; ++i) {//�ͽ�
            instance->m_ppObject[i + 12]->draw();
        }

        if (instance->howManyKey == 3 && instance->camerapos.x < -13.0f) {
            instance->camerapos.x = -12.0f;
            instance->camerapos.y = 0.2f;
            instance->camerapos.z = -9.5f;

            instance->camera.cameraAt = { 0,0,-1 };
            instance->Mode = 3;
            glutTimerFunc(17, Timer, 5);
        }

        instance->sphere[0].move_pos = { instance->camerapos.x,instance->camerapos.y + 4.0f,instance->camerapos.z };
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(instance->g_width * 4 / 5, instance->g_height * 4 / 5, instance->g_width / 5, instance->g_height / 5);
        //==================================================//�̴ϸ� �κ�

        if (instance->button_m) {
            instance->light_minimap.draw(instance->camerapos, instance->shaderProgramID, FALSE);
            vec3 temp = { instance->camerapos.x,instance->camerapos.y + 3.0f,instance->camerapos.z };
            instance->minimap_camera.cameraEye = temp;
            instance->minimap_camera.cameraUp = { instance->camera.cameraAt.x,instance->camera.cameraAt.y + 3.0f,instance->camera.cameraAt.z };
            instance->minimap_camera.Draw(temp, instance->shaderProgramID, perspective_projection);

        }
        else
        {
            instance->light_minimap.draw(instance->camerapos, instance->shaderProgramID, FALSE);
            vec3 temp1 = { 0,25,0 };
            instance->minimap_camera.cameraEye = temp1;
            instance->minimap_camera.cameraAt = { 0,-1,0 };
            instance->minimap_camera.cameraUp = { 1,0,0 };
            instance->minimap_camera.Draw(temp1, instance->shaderProgramID, orthogonal_projection);
        }
        instance->maze[0].Draw(instance->D_vertex, instance->D_normal, instance->D_vt, instance->shaderProgramID);
        if (instance->hintIndex != -1)
        {
            if (instance->m_ppObject[instance->hintIndex]->item_navi == 1) { //��ã�� ��Ʈ
                instance->m_ppObject[52]->draw();
            }

            if (instance->m_ppObject[instance->hintIndex]->item_where == 1) {
                for (int i = 0; i < 20; ++i) {
                    if (instance->m_ppObject[i + 32]->exist) {
                        instance->m_ppObject[i + 32]->scale[0] = 3.0f;
                        instance->m_ppObject[i + 32]->scale[1] = 3.0f;
                        instance->m_ppObject[i + 32]->scale[2] = 3.0f;
                        instance->m_ppObject[i + 32]->draw();
                    }
                }
            }

            if (instance->m_ppObject[instance->hintIndex]->item_Key_where == 1) {
                for (int i = 0; i < 3; ++i) {
                    if (instance->m_ppObject[i + 7]->exist) {
                        instance->m_ppObject[i + 7]->scale[0] = 0.3f;
                        instance->m_ppObject[i + 7]->scale[1] = 0.3f;
                        instance->m_ppObject[i + 7]->scale[2] = 0.3f;
                        instance->m_ppObject[i + 7]->draw();
                    }
                }
            }
        }
        for (int i = 0; i < 20; ++i) {
            instance->m_ppObject[i + 32]->scale[0] = 1.0f;
            instance->m_ppObject[i + 32]->scale[1] = 1.0f;
            instance->m_ppObject[i + 32]->scale[2] = 1.0f;
        }
        for (int i = 0; i < 3; ++i) {
            instance->m_ppObject[i + 7]->scale[0] = 0.05f;
            instance->m_ppObject[i + 7]->scale[1] = 0.05f;
            instance->m_ppObject[i + 7]->scale[2] = 0.05f;
        }
        for (int i = 0; i < 20; ++i) {
            instance->m_ppObject[i + 12]->scale[0] = 0.1f;
            instance->m_ppObject[i + 12]->scale[1] = 0.1f;
            instance->m_ppObject[i + 12]->scale[2] = 0.1f;
        }
        instance->sphere[0].Draw(instance->S_vertex, instance->S_normal, instance->S_vt, instance->shaderProgramID);
        instance->m_ppObject[0]->draw();
        instance->maze[0].Draw(instance->D_vertex, instance->D_normal, instance->D_vt, instance->shaderProgramID);
        for (int i = 0; i < 20; ++i) {
            instance->m_ppObject[i + 12]->draw();
        }

        //======================================================//
    }
    else if (instance->Mode == 3) {
        //camera.rotate_camera();
        instance->camera.cameraAt = { 0.0,0.0,-1.0f };
        instance->camera.Draw(instance->camerapos, instance->shaderProgramID, perspective_projection);
        instance->m_ppObject[11]->draw();//���1

        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(instance->g_width * 4 / 5, instance->g_height * 4 / 5, instance->g_width / 5, instance->g_height / 5);
    }


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glutSwapBuffers(); //ȭ�鿡 ���
		for (int i = 3; i < 7; i++)//door
}

//GLvoid Framework::Reshape(int w, int h)
//{
//   return GLvoid();
//}

GLvoid Framework::KeyBoardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'p':
        instance->creativemode = !instance->creativemode;
        instance->light_button = !instance->light_button;
        if (instance->creativemode)
        {
            instance->CMPOS = { instance->camerapos.x,instance->camerapos.y,instance->camerapos.z };
        }
        else
            instance->camerapos = instance->CMPOS;
        break;
    case 'd':
        if (instance->Mode == 1)
            instance->camera.direction = Right;
        break;
    case 'a':
        if (instance->Mode == 1)
            instance->camera.direction = Left;
        break;
    case 'w':
        if (instance->Mode == 1)
        {
            instance->camera.direction = Up;
        }


        break;
    case 's':
        if (instance->Mode == 1)
            instance->camera.direction = Down;
        break;
    case'm':
        instance->button_m = !instance->button_m;
        break;
    case 'y':
        instance->camerapos.y += 0.1f;
        break;
    case'Y':
        instance->camerapos.y -= 0.1f;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    case 32:
        if (instance->Mode == 0)
            PlaySound(NULL, NULL, 0);
        instance->Mode = 1;
        break;
    case '+':
        instance->ũ������Ƽ���� = true;
        instance->hintIndex = 51;

        break;
    case '-':
        instance->ũ������Ƽ���� = false;
        instance->hintIndex = -1;

        break;
    }

    if (instance->door_collision())
        cout << "ī�޶�x��ǥ:" << instance->camerapos.x << "y��ǥ:" << instance->camerapos.y << "z��ǥ:" << instance->camerapos.z << endl;
    else
    {

    }
    /*if (AABBcollision()) {
       printf("�浹�߽��ϴ�!!\n");
    }
    else
       printf("�浹 ����\n");*/
    glutPostRedisplay();
}

GLvoid Framework::Motion(int x, int y)
{
    if (instance->Mode > 0) {
        instance->camera.Move(x, y);
        glutWarpPointer(instance->g_width / 2, instance->g_height / 2);
    }
    glutPostRedisplay();
}

GLuint Framework::make_shaderProgram()
{
    GLuint shaderID;
    shaderID = glCreateProgram(); //--- ���̴� ���α׷� ����� - �� ���̴� �پ�ߵ�, vertex - fragment�� ¦�� �¾ƾߵ�
    glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
    glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
    glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�
    glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
        return false;
    }
    glUseProgram(shaderID);
    return shaderID;
}

GLvoid Framework::Timer(int value)
{
    switch (value) {
    case Live:
        instance->objectcollision();
        glutPostRedisplay();
        glutTimerFunc(17, Timer, 0);
        break;

    case CamMove:

        for (int i = 0; i < 10; i++)
        {
            instance->m_ppObject[i + 12]->Move(1);
        }

        for (int i = 10; i < 20; ++i)
        {
            instance->m_ppObject[i + 12]->Move(2);
        }

        switch (instance->camera.direction)
        {
        case Up:
            instance->camerapos.x += instance->camera.speed * instance->camera.cameraAt.x;
            if (instance->creativemode)
            {
                instance->camerapos.y += instance->camera.speed * instance->camera.cameraAt.y;
            }
            instance->camerapos.z += instance->camera.speed * instance->camera.cameraAt.z;
            if ((instance->maze_collision() || instance->door_collision()) || instance->player_collision() && !instance->creativemode)
            {
                PlaySound(TEXT("collision_sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                instance->camerapos.x -= 5 * instance->camera.speed * instance->camera.cameraAt.x;
                instance->camerapos.z -= 5 * instance->camera.speed * instance->camera.cameraAt.z;
            }
            break;
        case Down:
            instance->camerapos.x -= instance->camera.speed * instance->camera.cameraAt.x;
            if (instance->creativemode)
            {
                instance->camerapos.y -= instance->camera.speed * instance->camera.cameraAt.y;
            }
            instance->camerapos.z -= instance->camera.speed * instance->camera.cameraAt.z;
            if ((instance->maze_collision() || instance->door_collision()) || instance->player_collision() && !instance->creativemode)
            {
                PlaySound(TEXT("collision_sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                instance->camerapos.x += 5 * instance->camera.speed * instance->camera.cameraAt.x;
                instance->camerapos.z += 5 * instance->camera.speed * instance->camera.cameraAt.z;
            }
            break;

        case Left:
            instance->camerapos.x += instance->camera.speed * instance->camera.camerleft.x;
            if (instance->creativemode)
            {
                instance->camerapos.y += instance->camera.speed * instance->camera.camerleft.y;
            }
            instance->camerapos.z += instance->camera.speed * instance->camera.camerleft.z;
            if ((instance->maze_collision() || instance->door_collision()) || instance->player_collision() && !instance->creativemode)
            {
                PlaySound(TEXT("collision_sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                instance->camerapos.x += 5 * instance->camera.speed * instance->camera.camerright.x;
                instance->camerapos.z += 5 * instance->camera.speed * instance->camera.camerright.z;
            }
            break;

        case Right:
            instance->camerapos.x += instance->camera.speed * instance->camera.camerright.x;
            if (instance->creativemode)
            {

                instance->camerapos.y += instance->camera.speed * instance->camera.camerright.y;
            }
            instance->camerapos.z += instance->camera.speed * instance->camera.camerright.z;
            if ((instance->maze_collision() || instance->door_collision()) || instance->player_collision() && !instance->creativemode)
            {
                instance->camerapos.x += 5 * instance->camera.speed * instance->camera.camerleft.x;
                instance->camerapos.z += 5 * instance->camera.speed * instance->camera.camerleft.z;
            }

            break;
        default:
            break;
        }
		
        //�浹�ڽ� �Ű��ֱ�
        XMFLOAT3 center = { instance->camerapos.x,instance->camerapos.y, instance->camerapos.z };
        instance->m_ppObject[54]->m_box.Center = center;

        { //�÷��̾� ��ǥ send
			PlayerCoordPacket* packet = new PlayerCoordPacket;
			packet->x =instance->camerapos.x;
			packet->y =instance->camerapos.y;
			packet->z =instance->camerapos.z;
			packet->cameraAt = instance->camera.cameraAt; //�÷��̾ �ٶ󺸴� At���� ����
			instance->network.SendPacket(reinterpret_cast<char*>(packet), sizeof(PlayerCoordPacket));
		}
        glutTimerFunc(17, Timer, 1);
        break;
    case 2:         // ������ ȸ��
        for (int i = 0; i < 20; ++i) {
            instance->m_ppObject[i + 32]->rotateAngle[1] += 2.0f;
        }
        for (int i = 0; i < 3; ++i) {
            instance->m_ppObject[i + 7]->rotateAngle[1] += 2.0f;
        }
        glutTimerFunc(17, Timer, 2);
        break;
    case 3:         // ������ �� �Ʒ�
        for (int i = 0; i < 20; ++i) {
            if (instance->m_ppObject[i + 32]->m_direction) {
                instance->m_ppObject[i + 32]->move_pos.y += 0.001f;
                instance->m_ppObject[i + 32]->m_direcN++;
                if (instance->m_ppObject[i + 32]->m_direcN == 20) {
                    instance->m_ppObject[i + 32]->m_direction = false;
                }
            }
            else if (!instance->m_ppObject[i + 32]->m_direction) {
                instance->m_ppObject[i + 32]->move_pos.y -= 0.001f;
                instance->m_ppObject[i + 32]->m_direcN--;
                if (instance->m_ppObject[i + 32]->m_direcN == 0) {
                    instance->m_ppObject[i + 32]->m_direction = true;
                }
            }
        }
        for (int i = 0; i < 3; ++i) {
            if (instance->m_ppObject[i + 7]->m_direction) {
                instance->m_ppObject[i + 7]->move_pos.y += 0.001f;
                instance->m_ppObject[i + 7]->m_direcN++;
                if (instance->m_ppObject[i + 7]->m_direcN == 20) {
                    instance->m_ppObject[i + 7]->m_direction = false;
                }
            }
            else if (!instance->m_ppObject[i + 32]->m_direction) {
                instance->m_ppObject[i + 7]->move_pos.y -= 0.001f;
                instance->m_ppObject[i + 7]->m_direcN--;
                if (instance->m_ppObject[i + 7]->m_direcN == 0) {
                    instance->m_ppObject[i + 7]->m_direction = true;
                }
            }
        }
        glutTimerFunc(34, Timer, 3);
        break;
    case 4:         // �� ȸ��
        if (instance->camerapos.x >= -11.0f && instance->camerapos.x <= -9.0f) {
            for (int i = 0; i < 2; ++i) {
                if (!instance->m_ppObject[i + 3]->m_IsOpen) {
                    if (i % 2 == 0) {
                        instance->m_ppObject[i + 3]->rotateAngle[1] += -1.0f;
                        instance->m_ppObject[i + 3]->m_OpenN++;
                        if (instance->m_ppObject[i + 3]->m_OpenN == 89) {
                            instance->m_ppObject[i + 3]->m_IsOpen = 1;
                        }
                    }
                    else if (i % 2 == 1) {
                        instance->m_ppObject[i + 3]->rotateAngle[1] += 1.0f;
                        instance->m_ppObject[i + 3]->m_OpenN++;
                        if (instance->m_ppObject[i + 3]->m_OpenN == 89) {
                            instance->m_ppObject[i + 3]->m_IsOpen = 1;
                        }
                    }

                }
            }
        }
        if (instance->camerapos.x >= -10.0f && instance->camerapos.x <= -9.0f && instance->camerapos.z >= -7.0f && instance->camerapos.z <= -6.0f) {
            if (instance->howManyKey == 3) {
                for (int i = 2; i < 4; ++i) {
                    if (!instance->m_ppObject[i + 3]->m_IsOpen) {
                        if (i % 2 == 0) {
                            instance->m_ppObject[i + 3]->rotateAngle[1] += 1.0f;
                            instance->m_ppObject[i + 3]->m_OpenN++;
                            if (instance->m_ppObject[i + 3]->m_OpenN == 89) {
                                instance->m_ppObject[i + 3]->m_IsOpen = 1;
                            }
                        }
                        else if (i % 2 == 1) {
                            instance->m_ppObject[i + 3]->rotateAngle[1] += -1.0f;
                            instance->m_ppObject[i + 3]->m_OpenN++;
                            if (instance->m_ppObject[i + 3]->m_OpenN == 89) {
                                instance->m_ppObject[i + 3]->m_IsOpen = 1;
                            }
                        }

                    }
                }
            }
        }
        glutTimerFunc(34, Timer, 4);
        break;
    case 5:
        switch (instance->Mode)
        {
        case 0:
            PlaySound(TEXT("startsound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            break;
        case 3:

            PlaySound(TEXT("stageclear.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            break;
        default:
            break;
        }


        break;

    }
}

GLvoid Framework::KeyBoardUpFunc(unsigned char key, int x, int y)
{
    instance->camera.direction = Stop;
    instance->m_ppObject[54]->direction = Stop;
}

void Framework::BuildObjects()
{
    ReadObj("Maze_tri.obj", D_vertex, D_normal, D_vt);
    ReadObj("sphere.obj", S_vertex, S_normal, S_vt);
    maze.emplace_back(500, 500);
    maze[0].setFramework(this);
    sphere.emplace_back(500, 500);
    sphere[0].scale = 0.05;
    sphere[0].setFramework(this);
    for (int i = 0; i < S_vertex.size(); ++i)
    {
        S_vertex[i].y += 0.5f;
    }
    m_nGameObjects = 56;
    m_ppObject = new GameObject * [m_nGameObjects];
    int nObjects = 0;
    //0~1 : plane   
    //2   : bottom 
    //3~6 : door 
    //7~9 : key 
    //10~11: mode
    //12~31: ghost
    //32~51: item
    //52: hint
    //53 : playercheck
    //54 : player
    //55 : player2
    m_ppObject[nObjects++] = new planeObject(0);
    m_ppObject[nObjects++] = new planeObject(1);
    m_ppObject[nObjects++] = new bottomObject();
    m_ppObject[nObjects++] = new doorObject(1);
    m_ppObject[nObjects++] = new doorObject(2);
    m_ppObject[nObjects++] = new doorObject(3);
    m_ppObject[nObjects++] = new doorObject(4);
    m_ppObject[nObjects++] = new keyObject(0);
    m_ppObject[nObjects++] = new keyObject(1);
    m_ppObject[nObjects++] = new keyObject(2);
    m_ppObject[nObjects++] = new modeObject(0);
    m_ppObject[nObjects++] = new modeObject(1);
    for (int i = 0; i < 20; ++i) {
        m_ppObject[nObjects++] = new ghostObject(i);
    }
    for (int i = 0; i < 20; ++i) {
        m_ppObject[nObjects++] = new itemObject(i);
    }
    m_ppObject[nObjects++] = new hintObject();
    m_ppObject[nObjects++] = new playerCheck();
    m_ppObject[nObjects++] = new Player();
    m_ppObject[nObjects++] = new OtherPlayer();
    for (int i = 0; i < nObjects; i++)
    {
        m_ppObject[i]->setFramework(this);
    }
    int width = 1024;
    int height = 1024;
    int numberOfChannel = 0;
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(12, texture);                                                                   //--- �ؽ�ó ����
    //���� �ؽ���
    glBindTexture(GL_TEXTURE_2D, texture[0]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                        //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData1 = stbi_load("redBlock.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData1);              //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData1);

    //���� �ڽ� �ؽ���
    glBindTexture(GL_TEXTURE_2D, texture[1]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                        //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData2 = stbi_load("RandomBox.jpg", &width, &height, &numberOfChannel, 4);               //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData2);             //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData2);

    glBindTexture(GL_TEXTURE_2D, texture[2]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData3 = stbi_load("wall.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData3);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData3);

    glBindTexture(GL_TEXTURE_2D, texture[3]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData4 = stbi_load("sea.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData4);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData4);

    glBindTexture(GL_TEXTURE_2D, texture[4]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData5 = stbi_load("door.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData5);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData5);

    glBindTexture(GL_TEXTURE_2D, texture[5]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData6 = stbi_load("key.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData6);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData6);

    glBindTexture(GL_TEXTURE_2D, texture[6]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData7 = stbi_load("ghost.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData7);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData7);

    glBindTexture(GL_TEXTURE_2D, texture[7]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData8 = stbi_load("start.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData8);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData8);

    glBindTexture(GL_TEXTURE_2D, texture[8]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData9 = stbi_load("end.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData9);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData9);

    //P1�� check �ؽ�ó
    glBindTexture(GL_TEXTURE_2D, texture[9]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData10 = stbi_load("P1_check.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData10);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData10);

    //P1�� �ؽ�ó (��Ÿ)
    glBindTexture(GL_TEXTURE_2D, texture[10]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData11 = stbi_load("Player1.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData11);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData11);

    //P2�� �ؽ�ó (��������)
    glBindTexture(GL_TEXTURE_2D, texture[11]);                                                       //--- �ؽ�ó ���ε�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texutreData12 = stbi_load("Player2.jpg", &width, &height, &numberOfChannel, 4);                  //�� ������ ���ڰ� �ʺ� 4�ȼ��� �ǰ� �ϴ� ����
    //jpg�ϱ� GL_RGBA�� �ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData12);            //---�ؽ�ó �̹��� ����
    stbi_image_free(texutreData12);

    maze[0].makeBB(D_vertex);
}

void Framework::objectcollision()
{
    for (int i = 0; i < 20; ++i) {
        if (pow((m_ppObject[i + 32]->move_pos.x - camerapos.x), 2) + pow((m_ppObject[i + 32]->move_pos.z - camerapos.z), 2) + pow((m_ppObject[i + 32]->move_pos.y - camerapos.y), 2) < 0.04 && m_ppObject[i + 32]->exist == true && ũ������Ƽ���� == false) {   //������ �Ա�
            m_ppObject[i + 32]->exist = false;
            PlaySound(TEXT("itemsound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            if (m_ppObject[i + 32]->m_ability == 3)//��������
            {
                camerapos.x = -12.0f;
                camerapos.z = -9.5f;
            }
            else
            {
                m_ppObject[i + 32]->ability();
                hintIndex = i + 32;
            }
        }
        else if (ũ������Ƽ���� == true) {
            m_ppObject[51]->ability();
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (pow((m_ppObject[i + 7]->move_pos.x - camerapos.x), 2) + pow((m_ppObject[i + 7]->move_pos.z - camerapos.z), 2) + pow((m_ppObject[i + 7]->move_pos.y - camerapos.y), 2) < 0.04 && m_ppObject[i + 7]->exist == true) {   //������ �Ա�
            m_ppObject[i + 7]->exist = false;
            PlaySound(TEXT("keysound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            howManyKey++;
        }
    }
    if (!creativemode) {
        for (int i = 0; i < 20; ++i) {
            if (pow((m_ppObject[i + 12]->move_pos.x - camerapos.x), 2) + pow((m_ppObject[i + 12]->move_pos.z - camerapos.z), 2) + pow((m_ppObject[i + 12]->move_pos.y - camerapos.y), 2) < 0.01) {   //������ �Ա�
                camerapos = { -12.0f,0.2f,-9.5f };

            }
        }
    }
}

bool Framework::maze_collision()
{
    int i = 0;
    for (auto a : maze[0].bounding_box)
    {
        if ((a.Min_coord.x - 0.08 < camerapos.x && camerapos.x < a.Max_coord.x + 0.08) && (a.Min_coord.y - 0.1 < camerapos.y && camerapos.y < a.Max_coord.y + 0.1)
            && (a.Min_coord.z - 0.08 < camerapos.z && camerapos.z < a.Max_coord.z + 0.08))
        {
            cout << "�ε��� ��ȣ:" << i << endl;
            cout << "��� X��ǥ:" << a.Min_coord.x << "     " << a.Max_coord.x << endl;
            cout << "��� Y��ǥ:" << a.Min_coord.y << "     " << a.Max_coord.y << endl;
            cout << "��� Z��ǥ:" << a.Min_coord.z << "     " << a.Max_coord.z << endl;
            cout << "�浹��" << endl;
            return TRUE;
        }
        else
        {
            i++;
            //cout << "�浹����" << endl;
            //return FALSE;
        }
    }
    if (i == maze[0].bounding_box.size())
        return FALSE;
}

bool Framework::door_collision()
{
    int num = 0;
    for (int i = 0; i < 4; ++i) {
        glm::vec4 doorMin4 = glm::vec4(vec3(-0.15, 0, -1), 1.0f); // 4���� ���ͷ� ��ȯ
        glm::vec4 doorMax4 = glm::vec4(vec3(0.15, 1, 0), 1.0f);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_ppObject[i + 3]->rotateAngle[1]), AXIS_Y); // ȸ�� ��ȯ ��� ����
        glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_ppObject[i + 3]->move_pos); // ȸ�� ��ȯ ��� ����
        doorMin4 = transMatrix * rotationMatrix * doorMin4;
        doorMax4 = transMatrix * rotationMatrix * doorMax4;

        glm::vec3 rotatedDoorMinPoint = glm::vec3(doorMin4); // �ٽ� 3���� ���ͷ� ��ȯ
        glm::vec3 rotatedDoorMaxPoint = glm::vec3(doorMax4);
        if (rotatedDoorMaxPoint.x < rotatedDoorMinPoint.x)
            swap(rotatedDoorMaxPoint.x, rotatedDoorMinPoint.x);

        if (rotatedDoorMaxPoint.y < rotatedDoorMinPoint.y)
            swap(rotatedDoorMaxPoint.y, rotatedDoorMinPoint.y);

        if (rotatedDoorMaxPoint.z < rotatedDoorMinPoint.z)
            swap(rotatedDoorMaxPoint.z, rotatedDoorMinPoint.z);


        // ī�޶�� ȸ���� ����� ��� ���� �� �浹 �˻�
        if (rotatedDoorMinPoint.x <= camerapos.x && rotatedDoorMaxPoint.x >= camerapos.x &&
            rotatedDoorMinPoint.y <= camerapos.y && rotatedDoorMaxPoint.y >= camerapos.y &&
            rotatedDoorMinPoint.z <= camerapos.z && rotatedDoorMaxPoint.z >= camerapos.z)
        {
            return TRUE;
        }
        else
            ++num;
    }
    if (num == 4)
    {
        return FALSE;
    }


}

bool Framework::player_collision()
{
    if (m_ppObject[54]->m_box.Intersects(m_ppObject[55]->m_box))
        return TRUE;
    return false;
}
