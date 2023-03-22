#include "GamePCH.h"
#include "Game.h"

#include "Events/GameEvents.h"
#include "Objects/Shapes.h"

#include "Actors/Player.h"
#include "Actors/PlayerTrail.h"
#include "Actors/PlayerController.h"

#include "Actors/Enemies/Enemy.h"
#include "Actors/Enemies/SimpleEnemy.h"
#include "Actors/Enemies/BoomerangEnemy.h"

#include "Actors/Arena.h"


Game::Game(fw::FWCore* fwp): GameCore(fwp)
{
    wglSwapInterval(m_VSyncEnabled ? 1 : 0);
}

Game::~Game()
{
    delete m_pShader;
    delete m_pImguiMan;

    for (int i = 0; i < m_GameObjects.size(); i++)
    {
        delete m_GameObjects.at(i);
    }

    for (int i = 0; i < m_Meshes.size(); i++)
    {
        delete m_Meshes.at(i);
    }

    delete m_pPlayerController;
    delete m_pEventManager;

    delete m_pEnemySpawnTimer;
    delete m_pTransitionTimer;

}

void Game::Init()
{
    // Init
    {
        //ShowCursor(false);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_pEnemySpawnTimer = new fw::Timer(k_Enemy_SpawnTime, false);
        m_pTransitionTimer = new fw::Timer(k_Game_TransitionTime, false);

        m_pPlayerController = new PlayerController();
        m_pEventManager = new fw::EventManager();

        m_pImguiMan = new fw::ImGuiManager(m_pFramework);
        m_pImguiMan->Init();

        m_pShader = new fw::ShaderProgram("Data/Basic.vert", "Data/Basic.frag");
    }

    AddGameObject(Vector2(9, 1), "Data/Mesh/BunnyOutline.obj", "Bunny_2", Color::White(), Vector2(3.0f,3.0f), fw::k_RenderTypes::Lines);

    // ARENA
    {
        m_arenaMesh = new fw::Mesh();
        m_Meshes.push_back(m_arenaMesh);
        m_pArena = new Arena(fw::k_World_MidPoint, m_arenaMesh, m_pShader, this);
        m_ArenaTransitionInitialPosition = m_pArena->GetPosition();
        m_pArena->SetRadius(k_Arena_MenuRadius);
        m_pArena->SetColor(k_Color_Arena);
        m_GameObjects.push_back(m_pArena);
    }

    // PLAYER
    {
    m_playerMesh = new fw::Mesh();
    m_Meshes.push_back(m_playerMesh);

    m_playerMesh->MakeCircle(m_playerRadius, m_playerVertCount, m_playerIsFilled);
    m_pPlayer = new Player(fw::k_World_MidPoint, m_playerMesh, "Player", m_pShader, m_pPlayerController, m_pArena, this);
    m_pPlayer->SetColor(k_Color_Player);
    m_GameObjects.push_back(m_pPlayer);
    }

    m_pSimpleEnemyMesh = LoadMesh("Data/Mesh/SimpleShuriken.obj");
    m_pBoomerangEnemyMesh = LoadMesh("Data/Mesh/Boomerang.obj");

    for (size_t i = 0; i < k_Player_Trail_Count; i++)
    {
        fw::Mesh* trailmesh = new fw::Mesh();
        PlayerTrail* trailobject = new PlayerTrail(m_pPlayer, trailmesh, "PlayerTrail " + std::to_string(i), m_pShader, this);
        trailobject->SetCircle(m_playerRadius, m_playerVertCount, true);

        Color tcolor = k_Color_Player_Trail;
        tcolor.SetAlpha((float)pow(tcolor.w, (double)(i + 1)));

        trailobject->SetColor(tcolor);

        trailobject->init(
            k_Player_Trail_BaseLerpMultiplier * (float)pow(k_Player_Trail_LerpMultiplier_pow, (double)(i + 1)),
            k_Player_Trail_BaseRadius * (float)pow(k_Player_Trail_Radius_pow, (double)(i + 1))
            );

        m_Meshes.push_back(trailmesh);
        m_GameObjects.push_back(trailobject);
    }

    // COUNTER
    {
        m_pMeshNum1 = LoadMesh("Data/Mesh/Num1.obj");
        m_pMeshNum2 = LoadMesh("Data/Mesh/Num2.obj");
        m_pMeshNum3 = LoadMesh("Data/Mesh/Num3.obj");

        m_pCounter = new fw::GameObject(fw::k_World_MidPoint - Vector2(0,-1), m_pMeshNum1, "Counter", m_pShader, this);
        m_pCounter->SetColor(k_Color_Counter);
        m_pCounter->SetAngle(180.0f);
        m_pCounter->SetHidden(true);

        m_GameObjects.push_back(m_pCounter);
    }

        m_pMeshHeartL = LoadMesh("Data/Mesh/HeartL.obj");
        m_pMeshHeartR = LoadMesh("Data/Mesh/HeartR.obj");

        for (int i = 0; i < k_Player_Health; i++)
        {
            fw::GameObject* h = new fw::GameObject(Vector2(0.5f + 0.75f*(i/2) ,9.5f), i % 2 == 0 ? m_pMeshHeartR : m_pMeshHeartL, "Heart", m_pShader, this);
            h->SetColor(k_Color_Counter);
            h->SetScale(0.30f);
            h->SetAngle(180.0f);
            m_GameObjects.push_back(h);
            m_pHealthIndicator.push_back(h);
        }

    // LEVELS
    {
            std::vector<k_EnemyTypes> enemies;
            enemies.push_back(k_EnemyTypes::Boomerang);
            Level _level(0, 7.0f, enemies, "Level1: Boomerangs!");
            m_LevelList.push_back(_level);

            enemies.clear();
            enemies.push_back(k_EnemyTypes::Simple);
            _level = Level(1, 7.0f, enemies, "Level2: Shurikens!");
            m_LevelList.push_back(_level);

            enemies.clear();
            enemies.push_back(k_EnemyTypes::Simple);
            enemies.push_back(k_EnemyTypes::Boomerang);
            _level = Level(2, 7.0f, enemies, "Level3: Shumerangs!");
            m_LevelList.push_back(_level);

            m_CurrentLevel = m_LevelList.at(0);

    }

    m_CurrentGameState = k_GameState::Menu;
}

