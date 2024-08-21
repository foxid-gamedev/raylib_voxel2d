#include "pch.h"

#include "tilemap.h"

#include "FastNoiseLite.h"

float round_to_snap(float value, float cell_size);

game::Tilemap::Tilemap()
	: data(std::make_unique<std::array<game::TileType, TILEMAP_SIZE>>()) {

}

game::Tilemap::~Tilemap()
{
}

void game::Tilemap::render() const
{
	assert(data != nullptr);
	Vector2 cell_size = { TILEMAP_CELL_SIZE, TILEMAP_CELL_SIZE };

	size_t row = 0;
	for (size_t i = 0; i < TILEMAP_SIZE; i++) 
	{
		if (i - TILEMAP_WIDTH * row >= TILEMAP_WIDTH)
			row++;
		
		float x = static_cast<float>(i % TILEMAP_WIDTH)*(cell_size.x);
		float y = static_cast<float>(row) * cell_size.y;

		const TileType& tile = (*data)[i];

		if (tile == TileType::NONE)
			continue;

		switch (tile) {
			case TileType::GRASS: 
				DrawRectangleV({ x,y }, {cell_size.x, cell_size.y *0.2f}, GREEN);
				DrawRectangleV({ x,y + 0.2f * cell_size.y }, { cell_size.x, cell_size.y * 0.8f }, BROWN);
				break;
			case TileType::DIRT: 
				DrawRectangleV({ x,y }, cell_size, BROWN);
				break;
			case TileType::STONE:
				DrawRectangleV({ x,y }, cell_size, DARKGRAY);
				break;
		}

		DrawRectangleLinesEx({ x, y, cell_size.x, cell_size.y }, 0.8f, {0,0,0,18});
	}
}

game::TileType game::Tilemap::getCellType(size_t x, size_t y) const
{
	assert(data != nullptr);
	return (*data)[getCellIndex(x, y)];
}

void game::Tilemap::setCell(size_t x, size_t y, TileType tile)
{
	assert(data != nullptr);
	(*data)[getCellIndex(x,y)] = tile;
}

void game::Tilemap::generateCells()
{
	assert(data != nullptr);

	for (int i = 0; i < TILEMAP_SIZE; i++)
	{
		(*data)[i] = TileType::NONE;
	}

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.1);
	noise.SetSeed(static_cast<int>(std::time(0)));

	Vector2 cell_size = { TILEMAP_CELL_SIZE, TILEMAP_CELL_SIZE };

	int height = 64;

	for (int x = 0; x < TILEMAP_WIDTH; x++)
	{
		float weight = noise.GetNoise(static_cast<float>(x), static_cast<float>(height));
		float value = util::remap(weight, -1.0f, 1.0f, -7.0f, 7.0f);

		height += static_cast<int>(value);

		height = std::clamp(height, 64, 92);

		for (int y = 127; y > height+3; y--)
		{
			setCell(x, y, TileType::STONE);
		}

		setCell(x, height + 1, TileType::DIRT);
		setCell(x, height + 2, TileType::DIRT);
		setCell(x, height + 3, TileType::DIRT);

		setCell(x, height, TileType::GRASS);
	}
}

Vector2 game::Tilemap::getNearestCell(const Vector2& worldPos) const
{
	return { 
		round_to_snap(worldPos.x, TILEMAP_CELL_SIZE), 
		round_to_snap(worldPos.y, TILEMAP_CELL_SIZE) 
	};
}

size_t game::Tilemap::getCellIndex(size_t x, size_t y) const
{
	assert(data != nullptr);
	size_t index = y * TILEMAP_WIDTH + x;
	assert(index >= 0 && index < TILEMAP_SIZE);
	return index;
}

bool game::Tilemap::checkCollision(float _x, float _y, float _w, float _h) const
{
	constexpr int LOOK_RANGE_X = 2;
	constexpr int LOOK_RANGE_Y = 1;
	constexpr int WORLD_BORDER = static_cast<int>(TILEMAP_WIDTH-1);

	Rectangle worldRect = { _x,_y, _w, _h };
	Vector2 cellPos = getNearestCell({ _x,_y });

	int cellX = static_cast<int>(cellPos.x);
	int cellY = static_cast<int>(cellPos.y);

	// used tiles:
	// [][][][][]
	// [][]><[][]   x-2, x+2;
	// [][][][][]	y-1, y+1;
 
	for (int y = std::max(0, cellY - LOOK_RANGE_Y); y <= std::min(WORLD_BORDER, cellY + LOOK_RANGE_Y); y++)
	{
		for (int x = std::max(0, cellX - LOOK_RANGE_X); x <= std::min(WORLD_BORDER, cellX + LOOK_RANGE_X); x++)
		{
			TileType type = getCellType(static_cast<size_t>(x), static_cast<size_t>(y));
			
			if (type == TileType::NONE)
				continue;

			Rectangle cellRect = {
				x * TILEMAP_CELL_SIZE,
				y * TILEMAP_CELL_SIZE,
				TILEMAP_CELL_SIZE,
				TILEMAP_CELL_SIZE
			};

			if (CheckCollisionRecs(worldRect, cellRect))
				return true;
		}
	}
	return false;
}

float round_to_snap(float value, float cell_size)
{
	assert(cell_size > 0);
	float w = value / cell_size;
	float a = std::floorf(w);
	float b = std::ceilf(w);

	if (std::abs(w - a) <= std::abs(w - b))
		return a;
	else
		return b;
}
