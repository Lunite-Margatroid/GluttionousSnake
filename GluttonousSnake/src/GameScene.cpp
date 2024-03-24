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

		if (m_Tail == m_Head)
		{
			m_Tail->pos += v;
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
	}
	void GameScene::GetInput()
	{
		if (Input::IsKeyPressing(GLFW_KEY_LEFT))
		{
			m_Dir = HeadDir::left;
		}
		if (Input::IsKeyPressing(GLFW_KEY_RIGHT))
		{
			m_Dir = HeadDir::right;
		}
		if (Input::IsKeyPressing(GLFW_KEY_UP))
		{
			m_Dir = HeadDir::up;
		}
		if (Input::IsKeyPressing(GLFW_KEY_DOWN))
		{
			m_Dir = HeadDir::down;
		}
	}
	void GameScene::Init()
	{
		m_First = true;
		m_MoveTimer = 0.0;
		m_Tail = m_Head = new Node();
		m_Head->next = NULL;
		m_Head->last = NULL;
		m_Head->pos = glm::vec2(float(m_Width/2), float(m_Height/2));
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
	GameScene::GameScene(float speed, bool hitWall, int width, int height)
		:m_Speed(speed), m_HitWall(hitWall), m_Width(width), m_Height(height)
	{
		m_Head = NULL;
		m_Tail = NULL;
		m_Dir = HeadDir::right;
		Init();
	}
	GameScene::~GameScene()
	{
		GameOver();
	}
	void GameScene::Update()
	{
		TimerUpdate();
		GetInput();
		m_MoveTimer += m_DeltaTime;
		if (m_MoveTimer >= m_Speed)
		{
			Step();
			m_MoveTimer = 0.0;
		}
	}
	void GameScene::ForEach(std::function<void(const glm::vec2&)> func)
	{
		for (auto iter = m_Head; iter != NULL; iter = iter->next)
		{
			func(iter->pos);
		}
	}
}