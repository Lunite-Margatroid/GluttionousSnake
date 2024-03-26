#pragma once
#include "Input.h"
#include "MutexLock.h"
namespace GS
{
	struct Node
	{
		glm::vec2 pos;
		Node* next, *last;
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

		int m_Length;

		double m_MoveTimer;
		double m_Speed; // Ãë/¸ñ×Ó

		double m_DeltaTime;
		double m_CurrentTime;
		double m_LastTime;

		glm::vec2 m_FoodPos;

		HeadDir m_Dir;

		bool m_InterruptFlag;

		bool m_First;
		void TimerUpdate();
		bool HitWall();
		bool BiteSelf();
		void GenerateFood();
		void Step();
		void GetInput();
		void Init();
		void GameOver();
		bool IsGetFood();

		inline std::pair<float, float> GenerateRand()
		{
			return std::make_pair(float(rand() % m_Width), float(rand() % m_Height));
		}
	public:
		GameScene(float Speed = 1.0f, bool hitWall =false, int width = 30, int height = 30);
		~GameScene();
		void Update();
		void ForEach(std::function<void(const glm::vec2&)> func);
		const glm::vec2& GetFoodPos()const;
		bool IsGameOver();
		void Reset();
		void Interrupt();
		void Coutinue();
		bool GetInterruptFlag() const;
	};
}