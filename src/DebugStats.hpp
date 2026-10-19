
#pragma once

#include <array>
#include <cstddef>

struct DebugStats
{
    float fps = 0.f;
    float frameTimeMs = 0.f;
    float simulationTimeMs = 0.f;
    float renderTimeMs = 0.f;

    std::size_t agentCount = 0;
    float averageSpeed = 0.f;

    std::size_t neighbourChecks = 0;
    std::size_t neighbourInteractions = 0;

    // Cautious, Impatient, Sociable, Independent
    std::array<std::size_t, 4> personalityCounts{};
};
