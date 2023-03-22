#pragma once

#include "TileMap.h"

struct Layout
{
    Layout(const TileMap::TileType* _leveldata, IVector2 _leveldimensions) :
        m_LevelDimensions(_leveldimensions), m_LevelData(_leveldata) {};

    Layout(const Layout* srcLayout) :
        m_LevelDimensions(srcLayout->m_LevelDimensions), m_LevelData(srcLayout->m_LevelData) {};

    IVector2 m_LevelDimensions;
    const TileMap::TileType* m_LevelData;

    ~Layout() {}
};

extern const Layout k_Layout_1;
extern const Layout k_Layout_2;