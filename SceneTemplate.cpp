#pragma once
#include "SceneBase.h"

class Template : public SceneBase
{
public:
	void Imgui();
	void Init();
	void Exit();
	void Update();
	void Draw();
};


//�ȉ�cpp�t�@�C��
#include "Input.h"
#include "mesh.h"
#include "ModelMgr.h"

void Template::Imgui()
{
	// Start the Dear ImGui frame�A�J�n����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//�����܂�

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::SetNextWindowSize(ImVec2(280, 300));

	ImGui::Begin("config 1");

	static float slider1 = 0.0;
	static char text1[8] = "";

	//ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("group 1")) {

		ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);

		ImGui::SliderFloat("slider 1", &slider1, 0.0f, 1.0f);
		ImGui::InputText(u8"textbox 1", text1, sizeof(text1));
		if (ImGui::Button("button 1")) {
			slider1 = 0.0f;
			strcpy(text1, "button 1");
		}
		if (ImGui::Button("DebugFlg: ON / OFF")) {
			/*if (DebugFlg == true)
			{
				DebugFlg = false;
			}
			else
			{
				DebugFlg = true;
			}*/

		}

		ImGui::TreePop();
	}

	//ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("group 2")) {

		ImGui::Text("fps: %.2f", ImGui::GetIO().Framerate);
		ImGui::SliderFloat("slider 1b", &slider1, 0.0f, 1.0f);
		ImGui::InputText("textbox 1b", text1, sizeof(text1));
		if (ImGui::Button("button 1b")) {
			slider1 = 0.0f;
			strcpy(text1, "button 1b");
		}

		if (ImGui::TreeNode("group 3")) {
			if (ImGui::Button("button 2")) {

			}
			ImGui::TreePop();

		}

		ImGui::TreePop();
	}


	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	//�Q��
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(980, 20));
	ImGui::SetNextWindowSize(ImVec2(280, 300));

	ImGui::Begin(u8"�������");

	std::string str;
	str = u8"���@�F���E�L�[�F������ς���";
	ImGui::Text(str.c_str());
	str = u8"���@�Fw�F�O�i";
	ImGui::Text(str.c_str());
	str = u8"���@�Fs�F���";
	ImGui::Text(str.c_str());

	//���L�[
	static float num;
	num = Input::GetInstance().GetLStick().x;
	ImGui::InputFloat("input float", &num);
	num = Input::GetInstance().GetLStick().y;
	ImGui::InputFloat("input float", &num);

	//�E�L�[
	num = Input::GetInstance().GetRStick().x;
	ImGui::InputFloat("input float", &num);
	num = Input::GetInstance().GetRStick().y;
	ImGui::InputFloat("input float", &num);

	//if (Hit == true)
	//{
	//	num = 777.0f;
	//	ImGui::InputFloat("����", &num);

	//	ImGui::Text("TRUE");
	//	
	//	//ImGui::Text("fps: %.2f", ImGui::GetIO().DeltaTime);
	//}
	//else
	//{
	//	num = 0.0f;
	//	ImGui::InputFloat("����", &num);
	//	ImGui::InputFloat("����", &ImGui::GetIO().DeltaTime);
	//}

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// Rendering�A�㏈��
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Template::Init()
{
	DebugFlg = true;//�f�o�b�O���邩�ǂ���

	//�������I��
	SceneFlgInit();
	SceneStatus = SCENESTATUS::Editor;//�G�f�B�^�[�V�[���ɐݒ�
}

void Template::Exit()
{
	//�ۗL���Ă��郂�f���f�[�^�J��
	ModelMgr::GetInstance().Finalize();

	//���̃V�[�����w��
	//NextSceneStatus = SCENESTATUS::Game;

	//�t���O���Z�b�g�A���̃V�[���̈�
	SceneFlgReset();
}

void Template::Update()
{
	if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A)) {
		SceneFlgDispose();//�I�������ɓ���
	}
}

void Template::Draw()
{

}
