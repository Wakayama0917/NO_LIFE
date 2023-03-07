
#include "PXinput.h"
//#include <cmath>
//#include <limits>
//#include <iostream>
//#include	"CDirectInput.h"
#include <iostream>



//接続されているか調べる、特殊な型試験用
HRESULT PXinput::UpdateControllerState()
{
    DWORD dwResult;
    for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
    {
        dwResult = XInputGetState(i, &m_padNum[i].state);

        if (dwResult == ERROR_SUCCESS)
            m_padNum[i].bConnected = true;
        else
            m_padNum[i].bConnected = false;
    }

    return S_OK;
}


void PXinput::idle()
{
    UpdateControllerState();

    //変数
   

    for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
    //for (int i = 0; i < MAX_CONTROLLERS; i++) {
        if (m_padNum[i].bConnected) {//接続されていれば
           DWORD Buttons = m_padNum[i].state.Gamepad.wButtons;

           //ゲームパッドのAまたはBを押した時の処理：ゲームパッドの振動
           XINPUT_VIBRATION vibration;

            //左スティック
            if ((m_padNum[i].state.Gamepad.sThumbLY > Threshold)
                || (m_padNum[i].state.Gamepad.sThumbLY < -Threshold))
            {
                //65535/2のプラスとマイナス
                m_xkeySts.LY = (double)m_padNum[i].state.Gamepad.sThumbLY / ((double)65535 / 2);
                //XKEY_STATE.LY = GAME_PAD[i].state.Gamepad.sThumbLY;
            }
            else
            {
                m_xkeySts.LY = 0;
            }

            if ((m_padNum[i].state.Gamepad.sThumbLX > Threshold)
                || (m_padNum[i].state.Gamepad.sThumbLX < -Threshold))
            {
                //65535/2のプラスとマイナス
                m_xkeySts.LX = (double)m_padNum[i].state.Gamepad.sThumbLX / ((double)65535 / 2);
                //XKEY_STATE.LX = GAME_PAD[i].state.Gamepad.sThumbLX;
            }
            else
            {
                m_xkeySts.LX = 0;
            }

            //右スティック
            if ((m_padNum[i].state.Gamepad.sThumbRY > Threshold)
                || (m_padNum[i].state.Gamepad.sThumbRY < -Threshold))
            {
                //65535/2のプラスとマイナス
                m_xkeySts.RY = (double)m_padNum[i].state.Gamepad.sThumbRY / ((double)65535 / 180);
                //XKEY_STATE.LY = GAME_PAD[i].state.Gamepad.sThumbLY;
            }
            else
            {
                m_xkeySts.RY = 0;
            }

            if ((m_padNum[i].state.Gamepad.sThumbRX > Threshold)
                || (m_padNum[i].state.Gamepad.sThumbRX < -Threshold))
            {
                //65535/2のプラスとマイナス
                m_xkeySts.RX = (double)m_padNum[i].state.Gamepad.sThumbRX / ((double)65535 / 180);
                //XKEY_STATE.LX = GAME_PAD[i].state.Gamepad.sThumbLX;
            }
            else
            {
                m_xkeySts.RX = 0;
            }
            //if ((m_padNum[i].state.Gamepad.sThumbRY > Threshold)
            //    && (m_padNum[i].state.Gamepad.sThumbRY < -Threshold))
            //{
            //    //65535/2のプラスとマイナス。角度の為90×２
            //    m_xkeySts.RY = (double)m_padNum[i].state.Gamepad.sThumbRY / ((double)65535 / 180);
            //    //XKEY_STATE.LY = GAME_PAD[i].state.Gamepad.sThumbLY;
            //}
            //else
            //{
            //    m_xkeySts.RY = 0;
            //}

            //if ((m_padNum[i].state.Gamepad.sThumbRX > Threshold)
            //    && (m_padNum[i].state.Gamepad.sThumbRX < -Threshold))
            //{
            //    //65535/2のプラスとマイナス。角度の為90×２
            //    m_xkeySts.RX = (double)m_padNum[i].state.Gamepad.sThumbRX / ((double)65535 / 180);
            //    //XKEY_STATE.LX = GAME_PAD[i].state.Gamepad.sThumbLX;
            //}
            //else
            //{
            //    m_xkeySts.RX = 0;
            //}

            //ボタン系
            for (int i = 0; i < static_cast <int>(XKEY_NUM::XDIK_END); i++)
            {
                m_xkeySts.OLD_KEY[i] = m_xkeySts.NOW_KEY[i];//前の状態を保存

                switch (i)
                {
                case 0:
                    if (Buttons & XINPUT_GAMEPAD_A) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 1:
                    if (Buttons & XINPUT_GAMEPAD_B) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 2:
                    if (Buttons & XINPUT_GAMEPAD_X) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 3:
                    if (Buttons & XINPUT_GAMEPAD_Y) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 4:
                    if (Buttons & XINPUT_GAMEPAD_DPAD_UP) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 5:
                    if (Buttons & XINPUT_GAMEPAD_DPAD_DOWN) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 6:
                    if (Buttons & XINPUT_GAMEPAD_DPAD_LEFT) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 7:
                    if (Buttons & XINPUT_GAMEPAD_DPAD_RIGHT) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 8:
                    if (Buttons & XINPUT_GAMEPAD_START) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 9:
                    if (Buttons & XINPUT_GAMEPAD_BACK) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 10:
                    if (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 11:
                    if (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 12:
                    if (Buttons & XINPUT_GAMEPAD_LEFT_THUMB) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                case 13:
                    if (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB) m_xkeySts.NOW_KEY[i] = true;
                    else m_xkeySts.NOW_KEY[i] = false;
                    break;
                default:
                    break;
                }
                
            }

           // m_xkeySts.OLD_KEY[static_cast <int>(XKEY_NUM::XDIK_UP)] = m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_UP)];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_DPAD_UP) m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_UP)] = true;
           // else m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_UP)] = false;

           // m_xkeySts.OLD_KEY[static_cast <int>(XKEY_NUM::XDIK_DOWN)] = m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_DOWN)];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_DPAD_DOWN)m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_DOWN)] = true;
           // else m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_DOWN)] = false;

           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_LEFT] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_DPAD_LEFT)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT] = false;

           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_RIGHT] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_DPAD_RIGHT)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT] = false;

           // //各種ボタン
           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_A] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_A];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_A)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_A] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_A] = false;

           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_B] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_B];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_B)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_B] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_B] = false;
           //
           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_X] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_X];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_X)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_X] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_X] = false;

           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_Y] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_Y];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_Y)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_Y] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_Y] = false;

           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_START] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_START];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_START)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_START] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_START] = false;
           ///* {
           //     vibration.wRightMotorSpeed = 65535;
           //     XInputSetState(0, &vibration);
           // }
           // else
           // {
           //     vibration.wLeftMotorSpeed = 0;
           //     vibration.wRightMotorSpeed = 0;
           //     XInputSetState(0, &vibration);
           // }*/
           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_BACK] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_BACK];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_BACK)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_BACK] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_BACK] = false;
           //
           // //左アナログ方向キーをパッド奥に押した場合
           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_LEFT_THUMBT] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT_THUMBT];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_LEFT_THUMB)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT_THUMBT] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT_THUMBT] = false;

           // //右アナログ方向キーをパッド奥に押した場合
           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_RIGHT_THUMB] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT_THUMB];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT_THUMB] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT_THUMB] = false;

           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_LEFT_SHOULDER] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT_SHOULDER];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT_SHOULDER] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_LEFT_SHOULDER] = false;
           //
           // m_xkeySts.OLD_KEY[XKEY_NUM::XDIK_RIGHT_SHOULDER] = m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT_SHOULDER];//前の状態を保存
           // if (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT_SHOULDER] = true;
           // else m_xkeySts.NOW_KEY[XKEY_NUM::XDIK_RIGHT_SHOULDER] = false;
           
            //if (m_padNum[i].state.Gamepad.sThumbLX)
            if (m_xkeySts.NOW_KEY[static_cast <int>(XKEY_NUM::XDIK_UP)])
            {
                /*vibration.wRightMotorSpeed = 65535;*/
                vibration.wRightMotorSpeed = 65535;
                XInputSetState(0, &vibration);
            }
            else
            {
                vibration.wLeftMotorSpeed = 0;
                vibration.wRightMotorSpeed = 0;
                XInputSetState(0, &vibration);
            }
            //if (m_padNum[i].state.Gamepad.bRightTrigger);
        }
        else {
            //接続されていない場合
        }

     /*   if (m_padNum[i].state.Gamepad.wButtons & )
        {

        }*/
    }
    //Sleep(1);
    //glutPostRedisplay();
}

