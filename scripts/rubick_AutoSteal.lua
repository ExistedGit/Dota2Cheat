Modules.RubickAutoSteal = {
    Info = {
        Name = 'Rubick: AutoSteal',
        Description = 'Automatically steals ultimates as Rubick'
    },
    ---@type boolean
    IsRubick = false,
    LastSpells = {},
    Init = function(self)
        self.IsRubick = (localHero:GetUnitName() == "npc_dota_hero_rubick")
    end,
    Reset = function(self)
        self.IsRubick = false;
    end,
    IsSpellJustUsed = function(self, spell)
        return math.abs(spell:GetCooldown() - spell:GetCooldownLength()) <= 0.5;
    end,
    IsLastSpell = function(self, spell)
        for i, v in pairs(self.LastSpells) do
            if (v == spell) then
                return true
            end
        end
        return false;
    end,
    OnHero = function(self, enemy)
        if (not self.IsRubick) then return end

        for i, spell in pairs(self.LastSpells) do
            if (not self:IsSpellJustUsed(spell)) then
                self.LastSpells[i] = nil;
            end
        end

        if (enemy:GetTeam() == localHero:GetTeam()) then return end
        if (enemy:GetLifeState() ~= 0) then return end

        if (enemy:HasState(Enum.ModifierState.MODIFIER_STATE_MAGIC_IMMUNE) or
            enemy:HasState(Enum.ModifierState.MODIFIER_STATE_OUT_OF_GAME) or
            enemy:HasState(Enum.ModifierState.MODIFIER_STATE_INVULNERABLE)) then
            return
        end

        local ult = localHero:GetAbilityByIndex(5);
        local enemyUlt = enemy:GetAbilityByIndex(5);

        if (enemyUlt:GetCooldown() == 0) then return end
        if (self:IsLastSpell(enemyUlt)) then return end

        local range = ult:GetEffectiveCastRange();
        if (localHero:CanUseAbility(ult) and self:IsSpellJustUsed(enemyUlt)) then
            if (localHero:FindItemBySubstring('scepter')) then range = range + 400 end

            if (not IsWithinRadius(enemy:GetPos(), localHero:GetPos(), range)) then return end

            localPlayer:CastTarget(ult, enemy, localHero);
            self.LastSpells[#self.LastSpells + 1] = enemyUlt;
        end
    end
}
