#pragma once
#include "Tetris.h"
//_TChunkHub _default_tetris_tchunk_hub(7);
#include <algorithm>


#define _TetrominoPosMacro_0	POINT{0, 0}, POINT{0, 1}, POINT{0, 2}, POINT{0, 3}
#define _TetrominoPosMacro_1	POINT{1, 0}, POINT{1, 1}, POINT{0, 2}, POINT{1, 2}
//#define _TetrominoPosMacro_2	POINT{0, 0}, POINT{0, 1}, POINT{0, 2}, POINT{1, 2}
#define _TetrominoPosMacro_2	POINT{1, 0}, POINT{1, 1}, POINT{1, 2}, POINT{2, 2}
#define _TetrominoPosMacro_3	POINT{1, 0}, POINT{0, 1}, POINT{1, 1}, POINT{0, 2}
#define _TetrominoPosMacro_4	POINT{0, 0}, POINT{0, 1}, POINT{1, 1}, POINT{1, 2}
//#define _TetrominoPosMacro_5	POINT{0, 0}, POINT{1, 0}, POINT{2, 0}, POINT{1, 1}
#define _TetrominoPosMacro_5	POINT{0, 1}, POINT{1, 1}, POINT{2, 1}, POINT{1, 2}
#define _TetrominoPosMacro_6	POINT{0, 0}, POINT{1, 0}, POINT{0, 1}, POINT{1, 1}

namespace __DUMMY_TChunkHub
{
	struct _default_tetris_tchunk_hub_t
	{
		_default_tetris_tchunk_hub_t()
			: _default_tetris_tchunk_hub_value(7)
		{
			auto vecGene = [](POINT _0, POINT _1, POINT _2, POINT _3)->_TChunk::data_container_t
			{
				_TChunk::data_container_t temp(4);
				temp[0] = _0;
				temp[1] = _1;
				temp[2] = _2;
				temp[3] = _3;
				return temp;
			};
			auto toSizeX = [](POINT _0, POINT _1, POINT _2, POINT _3)->long { return std::max(std::max(std::max(_0.x, _1.x), _2.x), _3.x) + 1; };
			auto toSizeY = [](POINT _0, POINT _1, POINT _2, POINT _3)->long { return std::max(std::max(std::max(_0.y, _1.y), _2.y), _3.y) + 1; };
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_0), toSizeX(_TetrominoPosMacro_0), toSizeY(_TetrominoPosMacro_0))));	// ¦¢
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_1), toSizeX(_TetrominoPosMacro_1), toSizeY(_TetrominoPosMacro_1))));	// ¦¤
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_2), toSizeX(_TetrominoPosMacro_2), toSizeY(_TetrominoPosMacro_2))));	// ¦£
			//_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_2), 3, 3)));	// ¦£
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_3), toSizeX(_TetrominoPosMacro_3), toSizeY(_TetrominoPosMacro_3))));	// ¦¦¦¤
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_4), toSizeX(_TetrominoPosMacro_4), toSizeY(_TetrominoPosMacro_4))));	// ¦£¦¥
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_5), toSizeX(_TetrominoPosMacro_5), toSizeY(_TetrominoPosMacro_5))));	// ¦ª
			_default_tetris_tchunk_hub_value.insert(std::move(_TChunk(vecGene(_TetrominoPosMacro_6), toSizeX(_TetrominoPosMacro_6), toSizeY(_TetrominoPosMacro_6))));	// ¡à
		}
		_TChunkHub _default_tetris_tchunk_hub_value;
	} inline _default_tetris_tchunk_hub_wrapper;
}

inline const _TChunkHub& _default_tetris_tchunk_hub() { return __DUMMY_TChunkHub::_default_tetris_tchunk_hub_wrapper._default_tetris_tchunk_hub_value; }
