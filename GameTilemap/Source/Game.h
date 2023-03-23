#pragma once

class PlayerController;
class Player;
struct Layout;
class TileMap;
class TestEnemy;
class Enemy_Shade;
class Pathfinder;
class Enemy;

enum class GameState {
    Playing, Victory, Sandwich
};

class Game : public fw::GameCore
{
public:

    Game(fw::FWCore* fwp);

    virtual ~Game();
    void Init();

    virtual void OnEvent(fw::Event * pEvent) override;
    virtual void Update(float deltaTime) override;

    void SetCameraPosition(fw::Vector2 position, float deltaTime);
    virtual void Draw() override;
    virtual fw::ShaderProgram* GetDebugShader() override;
    //void Load(Layout* _layout);

    void WipeAllTileColliders();
    void AddTileColliderAt(Vector2 pos, Vector2 scale);
    void AddObject(Vector2 _where, Vector2 _scale, fw::Texture* _texture, fw::SpriteSheet* _spritesheet, fw::SpriteSheet::SpriteInfo* _spriteinfo, fw::k_ObjectType objectType, std::string name);

protected:

    std::map<std::string, fw::ShaderProgram*> m_pShaders;
    std::map<std::string, fw::Texture*> m_pTextures;
    std::map<std::string, fw::Mesh*> m_pMeshes;
    std::map<std::string, fw::SpriteSheet*> m_pSpriteSheets;

    std::vector<fw::GameObject*> GetSortedGameObjectIndices();

    void OnImGUI();
    void CheckCollisions();
    fw::ShaderProgram* GetBasicShader();

    PlayerController* m_pPlayerController = nullptr;
    fw::ImGuiManager* m_pImguiMan = nullptr;

    std::vector<fw::GameObject*> m_GameObjects;
    std::vector<fw::Texture*> m_Textures;
    std::vector<fw::RectangleCollider*> m_TileColliders;

    //std::vector<fw::Mesh*> m_Meshes;
    std::vector<Enemy*> m_EnemyList;

    bool m_VSyncEnabled = true;
    Player* m_pPlayer = nullptr;
    //Enemy_Shade* m_pShade = nullptr;
    fw::GameObject* m_MapRoot = nullptr;
    //TestEnemy* m_pEnemy = nullptr;

    GameState m_CurrentGameState = GameState::Sandwich;
    Vector2 m_SpawnPoint = Vector2(0, 0);
    TileMap* m_TileMap = nullptr;
    Pathfinder* m_pPathfinder = nullptr;

    float m_CameraZoom = 1.0f;

};

