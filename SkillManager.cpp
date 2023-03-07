#include "SkillManager.h"
#include "Input.h"
#include "Quad2d.h"
#include "Random.h"

bool SkillManager::Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player)
{
	//プレイヤーセット
	m_sPlayer = _player;

	//スキルUIオブジェクトを必要な数だけ実体化
	for (int i = 0; i < static_cast<int>(SKILLTYPEID::END); i++)
	{
		m_skill.push_back(std::make_unique<Skill>());
	}

	//スキルUI初期化
	for (int i = 0; i < static_cast<int>(SKILLTYPEID::END); i++)
	{
		//何のスキルか設定
		m_skill[i]->SetSkillType(static_cast<SKILLTYPEID>(i));
		//設定したスキルのステータスに初期化
		m_skill[i]->SkillCsvSet(_csvFile);

		//画像設定
		m_skill[i]->GetImage()->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		m_skill[i]->GetImage()->LoadTexture(_csvFile->GetModelName().at(i).c_str());
		
		//スキル系のみの処理
		if (i >= static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE))
		{
			//スキルUIは大きさ一括設定
			m_skill[i]->GetImage()->updateAnimeNoSize(250.0f, 250.0f);

			//テキスト設定
			m_skill[i]->GetText()->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			m_skill[i]->GetText()->LoadTexture(_csvFile->GetTexFolder().at(i).c_str());
			m_skill[i]->GetText()->updateAnimeNoSize(970.0f, 260.0f);
			m_skill[i]->GetText()->updateAnimeNoPos(800, 650);//最大1290、730
		}
	}
	//背景
	m_skill[static_cast<int>(SKILLTYPEID::BACKGROUND)]->GetImage()->updateAnimeNoSize(1290.0f, 730.0f);
	m_skill[static_cast<int>(SKILLTYPEID::BACKGROUND)]->GetImage()->updateAnimeNoPos(645, 365);//最大1290、730
	//フレーム
	m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoSize(300.0f, 350.0f);
	//キャラクター1
	m_skill[static_cast<int>(SKILLTYPEID::CHARACTER1)]->GetImage()->updateAnimeNoSize(300.0f, 400.0f);
	m_skill[static_cast<int>(SKILLTYPEID::CHARACTER1)]->GetImage()->updateAnimeNoPos(125, 365);//最大1290、730
	

    return false;
}

void SkillManager::Reset()
{
	//スキルの選択位置初期化
	m_setposs = 425;//フレームX座標リセット
	m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoPos(m_setposs, 325);//フレーム描画位置左にセット
	m_selectNum = 0;//選んでいる場所を一番左に
	m_allRate = 0;//抽選箱を空にする
	m_selectAllBox.erase(m_selectAllBox.begin(), m_selectAllBox.end());
}

