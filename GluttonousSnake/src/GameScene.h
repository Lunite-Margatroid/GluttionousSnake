#pragma once
#include "Input.h"
namespace GS
{
	struct Node
	{
		glm::vec2 pos;
		Node* next;
	};

	enum class HeadDir
	{
		left, right, up, down
	};

	class GameScene
	{
	private:
		Node* m_Head;
		Node* m_Tail;

		int m_Width;
		int m_Height;

		bool m_HitWall;

		float m_MoveTimer;
		float m_Speed; // Ãë/¸ñ×Ó

		float m_DeltaTime;
		float m_CurrentTime;
		float m_LastTime;

		glm::vec2 m_FoodPos;

		void TimerUpdate();
		bool HitWall();
		bool BiteSelf();
		void GenerateFood();
		void Step();
		void GetInput();
	public:
		GameScene(float m_Speed = 1.0f, bool hitWall =false, int width = 30, int height = 30);
		~GameScene();
		void Update();
		void ForEach(std::function<void(const glm::vec2&)> func);
		
	};
}