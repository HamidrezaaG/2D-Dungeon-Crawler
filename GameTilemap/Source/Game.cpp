#include "GamePCH.h"
#include "Game.h"

#include "Events/GameEvents.h"
#include "Objects/Shapes.h"

#include "Actors/Player.h"
#include "Actors/PlayerController.h"
#include "TileMaps/Layouts.h"

#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/Enemy_Shade.h"
#include "TileMaps/Pathfinder.h"

#include "Actors/LevelElements/Movable.h"

Game::Game(fw::FWCore* fwp): GameCore(fwp), m_MapRoot(), m_pPathfinder()
{
    wglSwapInterval(m_VSyncEnabled ? 1 : 0);
}

Game::~Game()
{
    delete m_pImguiMan;
    delete m_pPlayerController;
    delete m_pEventManager;

    for ( fw::GameObject* SelectedObject : m_GameObjects)
    {
        delete SelectedObject;
    }

    for (std::pair<std::string, fw::Mesh*> SelectedMesh : m_pMeshes)
    {
        delete SelectedMesh.second;
    }

    for (std::pair<std::string, fw::ShaderProgram*> SelectedShader : m_pShaders)
    {
        delete SelectedShader.second;
    }

    for (std::pair<std::string, fw::Texture*> SelectedTexture : m_pTextures)
    {
        delete SelectedTexture.second;
    }

    for (std::pair<std::string, fw::SpriteSheet*> SelectedSpriteSheet : m_pSpriteSheets)
    {
        delete SelectedSpriteSheet.second;
    }

    for (fw::RectangleCollider* tileCol : m_TileColliders)
    {
        delete tileCol;
    }

    delete m_MapRoot;
    delete m_pPathfinder;
    delete m_TileMap;
    delete m_pActiveCamera;
}

void Game::Init()
{
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_pPlayerController = new PlayerController();
        m_pEventManager = new fw::EventManager();

        m_pImguiMan = new fw::ImGuiManager(m_pFramework);
        m_pImguiMan->Init();

        m_pActiveCamera = new fw::Camera(Vector2(-2.6f, -1.f), Vector2(fw::k_World_Scale), 1.f);
        m_MapRoot = new fw::GameObject(nullptr, "MapRoot", nullptr, this);
    }

    // Loading Shaders
    {
        m_pShaders["Basic"] = new fw::ShaderProgram("Data/Shaders/Basic.vert", "Data/Shaders/Basic.frag");
        m_pShaders["Texture"] = new fw::ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag");
    }

    // Loading Meshes
    {
        m_pMeshes["SpriteMesh"] = new fw::Mesh();
        m_pMeshes["SpriteMesh"]->MakeRect(Vector2(0.5,0.3f), 2, 2, true);
        m_pMeshes["TileSprite"] = new fw::Mesh();
        m_pMeshes["TileSprite"]->MakeRect(Vector2(0.f), 1, 1, true);
    }

    // Loading Textures
    {
        m_pTextures["Player"] = new fw::Texture("Data/Textures/Player.png");
        m_pTextures["Shade"] = new fw::Texture("Data/Textures/Shade.png");
        m_pTextures["Bun"] = new fw::Texture("Data/Textures/Bun.png");
        m_pTextures["Map"] = new fw::Texture("Data/Textures/Map.png");
    }

    // Loading SpriteSheets
    {
        m_pSpriteSheets["Map"] = fw::SpriteSheet::Load("Data/Textures/Map.json");
        m_pSpriteSheets["Shade"] = fw::SpriteSheet::Load("Data/Textures/Shade.json");
        m_pSpriteSheets["Player"] = fw::SpriteSheet::Load("Data/Textures/Player.json");
    }

    // LEVEL
    {
        m_TileMap = new TileMap(m_pMeshes["TileSprite"], m_pActiveCamera, m_pShaders["Texture"], k_LayoutOffset, this);
        m_TileMap->SetSpriteSheet(m_pSpriteSheets["Map"]);
        m_TileMap->SetTexture(m_pTextures["Map"]);
        m_TileMap->SetScale(Vector2(1));
        m_TileMap->LoadLayout(&k_Layout_1);
    }

    // PLAYER
    {
        m_pPlayer = new Player(m_TileMap->ToWorldSpace(IVector2(3, 3)), m_pMeshes["SpriteMesh"], "Player", m_pShaders["Texture"], m_pPlayerController, this);
        m_pPlayer->SetTexture(m_pTextures["Player"]);
        m_pPlayer->SetSpriteSheet(m_pSpriteSheets["Player"]);
        m_pPlayer->SetScale(Vector2(1.0f));
        m_GameObjects.push_back(m_pPlayer);
    }

    // misc
    {
        m_pPathfinder = new Pathfinder(m_TileMap);
    }

    // SHADE
    {
        AddObject(Vector2(2, 5), 0.8f, m_pTextures["Shade"], m_pSpriteSheets["Shade"], nullptr, fw::k_ObjectType::Enemy, "Slim Shade-y");
        AddObject(Vector2(15, 6), 0.8f, m_pTextures["Shade"], m_pSpriteSheets["Shade"], nullptr, fw::k_ObjectType::Enemy, "Slim Shade-y");
        AddObject(Vector2(20, 2), 0.8f, m_pTextures["Shade"], m_pSpriteSheets["Shade"], nullptr, fw::k_ObjectType::Enemy, "Slim Shade-y");
    }

    m_pEventManager->AddEvent(new ResetGameEvent());
    m_pActiveCamera->SetCorrectedPosition(m_pPlayer->GetPosition());
}

