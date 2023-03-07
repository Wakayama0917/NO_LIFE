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
    // ヘッダー名保管
    std::vector<std::string> m_header;
    // インデックス名保管
    std::vector<std::string> m_index;
    // モデルファイル名保管
    std::vector<std::string> m_modelName;
    // テクスチャファイル保管
    std::vector<std::string> m_texfolder;
    // 要素保管
    std::vector<std::vector<float>> m_cell;
   
public:
   
    
    // コンストラクタ関数
    CSVFile() {
        m_isHeader = false;
        m_isIndex = false;
        m_delim = ',';
    }
    float CastCell(std::string _str);
    //全て数字で読み込ませる
    void CsvReadNum(std::string _filepath, bool _isHeader, bool _isIndex, char _delim);
    //モデル名など必要な所をstringで持ってくる
    void CsvRead(std::string _filepath, char _delim);
    void CsvWriteOLD(std::string _filepath, char _delim);
    void CsvWrite(std::string _filepath, char _delim);
    void CsvShow();

    /*************************************************************
    * GET関数類
    *************************************************************/
    //ヘッダー名
    std::vector<std::string> GetHeader() {
        return m_header;
    }
    // インデックス名
    std::vector<std::string> GetIndex() {
    return m_index;
    }
    // モデルファイル名
    std::vector<std::string> GetModelName() {
        return m_modelName;
    }
    // テクスチャファイル保管
    std::vector<std::string> GetTexFolder() {
        return m_texfolder;
    }
    //数値を格納した配列
    std::vector<std::vector<float>> GetCell() {
        return m_cell;
    }

    /*************************************************************
    * SET関数類
    *************************************************************/
    //セルの要素一つを変更、主にエディター用
    //エネミー型
    void SetCell(ENEMY_TYPE _enemyType,ENEMY_STATUSID _enemyStatusID,float _value) {
        m_cell.at(static_cast<int>(_enemyType)).at(static_cast<int>(_enemyStatusID)) = _value;
    }
    //弾型
    void SetCell(BULLET_TYPE _bulletType, BULLET_STATUSID _bulletStatusID, float _value) {
        m_cell.at(static_cast<int>(_bulletType)).at(static_cast<int>(_bulletStatusID)) = _value;
    }
    //プレイヤー型
    void SetCell(PLAYER_STATUSID _playerStatusID, float _value) {
        m_cell.at(0).at(static_cast<int>(_playerStatusID)) = _value;
    }
    //スキル型
    void SetCell(SKILLTYPEID _skillType, SKILL_STATUSID _skillStatusID, float _value) {
        m_cell.at(static_cast<int>(_skillType)).at(static_cast<int>(_skillStatusID)) = _value;
    }
    //ステージ型
    void SetCell(int _stageNum, STAGE_STATUSID _stageStatusID, float _value) {
        m_cell.at(_stageNum).at(static_cast<int>(_stageStatusID)) = _value;
    }

};



