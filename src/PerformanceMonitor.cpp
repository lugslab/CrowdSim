
#include "PerformanceMonitor.hpp"

#include <algorithm>

void PerformanceMonitor::reset()
{
    m_totalFrameMs = 0.0;
    m_totalSimulationMs = 0.0;
    m_totalRenderMs = 0.0;

    m_worstFrameMs = 0.f;
    m_frames = 0;
}

void PerformanceMonitor::recordFrame(
    float frameMs,
    float simulationMs,
    float renderMs
)
{
    m_totalFrameMs += frameMs;
    m_totalSimulationMs += simulationMs;
    m_totalRenderMs += renderMs;

    m_worstFrameMs = std::max(
        m_worstFrameMs, frameMs
    );

    ++m_frames;
}

PerformanceSummary PerformanceMonitor::getSummary() const
{
    PerformanceSummary summary;

    if (m_frames == 0)
        return summary;

    summary.frames = m_frames;

    summary.averageFrameMs =
        static_cast<float>(m_totalFrameMs / m_frames);

    summary.averageSimulationMs =
        static_cast<float>(m_totalSimulationMs / m_frames);

    summary.averageRenderMs =
        static_cast<float>(m_totalRenderMs / m_frames);

    summary.worstFrameMs = m_worstFrameMs;

    // FPS calculated from total frame time.
    if (m_totalFrameMs > 0.0)
    {
        summary.averageFps =
            static_cast<float>(
                1000.0 * m_frames / m_totalFrameMs
                );
    }

    return summary;
}
