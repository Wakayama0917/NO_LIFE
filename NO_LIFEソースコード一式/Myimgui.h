#pragma once
#include	<functional>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

void imguiInit();
void imguiDraw(std::function<void(void)> _func);
//void imguiDrawOpen();
//void imguiDrawOpen();
void imguiExit();

//GUIの処理を回すかどうかのフラグ
#define DEBUG
//#undef DEBUG /* デバッグ無効 */