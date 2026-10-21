#include <gtest/gtest.h>

#include "Personality.hpp"

TEST(PersonalityTests, CreatesCautiousPersonality)
{
    const Personality personality = Personality::create(PersonalityType::Cautious);

    EXPECT_EQ(personality.type, PersonalityType::Cautious);
    EXPECT_FLOAT_EQ(personality.preferredSpeed, 40.f);
    EXPECT_FLOAT_EQ(personality.detectionRadius, 55.f);
    EXPECT_FLOAT_EQ(personality.personalSpace, 22.f);
    EXPECT_FLOAT_EQ(personality.avoidanceWeight, 2.5f);
    EXPECT_FLOAT_EQ(personality.goalWeight, 0.8f);
    EXPECT_FLOAT_EQ(personality.socialWeight, 0.f);
}

TEST(PersonalityTests, CreatesImpatientPersonality)
{
    const Personality personality = Personality::create(PersonalityType::Impatient);

    EXPECT_EQ(personality.type, PersonalityType::Impatient);
    EXPECT_FLOAT_EQ(personality.preferredSpeed, 85.f);
    EXPECT_FLOAT_EQ(personality.detectionRadius, 25.f);
    EXPECT_FLOAT_EQ(personality.personalSpace, 8.f);
    EXPECT_FLOAT_EQ(personality.avoidanceWeight, 0.7f);
    EXPECT_FLOAT_EQ(personality.goalWeight, 1.4f);
    EXPECT_FLOAT_EQ(personality.socialWeight, 0.f);
}

TEST(PersonalityTests, CreatesSociablePersonality)
{
    const Personality personality = Personality::create(PersonalityType::Sociable);

    EXPECT_EQ(personality.type, PersonalityType::Sociable);
    EXPECT_FLOAT_EQ(personality.preferredSpeed, 50.f);
    EXPECT_FLOAT_EQ(personality.detectionRadius, 35.f);
    EXPECT_FLOAT_EQ(personality.personalSpace, 14.f);
    EXPECT_FLOAT_EQ(personality.avoidanceWeight, 1.3f);
    EXPECT_FLOAT_EQ(personality.goalWeight, 1.f);
    EXPECT_FLOAT_EQ(personality.socialWeight, 0.8f);
}

TEST(PersonalityTests, CreatesIndependentPersonalityWithDefaultValues)
{
    const Personality personality = Personality::create(PersonalityType::Independent);

    EXPECT_EQ(personality.type, PersonalityType::Independent);
    EXPECT_FLOAT_EQ(personality.preferredSpeed, 60.f);
    EXPECT_FLOAT_EQ(personality.detectionRadius, 30.f);
    EXPECT_FLOAT_EQ(personality.personalSpace, 12.f);
    EXPECT_FLOAT_EQ(personality.avoidanceWeight, 1.2f);
    EXPECT_FLOAT_EQ(personality.goalWeight, 1.f);
    EXPECT_FLOAT_EQ(personality.socialWeight, 0.f);
}