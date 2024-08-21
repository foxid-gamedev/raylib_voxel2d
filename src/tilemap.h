#pragma once

#include <memory>
#include <array>

#include "interface_collision_check.h"

namespace game
{
	constexpr size_t TILEMAP_WIDTH = 256;
	constexpr size_t TILEMAP_HEIGHT = 128;
	constexpr float TILEMAP_CELL_SIZE = 16.f;
	constexpr size_t TILEMAP_SIZE = TILEMAP_WIDTH * TILEMAP_HEIGHT;

	enum class TileType 
	{
		NONE = 0,
		GRASS = 1,
		DIRT = 2,
		STONE = 3,
	};


	inline void draw_tile_rect(float x, float y, TileType type)
	{
		switch (type)
		{
		case TileType::NONE:
			return;
		case TileType::GRASS:
			DrawRectangleGradientH(static_cast<int>(x), static_cast<int>(y), static_cast<int>(TILEMAP_CELL_SIZE), static_cast<int>(TILEMAP_CELL_SIZE * 0.2f), GREEN, DARKGREEN);
			DrawRectangle(static_cast<int>(x), static_cast<int>(y + TILEMAP_CELL_SIZE * 0.2f), static_cast<int>(TILEMAP_CELL_SIZE), static_cast<int>(TILEMAP_CELL_SIZE * 0.8f), BROWN);
			break;
		case TileType::DIRT:
			DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(TILEMAP_CELL_SIZE), static_cast<int>(TILEMAP_CELL_SIZE), BROWN);
			break;
		case TileType::STONE:
			DrawRectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(TILEMAP_CELL_SIZE), static_cast<int>(TILEMAP_CELL_SIZE), DARKGRAY);
			break;
		default:
			break;
		}
	}

	class Tilemap : public ICollisionCheck
	{
	public:
		explicit Tilemap();
		~Tilemap();

		void render() const;

		void setCell(size_t x, size_t y, TileType tile = TileType::NONE);

		void generateCells();

		TileType getCellType(size_t x, size_t y) const;
		Vector2 getNearestCell(const Vector2& worldPos) const;
		bool checkCollision(float x, float y, float w, float h) const override;
	private:
		size_t getCellIndex(size_t x, size_t y) const;
		std::unique_ptr<std::array<TileType, TILEMAP_SIZE>> data;

	};
}
