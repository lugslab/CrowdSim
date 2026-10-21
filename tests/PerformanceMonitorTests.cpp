#include <gtest/gtest.h>

#include "PerformanceMonitor.hpp"

TEST(PerformanceMonitorTests, RecordFrameAccumulatesTimingStatistics)
{
    PerformanceMonitor monitor;

    monitor.recordFrame(16.f, 10.f, 6.f);
    monitor.recordFrame(24.f, 15.f, 9.f);

    const PerformanceSummary summary = monitor.getSummary();

    EXPECT_EQ(summary.frames, 2u);
    EXPECT_FLOAT_EQ(summary.averageFrameMs, 20.f);
    EXPECT_FLOAT_EQ(summary.averageSimulationMs, 12.5f);
    EXPECT_FLOAT_EQ(summary.averageRenderMs, 7.5f);
    EXPECT_FLOAT_EQ(summary.worstFrameMs, 24.f);
    EXPECT_FLOAT_EQ(summary.averageFps, 50.f);
}

TEST(PerformanceMonitorTests, RecordFrameRetainsWorstFrameWhenLaterFrameIsShorter)
{
    PerformanceMonitor monitor;

    monitor.recordFrame(30.f, 20.f, 10.f);
    monitor.recordFrame(12.f, 8.f, 4.f);

    const PerformanceSummary summary = monitor.getSummary();

    EXPECT_EQ(summary.frames, 2u);
    EXPECT_FLOAT_EQ(summary.worstFrameMs, 30.f);
}

TEST(PerformanceMonitorTests, RecordFrameCountsZeroDurationFrames)
{
    PerformanceMonitor monitor;

    monitor.recordFrame(0.f, 0.f, 0.f);

    const PerformanceSummary summary = monitor.getSummary();

    EXPECT_EQ(summary.frames, 1u);
    EXPECT_FLOAT_EQ(summary.averageFrameMs, 0.f);
    EXPECT_FLOAT_EQ(summary.averageSimulationMs, 0.f);
    EXPECT_FLOAT_EQ(summary.averageRenderMs, 0.f);
    EXPECT_FLOAT_EQ(summary.worstFrameMs, 0.f);
    EXPECT_FLOAT_EQ(summary.averageFps, 0.f);
}