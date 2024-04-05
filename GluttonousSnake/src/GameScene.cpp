#include "pch.h"
#include "GameScene.h"
namespace GS
{
	GameScene* GameScene::s_Instance = nullptr;

	void GameScene::TimerUpdate()
	{
		if (m_First)
		{
			m_CurrentTime = m_LastTime = glfwGetTime();
			m_DeltaTime = 0.0;
			m_First = false;
		}
		m_CurrentTime = glfwGetTime();
		m_DeltaTime = m_CurrentTime - m_LastTime;
		m_LastTime = m_CurrentTime;
	}
	bool GameScene::HitWall()
	{
		MutexLock::Lock("Snake");
		if (m_HitWall)
		{
			int x = m_Head->pos.x;
			if (x >= m_Width || x < 0)
			{
				MutexLock::Unlock("Snake");
				return true;
			}

			int y = m_Head->pos.y;
			if (y >= m_Height || y < 0)
			{
				MutexLock::Unlock("Snake");
				return true;
			}
		}
		else
		{
			int x = m_Head->pos.x;
			if (x >= m_Width)
			{
				m_Head->pos.x = 0;
				MutexLock::Unlock("Snake");
				return false;
			}
			if (x < 0)
			{
				m_Head->pos.x = m_Width - 1;
				MutexLock::Unlock("Snake");
				return false;
			}
			int y = m_Head->pos.y;
			if (y >= m_Height)
			{
				m_Head->pos.y = 0;
				MutexLock::Unlock("Snake");
				return false;
			}
			if (y < 0)
			{
				m_Head->pos.y = m_Height - 1;
				MutexLock::Unlock("Snake");
				return false;
			}
		}
		MutexLock::Unlock("Snake");
		return false;
	}
	bool GameScene::BiteSelf()
	{
		MutexLock::Lock("Snake");
		for (auto iter = m_Head->next; iter != NULL; iter = iter->next)
		{
			if (m_Head->pos == iter->pos)
			{
				MutexLock::Unlock("Snake");
				return true;
			}
		}
		MutexLock::Unlock("Snake");
		return false;
	}
	void GameScene::GenerateFood()
	{
			bool flag = true;
			std::pair<float, float> coord;
			do
			{
				flag = true;
				std::pair<float, float> coord = GenerateRand();
				for (auto iter = m_Head; iter != NULL; iter = iter->next)
				{
					if (coord.first == iter->pos.x && coord.second == iter->pos.y)
					{
						flag = false;
						break;
					}
				}
				m_FoodPos.x = coord.first;
				m_FoodPos.y = coord.second;
			} while (!flag);
	}
	void GameScene::Step()
	{
		if (m_Head == NULL)
			return;
		
		glm::vec2 v(0.0f);
		switch (m_Dir)
		{
		case HeadDir::left:
			v = glm::vec2(-1.0f, 0.0f);
			break;
		case HeadDir::right:
			v = glm::vec2(1.0f, 0.0f);
			break;
		case HeadDir::up:
			v = glm::vec2(0.0f, 1.0f);
			break;
		case HeadDir::down:
			v = glm::vec2(0.0f, -1.0f);
			break;
		default:
			break;
		}


		if (IsGetFood())
		{
			MutexLock::Lock("Food");
			GenerateFood();
			MutexLock::Unlock("Food");
		}

		if (m_Lengthening > 0)
		{
			m_Lengthening--;
			MutexLock::Lock("Snake");
			auto newHead = new Node();
			newHead->pos = m_Head->pos + v;
			newHead->next = m_Head;
			newHead->last = NULL;
			m_Head->last = newHead;
			m_Head = newHead;
			MutexLock::Unlock("Snake");
			return;
		}

		MutexLock::Lock("Snake");
		if (m_Tail == m_Head)
		{
			m_Tail->pos += v;
			MutexLock::Unlock("Snake");
			return;
		}

		m_Tail->pos = m_Head->pos + v;

		Node* t = m_Tail->last;

		m_Head->last = m_Tail;
		m_Tail->next = m_Head;
		m_Tail->last = NULL;

		t->next = NULL;

		m_Head = m_Tail;
		m_Tail = t;
		MutexLock::Unlock("Snake");
	}
	void GameScene::GetInput()
	{
		if (m_NextInput != HeadDir::none && m_Dir != m_NextInput)
		{
			m_Dir = m_NextInput;
			m_GetInput = false;
			//m_NextInput = HeadDir::none;
			std::cout << "GetInput:\n";
			std::cout <<  "m_Dir: " <<(int)m_Dir << std::endl;
			std::cout << "m_NextInput: " << (int)m_NextInput << std::endl;
			return;
		}
		if (Input::IsKeyPressing(GLFW_KEY_LEFT) && m_Dir != HeadDir::right && m_Dir != HeadDir::left)
		{
			m_Dir = HeadDir::left;
			m_GetInput = false;
			m_NextInput = HeadDir::none;
			return;
		}
		if (Input::IsKeyPressing(GLFW_KEY_RIGHT) && m_Dir != HeadDir::left && m_Dir != HeadDir::right)
		{
			m_Dir = HeadDir::right;
			m_GetInput = false;
			m_NextInput = HeadDir::none;
			return;
		}
		if (Input::IsKeyPressing(GLFW_KEY_UP) && m_Dir != HeadDir::down && m_Dir != HeadDir::up)
		{
			m_Dir = HeadDir::up;
			m_GetInput = false;
			m_NextInput = HeadDir::none;
			return;
		}
		if (Input::IsKeyPressing(GLFW_KEY_DOWN) && m_Dir != HeadDir::up && m_Dir != HeadDir::down)
		{
			m_Dir = HeadDir::down;
			m_GetInput = false;
			m_NextInput = HeadDir::none;
			return;
		}
	}
	void GameScene::GetNextInput()
	{
		if (m_NextInput == HeadDir::none)
		{
			if (Input::IsKeyPressing(GLFW_KEY_LEFT) && m_Dir != HeadDir::right && m_Dir != HeadDir::left)
			{
				m_NextInput = HeadDir::left;
				return;
			}
			if (Input::IsKeyPressing(GLFW_KEY_RIGHT) && m_Dir != HeadDir::left && m_Dir != HeadDir::right)
			{
				m_NextInput = HeadDir::right;
				return;
			}
			if (Input::IsKeyPressing(GLFW_KEY_UP) && m_Dir != HeadDir::down && m_Dir != HeadDir::up)
			{
				m_NextInput = HeadDir::up;
				return;
			}
			if (Input::IsKeyPressing(GLFW_KEY_DOWN) && m_Dir != HeadDir::up && m_Dir != HeadDir::down)
			{
				m_NextInput = HeadDir::down;
				return;
			}
			if (m_NextInput != HeadDir::none) {
				std::cout << "m_Dir: " << (int)m_Dir << std::endl;
				std::cout << "m_NextInput: " << (int)m_NextInput << std::endl;
			}
		}
	}
	void GameScene::Init()
	{
		m_InterruptFlag = false;
		m_First = true;
		m_MoveTimer = 0.0;
		m_GameOver = true;
		m_Run = true;
		IsRun();
		m_Tail = m_Head = new Node();
		m_Head->next = NULL;
		m_Head->last = NULL;
		m_Head->pos = glm::vec2(float(m_Width/2), float(m_Height/2));
		GenerateFood();
	}
	void GameScene::GameOver()
	{
		MutexLock::Lock("GameOver");
		m_GameOver = true;
		MutexLock::Unlock("GameOver");
	}
	inline bool GameScene::IsGetFood()
	{
		bool ret = m_Head->pos == m_FoodPos;
		if(ret)
			m_Lengthening += m_Lengthen;
		return ret;
	}
	void GameScene::Clear()
	{
		MutexLock::Lock("Snake");
		if (m_Head)
			for (auto iter = m_Head->next; iter != NULL; iter = iter->next)
			{
				delete iter->last;
			}
		if (m_Tail)
		{
			delete m_Tail;
		}
		m_Head = NULL;
		m_Tail = NULL;
		MutexLock::Unlock("Snake");
		m_Lengthening = 0;
	}
	GameScene::GameScene(float speed, bool hitWall, int width, int height, int lengthen)
		:m_Speed(speed), m_HitWall(hitWall), m_Width(width), m_Height(height),m_Lengthen(lengthen)
	{
		m_Head = NULL;
		m_Tail = NULL;
		SetLengthen(lengthen);
		SetSpeed(speed);
		m_Lengthening  = 0;
		m_Dir = HeadDir::right;
		srand(time(0));
		Init();
	}
	GameScene::GameScene()
	{
		__debugbreak();
	}
	GameScene::~GameScene()
	{
		Clear();
		GameOver();
	}
	void GameScene::Update()
	{
		while (IsRun())
		{
			if (!GetInterruptFlag())
			{
				//std::cout << "[info] Game Running.\n";
				TimerUpdate();
				/*if (m_GetInput)
				{
					GetInput();
				}
				else
				{
					GetNextInput();
				}*/


				m_MoveTimer += m_DeltaTime;
				if (m_MoveTimer >= m_Speed)
				{
					MutexLock::Lock("InputQueue");
					if (!m_InQueue.empty())
					{
						m_Dir = m_InQueue.front();
						m_InQueue.pop();
					}
					MutexLock::Unlock("InputQueue");
					Step();
					m_GetInput = true;
					m_MoveTimer = 0.0;
				}
				if (HitWall() || BiteSelf())
				{
					GameOver();
					Interrupt();
				}
			}
		}
	}
	void GameScene::ForEach(std::function<void(const glm::vec2&)> func)
	{
		for (auto iter = m_Head; iter != NULL; iter = iter->next)
		{
			func(iter->pos);
		}
	}
	const glm::vec2& GameScene::GetFoodPos() const
	{
		return m_FoodPos;
	}
	bool GameScene::IsGameOver() const
	{
		bool t;
		MutexLock::Lock("GameOver");
		t = m_GameOver;
		MutexLock::Unlock("GameOver");
		return t;
	}
	void GameScene::Reset()
	{
		Clear();
		//GameOver();
		Init();
		GenerateFood();
		Continue();
		m_GameOver = false;
	}
	void GameScene::Interrupt()
	{
		MutexLock::Lock("InterruptFlag");
		m_InterruptFlag = true;
		MutexLock::Unlock("InterruptFlag");
	}
	void GameScene::Continue()
	{
		MutexLock::Lock("InterruptFlag");
		m_InterruptFlag = false;
		MutexLock::Unlock("InterruptFlag");
	}
	bool GameScene::GetInterruptFlag() const
	{
		MutexLock::Lock("InterruptFlag");
		bool ret = m_InterruptFlag;
		MutexLock::Unlock("InterruptFlag");
		return ret;
	}
	bool GameScene::IsRun() const
	{
		bool t;
		MutexLock::Lock("Run");
		t =  m_Run;
		MutexLock::Unlock("Run");
		return t;
	}
	void GameScene::GameThreadOver()
	{
		MutexLock::Lock("Run");
		m_Run = false;
		MutexLock::Unlock("Run");
	}
	void GameScene::SetSpeed(double speed)
	{
		MutexLock::Lock("Speed");
		m_Speed = speed;
		MutexLock::Unlock("Speed");
	}
	double GameScene::GetSpeed() const
	{
		return m_Speed;
	}
	void GameScene::SetLengthen(int lengthen)
	{
		MutexLock::Lock("Lengthen");
		m_Lengthen = lengthen;
		MutexLock::Unlock("Lengthen");
	}
	int GameScene::GetLengthen() const
	{
		return m_Lengthen;
	}
	bool GameScene::GetHitWall() const
	{
		return m_HitWall;
	}
	void GameScene::SetHitWall(bool hitWall)
	{
		m_HitWall = hitWall;
	}
	void GameScene::KeyCallback(int key, int action)
	{
		if (action == GLFW_PRESS)
		{
			MutexLock::Lock("InputQueue");
			switch (key)
			{
			case GLFW_KEY_RIGHT:
				m_InQueue.push(HeadDir::right); break;
			case GLFW_KEY_LEFT:
				m_InQueue.push(HeadDir::left); break;
			case GLFW_KEY_UP:
				m_InQueue.push(HeadDir::up); break;
			case GLFW_KEY_DOWN:
				m_InQueue.push(HeadDir::down); break;
			default:
				break;
			}
			MutexLock::Unlock("InputQueue");
		}
	}
	GameScene* GameScene::GetInstance()
	{
		if (s_Instance == nullptr)
			s_Instance = new GameScene(0.2f);
		return s_Instance;
	}
}