void Game::SpawnEnemy(k_EnemyTypes _type) // an enum
{



    switch (_type)
    {
    case k_EnemyTypes::Simple:
        Enemy* tempEnemySimple;
        tempEnemySimple = new SimpleEnemy(m_pPlayer, m_pSimpleEnemyMesh, "Enemy" + std::to_string(m_enemyCount), m_pShader, m_pArena, this);
        tempEnemySimple->SetColor(k_Color_SimpleEnemy);
        tempEnemySimple->SetScale(4.0f);
        m_GameObjects.push_back(tempEnemySimple);

        break;

    case k_EnemyTypes::Boomerang:
        Enemy* tempEnemyBoomerang;
        tempEnemyBoomerang = new BoomerangEnemy(m_pPlayer, m_pBoomerangEnemyMesh, "Enemy" + std::to_string(m_enemyCount), m_pShader, m_pArena, this);
        tempEnemyBoomerang->SetColor(k_Color_BoomerangEnemy);
        tempEnemyBoomerang->SetScale(8.0f);
        m_GameObjects.push_back(tempEnemyBoomerang);

        break;

    default:
        assert(false);
        break;
    }


}

void Game::Update(float deltaTime)
{

    m_pEventManager->DispatchAllEvents(deltaTime, this);
    m_pImguiMan->StartFrame(deltaTime);
    OnImGUI();

    for (fw::GameObject* pObject : m_GameObjects)
    {
        pObject->Update(deltaTime);

        if (pObject->GetType() == fw::k_ObjectType::Enemy)
        {
            if (static_cast<Enemy*>(pObject)->IsDead())
            {
                if(pObject->GetPosition().y < 0)
                    m_pEventManager->AddEvent(new RemoveFromGameEvent(pObject));
            }
            else 
            {
                Vector2 delta = pObject->GetPosition() - m_pArena->GetPosition();

                if (delta.SqrMagnitude() >= m_ArenaRadius * m_ArenaRadius)
                    m_pEventManager->AddEvent(new RemoveFromGameEvent(pObject));
            }
        }
    }
        HandlePlayer();

    switch (m_CurrentGameState)
    {
        case k_GameState::Menu:

            ImGui::Begin("MENU");
            if (Win)
            {
                ImGui::Text("You Won!");
            }
            ImGui::Text("R to start, and R to restart.");
            ImGui::End();

            if (m_pPlayerController->IsHeld(PlayerController::Mask::Restart))
            {
                TransitionGameState(k_GameState::Play);
            }
            break;

        case k_GameState::Play:

            if(m_pPlayer->IsAlive())
            m_GameDurationRecord += deltaTime;

            if (m_GameDurationRecord >= m_CurrentLevel.TimeLength)
            {
                for (unsigned int i = 0; i < m_GameObjects.size(); i++)
                {
                    if (m_GameObjects.at(i)->GetType() == fw::k_ObjectType::Enemy)
                    {
                        static_cast<Enemy*>(m_GameObjects.at(i))->Die();
                    }
                }

                m_GameDurationRecord = 0;
                if(m_CurrentLevel.LevelIndex + 1 <= 2){
                    m_CurrentLevel = m_LevelList.at(m_CurrentLevel.LevelIndex + 1);

                    TransitionGameState(k_GameState::Play);
                }
                else {
                    TransitionGameState(k_GameState::Menu);
                    Win = true;
                }
            }
            m_GameDurationDisplay = m_GameDurationRecord;

            if (m_pPlayerController->IsHeld(PlayerController::Mask::Restart))
            {
                for (unsigned int i = 0; i < m_GameObjects.size(); i++)
                {
                    if (m_GameObjects.at(i)->GetType() == fw::k_ObjectType::Enemy)
                    {
                        static_cast<Enemy*>(m_GameObjects.at(i))->Die();
                    }
                }
                m_CurrentLevel = m_LevelList.at(0);
                TransitionGameState(k_GameState::Play);

            }

            AnimateArena(deltaTime);

            m_pEnemySpawnTimer->Update(deltaTime);

            if (m_pEnemySpawnTimer->IsDone())
            {
                m_pEventManager->AddEvent(new SpawnEnemyEvent());
                m_pEnemySpawnTimer->Restart();
            }
            CheckCollisions();

            break;

        case k_GameState::Transition:
        {
                m_pTransitionTimer->Update(deltaTime);
                float percent = m_pTransitionTimer->GetPercentageElapsed();
                float trigPercent = percent * (float)fw::k_PI;
                float nonlinearPercent;

                switch (m_TargetGameState)  // TARGET STATE
                {
                    case k_GameState::Menu:

                        if (m_PreviousGameState == k_GameState::Play)
                        {
                            m_pPlayer->Reset(percent);
                            //nonlinearPercent = -0.5f * (1 - cos(2 * trigPercent)) * sin(3 * trigPercent);
                            nonlinearPercent = -0.2f * (1 - cosf(2 * trigPercent)) * sinf(2 * trigPercent) + 0.5f * (1 - cosf(trigPercent));

                            m_pHealthIndicator.at((int)(percent * (k_Player_Health - 1)))->SetHidden(false);

                            Vector2 arenatargetpos = Vector2::Lerp(m_ArenaTransitionInitialPosition, Vector2(5,5), nonlinearPercent);
                            float arenatargetRadius = k_Arena_PlayRadius + (k_Arena_MenuRadius - k_Arena_PlayRadius) * nonlinearPercent;

                            m_pArena->SetPosition(arenatargetpos);
                            m_pArena->SetRadius(arenatargetRadius);
                            m_ArenaRadius = arenatargetRadius;

                            if (m_pTransitionTimer->IsDone())
                            {
                                m_GameDurationRecord = 0;
                                m_ArenaAnim = 0;
                                m_ArenaTransitionInitialPosition = m_pArena->GetPosition();
                                m_CurrentGameState = k_GameState::Menu;
                                m_CurrentLevel = m_LevelList.at(0);
                            }
                        
                        }
                        break;

                    case k_GameState::Play:
                        
                        if (m_PreviousGameState == k_GameState::Menu) // PREVIOUS STATE = MENU -> BEGIN PLAY
                        {
                            if (m_bTransitionCounterDone) {
                                //float CounterPercent = (1.0f-cos(k_PI * (3 * percent - floorf(3 * percent))))*0.5f;
                                //float CounterPercent = abs(sin(3 * k_PI * percent));
                                float CounterPercent = 0.5f * (1 - cosf(6*trigPercent));
                                m_pCounter->SetHidden(false);

                                if(percent < 1.0f/3.0f)
                                {
                                    m_pCounter->SetMesh(m_pMeshNum3);
                                }
                                else if (percent < 2.0f/3.0f)
                                {
                                    m_pCounter->SetMesh(m_pMeshNum2);
                                }
                                else
                                {
                                    m_pCounter->SetMesh(m_pMeshNum1);
                                }

                                float counterScale = k_Counter_InitScale + (k_Counter_FinScale - k_Counter_InitScale) * CounterPercent;
                                float counterAlpha = k_Counter_InitScale - k_Counter_InitScale * CounterPercent;


                                m_pCounter->SetScale(counterScale);
                                Color counterColor = k_Color_Counter;
                                counterColor.SetAlpha(1 - counterAlpha);
                                m_pCounter->SetColor(counterColor);
                            }
                            else
                            {
                                nonlinearPercent = -0.2f * (1 - cosf(2 * trigPercent)) * sinf(2 * trigPercent) + 0.5f * (1 - cosf(trigPercent));
                                //float nonlinearPercent = -0.5f * (1 - cos(k_PI * percent)) * cos(k_PI * percent);
                                Vector2 arenatargetpos = Vector2::Lerp(m_ArenaTransitionInitialPosition, Vector2(5 + sinf(m_ArenaAnim), 5 + cosf(m_ArenaAnim)), nonlinearPercent);
                                float arenatargetRadius = k_Arena_MenuRadius + (k_Arena_PlayRadius - k_Arena_MenuRadius) * nonlinearPercent;
                                //m_pPlayer->SetPosition(Vector2::Lerp(m_PlayerTransitionInitialPosition, Vector2(5 + sin(m_ArenaAnim), 5 + cos(m_ArenaAnim)), nonlinearPercent));

                                m_pArena->SetPosition(arenatargetpos);
                                m_pArena->SetRadius(arenatargetRadius);
                                m_ArenaRadius = arenatargetRadius;
                            }
                                if (m_pTransitionTimer->IsDone())
                                {
                                    if (m_bTransitionCounterDone)
                                    {
                                        m_ArenaTransitionInitialPosition = m_pArena->GetPosition();
                                        m_pEnemySpawnTimer->Restart();
                                        m_pCounter->SetHidden(true);
                                        m_bTransitionCounterDone = false;
                                        m_CurrentGameState = k_GameState::Play;
                                    }
                                    else
                                    {
                                        m_pTransitionTimer->Restart();
                                        m_bTransitionCounterDone = true;
                                    }

                                }
                        }
                        else if (m_PreviousGameState == k_GameState::Play) // PREVIOUS STATE = PLAY -> RESET PLAY
                        {
                                
                                m_pPlayer->Reset(percent);
                                //nonlinearPercent = -0.5f * (1 - cos(2 * trigPercent)) * sin(3 * trigPercent);
                                nonlinearPercent = -0.5f * (1 - cosf(2 * trigPercent)) * sinf(3 * trigPercent) - 0.1f * (cosf(8 * trigPercent) - 1) * abs(cosf(trigPercent));

                                m_pHealthIndicator.at((int)(percent * (k_Player_Health - 1)))->SetHidden(false);

                                Vector2 arenatargetpos = Vector2::Lerp(m_ArenaTransitionInitialPosition, Vector2(5 + sinf(m_ArenaAnim), 5 + cosf(m_ArenaAnim)), nonlinearPercent);
                                float arenatargetRadius = k_Arena_PlayRadius + (k_Arena_MenuRadius - k_Arena_PlayRadius) * nonlinearPercent;
                                //m_pPlayer->SetPosition(Vector2::Lerp(m_PlayerTransitionInitialPosition, Vector2(5 + sin(m_ArenaAnim), 5 + cos(m_ArenaAnim)), nonlinearPercent));
                                m_GameDurationDisplay = m_GameDurationRecord - m_GameDurationRecord * percent;

                                m_pArena->SetPosition(arenatargetpos);
                                m_pArena->SetRadius(arenatargetRadius);
                                m_ArenaRadius = arenatargetRadius;

                                if (m_pTransitionTimer->IsDone())
                                {
                                    m_GameDurationRecord = 0;
                                    m_ArenaTransitionInitialPosition = m_pArena->GetPosition();
                                    m_CurrentGameState = k_GameState::Play;
                                    m_pEnemySpawnTimer->Restart();
                                }
                        }
                        else
                        {
                        }

                        break;

                    default:
                        break;
                }
                break;
        }
        default:
            // UNKNOWN GAME STATE
            assert(false);
            break;
    }

    if (m_CurrentGameState != k_GameState::Menu)
    {
        ImGui::Begin(m_CurrentLevel.Name.c_str());
        ImGui::Text("Time to next level: %.2f", m_CurrentLevel.TimeLength - m_GameDurationDisplay);
        ImGui::End();
    }
}

