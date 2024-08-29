#pragma once

#include <array>
#include <memory>

#include "tilemap.h"
#include "interface_drawable.h"

namespace game
{
	constexpr size_t CHUNK_WIDTH = 64;
	constexpr size_t CHUNK_HEIGHT = 64;
	constexpr size_t CHUNK_SIZE = CHUNK_WIDTH * CHUNK_HEIGHT;

	class Chunk : public IDrawable
	{
	public:
		explicit Chunk(int chunkX, int chunkY);
		~Chunk() = default;

		TileType getTile(size_t x, size_t y) const;

		void setTile(size_t x, size_t y, TileType tile = TileType::NONE);

		void render() const override;

		int getChunkX() const;
		int getChunkY() const;

		void fillAll(TileType type);
	private:
		size_t getTileIndex(size_t x, size_t y) const;

		int chunkX, chunkY;

		TileType tiles[CHUNK_SIZE];
	};
}
