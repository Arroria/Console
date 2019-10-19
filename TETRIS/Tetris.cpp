#include "Tetris.h"

_TChunk::_TChunk(_TChunk&& _moved_tchunk)
	: m_blockPos(std::move(_moved_tchunk.m_blockPos))
	, m_size(_moved_tchunk.m_size)
{
}

_TChunk::_TChunk(data_container_t tchunkBase, size_t sizeX, size_t sizeY)
	: m_blockPos{ std::move(tchunkBase), }
	, m_size(std::max(sizeX, sizeY))
{
	auto& _0 = m_blockPos[0];
	auto& _90 = m_blockPos[1];
	auto& _180 = m_blockPos[2];
	auto& _270 = m_blockPos[3];

	size_t sizeOf_0 = _0.size();
	_90.resize(sizeOf_0);
	_180.resize(sizeOf_0);
	_270.resize(sizeOf_0);

	const size_t dataOffsetY = (m_size - sizeY) / 2;
	const size_t dataOffsetX = (m_size - sizeX) / 2;
	const size_t dataMax = m_size - 1;
	for (int i = 0; i < _0.size(); ++i)
	{
		// _0 modify
		_0[i].x += dataOffsetX;
		_0[i].y += dataOffsetY;

		// _90 modify
		_90[i].x = _0[i].y;
		_90[i].y = dataMax - _0[i].x;

		// _180 modify
		_180[i].x = dataMax - _0[i].x;
		_180[i].y = dataMax - _0[i].y;

		// _270 modify
		_270[i].x = dataMax - _0[i].y;
		_270[i].y = _0[i].x;
	}
}

_TChunk::~_TChunk()
{
}




Tetris::Tetris(size_t maxX, size_t maxY, const _TChunkHub* const tchunkHub)
	: m_tchunkHub(tchunkHub)

	, m_maxX(maxX)
	, m_maxY(maxY)
	, m_map(maxX * maxY, _false)

	, m_controllerChunk(nullptr)
	, m_controllerIndex(NULL)
	, m_controllerPos{ NULL, NULL }
{
	ControllerTchunkAllocate();
}

Tetris::~Tetris()
{
}



bool Tetris::ControllerTeleport(int x, int y)
{
	if (ControllerInteractionValidation(x, y, m_controllerIndex))
	{
		m_controllerPos.x += x;
		m_controllerPos.y += y;
		return true;
	}
	return false;
}

bool Tetris::ControllerRotationL()
{
	const int nextIndex = m_controllerIndex == 0 ? 3 : m_controllerIndex - 1;
	if (ControllerInteractionValidation(0, 0, nextIndex))
	{
		m_controllerIndex = nextIndex;
		return true;
	}
	return false;
}

bool Tetris::ControllerRotationR()
{
	const int nextIndex = m_controllerIndex == 3 ? 0 : m_controllerIndex + 1;
	if (ControllerInteractionValidation(0, 0, nextIndex))
	{
		m_controllerIndex = nextIndex;
		return true;
	}
	return false;
}

bool Tetris::ControllerInteractionValidation(int moveX, int moveY, int tchunkIndex)
{
	auto& ctd = _controllerTchunkData(tchunkIndex);
	for (auto iter = ctd.begin(); iter != ctd.end(); ++iter)
	{
		const int calculateX = m_controllerPos.x + iter->x + moveX;
		const int calculateY = m_controllerPos.y + iter->y + moveY;

		if (_checkPlaceUnsafe(calculateX, calculateY))
			return false;
	}
	return true;
}


#include <set>
void Tetris::ControllerEmplace()
{
	std::set<size_t> lineCheck;

	// 맵에 블럭 값 배치
	auto& nctd = _nowControllerTchunkData();
	for (auto iter = nctd.begin(); iter != nctd.end(); ++iter)
	{
		size_t calculateX = m_controllerPos.x + iter->x;
		size_t calculateY = m_controllerPos.y + iter->y;
		_data(calculateX, calculateY) = _true;
		
		lineCheck.insert(calculateY);
	}

	// 줄 제거
	for (auto iter = lineCheck.rbegin(); iter != lineCheck.rend(); iter++)
	{
		size_t line = *iter;
		if (LineCheck(line))
			LineErase(line);
	}
	
	// 컨트롤러에 새로운 T청크 할당
	ControllerTchunkAllocate();
}



bool Tetris::LineCheck(size_t line) const
{
	for (size_t i = 0; i < m_maxX; i++)
	{
		if (_data(i, line) == _false)
			return false;
	}
	return true;
}

void Tetris::LineErase(size_t line)
{
	for (size_t y = line + 1; y < m_maxY; y++)
	{
		for (size_t x = 0; x < m_maxX; x++)
		{
			_data(x, y - 1) = _data(x, y);

			if (y + 1 == m_maxY)
				_data(x, y) == _false;
		}
	}
}



void Tetris::Render(ConsoleDoubleBuffer& cdb)
{
	cdb.Clear();

	auto _drawPiece = [this, &cdb](size_t x, size_t y, int piecePlaced)
	{
		cdb.SetCursorPos(x * 2, m_maxY - y - 1);
		std::cout << (piecePlaced == _true ? "■" : "□");
	};

	size_t mapSize = m_map.size();
	for (size_t mapIndex = 0; mapIndex < mapSize; ++mapIndex)
	{
		int data = m_map[mapIndex];
		size_t posY = mapIndex / m_maxX;
		size_t posX = mapIndex - posY * m_maxX;

		_drawPiece(posX, posY, data);
	}

	auto& tchunkData = m_controllerChunk->data(m_controllerIndex);
	for (auto iter = tchunkData.begin(); iter != tchunkData.end(); ++iter)
		_drawPiece(m_controllerPos.x + iter->x, m_controllerPos.y + iter->y, _true);

	cdb.Flipping(); 
}



void Tetris::ControllerTchunkAllocate()
{
	m_controllerChunk = _random_tchunk_by_hub();
	m_controllerIndex = 0;
	m_controllerPos.x = float(m_maxX) / 2.f - m_controllerChunk->focus();
	m_controllerPos.y = m_maxY - m_controllerChunk->chunkSize();
}
