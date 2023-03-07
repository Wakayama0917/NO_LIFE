#include "SkillManager.h"
#include "Input.h"
#include "Quad2d.h"
#include "Random.h"

bool SkillManager::Init(unique_ptr<CSVFile>& _csvFile, shared_ptr<Player>& _player)
{
	//�v���C���[�Z�b�g
	m_sPlayer = _player;

	//�X�L��UI�I�u�W�F�N�g��K�v�Ȑ��������̉�
	for (int i = 0; i < static_cast<int>(SKILLTYPEID::END); i++)
	{
		m_skill.push_back(std::make_unique<Skill>());
	}

	//�X�L��UI������
	for (int i = 0; i < static_cast<int>(SKILLTYPEID::END); i++)
	{
		//���̃X�L�����ݒ�
		m_skill[i]->SetSkillType(static_cast<SKILLTYPEID>(i));
		//�ݒ肵���X�L���̃X�e�[�^�X�ɏ�����
		m_skill[i]->SkillCsvSet(_csvFile);

		//�摜�ݒ�
		m_skill[i]->GetImage()->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		m_skill[i]->GetImage()->LoadTexture(_csvFile->GetModelName().at(i).c_str());
		
		//�X�L���n�݂̂̏���
		if (i >= static_cast<int>(SKILLTYPEID::MAXHPUP_LITTLE))
		{
			//�X�L��UI�͑傫���ꊇ�ݒ�
			m_skill[i]->GetImage()->updateAnimeNoSize(250.0f, 250.0f);

			//�e�L�X�g�ݒ�
			m_skill[i]->GetText()->Init(10.0f, 10.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			m_skill[i]->GetText()->LoadTexture(_csvFile->GetTexFolder().at(i).c_str());
			m_skill[i]->GetText()->updateAnimeNoSize(970.0f, 260.0f);
			m_skill[i]->GetText()->updateAnimeNoPos(800, 650);//�ő�1290�A730
		}
	}
	//�w�i
	m_skill[static_cast<int>(SKILLTYPEID::BACKGROUND)]->GetImage()->updateAnimeNoSize(1290.0f, 730.0f);
	m_skill[static_cast<int>(SKILLTYPEID::BACKGROUND)]->GetImage()->updateAnimeNoPos(645, 365);//�ő�1290�A730
	//�t���[��
	m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoSize(300.0f, 350.0f);
	//�L�����N�^�[1
	m_skill[static_cast<int>(SKILLTYPEID::CHARACTER1)]->GetImage()->updateAnimeNoSize(300.0f, 400.0f);
	m_skill[static_cast<int>(SKILLTYPEID::CHARACTER1)]->GetImage()->updateAnimeNoPos(125, 365);//�ő�1290�A730
	

    return false;
}

void SkillManager::Reset()
{
	//�X�L���̑I���ʒu������
	m_setposs = 425;//�t���[��X���W���Z�b�g
	m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoPos(m_setposs, 325);//�t���[���`��ʒu���ɃZ�b�g
	m_selectNum = 0;//�I��ł���ꏊ����ԍ���
	m_allRate = 0;//���I������ɂ���
	m_selectAllBox.erase(m_selectAllBox.begin(), m_selectAllBox.end());
}

void SkillManager::SkillSelect()
{
	//���I���ꂽ���̂�Ԃ��z��
	//TArray<FSkillBase> stSelected;
	////3�K�v�Ȃ̂ő��₷
	//stSelected.SetNum(3);

	//m_stSelected.SetNum(3);//�ی��f�o�b�O

	int r1Decision = 0;//�P�ڂ̓��I�����X�L���ԍ���Ԃ�
	int r2Decision = 0;//�Q�ڂ̓��I�����X�L���ԍ���Ԃ�
	int r3Decision = 0;//�R�ڂ̓��I�����X�L���ԍ���Ԃ�
	
	//selected.Num()�v�f��


	//FMath�Ƃ����̂�����A�����h�����W���Ă����̂�����̂ł�����g�����ق��������B
	//���x���ő�łȂ����̂�ǉ�
	for (int i = static_cast<int>(SKILLTYPEID::ATKUP); i < static_cast<int>(SKILLTYPEID::END); i++)
	{
		//�ő僌�x���ɂȂ��Ă�����̂͏���
		if (m_skill[i]->GetCSVStatus(SKILL_STATUSID::LEVEL) <
			m_skill[i]->GetCSVStatus(SKILL_STATUSID::LEVELMAX))
		{
			m_selectAllBox.emplace_back(i);//�ő僌�x���łȂ����̂�����
			//�e�X�L���̃��[�g�����ׂđ����Ă���
			m_allRate += m_skill[i]->GetCSVStatus(SKILL_STATUSID::RATE);
		}
	}

	//�z��̒��g��������
	for (int i = 0; i < m_selectAllBox.size(); i++)
	{
		int tmpIdx = Random::GetInstance()->RandomGeneration(0, (m_selectAllBox.size() - 1));
		int tmpSkill = m_selectAllBox[i];
		m_selectAllBox[i] = m_selectAllBox[tmpIdx];
		m_selectAllBox[tmpIdx] = tmpSkill;
	}

	//�X�L�������邽��3��A��
	for (int i = 0; i < 3; i++)
	{
		//���ׂẴX�L�����擾���Ă��Ȃ����
		if (m_selectAllBox.size() != 0)
		{
			switch (i)
			{
			case 0:
			{
				//�����؂����ԍ����i�[
				int r1 = SelectSkillRote();
				r1Decision = m_selectAllBox[r1];
				m_skillbox[0] = static_cast<SKILLTYPEID>(r1Decision);

				//���I�����ԍ�������
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
				//�����؂����ԍ����i�[
				int r2 = SelectSkillRote();
				r2Decision = m_selectAllBox[r2];
				m_skillbox[1] = static_cast<SKILLTYPEID>(r2Decision);

				//���I�����ԍ�������
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
				//�����؂����ԍ����i�[
				int r3 = SelectSkillRote();
				r3Decision = m_selectAllBox[r3];
				m_skillbox[2] = static_cast<SKILLTYPEID>(r3Decision);

				////��������
				//int r3 = FMath::RandRange(0, (selected.Num() - 1));
				////���̒l��ۑ�
				//r3Decision = selected[r3];
				//�Ō�Ȃ̂ŏ����K�v�Ȃ�
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
				//�n�Y���X�L���ɂ���
				m_skillbox[0] = SKILLTYPEID::MAXHPUP_LITTLE;
				break;
			case 1:
				//�n�Y���X�L���ɂ���
				m_skillbox[1] = SKILLTYPEID::MAXHPUP_LITTLE;
				break;
			case 2:
				//�n�Y���X�L���ɂ���
				m_skillbox[2] = SKILLTYPEID::MAXHPUP_LITTLE;
				break;
			default:
				break;
			}

		}

	}

	//�X�L���̃A�C�R���`��ʒu�ύX
	m_skill[static_cast<int>(m_skillbox[0])]->GetImage()->updateAnimeNoPos(425, 315);//�ő�1290�A730
	m_skill[static_cast<int>(m_skillbox[1])]->GetImage()->updateAnimeNoPos(750, 315);//�ő�1290�A730
	m_skill[static_cast<int>(m_skillbox[2])]->GetImage()->updateAnimeNoPos(1075, 315);//�ő�1290�A730
}

bool SkillManager::Update()
{
		//�I����҂���
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT) || (Input::GetInstance().CheckKeyTrigger(DIK_LEFT)))
		{
			if (m_selectNum > 0)
			{
				m_selectNum -= 1;
				m_setposs -= 325;
				m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoPos(m_setposs, 325);//�ő�1290�A730
				//425�A750,1075

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
				m_skill[static_cast<int>(SKILLTYPEID::FLAME)]->GetImage()->updateAnimeNoPos(m_setposs, 325);//�ő�1290�A730
				//425�A750,1075

			}
			else
			{
				m_selectNum = 2;
			}
		}

		//����
		if (Input::GetInstance().CheckButtonTrigger(XINPUT_GAMEPAD_A) || (Input::GetInstance().CheckKeyTrigger(DIK_RETURN))) {

			//�e�X�L���̏������s��
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

			//�I�������X�L���̃X�L�����x�����㏸
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
	//���I���s��1�x�����ʂ�����
	m_skill[static_cast<int>(m_skillbox[0])]->GetImage()->Draw();
	m_skill[static_cast<int>(m_skillbox[1])]->GetImage()->Draw();
	m_skill[static_cast<int>(m_skillbox[2])]->GetImage()->Draw();
	//�I�����Ă���X�L���̐����e�L�X�g�`��
	m_skill[static_cast<int>(m_skillbox[m_selectNum])]->GetText()->Draw();
}



void SkillManager::SetAllStatus(int _skillNum, std::vector<float> _value)
{
	m_skill[_skillNum]->SetAllStatus(_value);
}

int SkillManager::SelectSkillRote()
{
	//���[�g���烉���_������
	int selRate = Random::GetInstance()->RandomGeneration(1, m_allRate);//�O���o���ƃG���[����̂łP����
	int selIdx = 0;
	for (; selRate > 0 && selIdx < m_selectAllBox.size(); selIdx++)
	{
		selRate -= m_skill[m_selectAllBox[selIdx]]->GetCSVStatus(SKILL_STATUSID::RATE);
	}
	//�O���o���ꍇ�G���[�ɂȂ�Ȃ��悤�ɁA��łP����ɂ��Ă邵���v���Ǝv������
	if (selIdx <= 0)
	{
		selIdx = 0;
	}
	else
	{
		selIdx -= 1;//�������i��ł���̂Ŗ߂�
	}

	//���I���ꂽ����S�̂̃��[�g�������
	m_allRate -= m_skill[m_selectAllBox[selIdx]]->GetCSVStatus(SKILL_STATUSID::RATE);

	return selIdx;//���I�ԍ���Ԃ�
}
