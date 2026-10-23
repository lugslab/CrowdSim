
#pragma once

#include "Agent.hpp"

#include <cstddef>
#include <iterator>
#include <vector>

class AgentRange
{
public:
    AgentRange(
        const std::vector<Agent>& agents,
        const std::vector<std::size_t>& indices)
        : m_agents_snapshot(agents)
        , m_indices(indices)
    {
    }

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Agent;
        using difference_type = std::ptrdiff_t;
        using pointer = const Agent*;
        using reference = const Agent&;

        Iterator(
            const std::vector<Agent>& agents,
            std::vector<std::size_t>::const_iterator indexIterator)
            : m_agents_snapshot(agents)
            , m_indexIterator(indexIterator)
        {
        }

        reference operator*() const
        {
            return m_agents_snapshot.at(*m_indexIterator);
        }

        pointer operator->() const
        {
            return &operator*();
        }

        Iterator& operator++()
        {
            ++m_indexIterator;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(const Iterator& other) const
        {
            return m_indexIterator == other.m_indexIterator;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

    private:
        const std::vector<Agent>& m_agents_snapshot;
        std::vector<std::size_t>::const_iterator m_indexIterator;
    };

    Iterator begin() const
    {
        return Iterator(m_agents_snapshot, m_indices.begin());
    }

    Iterator end() const
    {
        return Iterator(m_agents_snapshot, m_indices.end());
    }

private:
    const std::vector<Agent>& m_agents_snapshot;
    const std::vector<std::size_t>& m_indices;
};