void AddTileCollider(int x, int y, float w, float h)
{

}

void Game::Update(float deltaTime)
{

    m_pEventManager->DispatchAllEvents(deltaTime, this);
    m_pImguiMan->StartFrame(deltaTime);
    OnImGUI();

    for (fw::GameObject* pObject : m_GameObjects)
    {
        pObject->Update(deltaTime);
    }

    if (m_pPlayerController->WasNewlyPressed(PlayerController::Mask::Restart))
    {
        m_pEventManager->AddEvent(new ResetGameEvent());
    }

    if (m_pPlayerController->WasNewlyPressed(PlayerController::Mask::ZoomIn))
    {
        if(m_CameraZoom + k_Camera_ZoomStep < k_Camera_MaxZoom)
        m_CameraZoom += k_Camera_ZoomStep;
    }

    if (m_pPlayerController->WasNewlyPressed(PlayerController::Mask::ZoomOut))
    {
        if (m_CameraZoom - k_Camera_ZoomStep >k_Camera_MinZoom)
        m_CameraZoom -= k_Camera_ZoomStep;
    }

    CheckCollisions();
    SetCameraPosition(m_pPlayer->GetPosition(), deltaTime);
}

void Game::SetCameraPosition(fw::Vector2 targetPosition, float deltaTime)
{
    Vector2 v = m_pActiveCamera->GetCorrectedPosition();

    Vector2 finv = targetPosition;
    
    finv.x = (float)round(finv.x * fw::PositionRoundingFactor.x) / fw::PositionRoundingFactor.x ;
    finv.y = (float)round(finv.y * fw::PositionRoundingFactor.y) / fw::PositionRoundingFactor.y ;

    m_pActiveCamera->SetCorrectedPosition(finv);
    
    float zoomcalc = fw::Math::Lerp(m_pActiveCamera->GetZoom(), m_CameraZoom, deltaTime * k_Camera_ZoomSpeed);

    m_pActiveCamera->SetZoom(zoomcalc);
}

