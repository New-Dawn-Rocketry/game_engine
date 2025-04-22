#include "ecs_globals.hpp"
#include "entity.hpp"
#include "component.hpp"
#include "system.hpp"


/// manages inter_manager communications
class world
{
public:

    entity create_entity()
    {
        entity _entity;
        _entity_manager.create_entity(&_entity);
        return _entity;
    }

    void destroy_entity(entity* _entity)
    {
        _entity_manager.destroy_entity(_entity);
        _component_manager.entity_destroyed(_entity);
        _system_manager.entity_destroyed(_entity);
    }

    template<typename T>
    void register_component()
    {
        _component_manager.register_component<T>();
    }
    
    template <typename T>
    void add_component(entity* _entity, T instance)
    {
        _component_manager.add_instance<T>(_entity, instance);
        _entity_manager.add_component_id(_entity, _component_manager.get_component_id<T>());
        _system_manager.update_entity_signature(_entity, _entity_manager.get_entity_signature(_entity));
    }

    template <typename T>
    void remove_component(entity* _entity)
    {
        _component_manager.remove_instance<T>(_entity);
        _entity_manager.remove_component_id(_entity, _component_manager.get_component_id<T>());
        _system_manager.update_entity_signature(_entity, _entity_manager.get_entity_signature(_entity));
    }

    template <typename T>
    void register_system(entity_signature system_signature)
    {
        _system_manager.register_system<T>();
        _system_manager.set_signature<T>(system_signature);
    }
    
    
    /// facilitates intercommunication
private:
    entity_manager _entity_manager;
    component_manager _component_manager;
    system_manager _system_manager;
};