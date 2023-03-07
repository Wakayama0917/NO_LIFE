#include "BulletObject.h"
#include "ModelMgr.h"

bool BulletObject::BulletCsvSet(CSVFile _csvFile)
{
    if (m_bulletType == BULLET_TYPE::END)
    {
        //初期化されていないのでメッセージを表示
        Texture tex;
        MessageBox(nullptr, "何の弾か設定されていません",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

        return false;
    }
    else
    {
        //共通変数初期化
        //GetCell().at(この敵の種類列).at(値)
        
        //共通変数初期化
        //GetCell().at(この敵の種類列).at(値)
        m_modelNum = static_cast<ALLMODEL>(_csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::MODEL_NUM)));//モデルナンバー
        m_attackSpeed = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED));//攻撃速度
        //m_reload = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::RELOAD));//次の弾を撃つまでの時間
        m_attackPower = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::ATTACK_POWER));//攻撃力
        m_liveTime = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::LIVE_TIME));//生存時間
        m_maxRot = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::MAX_ROT));//１フレーム内の最大回転角度
        m_expand = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::SIZE));//拡大率、GameObjectの中に入ってる
        m_radius = _csvFile.GetCell().at(static_cast<int>(m_bulletType)).at(static_cast<int>(BULLET_STATUSID::RADIUS));//半径、当たり判定用
    }

    return true;
}

void BulletObject::SetAllStatus(float _value[static_cast<int>(BULLET_STATUSID::END)])
{   
    //ステータス一括変更
    m_modelNum = static_cast<ALLMODEL>(_value[(static_cast<int>(BULLET_STATUSID::MODEL_NUM))]);//モデルナンバー
    m_attackSpeed = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED))];//攻撃速度
    //m_reload = _value[(static_cast<int>(BULLET_STATUSID::RELOAD))];//次の弾を撃つまでの時間
    m_attackPower = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_POWER))];//攻撃力
    m_liveTime = _value[(static_cast<int>(BULLET_STATUSID::LIVE_TIME))];//生存時間
    m_maxRot = _value[(static_cast<int>(BULLET_STATUSID::MAX_ROT))];//１フレーム内の最大回転角度
    m_expand = _value[(static_cast<int>(BULLET_STATUSID::SIZE))];//拡大率、GameObjectの中に入ってる
    m_radius = _value[(static_cast<int>(BULLET_STATUSID::RADIUS))];//半径、当たり判定用
}

void BulletObject::SetAllStatus(std::vector<float> _value)
{
    if (_value.size() == static_cast<int>(BULLET_STATUSID::END))
    {
        //ステータス一括変更
        m_modelNum = static_cast<ALLMODEL>(_value[(static_cast<int>(BULLET_STATUSID::MODEL_NUM))]);//モデルナンバー
        m_attackSpeed = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_SPEED))];//攻撃速度
        //m_reload = _value[(static_cast<int>(BULLET_STATUSID::RELOAD))];//次の弾を撃つまでの時間
        m_attackPower = _value[(static_cast<int>(BULLET_STATUSID::ATTACK_POWER))];//攻撃力
        m_liveTime = _value[(static_cast<int>(BULLET_STATUSID::LIVE_TIME))];//生存時間
        m_maxRot = _value[(static_cast<int>(BULLET_STATUSID::MAX_ROT))];//１フレーム内の最大回転角度
        m_expand = _value[(static_cast<int>(BULLET_STATUSID::SIZE))];//拡大率、GameObjectの中に入ってる
        m_radius = _value[(static_cast<int>(BULLET_STATUSID::RADIUS))];//半径、当たり判定用
    }
    else
    {
        //要素数がおかしい
        Texture tex;
        MessageBox(nullptr, "弾の要素数がおかしいです",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;

    }
   

}

void BulletObject::DirectionSet(XMFLOAT4X4 _mtx,float _angle)
{
    XMFLOAT4 axisY;
    XMFLOAT4 axisZ;
    XMFLOAT4X4 mtx = _mtx;

    //Y軸を取り出す
    axisY.x = mtx._21;
    axisY.y = mtx._22;
    axisY.z = mtx._23;
    axisY.w = 0.0f;

    XMFLOAT4 qt, qty;//クォータニオン

    //行列からクオータニオンを生成
    DX11GetQtfromMatrix(mtx, qt);

    DX11QtRotationAxis(qty, axisY, _angle);

    //クオータニオン合成
    XMFLOAT4 tempqt1;
    DX11QtMul(tempqt1, qt, qty);

    //クオータニオンをノーマライズ
    DX11QtNormalize(tempqt1, tempqt1);

    //クオータニオンから行列を作成
    DX11MtxFromQt(mtx, tempqt1);

    //Z軸を取り出す
    axisZ.x = mtx._31;
    axisZ.y = mtx._32;
    axisZ.z = mtx._33;
    axisZ.w = 0.0f;

    m_direction = DirectX::XMFLOAT3(axisZ.x, axisZ.y, axisZ.z);//設定
}

float BulletObject::GetCSVStatus(BULLET_STATUSID _statusid)
{
    switch (_statusid)
    {
    case BULLET_STATUSID::ATTACK_SPEED: {
        return m_attackSpeed;
    }
        break;
    case BULLET_STATUSID::ATTACK_POWER: {
        return m_attackPower;
    }
        break;
    case BULLET_STATUSID::LIVE_TIME: {
        return m_liveTime;
    }
        break;
    case BULLET_STATUSID::MAX_ROT: {
        return m_maxRot;
    }
        break;
    case BULLET_STATUSID::SIZE: {
        return m_expand;
    }
        break;
    case BULLET_STATUSID::RADIUS: {
        return m_radius;
    }
        break;
    case BULLET_STATUSID::END: {
        // 保険
        Texture tex;
        MessageBox(nullptr, "エネミーオブジェクト：無効な数値が入っています",
            "Error!", MB_ICONERROR | MB_OK);
        tex.texture = nullptr;
        return 0.0f;
    }
                             break;
    default:
        break;
    }
    //保険
    Texture tex;
    MessageBox(nullptr, "エネミーオブジェクト：無効な数値が入っています",
        "Error!", MB_ICONERROR | MB_OK);
    tex.texture = nullptr;
    return 0.0f;
}

void BulletObject::Draw()
{
    //モデル描画
    if (m_state == STATE::LIVE) {
        //大きさを変えるための処理
        DirectX::XMFLOAT4X4 big;
        //DX11MtxScale(m_expand, m_expand, m_expand, big);//大きくするための4x4を作製
        DX11MtxMultiply(big, m_bigMtx, m_mtx);//先頭に後ろ2つをかけたものを入れる、現在位置

        //描画
        ModelMgr::GetInstance().Draw(m_modelNum, big);
        
    }
}
