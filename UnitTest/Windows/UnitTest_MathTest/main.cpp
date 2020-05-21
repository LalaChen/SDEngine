#include "SDEngine.h"
#include "WindowsLogManager.h"

using namespace SDE;
using namespace SDE::Math;
using namespace SDE::Basic;

//------------------------- Log ----------------------

class UnitTestLogManager : public WindowsLogManager
{
public:
    void LogToOutput(LogType i_type) override;
};

//construct for log.
void UnitTestLogManager::LogToOutput(LogType i_type)
{
    if (i_type == LogType::Normal) {
        std::cout << ((std::string("[Normal] ") + std::string(m_log_buffer) + std::string("\n")));
    }
    else if (i_type == LogType::Warning) {
        std::cout << ((std::string("[Warning] ") + std::string(m_log_buffer) + std::string("\n")));
    }
    else if (i_type == LogType::Internal) {
#ifndef _DEBUG
        //std::cout << "[Internal] " << m_log_buffer << std::endl;
#else
        std::cout << ((std::string("[Internal] ") + std::string(m_log_buffer) + std::string("\n")));
#endif
    }
    else {
        std::cout << ((std::string("[Error] ") + std::string(m_log_buffer) + std::string("\n")));
    }
}

int main(int argc, char **argv)
{
    std::cout << "UnitTest - MathSystem" << std::endl;
    new UnitTestLogManager();

    //T2
    Vector3f t2(1.0f, 2.0f, 3.0f, 1.0f);
    Quaternion r2(Vector3f(0.0f, 1.0f, 0.0f), 260.0f);
    Vector3f s2(1.0f, 2.0f, 3.0f, 0.0f);
    Matrix4X4f tr2;
    tr2.translate(t2);
    tr2.rotate(r2);
    tr2.scale(s2);
    SDLOG("LCS2:");
    SDLOG("%s", tr2.ToString().c_str());
    //T1
    Vector3f t1(2.0f, 3.0f, 4.0f, 1.0f);
    Quaternion r1(Vector3f(1.0f, 1.0f, 0.0f), 45.0f);
    Vector3f s1(2.0f, 3.0f, 4.0f, 0.0f);
    Matrix4X4f tr1;
    tr1.translate(t1);
    tr1.rotate(r1);
    tr1.scale(s1);
    SDLOG("LCS1:");
    SDLOG("%s", tr1.ToString().c_str());
    //T0
    Vector3f t0(3.0f, 4.0f, 5.0f, 1.0f);
    Quaternion r0(Vector3f(0.0f, 1.0f, 1.0f), 45.0f);
    Vector3f s0(3.0f, 4.0f, 5.0f, 0.0f);
    Matrix4X4f tr0;
    tr0.translate(t0);
    tr0.rotate(r0);
    tr0.scale(s0);
    SDLOG("LCS0:");
    SDLOG("%s", tr0.ToString().c_str());

    Matrix4X4f result = tr2 * tr1 * tr0;
    SDLOG("Result:");
    SDLOG("%s", result.ToString().c_str());

    //Decompose
    Vector3f de_trans, de_scale, de_skew, de_prespective;
    Quaternion de_rotation;
    Matrix4X4f::decompose(result, de_scale, de_rotation, de_skew, de_trans, de_prespective);
    SDLOG("Decomp Result:");
    SDLOG("--- Trans    : %s", de_trans.ToString().c_str());
    SDLOG("--- Rotation : %s", de_rotation.ToString().c_str());
    SDLOG("--- Scale    : %s", de_scale.ToString().c_str());
    SDLOG("--- Skew     : %s", de_skew.ToString().c_str());
    SDLOG("--- Prep     : %s", de_prespective.ToString().c_str());

    //Transform :: lookat
    Transform trans;
    trans = Transform::LookAt(Vector3f(0.0, 0.0, 1.0, 1.0), Vector3f::Origin, Vector3f::PositiveY, true);
    SDLOG("Transfrom World: %s", trans.MakeWorldMatrix().ToString().c_str());
    SDLOG("Transfrom View: %s", trans.MakeViewMatrix().ToString().c_str());
}