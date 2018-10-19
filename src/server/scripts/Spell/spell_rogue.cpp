/*
 * Copyright (C) 2008-2018 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_ROGUE and SPELLFAMILY_GENERIC spells used by rogue players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_rog_".
 */

#include "ScriptPCH.h"

enum RogueSpells
{
    ROGUE_SPELL_CHEATING_DEATH                   = 45182
};

class spell_rog_cheat_death_SpellScript : public SpellScript
{
    bool Validate(SpellEntry const * spellEntry)
    {
        if (!sSpellStore.LookupEntry(ROGUE_SPELL_CHEATING_DEATH))
            return false;
        return true;
    }

    void HandleDummy(SpellEffIndex effIndex)
    {
        Unit *caster = GetCaster();
        caster->CastSpell(caster, ROGUE_SPELL_CHEATING_DEATH, true);
    }

    void Register()
    {
        // add dummy effect spell handler to Cheat Death
        EffectHandlers += EffectHandlerFn(spell_rog_cheat_death_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

SpellScript *GetSpellScript_spell_rog_cheat_death()
{
    return new spell_rog_cheat_death_SpellScript();
}

void AddSC_rogue_spell_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "spell_rog_cheat_death";
    newscript->GetSpellScript = &GetSpellScript_spell_rog_cheat_death;
    newscript->RegisterSelf();
}
