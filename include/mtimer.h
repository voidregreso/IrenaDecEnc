#ifndef _MTIMER_H
#define _MTIMER_H

#include <utils.h>
#include <chrono>

namespace utils {

    enum TimerState {
        STOPPED = 0,
        RUNNING
    };

    typedef std::chrono::high_resolution_clock clock_t;
    typedef std::chrono::time_point<clock_t> time_t;

    class CTimer {
    public:
        CTimer();
        virtual ~CTimer();
        virtual bool start();
        virtual bool stop();
        virtual bool isRunning();
        virtual void reset();
        virtual double getSeconds();
        virtual double getTotalSeconds();
    protected:
        time_t m_start;
        time_t m_stop;
        TimerState m_state;
        double m_total;
    };

    class ITimer {
    public:
        ITimer();
        virtual ~ITimer();
        virtual CTimer &getTimer();
    protected:
        CTimer m_timer;
    };

}

#endif //_MTIMER_H
