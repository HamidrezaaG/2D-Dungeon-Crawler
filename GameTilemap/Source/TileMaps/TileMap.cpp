#include "GamePCH.h"
#include "TileMap.h"
#include "Layouts.h"
#include "Game.h"

TileMap::TileMap(fw::Mesh* _pMesh, fw::Camera* _pcamera, fw::ShaderProgram* _pshader, fw::Vector2 _offset, Game* _pgame) :
    m_pLayout(nullptr), m_DrawData(), m_pGame(_pgame), m_pShader(_pshader), m_pTileMesh(_pMesh), m_pCamera(_pcamera), m_Properties(), m_WalkableTiles()
{
    m_Offset = _offset;
    // Properties
    {
        m_Properties[(int)TileType::Floor] = TileProperties("Floor" , false, false, false);
        m_Properties[(int)TileType::Wall]  = TileProperties("Wall"  , true , false, false);
        m_Properties[(int)TileType::Empty] = TileProperties("Empty" , true , false, false);
        m_Properties[(int)TileType::Column]= TileProperties("Column", true , true , false);
        m_Properties[(int)TileType::Box]   = TileProperties("Box"   , false, true , true );
        m_Properties[(int)TileType::Door]  = TileProperties("Door"  , true , false, false);
    }
}

bool TileMap::GetIsWalkable(int x, int y)
{
    if (x < 0 || x >= m_LayoutDimensions.x || y < 0 || y >= m_LayoutDimensions.y)
        return false;

    return m_WalkableTiles.at((size_t)x + ((size_t)m_LayoutDimensions.y - 1 - (size_t)y) * (size_t)m_LayoutDimensions.x);
}

