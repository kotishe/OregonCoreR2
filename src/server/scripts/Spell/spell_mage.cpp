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
 * Scripts for spells with SPELLFAMILY_MAGE and SPELLFAMILY_GENERIC spells used by mage players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mag_".
 */

#include "ScriptPCH.h"

enum MageSpells
{
    SPELL_MAGE_SQUIRREL_FORM                     = 32813,
    SPELL_MAGE_GIRAFFE_FORM                      = 32816,
    SPELL_MAGE_SERPENT_FORM                      = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM                   = 32818,
    SPELL_MAGE_WORGEN_FORM                       = 32819,
    SPELL_MAGE_SHEEP_FORM                        = 32820
};

class spell_mage_polymorph_cast_visual_SpellScript : public SpellScript
{
    bool Validate(SpellEntry const * spellEntry)
    {
        const uint32 spell_list[6] = {
            SPELL_MAGE_SQUIRREL_FORM,
            SPELL_MAGE_GIRAFFE_FORM,
            SPELL_MAGE_SERPENT_FORM,
            SPELL_MAGE_DRAGONHAWK_FORM,
            SPELL_MAGE_WORGEN_FORM,
            SPELL_MAGE_SHEEP_FORM
        };

        // check if spell ids exist in dbc
        for (int i = 0; i < 6; i++)
            if (!sSpellStore.LookupEntry(spell_list[i]))
                return false;
        return true;
    };

    void HandleDummy(SpellEffIndex effIndex)
    {
        const uint32 spell_list[6] = {
            SPELL_MAGE_SQUIRREL_FORM,
            SPELL_MAGE_GIRAFFE_FORM,
            SPELL_MAGE_SERPENT_FORM,
            SPELL_MAGE_DRAGONHAWK_FORM,
            SPELL_MAGE_WORGEN_FORM,
            SPELL_MAGE_SHEEP_FORM
        };

        if (Unit *unitTarget = GetHitUnit())
            if (unitTarget->GetTypeId() == TYPEID_UNIT)
                unitTarget->CastSpell(unitTarget, spell_list[urand(0, 5)], true);
    }

    void Register()
    {
        // add dummy effect spell handler to Polymorph visual
        EffectHandlers += EffectHandlerFn(spell_mage_polymorph_cast_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

SpellScript * GetSpellScript_spell_mage_polymorph_visual()
{
    return new spell_mage_polymorph_cast_visual_SpellScript();
}

void AddSC_mage_spell_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "spell_mage_polymorph_visual";
    newscript->GetSpellScript = &GetSpellScript_spell_mage_polymorph_visual;
    newscript->RegisterSelf();
}
