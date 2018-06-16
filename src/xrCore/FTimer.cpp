#include "stdafx.h"
#include "xrCommon/xr_vector.h"

XRCORE_API bool g_bEnableStatGather = false;

CStatTimer::CStatTimer()
{
    accum = 0;
    result = 0.f;
    count = 0;
}

void CStatTimer::FrameStart()
{
    accum = 0;
    count = 0;
}

void CStatTimer::FrameEnd()
{
    const float time = 1000.f * float(double(accum) / double(CPU::qpc_freq));
    if (time > result)
        result = time;
    else
        result = 0.99f * result + 0.01f * time;
}

XRCORE_API pauseMngr& g_pauseMngr()
{
    static pauseMngr manager;
    return manager;
}

pauseMngr::pauseMngr() : paused(false) { m_timers.reserve(3); }
void pauseMngr::Pause(const bool b)
{
    if (paused == b)
        return;

    for (auto& timer : m_timers)
    {
        timer->Pause(b);
    }

    paused = b;
}

void pauseMngr::Register(CTimer_paused& t) { m_timers.push_back(&t); }

void pauseMngr::UnRegister(CTimer_paused& t)
{
    const auto it = std::find(m_timers.cbegin(), m_timers.cend(), &t);
    if (it != m_timers.end())
        m_timers.erase(it);
}