void TileMap::LoadLayout(const Layout* _pLayout)
{
    assert(_pLayout != nullptr);

    { // INIT
        if (m_pLayout != nullptr)
            delete m_pLayout;

        for (int i = 0; i < m_DrawData.size(); i++)
        {
            m_DrawData.erase(m_DrawData.begin() + i);
        }

        m_pLayout = new Layout(_pLayout);

        assert(!m_pLayout->m_LevelDimensions.IsZero());

        m_WalkableTiles.clear();
    }

    m_LayoutDimensions = m_pLayout->m_LevelDimensions;

    for (int i = 0; i < m_LayoutDimensions.x * m_LayoutDimensions.y; i++)
    {
        //Skip empty tiles
        if (m_pLayout->m_LevelData[i] == TileType::Empty)
        {
            m_DrawData.push_back(nullptr);
            m_WalkableTiles.push_back(false);
            continue;
        }

        //Init variables
        int row = (m_LayoutDimensions.y - 1) - (i / (m_LayoutDimensions.x));
        int col = (i % m_LayoutDimensions.x);

        TileProperties properties;
        properties = m_Properties[(int)(m_pLayout->m_LevelData[i])];

        bool TileHasCollider = properties.HasCollider;
        bool TileHasFlooring = properties.HasFloorUnderneath;
        Vector2 pos = m_Offset + Vector2(col * m_ScaleAdjustment.x,
                                         row * m_ScaleAdjustment.y);
        std::string spriteName = properties.SpriteName;
        std::string WallFlagString = "_";
        unsigned char ScanFlags = 0;
        unsigned char AdjacentEmptyCount = 0;
        unsigned char AdjacentWallCount = 0;

        //Read adjacent walls and empties
        {
            TileType checkedTile;
            checkedTile = GetTile(col, row + 1, m_LayoutDimensions);
            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
            {
                WallFlagString.append("T");
                ScanFlags += Mask_Wall_Top;
                AdjacentWallCount++;
            }
            else if (checkedTile == TileType::Empty)
            {
                ScanFlags += Mask_Empty_Top;
                AdjacentEmptyCount++;
            }

            checkedTile = GetTile(col - 1, row, m_LayoutDimensions);
            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
            {
                WallFlagString.append("L");
                ScanFlags += Mask_Wall_Left;
                AdjacentWallCount++;
            }
            else if (checkedTile == TileType::Empty)
            {
                ScanFlags += Mask_Empty_Left;
                AdjacentEmptyCount++;
            }

            checkedTile = GetTile(col, row - 1, m_LayoutDimensions);
            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
            {
                WallFlagString.append("B");
                ScanFlags += Mask_Wall_Bottom;
                AdjacentWallCount++;
            }
            else if (checkedTile == TileType::Empty)
            {
                ScanFlags += Mask_Empty_Bottom;
                AdjacentEmptyCount++;
            }

            checkedTile = GetTile(col + 1, row, m_LayoutDimensions);
            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
            {
                WallFlagString.append("R");
                ScanFlags += Mask_Wall_Right;
                AdjacentWallCount++;
            }
            else if (checkedTile == TileType::Empty)
            {
                ScanFlags += Mask_Empty_Right;
                AdjacentEmptyCount++;
            }
        }

        //Define the name of each sprite based on its type and adjacent tiles
        switch (m_pLayout->m_LevelData[i])
        {
        case TileMap::TileType::Floor:
        {
            if (ScanFlags != 0)
            {
                if (WallFlagString[1] == 'B')
                {
                    WallFlagString = "";
                }
                else if (WallFlagString[2] == 'B')
                    WallFlagString.erase(WallFlagString.begin() + 2);
                else if (WallFlagString[3] == 'B')
                    WallFlagString.erase(WallFlagString.begin() + 3);
                else {
                    //WallFlagString = "";
                };

                if (WallFlagString != "_")
                    spriteName.append(WallFlagString);
            }
            break;
        }
        case TileMap::TileType::Door:
        {
            if (GetTile(col - 1, row, m_LayoutDimensions) == TileType::Door)
            {
                spriteName.append("_Wide_R");
            }
            else if (GetTile(col + 1, row, m_LayoutDimensions) == TileType::Door)
            {
                spriteName.append("_Wide_L");
            }
            else {
                spriteName.append("_Small");
            }
            break;
        }
        case TileMap::TileType::Column:
        {
            if (GetTile(col, row + 1, m_LayoutDimensions) == TileType::Column)
            {
                spriteName.append("_Bottom");
            }
            else {
                spriteName.append("_Top");
            }

            if (GetTile(col, row - 1, m_LayoutDimensions) == TileType::Column)
            {
                TileHasCollider = false;
            }

            TileHasFlooring = true;

            break;
        }
        case TileMap::TileType::Box:
        {
            spriteName.append("_Wood");
            TileHasFlooring = true;

            break;
        }
        case TileMap::TileType::Wall:
        {
            if (AdjacentWallCount == 2) {
                if (ScanFlags & Mask_Wall_Top && ScanFlags & Mask_Wall_Bottom || ScanFlags & Mask_Wall_Left && ScanFlags & Mask_Wall_Right)
                {
                    // STRIGHT WALL
                    spriteName.append("_Straight" + WallFlagString + "_Floor");

                    if (ScanFlags & Mask_Empty_Top)
                        spriteName.append("Below"); // empty above, therefore floor is below
                    else if (ScanFlags & Mask_Empty_Left)
                        spriteName.append("Right"); // so on and so fourth
                    else if (ScanFlags & Mask_Empty_Bottom)
                        spriteName.append("Above");
                    else
                        spriteName.append("Left");

                }
                else if (
                    ScanFlags & Mask_Wall_Top && ScanFlags & Mask_Wall_Left
                    ||
                    ScanFlags & Mask_Wall_Bottom && ScanFlags & Mask_Wall_Left
                    ||
                    ScanFlags & Mask_Wall_Top && ScanFlags & Mask_Wall_Right
                    ||
                    ScanFlags & Mask_Wall_Bottom && ScanFlags & Mask_Wall_Right
                    )
                    // CORNER WALL
                {
                    // IF THERE'S TWO EMPTIES, WE'RE AT AN INNER CORNER. IF THERE'S NO EMPTIES, IT'S AN OUTER CORNER
                    if (ScanFlags & (0xF0)) // all "empty" bits
                    {
                        // there's empties
                        spriteName.append("_Corner_Outer" + WallFlagString);
                    }
                    else {
                        // there's no empties
                        spriteName.append("_Corner_Inner" + WallFlagString);
                    }
                }
                else { // DEFAULT CASE
                    spriteName = "Wall_Straight_LR_FloorBelow";
                }
            }
            else {// DEFAULT CASE
                spriteName = "Wall_Straight_LR_FloorAbove";
            }
            break;
        }
        default:
            break;
        }

        fw::SpriteSheet::SpriteInfo* pfloorSprite = nullptr;
        //Draw a copy of a floor tile below objects that need it.
        if (TileHasFlooring)
        {
            if (ScanFlags != 0)
            {
                if (WallFlagString[1] == 'B')
                {
                    WallFlagString = "";
                }
                else if (WallFlagString[2] == 'B')
                    WallFlagString.erase(WallFlagString.begin() + 2);
                else if (WallFlagString[3] == 'B')
                    WallFlagString.erase(WallFlagString.begin() + 3);
            }
            else {
                WallFlagString = "";
            }

            pfloorSprite = m_pSpriteSheet->Sprites["Floor" + WallFlagString];
        }



        if (properties.IsADynamicObject)
        {
            DrawData* d = new DrawData(pos, m_pLayout->m_LevelData[i], m_pSpriteSheet->Sprites[spriteName], pfloorSprite, properties.Color);

            switch (m_pLayout->m_LevelData[i])
            {
            case TileMap::TileType::Box:
                
                m_pGame->AddObject(pos + Vector2(0.5f), 0.5f, m_pTexture, m_pSpriteSheet, m_pSpriteSheet->Sprites[spriteName], fw::k_ObjectType::Dynamic, "Crate");

                delete d;
                d = new DrawData(pos, m_pLayout->m_LevelData[i], pfloorSprite, nullptr, properties.Color);
                //m_DrawData.push_back(d);
                
                break;
            case TileMap::TileType::Door:
                break;
            default:
                break;
            }

            m_DrawData.push_back(d);
            m_WalkableTiles.push_back(!properties.HasCollider);
        }
        else
        {
            if (properties.HasCollider)
            {
                m_pGame->AddTileColliderAt(pos + Vector2(0.5f));
            }

            DrawData* d = new DrawData(pos, m_pLayout->m_LevelData[i], m_pSpriteSheet->Sprites[spriteName], pfloorSprite, properties.Color);
            m_DrawData.push_back(d);
            m_WalkableTiles.push_back(!properties.HasCollider);
        }
        


    }
}

