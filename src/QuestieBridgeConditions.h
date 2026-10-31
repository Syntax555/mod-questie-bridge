/*
 * Copyright (C) 2026 Questie contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef QUESTIE_BRIDGE_CONDITIONS_H
#define QUESTIE_BRIDGE_CONDITIONS_H

#include <cstdint>
#include <set>

namespace QuestieBridge
{
// Quests a character cannot take because their quest availability conditions fail, such as
// personal progression milestones. Questie hides them like disabled quests until they open;
// a quest already in the log stays tracked. Server-wide disables are reported separately.
template <typename Quests, typename Satisfied, typename Disabled>
std::set<std::uint32_t> ConditionLockedQuests(Quests const& quests, Satisfied&& satisfied, Disabled&& disabled)
{
    std::set<std::uint32_t> locked;
    for (std::uint32_t questId : quests)
        if (!disabled(questId) && !satisfied(questId))
            locked.insert(questId);
    return locked;
}

// Whether an observed questgiver spawn must be reported as offering the character no quest. A visible
// NPC lacks live questgiver functionality; an NPC its scripts or visibility conditions hide offers
// nothing, unless every quest it starts is hidden already and its markers vanish anyway.
template <typename StartedQuests, typename QuestHidden>
bool UnavailableGiver(bool hidden, bool questgiver, StartedQuests const& started, QuestHidden&& questHidden)
{
    if (!hidden)
        return !questgiver;
    for (std::uint32_t questId : started)
        if (!questHidden(questId))
            return true;
    return false;
}
}

#endif