void Game::TransitionGameState(k_GameState _state)
{
    m_PreviousGameState = m_CurrentGameState;
    m_CurrentGameState = k_GameState::Transition;
    m_TargetGameState = _state;

    m_pEnemySpawnTimer->Reset();
    m_pTransitionTimer->Restart();
    m_PlayerTransitionInitialPosition = m_pPlayer->GetPosition();
    m_ArenaTransitionInitialPosition = m_pArena->GetPosition();
}

void Game::CheckCollisions()
{
    for (unsigned int i = 0; i < m_GameObjects.size(); i++)
    {
        for (unsigned int j = i+1; j < m_GameObjects.size(); j++)
        {
            if (i == j)
                continue;

            fw::Collider* ColliderA = m_GameObjects.at(i)->GetCollider();
            fw::Collider* ColliderB = m_GameObjects.at(j)->GetCollider();

            // if both objects have colliders
            if (ColliderA != nullptr && ColliderB != nullptr)
            {
                // if both colliders are enabled
                if (ColliderA->IsEnabled() && ColliderB->IsEnabled())
                {
                    // ONLY CHECKING PLAYER COLLISIONS RIGHT NOW
                    fw::k_ObjectType Atype = m_GameObjects.at(i)->GetType();
                    fw::k_ObjectType Btype = m_GameObjects.at(j)->GetType();
                    if (Atype == fw::k_ObjectType::Player || Btype == fw::k_ObjectType::Player)
                    {
                        if (ColliderA->CheckCollision(ColliderB) || ColliderB->CheckCollision(ColliderA))
                        {
                            m_pEventManager->AddEvent(new CollisionEvent(m_GameObjects.at(i), m_GameObjects.at(j)));
                        }
                    }
                }
            }
        }
    }
}


