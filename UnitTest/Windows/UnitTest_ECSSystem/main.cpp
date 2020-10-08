#include "SDEngine.h"
#include "SDEnginePlatform.h"
#include "GLFWApplication.h"

using namespace SDE;
using namespace SDE::App;
using namespace SDE::Basic;
using namespace SDE::Graphics;

class ComponentA : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION;
public:
    explicit ComponentA(const ObjectName &i_object_name, uint32_t i_a, uint32_t i_b, uint32_t i_c) : Component(i_object_name), m_a(i_a), m_b(i_b), m_c(i_c){}
    virtual ~ComponentA() {}
public:
    uint32_t m_a, m_b, m_c;
};

SD_COMPONENT_POOL_TYPE_IMPLEMENTATION(ComponentA, ComponentA);

class ComponentB : public Component
{
public:
    SD_COMPONENT_POOL_TYPE_DECLARATION;
public:
    explicit ComponentB(const ObjectName &i_object_name, const std::string &i_name1, const std::string &i_name2) : Component(i_object_name), m_name1(i_name1), m_name2(i_name2){}
    virtual ~ComponentB() {}
public:
    std::string m_name1;
    std::string m_name2;
};

SD_COMPONENT_POOL_TYPE_IMPLEMENTATION(ComponentB, ComponentB);

class GameSystem : public System
{
public:
    explicit GameSystem(const ObjectName &i_object_name) : System(i_object_name) {}
    virtual ~GameSystem() {}
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
protected:
    EntityGroupWeakReferenceObject m_target_eg_wref;
};

void GameSystem::Initialize()
{
    m_target_eg_wref = ECSManager::GetRef().AddEntityGroup(
        "GameSystem",
        {
            std::type_index(typeid(ComponentA)),
            std::type_index(typeid(ComponentB))
        }
    );

    EntityWeakReferenceObject eab_wref = ECSManager::GetRef().CreateEntity("EAB_1");
    ECSManager::GetRef().AddComponentForEntity<ComponentA>(eab_wref, "EAB_1_CA", 1, 2, 3);
    ECSManager::GetRef().AddComponentForEntity<ComponentB>(eab_wref, "EAB_1_CA", "ComponentBTest1!!!", "ComponentBTest2!!!");
}

void GameSystem::Update()
{
}

void GameSystem::Destroy()
{
}

class ECSTestApplication : public GLFWApplication
{
public:
    explicit ECSTestApplication(const std::string &i_win_title, const Resolution &i_win_res, 
        FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
    virtual ~ECSTestApplication();
};


ECSTestApplication::ECSTestApplication(const std::string& i_win_title, const Resolution& i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char** i_argv)
: GLFWApplication(i_win_title, i_win_res, i_full_window, i_adopt_library, i_argc, i_argv)
{
}

ECSTestApplication::~ECSTestApplication()
{
}

int main()
{
    ECSTestApplication app(
        std::string("UnitTest_FeatureDeveloping"),
        Resolution(1200, 800),
        false, //full screen can't debug.
        GraphicsLibrary_Vulkan,
        0,
        nullptr
    );

    app.Initialize();

    app.RunMainLoop();
}