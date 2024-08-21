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
		DIRT = 1,
		GRASS = 2,
		STONE = 3,
	};

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