void Game::AnimateArena(float deltaTime)
{
    m_ArenaAnim += deltaTime;

    if (m_ArenaAnim > 2 * (float)fw::k_PI)
        m_ArenaAnim -= 2 * (float)fw::k_PI;

    m_pArena->SetPosition(5 + sinf(m_ArenaAnim), 5 + cosf(m_ArenaAnim));
    m_pArena->SetCalculatedVelocity(Vector2(cosf(m_ArenaAnim) , - sinf(m_ArenaAnim)));
}


void Game::HandlePlayer() {

    Vector2 delta = m_pPlayer->GetPosition() - m_pArena->GetPosition();

    if(!m_playerManualRotation)
    {
        float angle = delta.GetAngle();

        if (angle < 0)
            angle += 2 * (float)fw::k_PI;

        angle *= (float)fw::k_Convert_Rad2Deg;

        //ImGui::Text("%f", angle);

        m_playerAngle = -(m_ArenaRadius / m_playerRadius) * angle ;
        m_pPlayer->SetAngle(m_playerAngle);
    }

    if (delta.SqrMagnitude() >= (m_ArenaRadius - m_playerRadius) * (m_ArenaRadius - m_playerRadius))
    {
        delta.Normalize();
        m_pPlayer->RemoveAccelerationInDirection(delta);
        m_pPlayer->SetPosition(m_pArena->GetPosition() + delta * (m_ArenaRadius - m_playerRadius));
    }

}

