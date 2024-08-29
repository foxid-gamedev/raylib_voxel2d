#include "pch.h"
#include "chunk.h"

game::Chunk::Chunk(int chunkX, int chunkY) :
    chunkX(chunkX), chunkY(chunkY)
{
    fillAll(TileType::NONE);
}

game::TileType game::Chunk::getTile(size_t x, size_t y) const
{
    if (x < CHUNK_WIDTH && y < CHUNK_HEIGHT)
    {
        return tiles[getTileIndex(x, y)];
    }
    return TileType::NONE;
}

void game::Chunk::setTile(size_t x, size_t y, TileType tile)
{
    if (x < CHUNK_WIDTH && y < CHUNK_HEIGHT)
    {
        tiles[getTileIndex(x, y)] = tile;
    }
}

void game::Chunk::render() const
{
    float worldX = chunkX * CHUNK_WIDTH * TILEMAP_CELL_SIZE;
    float worldY = chunkY * CHUNK_HEIGHT * TILEMAP_CELL_SIZE;

    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (int x = 0; x < CHUNK_WIDTH; x++)
        {
            draw_tile_rect
            (
                worldX + x * TILEMAP_CELL_SIZE, 
                worldY + y * TILEMAP_CELL_SIZE, 
                tiles[getTileIndex(x, y)]
            );
        }
    }
}

int game::Chunk::getChunkX() const
{
    return chunkX;
}

int game::Chunk::getChunkY() const
{
    return chunkY;
}

void game::Chunk::fillAll(game::TileType type)
{
    for (size_t i = 0; i < CHUNK_SIZE; ++i)
    {
        tiles[i] = type;
    }
}

size_t game::Chunk::getTileIndex(size_t x, size_t y) const
{
    return y * CHUNK_WIDTH + x;
}
