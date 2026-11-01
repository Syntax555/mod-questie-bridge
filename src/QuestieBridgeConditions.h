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

// A hidden giver needs a negative observation only when one of its quests still has a marker.
template <typename StartedQuests, typename QuestHidden>
bool HiddenGiverHasVisibleQuest(StartedQuests const& started, QuestHidden&& questHidden)
{
    for (std::uint32_t questId : started)
        if (!questHidden(questId))
            return true;
    return false;
}
}

#endif