void Game::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent(pEvent);

    if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), pObject);
        m_GameObjects.erase(it);
        delete pObject;
        //m_enemyCount--;
    }

    if (pEvent->GetType() == SpawnEnemyEvent::GetStaticEventType())
    {
        k_EnemyTypes selectedEnemyType = m_CurrentLevel.EnemyTypes.at(rand() % (m_CurrentLevel.EnemyTypes.size()) );
        SpawnEnemy(selectedEnemyType);
        m_enemyCount++;
    }

    if (pEvent->GetType() == CollisionEvent::GetStaticEventType())
    {
        CollisionEvent* event = static_cast<CollisionEvent*>(pEvent);
        event->GetObjectA()->OnCollisionHit(event->GetObjectB());
        event->GetObjectB()->OnCollisionHit(event->GetObjectA());
    }

    if (pEvent->GetType() == UpdatePlayerHealthEvent::GetStaticEventType())
    {
        for (int i = 0; i < k_Player_Health - m_pPlayer->GetHealth(); i++)
        {
            int index = k_Player_Health - 1 - i;

            if (index < 0)
                index = 0;

            m_pHealthIndicator.at(index)->SetHidden(true);
        }
    }
}

void Game::Draw()
{

    glClearColor(k_Color_Background.x, k_Color_Background.y, k_Color_Background.z, k_Color_Background.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10);
    glLineWidth(3);

    for (size_t i = 0; i < m_GameObjects.size(); i++)
    {
        m_GameObjects.at(i)->Draw();
    }

    m_pImguiMan->EndFrame();

}


