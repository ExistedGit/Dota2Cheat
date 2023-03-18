Modules.Core = {
    CallModuleFunc = function(name, ...)
        for k, v in pairs(Modules) do
            if (k ~= "Core" and v[name] ~= nil) then
                v[name](v, ...);
            end
        end
    end,
    EntityIteration = function(self)
        for idx, ent in ctx.heroes:pairs() do
            self.CallModuleFunc("OnHero", ent);
        end
    end
}
