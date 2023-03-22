#pragma once

class PlayerController;
class Player;
class Arena;
enum class k_EnemyTypes;

    enum class k_GameState { Unknown, Menu, Transition, Play };

class Game : public fw::GameCore
{

    struct Level {
        Level() {};
        Level(int _index, float _length, std::vector<k_EnemyTypes> _enemytypes, std::string _name) : LevelIndex(_index), TimeLength(_length), EnemyTypes(_enemytypes), Name(_name){}
        int LevelIndex = 0;
        float TimeLength = 0.0f;
        std::vector<k_EnemyTypes> EnemyTypes;
        std::string Name;
    };

public:

    Game(fw::FWCore* fwp);

    virtual ~Game();
    void Init();

    void SpawnEnemy(k_EnemyTypes _type);
    void CheckCollisions();

    virtual void OnEvent(fw::Event * pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void TransitionGameState(k_GameState _state);

    void AddGameObject(Vector2 _position, std::string _file, std::string _name, Color _color, Vector2 _scale, fw::k_RenderTypes _rendertype);
    
    fw::Mesh* LoadMesh(std::string _address);

protected:

    void AnimateArena(float deltaTime);
    void HandlePlayer(/*float arenaanimvalue*/);
    void OnImGUI();

    k_GameState m_CurrentGameState;
    k_GameState m_TargetGameState;
    k_GameState m_PreviousGameState;

    PlayerController* m_pPlayerController = nullptr;
    void HandleImgui(fw::GameObject* _Object);
    fw::ShaderProgram* m_pShader = nullptr;

    std::vector<fw::GameObject*> m_GameObjects;
    std::vector<fw::Mesh*> m_Meshes;

    fw::ImGuiManager* m_pImguiMan = nullptr;

    bool m_VSyncEnabled = true;

    Player* m_pPlayer = nullptr;
    bool m_playerManualRotation = false;
    bool m_playerIsFilled = true;
    float m_playerRadius = k_Player_DefaultRadius;
    float m_playerAngle = 0;

    int m_playerVertCount = k_Player_DefaultVertCount;
    
    Arena* m_pArena = nullptr;
    Vector2 m_ArenaTransitionInitialPosition;
    Vector2 m_PlayerTransitionInitialPosition;
    bool m_bTransitionCounterDone = false;

    float m_ArenaRadius = k_Arena_MenuRadius;
    float m_GameDurationDisplay = 0;
    float m_GameDurationRecord = 0;

    fw::Mesh* m_arenaMesh;
    fw::Mesh* m_playerMesh;
    fw::Mesh* m_pSimpleEnemyMesh;
    fw::Mesh* m_pBoomerangEnemyMesh;

    fw::Mesh* m_pMeshNum1;
    fw::Mesh* m_pMeshNum2;
    fw::Mesh* m_pMeshNum3;

    fw::Mesh* m_pMeshHeartL;
    fw::Mesh* m_pMeshHeartR;

    fw::GameObject* m_pCounter;

    std::vector<fw::GameObject*> m_pHealthIndicator;

    float m_ArenaAnim = 0;

    int m_enemyCount = 0;

    fw::Timer* m_pEnemySpawnTimer;
    fw::Timer* m_pTransitionTimer;

    std::vector<Level>m_LevelList;
    Level m_CurrentLevel;
    bool Win = false;
};