void Game::CheckCollisions()
{
    // GAMEOBJECT - GAMEOBJECT COLLISIONS
    for (unsigned int i = 0; i < m_GameObjects.size(); i++)
    {
        for (unsigned int j = i + 1; j < m_GameObjects.size(); j++)
        {
            if (i == j)
                continue;

            fw::Collider* ColliderA = m_GameObjects.at(i)->GetCollider();
            fw::Collider* ColliderB = m_GameObjects.at(j)->GetCollider();

            if (m_GameObjects.at(i)->GetType() == fw::k_ObjectType::Dynamic)
                int bp = 1;
            // if both objects have colliders
            if (ColliderA != nullptr && ColliderB != nullptr)
            {
                // if both colliders are enabled
                if (ColliderA->IsEnabled() && ColliderB->IsEnabled())
                {
                    //fw::k_ObjectType Atype = m_GameObjects.at(i)->GetType();
                    //fw::k_ObjectType Btype = m_GameObjects.at(j)->GetType();
                    //if (Atype == fw::k_ObjectType::Player || Btype == fw::k_ObjectType::Player)
                    {
                        if (ColliderA->CheckCollision(ColliderB) || ColliderB->CheckCollision(ColliderA))
                        {
                            m_pEventManager->AddEvent(new DynamicCollisionEvent(m_GameObjects.at(i), m_GameObjects.at(j)));
                        }
                    }
                }
            }
        }
    }

    // TILE - GAMEOBJECT COLLISIONS
    for (unsigned int i = 0; i < m_GameObjects.size(); i++)
    {
        for (unsigned int j = 0; j < m_TileColliders.size(); j++)
        {
            fw::Collider* ColliderA = m_GameObjects.at(i)->GetCollider();
            fw::Collider* ColliderB = m_TileColliders.at(j);

            // if both objects have colliders
            if (ColliderA != nullptr && ColliderB != nullptr)
            {
                // if both colliders are enabled
                if (ColliderA->IsEnabled() && ColliderB->IsEnabled())
                {
                        if (ColliderA->CheckCollision(ColliderB) || ColliderB->CheckCollision(ColliderA))
                        {
                            m_pEventManager->AddEvent(new TileCollisionEvent(m_GameObjects.at(i), m_TileColliders.at(j)));
                        }
                }
            }
        }
    }

}

void Game::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent(pEvent);

    if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        if (pObject->GetType() == fw::k_ObjectType::Enemy)
        {
            for (unsigned int i = 0; i < m_EnemyList.size(); i++)
            {
                if (m_EnemyList.at(i) == pObject)
                    m_EnemyList.erase(m_EnemyList.begin() + i);
            }
        }

        auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), pObject);
        m_GameObjects.erase(it);
        delete pObject;

        //m_enemyCount--;
    }

    if (pEvent->GetType() == DynamicCollisionEvent::GetStaticEventType())
    {
        DynamicCollisionEvent* event = static_cast<DynamicCollisionEvent*>(pEvent);
        event->GetObjectA()->OnCollisionHit(event->GetObjectB());
        event->GetObjectB()->OnCollisionHit(event->GetObjectA());
    }

    if (pEvent->GetType() == TileCollisionEvent::GetStaticEventType())
    {
        TileCollisionEvent* event = static_cast<TileCollisionEvent*>(pEvent);
        if (event->GetObject()->GetType() == fw::k_ObjectType::Player)
        {
            m_pPlayer->OnTileCollision(event->GetTileCollider());
        }
    }

    if (pEvent->GetType() == ResetGameEvent::GetStaticEventType())
    {
        //RESET CODE
    }

    if (pEvent->GetType() == PlayerDamageEvent::GetStaticEventType())
    {
        PlayerDamageEvent* event = static_cast<PlayerDamageEvent*>(pEvent);
        m_pPlayer->TakeDamage(event->GetInstigator(), event->GetDamage());
    }
    
    if (pEvent->GetType() == EnemyDamageEvent::GetStaticEventType())
    {
        EnemyDamageEvent* event = static_cast<EnemyDamageEvent*>(pEvent);
        int x = 0;
        for (unsigned int i = 0; i < m_EnemyList.size(); i++)
        {
            if (m_EnemyList.at(i)->GetCollider()->GetRelativePosition().SqrDistanceFrom(event->GetLocation()) < 0.25f)
            {
                m_EnemyList.at(i)->TakeDamage(event->GetInstigator(),event->GetDamage());
            }
        }
    }

}

