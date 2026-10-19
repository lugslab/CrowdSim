
#pragma once

#include <cstddef>

struct PerformanceSummary
{
    float averageFps = 0.f;
    float averageFrameMs = 0.f;
    float averageSimulationMs = 0.f;
    float averageRenderMs = 0.f;
    float worstFrameMs = 0.f;

    std::size_t frames = 0;
};

class PerformanceMonitor
{
public:
    void reset();

    void recordFrame(
        float frameMs,
        float simulationMs,
        float renderMs
    );

    PerformanceSummary getSummary() const;

private:
    double m_totalFrameMs = 0.0;
    double m_totalSimulationMs = 0.0;
    double m_totalRenderMs = 0.0;

    float m_worstFrameMs = 0.f;

    std::size_t m_frames = 0;
};
