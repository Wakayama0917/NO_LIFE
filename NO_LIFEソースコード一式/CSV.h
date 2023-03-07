#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "EnumListEnemy.h"
#include "EnumListBullet.h"
#include "EnumListPlayer.h"
#include "EnumListSkill.h"
#include "EnumListStage.h"


//template <typename T>
class CSVFile {
private:
    std::string m_filepath;
    bool m_isHeader;
    bool m_isIndex;
    char m_delim;
    // �w�b�_�[���ۊ�
    std::vector<std::string> m_header;
    // �C���f�b�N�X���ۊ�
    std::vector<std::string> m_index;
    // ���f���t�@�C�����ۊ�
    std::vector<std::string> m_modelName;
    // �e�N�X�`���t�@�C���ۊ�
    std::vector<std::string> m_texfolder;
    // �v�f�ۊ�
    std::vector<std::vector<float>> m_cell;
   
public:
   
    
    // �R���X�g���N�^�֐�
    CSVFile() {
        m_isHeader = false;
        m_isIndex = false;
        m_delim = ',';
    }
    float CastCell(std::string _str);
    //�S�Đ����œǂݍ��܂���
    void CsvReadNum(std::string _filepath, bool _isHeader, bool _isIndex, char _delim);
    //���f�����ȂǕK�v�ȏ���string�Ŏ����Ă���
    void CsvRead(std::string _filepath, char _delim);
    void CsvWriteOLD(std::string _filepath, char _delim);
    void CsvWrite(std::string _filepath, char _delim);
    void CsvShow();

    /*************************************************************
    * GET�֐���
    *************************************************************/
    //�w�b�_�[��
    std::vector<std::string> GetHeader() {
        return m_header;
    }
    // �C���f�b�N�X��
    std::vector<std::string> GetIndex() {
    return m_index;
    }
    // ���f���t�@�C����
    std::vector<std::string> GetModelName() {
        return m_modelName;
    }
    // �e�N�X�`���t�@�C���ۊ�
    std::vector<std::string> GetTexFolder() {
        return m_texfolder;
    }
    //���l���i�[�����z��
    std::vector<std::vector<float>> GetCell() {
        return m_cell;
    }

    /*************************************************************
    * SET�֐���
    *************************************************************/
    //�Z���̗v�f���ύX�A��ɃG�f�B�^�[�p
    //�G�l�~�[�^
    void SetCell(ENEMY_TYPE _enemyType,ENEMY_STATUSID _enemyStatusID,float _value) {
        m_cell.at(static_cast<int>(_enemyType)).at(static_cast<int>(_enemyStatusID)) = _value;
    }
    //�e�^
    void SetCell(BULLET_TYPE _bulletType, BULLET_STATUSID _bulletStatusID, float _value) {
        m_cell.at(static_cast<int>(_bulletType)).at(static_cast<int>(_bulletStatusID)) = _value;
    }
    //�v���C���[�^
    void SetCell(PLAYER_STATUSID _playerStatusID, float _value) {
        m_cell.at(0).at(static_cast<int>(_playerStatusID)) = _value;
    }
    //�X�L���^
    void SetCell(SKILLTYPEID _skillType, SKILL_STATUSID _skillStatusID, float _value) {
        m_cell.at(static_cast<int>(_skillType)).at(static_cast<int>(_skillStatusID)) = _value;
    }
    //�X�e�[�W�^
    void SetCell(int _stageNum, STAGE_STATUSID _stageStatusID, float _value) {
        m_cell.at(_stageNum).at(static_cast<int>(_stageStatusID)) = _value;
    }

};