void SkillManager::SkillSelect()
{
	//抽選されたものを返す配列
	//TArray<FSkillBase> stSelected;
	////3つ必要なので増やす
	//stSelected.SetNum(3);

	//m_stSelected.SetNum(3);//保険デバッグ

	int r1Decision = 0;//１つ目の当選したスキル番号を返す
	int r2Decision = 0;//２つ目の当選したスキル番号を返す
	int r3Decision = 0;//３つ目の当選したスキル番号を返す
	
	//selected.Num()要素数


	//FMathというのがあり、ランドレンジっていうのがあるのでそれを使ったほうがいい。
	//レベル最大でないものを追加
	for (int i = static_cast<int>(SKILLTYPEID::ATKUP); i < static_cast<int>(SKILLTYPEID::END); i++)
	{
		//最大レベルになっているものは除く
		if (m_skill[i]->GetCSVStatus(SKILL_STATUSID::LEVEL) <
			m_skill[i]->GetCSVStatus(SKILL_STATUSID::LEVELMAX))
		{
			m_selectAllBox.emplace_back(i);//最大レベルでないものを入れる
			//各スキルのレートをすべて足していく
			m_allRate += m_skill[i]->GetCSVStatus(SKILL_STATUSID::RATE);
		}
	}

	//配列の中身を混ぜる
	for (int i = 0; i < m_selectAllBox.size(); i++)
	{
		int tmpIdx = Random::GetInstance()->RandomGeneration(0, (m_selectAllBox.size() - 1));
		int tmpSkill = m_selectAllBox[i];
		m_selectAllBox[i] = m_selectAllBox[tmpIdx];
		m_selectAllBox[tmpIdx] = tmpSkill;
	}

	//スキルを入れるため3回、回す
	for (int i = 0; i < 3; i++)
	{
		//すべてのスキルを取得していなければ
		if (m_selectAllBox.size() != 0)
		{
			switch (i)
			{
			case 0:
			{
				//引き切った番号を格納
				int r1 = SelectSkillRote();
				r1Decision = m_selectAllBox[r1];
				m_skillbox[0] = static_cast<SKILLTYPEID>(r1Decision);

				//当選した番号を消す
				for (auto i = m_selectAllBox.begin(); i != m_selectAllBox.end(); i++)
				{
					if(*i == r1Decision)
					{
						m_selectAllBox.erase(i);
						break;
					}
				}
			}
				break;
			
			case 1:
			{
				//引き切った番号を格納
				int r2 = SelectSkillRote();
				r2Decision = m_selectAllBox[r2];
				m_skillbox[1] = static_cast<SKILLTYPEID>(r2Decision);

				//当選した番号を消す
				for (auto i = m_selectAllBox.begin(); i != m_selectAllBox.end(); i++)
				{
					if (*i == r2Decision)
					{
						m_selectAllBox.erase(i);
						break;
					}
				}
			}
				break;
			
			case 2:
			{
				//引き切った番号を格納
				int r3 = SelectSkillRote();
				r3Decision = m_selectAllBox[r3];
				m_skillbox[2] = static_cast<SKILLTYPEID>(r3Decision);

				////乱数生成
				//int r3 = FMath::RandRange(0, (selected.Num() - 1));
				////中の値を保存
				//r3Decision = selected[r3];
				//最後なので消す必要なし
			}
				break;
			
			default:
				break;
			}
		}
		else
		{
			switch (i)
			{
			case 0:
				//ハズレスキルにする
				m_skillbox[0] = SKILLTYPEID::MAXHPUP_LITTLE;
				break;
			case 1:
				//ハズレスキルにする
				m_skillbox[1] = SKILLTYPEID::MAXHPUP_LITTLE;
				break;
			case 2:
				//ハズレスキルにする
				m_skillbox[2] = SKILLTYPEID::MAXHPUP_LITTLE;
				break;
			default:
				break;
			}

		}

	}

	//スキルのアイコン描画位置変更
	m_skill[static_cast<int>(m_skillbox[0])]->GetImage()->updateAnimeNoPos(425, 315);//最大1290、730
	m_skill[static_cast<int>(m_skillbox[1])]->GetImage()->updateAnimeNoPos(750, 315);//最大1290、730
	m_skill[static_cast<int>(m_skillbox[2])]->GetImage()->updateAnimeNoPos(1075, 315);//最大1290、730
}