bool PXinput::Trigger(bool keyno)
{
    return false;
}

bool PXinput::Release(bool keyno)
{
    return false;
}

bool PXinput::Check(bool keyno)
{
   /* if (m_xkeySts.NOW_KEY[keyno])
    {
        return true;
    }
    else
    {
        return false;
    }*/

    return m_xkeySts.NOW_KEY[keyno];
}




//void Xinput()
//{
//    //------------------------------------------------------------
//    // 初期設定
//    //------------------------------------------------------------
//    
//
//    //------------------------------------------------------------
//    // キー入力関係
//    //------------------------------------------------------------
//   
//    //ゲームパッドの状態を取得
//    XINPUT_STATE state;
//    XInputGetState(0, &state);
//    int iPad_left = 0, iPad_right = 0, iPad_up = 0, iPad_down = 0;
//    int iPad_leftshoulder = 0, iPad_rightshoulder = 0;
//    int iPad_A = 0, iPad_B = 0, iPad_X = 0, iPad_Y = 0;
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) iPad_left = 1;//ゲームパッド十字キー左
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) iPad_right = 1;//ゲームパッド十字キー右
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) iPad_up = 1;//ゲームパッド十字キー上
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) iPad_down = 1;//ゲームパッド十字キー下
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) iPad_leftshoulder = 1;//ゲームパッドL
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) iPad_rightshoulder = 1;//ゲームパッドR
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) iPad_A = 1;//ゲームパッドA
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) iPad_B = 1;//ゲームパッドB
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) iPad_X = 1;//ゲームパッドX
//    if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) iPad_Y = 1;//ゲームパッドY
//    //ゲームパッドアナログスティックのデッドゾーン処理
//    if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
//        (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
//    {
//        state.Gamepad.sThumbLX = 0;
//        state.Gamepad.sThumbLY = 0;
//    }
//
//    //十字キー入力時の処理
//    double dValue = 1;
//    if (iPad_left || iPad_right)
//    {
//        if (iPad_up || iPad_down)
//        {
//            dValue = 1 / sqrt(2);//斜め入力時は1/√2倍する
//        }
//        else
//        {
//            dValue = 1;
//        }
//    }
//    else if (iPad_up || iPad_down)
//    {
//        dValue = 1;
//    }
//
//    if (state.Gamepad.sThumbLX != 0 || state.Gamepad.sThumbLY != 0)//ゲームパッドアナログスティック入力時処理
//    {
//        /*fPosX1 += static_cast<FLOAT>(state.Gamepad.sThumbLX / 32767.0 * Window::GetFrameTime() * dSpeed);
//        if (fPosX1 < fRadius1)
//        {
//            fPosX1 = fRadius1;
//        }
//        if (fPosX1 > Window::GetClientWidth() - fRadius1)
//        {
//            fPosX1 = Window::GetClientWidth() - fRadius1;
//        }
//        fPosY1 -= static_cast<FLOAT>(state.Gamepad.sThumbLY / 32767.0 * Window::GetFrameTime() * dSpeed);
//        if (fPosY1 < fRadius1)
//        {
//            fPosY1 = fRadius1;
//        }
//        if (fPosY1 > Window::GetClientHeight() - fRadius1)
//        {
//            fPosY1 = Window::GetClientHeight() - fRadius1;
//        }*/
//    }
//    else//キーボード入力時処理
//    {
//        if (iPad_left)
//        {
//           /* fPosX1 -= static_cast<FLOAT>(Window::GetFrameTime() * dSpeed * dValue);
//            if (fPosX1 < fRadius1)
//            {
//                fPosX1 = fRadius1;
//            }*/
//        }
//        if (iPad_right)
//        {
//          /*  fPosX1 += static_cast<FLOAT>(Window::GetFrameTime() * dSpeed * dValue);
//            if (fPosX1 > Window::GetClientWidth() - fRadius1)
//            {
//                fPosX1 = Window::GetClientWidth() - fRadius1;
//            }*/
//        }
//        if (iPad_up)
//        {
//           /* fPosY1 -= static_cast<FLOAT>(Window::GetFrameTime() * dSpeed * dValue);
//            if (fPosY1 < fRadius1)
//            {
//                fPosY1 = fRadius1;
//            }*/
//        }
//        if (iPad_down)
//        {
//           /* fPosY1 += static_cast<FLOAT>(Window::GetFrameTime() * dSpeed * dValue);
//            if (fPosY1 > Window::GetClientHeight() - fRadius1)
//            {
//                fPosY1 = Window::GetClientHeight() - fRadius1;
//            }*/
//        }
//    }
//
//    //ゲームパッドのAまたはBを押した時の処理：ゲームパッドの振動
//    XINPUT_VIBRATION vibration;
//    vibration.wLeftMotorSpeed = 0;
//    vibration.wRightMotorSpeed = 0;
//    XInputSetState(0, &vibration);
//    if (iPad_A)
//    {
//        vibration.wLeftMotorSpeed = 65535;
//        XInputSetState(0, &vibration);
//    }
//    if (iPad_B)
//    {
//        vibration.wRightMotorSpeed = 65535;
//        XInputSetState(0, &vibration);
//    }
//}