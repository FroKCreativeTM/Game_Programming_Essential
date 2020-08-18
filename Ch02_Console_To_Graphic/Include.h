#pragma once

// 이 파일은 포함 파일들을 추가하기 위한 헤더 파일입니다.
// C/C++ 런타임 헤더
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

// 에러 메시지 출력용
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// 프레임워크 관련 헤더
#include <GameLib/2DGraphics1/src/Framework.h>

// 게임을 위한 추가 헬퍼 헤더
#include "Array2D.h"
#include "Array2D.cpp"
#include "Object.h"
#include "CFileSystem.h"