bool SkillManager::Update()
{
		//選択を待つ処理
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT) || (Input::GetInstance().CheckKeyTrigger(DIK_LEFT)))
		{
			if (m_selectNum > 0)
			{
				m_selectNum -= 1;
				m_setposs -= 325;
				m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoPos(m_setposs, 325);//最大1290、730
				//425、750,1075

			}
			else
			{
				m_selectNum = 0;
			}
		}
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) || (Input::GetInstance().CheckKeyTrigger(DIK_RIGHT)))
		{
			if (m_selectNum < 2)
			{
				m_selectNum += 1;
				m_setposs += 325;
				m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoPos(m_setposs, 325);//最大1290、730
				//425、750,1075

			}
			else
			{
				m_selectNum = 2;
			}
		}

		//決定
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A) || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {

			//各スキルの処理を行う
			switch (m_skillbox[m_selectNum])
			{
			case SKILLTYPEID::MAXHPUP_LITTLE:
				m_sPlayer->UPMAXHP(m_skill[static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE)]
					->GetCSVStatus(SKILL_STATUSID::UP_VALUE));
				break;
			case SKILLTYPEID::ATKUP:
				m_sPlayer->UPAttack(m_skill[static_cast<int>(SKILLTYPEID::ATKUP)]
					->GetCSVStatus(SKILL_STATUSID::UP_VALUE));
				break;
			case SKILLTYPEID::HEEL:
				m_sPlayer->HPIncrease(m_skill[static_cast<int>(SKILLTYPEID::HEEL)]
					->GetCSVStatus(SKILL_STATUSID::UP_VALUE));
				break;
			case SKILLTYPEID::MAXHPUP:
				m_sPlayer->UPMAXHP(m_skill[static_cast<int>(SKILLTYPEID::MAXHPUP)]
					->GetCSVStatus(SKILL_STATUSID::UP_VALUE));
				break;
			case SKILLTYPEID::SPEEDUP:
				m_sPlayer->UPSpeed(m_skill[static_cast<int>(SKILLTYPEID::SPEEDUP)]
					->GetCSVStatus(SKILL_STATUSID::UP_VALUE));
				break;
			case SKILLTYPEID::RELOADUP:
				
				m_sPlayer->UPReload(m_skill[static_cast<int>(SKILLTYPEID::RELOADUP)]
					->GetCSVStatus(SKILL_STATUSID::UP_VALUE));
				break;
			case SKILLTYPEID::BULLETFRONT:
				m_sPlayer->UPFrontBullet(m_skill[static_cast<int>(SKILLTYPEID::BULLETFRONT)]
					->GetCSVStatus(SKILL_STATUSID::LEVEL));
				break;
			case SKILLTYPEID::BULLETBACK:
				m_sPlayer->UPBackBullet(m_skill[static_cast<int>(SKILLTYPEID::BULLETBACK)]
					->GetCSVStatus(SKILL_STATUSID::LEVEL));
				break;
			default:
				break;
			}

			//選択したスキルのスキルレベルを上昇
			m_skill[static_cast<int>(m_skillbox[m_selectNum])]->LevelUP();

			return true;
		}
	
	return false;
}

void SkillManager::Draw()
{
	for (int i = 0; i < static_cast<int>(SKILLTYPEID::CHARACTER1) + 1; i++)
	{
		m_skill[i]->GetImage()->Draw();
	}
	//抽選を行う1度だけ通す処理
	m_skill[static_cast<int>(m_skillbox[0])]->GetImage()->Draw();
	m_skill[static_cast<int>(m_skillbox[1])]->GetImage()->Draw();
	m_skill[static_cast<int>(m_skillbox[2])]->GetImage()->Draw();
	//選択しているスキルの説明テキスト描画
	m_skill[static_cast<int>(m_skillbox[m_selectNum])]->GetText()->Draw();
}



void SkillManager::SetAllStatus(int _skillNum, std::vector<float> _value)
{
	m_skill[_skillNum]->SetAllStatus(_value);
}

int SkillManager::SelectSkillRote()
{
	//レートからランダム発生
	int selRate = Random::GetInstance()->RandomGeneration(1, m_allRate);//０を出すとエラーするので１から
	int selIdx = 0;
	for (; selRate > 0 && selIdx < m_selectAllBox.size(); selIdx++)
	{
		selRate -= m_skill[m_selectAllBox[selIdx]]->GetCSVStatus(SKILL_STATUSID::RATE);
	}
	//０が出た場合エラーにならないように、上で１からにしてるし大丈夫だと思うけど
	if (selIdx <= 0)
	{
		selIdx = 0;
	}
	else
	{
		selIdx -= 1;//数字が進んでいるので戻す
	}

	//抽選された分を全体のレートから引く
	m_allRate -= m_skill[m_selectAllBox[selIdx]]->GetCSVStatus(SKILL_STATUSID::RATE);

	return selIdx;//当選番号を返す
}
