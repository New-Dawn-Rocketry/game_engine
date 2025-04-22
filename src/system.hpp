#include "ecs_globals.hpp"

class system_base
{
public:
    std::set<entity> entities;
};

class system_manager
{
public:
    template <typename T>
    std::shared_ptr<T> register_system()
    {
        const char system_id = typeid(T);
        auto system = std::make_shared<T>();
        systems[system_id] = system;
        return system;
    }

    template <typename T>
    void set_signature(entity_signature signature)
    {
        const char system_id = typeid(T);
        signatures[system_id] = signature;
    }

    void entity_destroyed(entity* _entity)
    {
        for (auto& pair : systems)
        {
            auto const& system = pair.second;
            system->entities.erase(*_entity);
        }
    }

    void update_entity_signature(entity* _entity, entity_signature signature)
    {
        for (auto& pair : systems)
        {
            auto const& system_id = pair.first;
            auto const& system = pair.second;
            auto const& system_signature = signatures[system_id];

            if ((signature & system_signature) == system_signature)
            {
                system->entities.insert(*_entity);
            }
            else
            {
                system->entities.erase(*_entity);
            }
        }
    }

private:
    std::unordered_map<const char*, entity_signature> signatures;
    std::unordered_map<const char*, std::shared_ptr<system_base>> systems;
};