void Game::AddGameObject(Vector2 position, std::string _file, std::string _name, Color _color, Vector2 _scale, fw::k_RenderTypes _rendertype)
{
    fw::Mesh* mesh = new fw::Mesh(_file, _rendertype);
    m_Meshes.push_back(mesh);

    fw::GameObject* gameobject = new fw::GameObject(position.x, position.y, mesh, _name, m_pShader, this);
    gameobject->SetScale(_scale);
    gameobject->SetColor(_color);
    m_GameObjects.push_back(gameobject);
}

fw::Mesh* Game::LoadMesh(std::string _address)
{
    fw::Mesh* ptr = new fw::Mesh(_address);
    m_Meshes.push_back(ptr);
    return ptr;
}

void Game::OnImGUI()
{
    if (k_Game_ShowDebug)
    {
        if (m_pPlayer != nullptr)
        {
            if (ImGui::CollapsingHeader("PLAYER"))
            {
                //ImGui::Text("Player");
                bool redrawPlayer = false;


                if (ImGui::SliderFloat("pRadius", &m_playerRadius, k_Player_MinRadius, m_ArenaRadius, "%.1f", 0))
                    redrawPlayer = true;

                if (ImGui::SliderFloat("Rotation", &m_playerAngle, 0.1f, 360.0f, "%.1f", 0))
                {
                    redrawPlayer = true;  m_playerManualRotation = true;
                }

                if (ImGui::SliderInt("Sides", &m_playerVertCount, 3, 72, "%d", 0))
                    redrawPlayer = true;

                if (ImGui::Checkbox("Filled", &m_playerIsFilled))
                    redrawPlayer = true;

                if (redrawPlayer)
                {
                    m_pPlayer->SetAngle(m_playerAngle);
                    m_pPlayer->SetCircle(m_playerRadius, m_playerVertCount, m_playerIsFilled);
                }

                //if (
                //    ImGui::SliderFloat("pRadius", &m_playerRadius, 0.05f, m_ArenaRadius, "%.1f", 0) ||
                //    ImGui::SliderFloat("Rotation", &m_playerAngle, 0.1f, 360.0f, "%.1f", 0) ||
                //    ImGui::SliderInt("Sides", &m_playerVertCount, 3, 72, "%d", 0) ||
                //    ImGui::Checkbox("Filled", &m_playerIsFilled)
                //    ) {
                //    m_pPlayer->GetMesh()->MakeCircle(m_playerRadius, m_playerVertCount, m_playerAngle, m_playerIsFilled);
                //    m_playerManualRotation = true;
                //}

                ImGui::Checkbox("Manual Rotation", &m_playerManualRotation);
            }
        }

        //ImGui::ShowDemoWindow();

        if (m_pArena != nullptr)
        {
            if (ImGui::CollapsingHeader("ARENA"))
            {
                //ImGui::Text("Arena");
                if (ImGui::SliderFloat("aRadius", &m_ArenaRadius, k_Arena_MinRadius, k_Arena_MaxRadius, "%.01f", 0))
                {
                    m_pArena->SetRadius(m_ArenaRadius);

                    if (m_ArenaRadius <= m_playerRadius)
                    {
                        m_playerRadius = m_ArenaRadius;
                        m_pPlayer->SetCircle(m_playerRadius, m_playerVertCount, m_playerIsFilled);
                    }
                }
            }
        }

        if (ImGui::CollapsingHeader("OBJECTS"))
        {

            for (fw::GameObject* pObject : m_GameObjects)
            {

                ImGui::PushID(pObject);
                if (ImGui::Button("Delete"))
                {
                    m_pEventManager->AddEvent(new RemoveFromGameEvent(pObject));
                }

                ImGui::SameLine();
                ImGui::Text("Name: %s", pObject->GetName());
                ImGui::PopID();
            }
        }

        ImGui::Checkbox("Vsync", &m_VSyncEnabled);
    };
}
