Modules.ZeusAutoUlt = {
	Info = {
		Name = "Zeus: AutoUlt",
		Description = "Automatically uses Zeus' ultimate when an enemy has low health"
	},
	---@type boolean
	IsZeus = false,
	Init = function(self)
		self.IsZeus = (localHero:GetUnitName() == "npc_dota_hero_zuus")
	end,
	Reset = function(self)
		self.IsZeus = false;
	end,
	OnHero = function(self, enemy)
		if (not self.IsZeus) then return end

		if (enemy:GetTeam() == localHero:GetTeam()) then return end
		if (enemy:GetLifeState() ~= 0) then return end

		if (enemy:HasState(Enum.ModifierState.MODIFIER_STATE_MAGIC_IMMUNE) or
			enemy:HasState(Enum.ModifierState.MODIFIER_STATE_OUT_OF_GAME) or
			enemy:HasState(Enum.ModifierState.MODIFIER_STATE_INVULNERABLE)) then
			return
		end

		local ult = localHero:GetAbilityByIndex(5);
		if (localHero:CanUseAbility(ult)
			and enemy:GetHealth() + enemy:GetHealthRegen() <= ult:GetLevelSpecialValueFor("damage", -1) * (1 - enemy:GetMagicalArmorValue())) then
			localPlayer:CastNoTarget(ult, localHero)
		end
	end
}
