#pragma once

enum Object;
template <typename T>
class Array2D;

// 게임 관련 전반을 관리한다.
class CState
{
private : 
	int m_nWidth;
	int m_nHeight;
	Array2D<Object> m_Objects;
	Array2D<bool> m_GoalFlags;

public : 
	CState(char* stageData, int fileSize);

private : 
	void SetSize(const char* stageData, int size);
	static void DrawCell(int x, int y, unsigned color);

public : 
	bool checkClear();
	void Draw();
	void Update(char input);
};

