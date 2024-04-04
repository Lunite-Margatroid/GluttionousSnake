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
		none, left, right, up, down
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
		double m_Speed;			// 秒/格子

		double m_DeltaTime;
		double m_CurrentTime;
		double m_LastTime;

		glm::vec2 m_FoodPos;

		HeadDir m_Dir;
		HeadDir m_NextInput;

		bool m_InterruptFlag;

		bool m_First;

		bool m_GameOver;

		bool m_Run;

		int m_Lengthen;

		int m_Lengthening;

		bool m_GetInput;

		void TimerUpdate();
		bool HitWall();
		bool BiteSelf();
		void GenerateFood();
		void Step();
		void GetInput();
		void GetNextInput();
		void Init();
		void GameOver();
		bool IsGetFood();
		void Clear();

		inline std::pair<float, float> GenerateRand()
		{
			return std::make_pair(float(rand() % m_Width), float(rand() % m_Height));
		}
	public:
		// speed的单位是秒/格  越小越快
		GameScene(float Speed = 1.0f, bool hitWall =false, int width = 30, int height = 30, int lengthen = 1);
		~GameScene();
		void Update();
		void ForEach(std::function<void(const glm::vec2&)> func);
		const glm::vec2& GetFoodPos()const;
		bool IsGameOver() const;
		void Reset();
		void Interrupt();
		void Continue();
		bool GetInterruptFlag() const;
		bool IsRun() const;
		void GameThreadOver();
		void SetSpeed(double speed);
		double GetSpeed() const;
		void SetLengthen(int lengthen);
		int GetLengthen() const;
		bool GetHitWall() const;
		void SetHitWall(bool hitWall);
	};
}