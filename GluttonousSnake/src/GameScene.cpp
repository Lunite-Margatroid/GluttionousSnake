#include "pch.h"
#include "GameScene.h"
namespace GS
{
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
		if (m_HitWall)
		{
			int x = m_Head->pos.x;
			if (x >= m_Width || x < 0)
				return true;

			int y = m_Head->pos.y;
			if (y >= m_Height || y < 0)
				return true;
		}
		else
		{
			int x = m_Head->pos.x;
			if (x >= m_Width)
			{
				m_Head->pos.x = 0;
				return false;
			}
			if (x < 0)
			{
				m_Head->pos.x = m_Width - 1;
				return false;
			}
			int y = m_Head->pos.y;
			if (y >= m_Height)
			{
				m_Head->pos.y = 0;
				return false;
			}
			if (y < 0)
			{
				m_Head->pos.y = m_Height - 1;
				return false;
			}
		}
		return false;
	}
	bool GameScene::BiteSelf()
	{
		for (auto iter = m_Head->next; iter != NULL; iter = iter->next)
		{
			if (m_Head->pos == iter->pos)
			{
				return true;
			}
		}
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
			MutexLock::Lock("Snake");
			auto newHead = new Node();
			newHead->pos = m_Head->pos + v;
			newHead->next = m_Head;
			newHead->last = NULL;
			m_Head->last = newHead;
			m_Head = newHead;
			MutexLock::Unlock("Snake");

			MutexLock::Lock("Food");
			GenerateFood();
			MutexLock::Unlock("Food");
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
		if (Input::IsKeyPressing(GLFW_KEY_LEFT) && m_Dir != HeadDir::right)
		{
			m_Dir = HeadDir::left;
		}
		if (Input::IsKeyPressing(GLFW_KEY_RIGHT) && m_Dir != HeadDir::left)
		{
			m_Dir = HeadDir::right;
		}
		if (Input::IsKeyPressing(GLFW_KEY_UP) && m_Dir != HeadDir::down)
		{
			m_Dir = HeadDir::up;
		}
		if (Input::IsKeyPressing(GLFW_KEY_DOWN) && m_Dir != HeadDir::up)
		{
			m_Dir = HeadDir::down;
		}
	}
	void GameScene::Init()
	{
		m_InterruptFlag = false;
		m_First = true;
		m_MoveTimer = 0.0;
		m_Tail = m_Head = new Node();
		m_Head->next = NULL;
		m_Head->last = NULL;
		m_Head->pos = glm::vec2(float(m_Width/2), float(m_Height/2));
		GenerateFood();
	}
	void GameScene::GameOver()
	{
		if(m_Head)
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
	}
	inline bool GameScene::IsGetFood()
	{
		return m_Head->pos == m_FoodPos;
		
	}
	GameScene::GameScene(float speed, bool hitWall, int width, int height)
		:m_Speed(speed), m_HitWall(hitWall), m_Width(width), m_Height(height)
	{
		m_Head = NULL;
		m_Tail = NULL;
		m_Dir = HeadDir::right;
		srand(time(0));
		Init();
	}
	GameScene::~GameScene()
	{
		GameOver();
	}
	void GameScene::Update()
	{
		while (!GetInterruptFlag())
		{
			std::cout << "[info] Game Running.\n";
			TimerUpdate();
			GetInput();
			m_MoveTimer += m_DeltaTime;
			if (m_MoveTimer >= m_Speed)
			{
				Step();
				m_MoveTimer = 0.0;
			}
			if (HitWall())
			{
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
	void GameScene::Interrupt()
	{
		MutexLock::Lock("InterruptFlag");
		m_InterruptFlag = true;
		MutexLock::Unlock("InterruptFlag");
	}
	void GameScene::Coutinue()
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
}