//void TileMap::Draw()
//{
//    for (int i = 0; i < m_LayoutDimensions.x * m_LayoutDimensions.y; i++)
//    {
//        //Skip empty tiles
//        if (m_pLayout->m_LevelData[i] == TileType::Empty)
//            continue;
//
//        //Init variables
//        int row = (m_LayoutDimensions.y - 1) - (i / (m_LayoutDimensions.x) );
//        int col = (i % m_LayoutDimensions.x);
//        
//        TileProperties properties = m_Properties[(int)m_pLayout->m_LevelData[i]];
//        bool TileHasCollider = properties.HasCollider;
//        bool TileHasFlooring = properties.HasFloorUnderneath;
//        Vector2 pos = m_Offset + Vector2(col * m_Scale.x * m_pSpriteSheet->SpriteSize.x / fw::k_Window_Dimensions.x,
//                      row * m_Scale.y * m_pSpriteSheet->SpriteSize.y / fw::k_Window_Dimensions.y) * fw::k_World_Scale ;
//        std::string spriteName = properties.SpriteName;
//        std::string WallFlagString = "_";
//        unsigned char ScanFlags = 0;
//        unsigned char AdjacentEmptyCount = 0;
//        unsigned char AdjacentWallCount = 0;
//
//        //Read adjacent walls and empties
//        {
//            TileType checkedTile;
//            checkedTile = GetTile(col, row + 1, m_LayoutDimensions);
//            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
//            {
//                WallFlagString.append("T");
//                ScanFlags += Mask_Wall_Top;
//                AdjacentWallCount++;
//            }
//            else if (checkedTile == TileType::Empty)
//            {
//                ScanFlags += Mask_Empty_Top;
//                AdjacentEmptyCount++;
//            }
//
//            checkedTile = GetTile(col - 1, row, m_LayoutDimensions);
//            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
//            {
//                WallFlagString.append("L");
//                ScanFlags += Mask_Wall_Left;
//                AdjacentWallCount++;
//            }
//            else if (checkedTile == TileType::Empty)
//            {
//                ScanFlags += Mask_Empty_Left;
//                AdjacentEmptyCount++;
//            }
//
//            checkedTile = GetTile(col, row - 1, m_LayoutDimensions);
//            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
//            {
//                WallFlagString.append("B");
//                ScanFlags += Mask_Wall_Bottom;
//                AdjacentWallCount++;
//            }
//            else if (checkedTile == TileType::Empty)
//            {
//                ScanFlags += Mask_Empty_Bottom;
//                AdjacentEmptyCount++;
//            }
//
//            checkedTile = GetTile(col + 1, row, m_LayoutDimensions);
//            if (checkedTile == TileType::Wall || checkedTile == TileType::Door)
//            {
//                WallFlagString.append("R");
//                ScanFlags += Mask_Wall_Right;
//                AdjacentWallCount++;
//            }
//            else if (checkedTile == TileType::Empty)
//            {
//                ScanFlags += Mask_Empty_Right;
//                AdjacentEmptyCount++;
//            }
//        }
//
//        //Define the name of each sprite based on its type and adjacent tiles
//        switch (m_pLayout->m_LevelData[i])
//        {
//        case TileMap::TileType::Floor:
//        {
//            if (ScanFlags != 0)
//            {
//                if (WallFlagString[1] == 'B')
//                {
//                    WallFlagString = "";
//                }
//                else if (WallFlagString[2] == 'B')
//                    WallFlagString.erase(WallFlagString.begin() + 2);
//                else if (WallFlagString[3] == 'B')
//                    WallFlagString.erase(WallFlagString.begin() + 3);
//                else {
//                    //WallFlagString = "";
//                };
//
//                if (WallFlagString != "_")
//                    spriteName.append(WallFlagString);
//            }
//            break;
//        }
//        case TileMap::TileType::Door:
//        {
//            if (GetTile(col - 1, row, m_LayoutDimensions) == TileType::Door)
//            {
//                spriteName.append("_Wide_R");
//            }
//            else if (GetTile(col + 1, row, m_LayoutDimensions) == TileType::Door)
//            {
//                spriteName.append("_Wide_L");
//            }
//            else {
//                spriteName.append("_Small");
//            }
//            break;
//        }
//        case TileMap::TileType::Column:
//        {
//            if (GetTile(col, row + 1, m_LayoutDimensions) == TileType::Column)
//            {
//                spriteName.append("_Bottom");
//            }
//            else {
//                spriteName.append("_Top");
//            }
//
//            if (GetTile(col, row - 1, m_LayoutDimensions) == TileType::Column)
//            {
//                TileHasCollider = false;
//            }
//
//            TileHasFlooring = true;
//
//            break;
//        }
//        case TileMap::TileType::Box:
//        {
//            spriteName.append("_Wood");
//            TileHasFlooring = true;
//
//            break;
//        }
//        case TileMap::TileType::Wall:
//        {
//            if (AdjacentWallCount == 2) {
//                if (ScanFlags & Mask_Wall_Top && ScanFlags & Mask_Wall_Bottom || ScanFlags & Mask_Wall_Left && ScanFlags & Mask_Wall_Right)
//                {
//                    // STRIGHT WALL
//                    spriteName.append("_Straight" + WallFlagString + "_Floor");
//
//                    if (ScanFlags & Mask_Empty_Top)
//                        spriteName.append("Below"); // empty above, therefore floor is below
//                    else if (ScanFlags & Mask_Empty_Left)
//                        spriteName.append("Right"); // so on and so fourth
//                    else if (ScanFlags & Mask_Empty_Bottom)
//                        spriteName.append("Above");
//                    else
//                        spriteName.append("Left");
//
//                }
//                else if (
//                    ScanFlags & Mask_Wall_Top && ScanFlags & Mask_Wall_Left
//                    ||
//                    ScanFlags & Mask_Wall_Bottom && ScanFlags & Mask_Wall_Left
//                    ||
//                    ScanFlags & Mask_Wall_Top && ScanFlags & Mask_Wall_Right
//                    ||
//                    ScanFlags & Mask_Wall_Bottom && ScanFlags & Mask_Wall_Right
//                    )
//                    // CORNER WALL
//                {
//                    // IF THERE'S TWO EMPTIES, WE'RE AT AN INNER CORNER. IF THERE'S NO EMPTIES, IT'S AN OUTER CORNER
//                    if (ScanFlags & (0xF0)) // all "empty" bits
//                    {
//                        // there's empties
//                        spriteName.append("_Corner_Outer" + WallFlagString);
//                    }
//                    else {
//                        // there's no empties
//                        spriteName.append("_Corner_Inner" + WallFlagString);
//                    }
//                }
//                else { // DEFAULT CASE
//                    spriteName = "Wall_Straight_LR_FloorBelow";
//                }
//            }
//            else {
//                spriteName = "Wall_Straight_LR_FloorAbove";
//            }
//            break;
//        }
//        default:
//            break;
//        }
//
//        //Draw a copy of a floor tile below objects that need it.
//        if (TileHasFlooring)  
//        {
//            if (ScanFlags != 0)
//            {
//                if (WallFlagString[1] == 'B')
//                {
//                    WallFlagString = "";
//                }
//                else if (WallFlagString[2] == 'B')
//                    WallFlagString.erase(WallFlagString.begin() + 2);
//                else if (WallFlagString[3] == 'B')
//                    WallFlagString.erase(WallFlagString.begin() + 3);
//            }
//
//            Vector2 FloorUVScale = m_pSpriteSheet->Sprites["Floor" + WallFlagString]->UV_Scale;
//            Vector2 FloorUVOffset = m_pSpriteSheet->Sprites["Floor" + WallFlagString]->UV_Offset;
//            m_pTileMesh->Draw(pos, 0, m_pCamera, m_pShader, m_Scale, m_pTexture, properties.Color, FloorUVScale, FloorUVOffset);
//        }
//
//        //Grab UV data and draw
//        {
//            Vector2 UVScale = m_pSpriteSheet->Sprites[spriteName]->UV_Scale;
//            Vector2 UVOffset = m_pSpriteSheet->Sprites[spriteName]->UV_Offset;
//
//            m_pTileMesh->Draw(pos, 0, m_pCamera, m_pShader, m_Scale, m_pTexture, properties.Color, UVScale, UVOffset);
//        }
//    }
//}

