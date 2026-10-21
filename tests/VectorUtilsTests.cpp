#include <gtest/gtest.h>
#include "VectorUtils.hpp"

TEST(VectorUtilsTests, NormaliseNonZeroCalculations)
{
    sf::Vector2f vector_a(200.f, 0.f);

    const auto normalised_a = VectorUtils::normalise(vector_a);

    EXPECT_FLOAT_EQ(normalised_a.x, 1.f);
    EXPECT_FLOAT_EQ(normalised_a.y, 0.f);

    sf::Vector2f vector_b(0.f, 50.f);

    const auto normalised_b = VectorUtils::normalise(vector_b);

    EXPECT_FLOAT_EQ(normalised_b.x, 0.f);
    EXPECT_FLOAT_EQ(normalised_b.y, 1.f);

    sf::Vector2f vector_c(4.f, 4.f);

    const auto normalised_c = VectorUtils::normalise(vector_c);

    EXPECT_FLOAT_EQ(normalised_c.x, 0.70710677f);
    EXPECT_FLOAT_EQ(normalised_c.y, 0.70710677f);
}

TEST(VectorUtilsTests, NormaliseZeroVector)
{
    sf::Vector2f vector(0.f, 0.f);

    const auto normalised = VectorUtils::normalise(vector);

    EXPECT_FLOAT_EQ(normalised.x, 0.f);
    EXPECT_FLOAT_EQ(normalised.y, 0.f);
}

TEST(VectorUtilsTests, IsZeroRecognisesZeroVector)
{
    EXPECT_TRUE(VectorUtils::isZero({ 0.f, 0.f }));
}

TEST(VectorUtilsTests, IsZeroRecognisesVectorsInsideEpsilon)
{
    EXPECT_TRUE(VectorUtils::isZero({ 0.000099f, 0.f }));
    EXPECT_TRUE(VectorUtils::isZero({ -0.000099f, 0.f }));
    EXPECT_TRUE(VectorUtils::isZero({ 0.00005f, 0.00005f }));
}

TEST(VectorUtilsTests, IsZeroRejectsVectorAtEpsilon)
{
    EXPECT_FALSE(VectorUtils::isZero({ 0.0001f, 0.f }));
    EXPECT_FALSE(VectorUtils::isZero({ -0.0001f, 0.f }));
}

TEST(VectorUtilsTests, IsZeroRejectsVectorsOutsideEpsilon)
{
    EXPECT_FALSE(VectorUtils::isZero({ 0.000101f, 0.f }));
    EXPECT_FALSE(VectorUtils::isZero({ 0.f, -0.000101f }));
    EXPECT_FALSE(VectorUtils::isZero({ 1.f, 1.f }));
}

