#include <mtimer.h>

namespace utils {

    CTimer::CTimer() :
#if USE(TIMER_REAL_TIME)
        m_start(0.0),
        m_stop(0.0),
#endif
        m_state(STOPPED),
        m_total(0.0)
    {}

    CTimer::~CTimer() {
    }

    bool CTimer::start() {
        switch(m_state) {
        case STOPPED:
            m_state = RUNNING;
            m_start = clock_t::now();
            break;
        case RUNNING:
            return false;
        default:
            return false;
        }
        return true;
    }

    bool CTimer::stop() {
        switch(m_state) {
        case STOPPED:
            return false;
        case RUNNING:
            m_state = STOPPED;
            m_stop = clock_t::now();
            m_total += getSeconds();
            break;
        default:
            return false;
            break;
        }
        return true;
    }

    bool CTimer::isRunning() {
        return (m_state == RUNNING);
    }

    double CTimer::getSeconds() {
        return (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(m_stop - m_start)).count() / 1000000000.0;
    }

    double CTimer::getTotalSeconds() {
        return m_total;
    }

    void CTimer::reset() {
        m_total = 0.0;
        m_start = time_t();
        m_stop = time_t();
        m_state = STOPPED;
    }

    ITimer::ITimer() {
    }

    ITimer::~ITimer() {
    }

    CTimer &ITimer::getTimer() {
        return m_timer;
    }

}
