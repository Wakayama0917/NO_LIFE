#pragma once
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
enum Scene_Transition {
    SCENE_ROGO,         //���S�V�[��
    SCENE_TITLE,        //�^�C�g���V�[��
    SCENE_STAGESELECT,  //�X�e�[�W�Z���N�g�V�[��
    SCENE_GAMESCENE,    //�Q�[���V�[��
    SCENE_RESULT,       //���U���g�V�[��
    SCENE_OPTION,       //�I�v�V�����A�|�[�Y�V�[��
    SCENE_END,
    RESET,              //���Z�b�g
};

class SupervisionScene
{
public:
    bool Get_Initialize_flag() { return this->Initialize_flag; }
    bool Get_Finish_flag() { return this->Finish_flag; }
    bool Get_Fade_flag() { return this->Fade_flag; }
    Scene_Transition Get_Next_scene() { return this->Next_scene; }
    void Set_Initialize_TRUE() { this->Initialize_flag = true; }
    void Set_Finish_FALSE() { this->Finish_flag = false; }
    void Set_Fade_flag(bool n) { this->Fade_flag = n; }
    int CheckFile(const char* path) { return PathFileExistsA(path); }
protected:
    bool Initialize_flag = true;
    bool Finish_flag;
    bool Fade_flag;
    Scene_Transition Next_scene;
};

