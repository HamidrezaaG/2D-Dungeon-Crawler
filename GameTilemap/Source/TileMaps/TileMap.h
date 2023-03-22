#pragma once

struct Layout;
class Game;

class TileMap
{
public:
	
	enum class TileType
	{
		Empty,
		Floor,
		Wall,
		Column,
		Box,
		Door,
		NumTypes
	};

	struct TileProperties
	{
		TileProperties(): SpriteName(""), HasCollider(false), HasFloorUnderneath(false), IsADynamicObject(false){};
		//TileProperties(std::string _spriteName, bool _hasCollider):SpriteName(_spriteName), HasCollider(_hasCollider), HasFloorUnderneath(false) {};
		TileProperties(std::string _spriteName, bool _hasCollider, bool _hasFloorUnderneath, bool _isDynamic):SpriteName(_spriteName), HasCollider(_hasCollider), HasFloorUnderneath(_hasFloorUnderneath), IsADynamicObject(_isDynamic) {};
		
		TileProperties(const TileProperties& _tp) {
			SpriteName = _tp.SpriteName; HasCollider = _tp.HasCollider; Color = _tp.Color; HasFloorUnderneath = _tp.HasFloorUnderneath; IsADynamicObject = _tp.IsADynamicObject;
		};
		
		TileProperties& operator=(const TileProperties& _tp) 
		{
			SpriteName = _tp.SpriteName;
			HasCollider = _tp.HasCollider; 
			IsADynamicObject = _tp.IsADynamicObject;
			HasFloorUnderneath = _tp.IsADynamicObject;
			return *this; 
		};

		std::string SpriteName;
		bool HasCollider;
		bool HasFloorUnderneath;
		bool IsADynamicObject;
		Color Color = Color::White();
	};

	struct DrawData {
		DrawData(Vector2 p, TileType t, fw::SpriteSheet::SpriteInfo* S, fw::SpriteSheet::SpriteInfo* floorS, Color c): Position(p), Type(t), pSprite(S), pFloorSprite(floorS), Color(c) {}
		DrawData(const DrawData& d) { Position = d.Position; Type = d.Type; pSprite = d.pSprite; pFloorSprite = d.pFloorSprite; Color = d.Color; }
		Vector2 Position;
		TileType Type;
		fw::SpriteSheet::SpriteInfo* pSprite;
		fw::SpriteSheet::SpriteInfo* pFloorSprite;
		Color Color = Color::White();
	};

	void Draw();

	TileMap::TileType GetTile(int _col, int _row,  IVector2 _dimensions);

	TileMap(fw::Mesh* _pMesh, fw::Camera* _pcamera, fw::ShaderProgram* _pshader, fw::Vector2 _offset, Game* _pgame);
	~TileMap();

	void SetSpriteSheet(fw::SpriteSheet* _spriteSheet);
	void SetTexture(fw::Texture* _texture);
	void SetScale(Vector2 _scale);
	void LoadLayout(const Layout* _pLayout);
	bool TileMap::GetIsWalkable(int x, int y);
	IVector2 m_LayoutDimensions;

	Vector2 ToWorldSpace(int col, int row);
	Vector2 ToWorldSpace(IVector2 _gridSpace);
	IVector2 ToGridSpace(Vector2 _worldSpace);
	IVector2 IndexToGrid(int Index);

private:

	Layout* m_pLayout;

	fw::SpriteSheet* m_pSpriteSheet;

	Game* m_pGame;
	fw::ShaderProgram* m_pShader;
	fw::Mesh* m_pTileMesh;
	fw::Camera* m_pCamera;
	fw::Texture* m_pTexture;

	Vector2 m_Scale = Vector2::One();
	Vector2 m_ScaleAdjustment = Vector2::One();
	Vector2 m_Offset = Vector2::Zero();

	unsigned char Mask_Wall_Top		= 1 << 0;
	unsigned char Mask_Wall_Left	= 1 << 1;
	unsigned char Mask_Wall_Bottom	= 1 << 2;
	unsigned char Mask_Wall_Right	= 1 << 3;
	unsigned char Mask_Empty_Top	= 1 << 4;
	unsigned char Mask_Empty_Left	= 1 << 5;
	unsigned char Mask_Empty_Bottom = 1 << 6;
	unsigned char Mask_Empty_Right	= 1 << 7;

	TileProperties m_Properties[(unsigned int)TileType::NumTypes];
	std::vector<bool>m_WalkableTiles;
	std::vector<DrawData*>m_DrawData;
};

