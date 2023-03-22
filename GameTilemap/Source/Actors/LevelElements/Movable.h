#pragma once

//class fw::Mesh;

class Movable : public fw::GameObject
{
public:
	Movable(Vector2 position, Vector2 scale, fw::Mesh* _pmesh, std::string _name, fw::ShaderProgram* _pshader, fw::GameCore* _pgame);
	~Movable();

	void SetSprite(fw::SpriteSheet* _pspritesheet, fw::SpriteSheet::SpriteInfo* _pspriteInfo);
	virtual void Draw() override;
	virtual fw::k_ObjectType GetType() override;
	virtual fw::Collider* GetCollider() override;

private:

	fw::SpriteSheet* m_pSpriteSheet;
	fw::Collider* m_pCollider;
	
	bool m_DrawDebugCollider = fw::k_Debug_DrawColliders;
};

