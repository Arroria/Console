#pragma once
#include <vector>
#include <array>

#define NOMINMAX
#include <Windows.h>

class _TChunk
{
public:
	using index_t = int;
	using data_container_t = std::vector<POINT>;
	_TChunk(_TChunk&& _moved_tchunk);
	_TChunk(data_container_t tchunkBase, size_t sizeX, size_t sizeY);
	~_TChunk();

	const data_container_t& data(index_t index) const { return m_blockPos[index]; }
	const data_container_t& operator[](index_t index) const { return data(index); }
	size_t chunkSize() const { return m_size; }
	float focus() const { return float(chunkSize()) / 2.f; }

private:
	std::array<data_container_t, 4> m_blockPos;
	size_t m_size;
};

class _TChunkHub
{
public:
	_TChunkHub() {}
	_TChunkHub(size_t size) : m_tchunkList() { m_tchunkList.reserve(size); }
	~_TChunkHub() {}


	void insert(_TChunk tchunk) { m_tchunkList.push_back(std::move(tchunk)); }
	const _TChunk* data(size_t index) const { return std::addressof(m_tchunkList[index]); }
	const _TChunk* operator[](size_t index) const { return data(index); }

	std::vector<_TChunk>::const_iterator begin() const { return m_tchunkList.cbegin(); }
	std::vector<_TChunk>::const_iterator end() const { return m_tchunkList.cend(); }
	size_t size() const { return m_tchunkList.size(); }

private:
	std::vector<_TChunk> m_tchunkList;
};



#include "defaultTchunkHub.inl"
#include "../console doublebuffering(2)/ConsoleDoubleBuffer.h"
class Tetris
{
public:
	static constexpr int _true = 1;
	static constexpr int _false = 0;

	Tetris(size_t maxX, size_t maxY, const _TChunkHub* const tchunkHub = std::addressof(_default_tetris_tchunk_hub()));
	~Tetris();


	// Controller Movement
	bool ControllerMoveL() { return ControllerTeleport(-1, 0); }
	bool ControllerMoveR() { return ControllerTeleport(+1, 0); }
	bool ControllerDown() { return ControllerTeleport(0, -1); }
	bool ControllerTeleport(int x, int y);
	// Controller Rotation
	bool ControllerRotationL();
	bool ControllerRotationR();
	// Controller Base Controll Func
	bool ControllerInteractionValidation(int moveX, int moveY, int tchunkIndex);

	void ControllerEmplace();


	bool LineCheck(size_t line) const;
	void LineErase(size_t line);


	void Render(ConsoleDoubleBuffer& cdb);



private:
	const _TChunkHub* const m_tchunkHub;

	const size_t m_maxX;
	const size_t m_maxY;
	std::vector<int> m_map;

	const _TChunk* m_controllerChunk;
	_TChunk::index_t m_controllerIndex;
	POINT m_controllerPos;

	//const size_t m_max_nextBlock;
	//std::queue<_TetrisBlock> m_nextBlock;
	//
	//bool m_nowReserved;
	//_TetrisBlock m_reservedBlock;



	void ControllerTchunkAllocate();


	
	// 간결한 함수들
	inline bool _checkPlaceUnsafe(int x, int y) const { return _outOfMap(x, y) || _data(x, y) == _true; }

	inline auto& _controllerTchunkData(_TChunk::index_t index)	{ return m_controllerChunk->data(index); }
	inline auto& _nowControllerTchunkData()						{ return _controllerTchunkData(m_controllerIndex); }

	inline int& _data(size_t x, size_t y)		{ return m_map.at(x + y * m_maxX); }
	inline int _data(size_t x, size_t y) const	{ return m_map.at(x + y * m_maxX); }
	inline bool _outOfMap(int x, int y) const	{ return x < 0 || m_maxX <= x || y < 0 || m_maxY <= y; }


	int __random(int max) const { return rand() % max; }
	const _TChunk* _random_tchunk_by_hub() const { return m_tchunkHub->data(__random(m_tchunkHub->size())); }
};


extern int a;