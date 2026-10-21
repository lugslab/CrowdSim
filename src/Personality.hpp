
#pragma once

enum class PersonalityType
{
    Cautious,
    Impatient,
    Sociable,
    Independent
};

struct Personality
{
    // default values for Independent personality type
    PersonalityType type = PersonalityType::Independent;

    float preferredSpeed = 60.f;
    float detectionRadius = 30.f;
    float personalSpace = 12.f;

    float avoidanceWeight = 1.2f;
    float goalWeight = 1.f;
    float socialWeight = 0.f;

    static Personality create(PersonalityType type)
    {
        Personality p;
        p.type = type;

        switch (type)
        {
        case PersonalityType::Cautious:
            p.preferredSpeed = 40.f;
            p.detectionRadius = 55.f;
            p.personalSpace = 22.f;
            p.avoidanceWeight = 2.5f;
            p.goalWeight = 0.8f;
            break;

        case PersonalityType::Impatient:
            p.preferredSpeed = 85.f;
            p.detectionRadius = 25.f;
            p.personalSpace = 8.f;
            p.avoidanceWeight = 0.7f;
            p.goalWeight = 1.4f;
            break;

        case PersonalityType::Sociable:
            p.preferredSpeed = 50.f;
            p.detectionRadius = 35.f;
            p.personalSpace = 14.f;
            p.avoidanceWeight = 1.3f;
            p.socialWeight = 0.8f;
            break;

        case PersonalityType::Independent:
            // already configured with default values
            break;
        }

        return p;
    }
};