void Game::Draw()
{

    glClearColor(k_Color_Background.x, k_Color_Background.y, k_Color_Background.z, k_Color_Background.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10);
    glLineWidth(3);

    m_TileMap->Draw();

    //int* order = 
    std::vector<fw::GameObject*> drawOrder = GetSortedGameObjectIndices();
    
    for (size_t i = 0; i < m_GameObjects.size(); i++)
    {
        drawOrder.at(i)->Draw();
    }

    //for (size_t i = 0; i < m_GameObjects.size(); i++)
    //{
    //    m_GameObjects.at(i)->Draw();
    //}

    if(fw::k_Debug_DrawColliders)
    for (fw::RectangleCollider* tileCol : m_TileColliders)
    {
        tileCol->Draw();
    }


    m_pImguiMan->EndFrame();
}

std::vector<fw::GameObject*> Game::GetSortedGameObjectIndices()
{
    //std::vector<int> r;
    //int temp, minIndex = 0;

    //for (int i = 0; i < m_GameObjects.size(); i++)
    //{
    //    r.push_back(i);
    //}

    //for (int i = 0; i < m_GameObjects.size()-1; i++)
    //{
    //    minIndex = i;
    //
    //    for (int j = i + 1; j < m_GameObjects.size(); j++)
    //    {
    //        if (m_GameObjects.at(r[minIndex])->GetPosition().y > m_GameObjects.at(j)->GetPosition().y)
    //        {
    //            minIndex = j;
    //        }
    //    }
    //    temp = r[i];
    //    r[i] = r[minIndex];
    //    r[minIndex] = temp;
    //
    //}

    std::vector<fw::GameObject*> r;
    fw::GameObject* temp;

    for (int i = 0; i < m_GameObjects.size(); i++)
    {
        r.push_back(m_GameObjects.at(i));
    }
   
    for (int i = 0; i < m_GameObjects.size()-1; i++)
    {
        int mindex = i;
    
        for (int j = i + 1; j < m_GameObjects.size(); j++)
        {
            if (r.at(mindex)->GetPosition().y < m_GameObjects.at(j)->GetPosition().y)
            {
                mindex = j;
            }
        }
        temp = r[i];
        r[i] = r[mindex];
        r[mindex] = temp;
    }
    return r;
};

fw::ShaderProgram* Game::GetDebugShader()
{
    return m_pShaders["Basic"];
}

void Game::OnImGUI()
{
    ImGui::Begin("Controls");
    ImGui::TextWrapped("WASD to move\nSpace to attack the shades.\nShades have 3 health.\nYou have 5 health.");
    ImGui::End();
}

void Game::WipeAllTileColliders()
{
    for (fw::RectangleCollider* tileCol : m_TileColliders)
    {
        delete tileCol;
    }
}


void Game::AddTileColliderAt(Vector2 pos, Vector2 scale)
{
    fw::RectangleCollider* c = new fw::RectangleCollider(m_MapRoot, scale, pos, m_pActiveCamera, GetDebugShader(), false);
    m_TileColliders.push_back(c);

}

void Game::AddObject(Vector2 _where, Vector2 _scale, fw::Texture* _texture, fw::SpriteSheet* _spritesheet, fw::SpriteSheet::SpriteInfo* _spriteinfo, fw::k_ObjectType objectType, std::string name)
{
    fw::GameObject* GO;

    switch (objectType)
    {
    case fw::k_ObjectType::Enemy:

        GO = new Enemy_Shade(_where, m_pMeshes["SpriteMesh"], name, m_pShaders["Texture"], m_pPathfinder, m_pPlayer, this);
        GO->SetTexture(_texture);
        static_cast<Enemy_Shade*>(GO)->SetSpriteSheet(_spritesheet);
        GO->SetScale(Vector2(_scale));
        m_GameObjects.push_back(GO);
        m_EnemyList.push_back(static_cast<Enemy*>(GO));

        break;
    case fw::k_ObjectType::Dynamic:

        GO = new Movable(_where, _scale, m_pMeshes["SpriteMesh"], name, m_pShaders["Texture"], this);
        static_cast<Movable*>(GO)->SetSprite(_spritesheet, _spriteinfo);
        GO->SetTexture(_texture);
        m_GameObjects.push_back(GO);

        break;
    default:
        break;
    }

}