void TileMap::Draw()
{
    for (int i = 0; i < m_LayoutDimensions.x * m_LayoutDimensions.y; i++)
    {
            DrawData* d = m_DrawData.at(i);

            if (d == nullptr || d->pSprite == nullptr)
                continue;

            if (d->pFloorSprite != nullptr)
            {
                Vector2 FloorUVScale = d->pFloorSprite->UV_Scale;
                Vector2 FloorUVOffset = d->pFloorSprite->UV_Offset;
                m_pTileMesh->Draw(d->Position, 0.0f, m_pCamera, m_pShader, m_Scale, m_pTexture, d->Color, FloorUVScale, FloorUVOffset);
            }

            Vector2 UVScale = d->pSprite->UV_Scale;
            Vector2 UVOffset = d->pSprite->UV_Offset;

            m_pTileMesh->Draw(d->Position, 0.0f, m_pCamera, m_pShader, m_Scale, m_pTexture, d->Color, UVScale, UVOffset);
    }
}

TileMap::TileType TileMap::GetTile(int _col, int _row, IVector2 _dimensions) {

    //If requested tile is out of range return empty
    if (_col < 0 || _col >= _dimensions.x || _row < 0 || _row >= _dimensions.y) {
        return TileMap::TileType::Empty;
    }

    int index = _col + ((_dimensions.y - 1) - _row) * (_dimensions.x);
    return m_pLayout->m_LevelData[index];
}
void TileMap::SetSpriteSheet(fw::SpriteSheet* _spriteSheet)
{
    m_pSpriteSheet = _spriteSheet;
};
void TileMap::SetTexture(fw::Texture* _texture)
{
    m_pTexture = _texture;
}
void TileMap::SetScale(Vector2 _scale)
{
    m_Scale = _scale;
    m_Scale.x *= fw::k_Window_Aspect;
    m_ScaleAdjustment = Vector2(m_Scale.x * m_pSpriteSheet->SpriteSize.x / fw::k_Window_Dimensions.x, m_Scale.y * m_pSpriteSheet->SpriteSize.y / fw::k_Window_Dimensions.y) * fw::k_World_Scale;
}

TileMap::~TileMap()
{
    if(m_pLayout != nullptr)
    delete m_pLayout;

    for (auto p : m_DrawData)
    {
        delete p;
    }

    m_DrawData.clear();

}

IVector2 TileMap::IndexToGrid(int Index)
{
    assert(Index >= 0 && Index < m_LayoutDimensions.x * m_LayoutDimensions.y);
    int row = (Index / (m_LayoutDimensions.x));
    int col = (Index % m_LayoutDimensions.x);
    return IVector2(col, row);
}

Vector2 TileMap::ToWorldSpace(IVector2 _gridSpace)
{
    return TileMap::ToWorldSpace(_gridSpace.x, _gridSpace.y);
}

Vector2 TileMap::ToWorldSpace(int col, int row)
{
    return m_Offset + Vector2(col * m_ScaleAdjustment.x, row * m_ScaleAdjustment.y) + m_ScaleAdjustment * 0.5f;
}

IVector2 TileMap::ToGridSpace(Vector2 _worldSpace)
{
    Vector2 input = _worldSpace - m_Offset;
    input.x /= m_ScaleAdjustment.x;
    input.y /= m_ScaleAdjustment.y;
    return IVector2((int)floor(input.x), (int)floor(input.y));
}