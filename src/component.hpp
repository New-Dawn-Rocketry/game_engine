#include "ecs_globals.hpp"

class component_array_base
{
public:
    virtual ~component_array_base() = default;
    virtual void entity_destroyed(entity* _entity) = 0;
};

template <typename T>
class component_array : public component_array_base
{
public:
    void insert_new_component(entity* _entity, T new_component)
    {
        // get the new index based on the current size
        size_t new_index = current_idx;
        id2component[_entity->id] = new_index;
        component2id[new_index] = _entity->id;
        components[new_index] = new_component;
        current_idx++;
    }
    void remove_component(entity* _entity)
    {
        /// move last index component
        size_t invalid_index = id2component[_entity->id];
        size_t last_index = current_idx - 1;
        components[invalid_index] = last_index;

        entity moved_entity = component2id[last_index];
        id2component[moved_entity] = invalid_index;
        component2id[invalid_index] = moved_entity;

        id2component.erase(_entity->id);
        component2id.erase(last_index);
        current_idx--;
    }
    const T& get_component(entity* _entity)
    {
        return components[id2component[_entity->id]];
    }
    void entity_destroyed(entity* _entity) override
    {
        if (id2component[_entity->id] != id2component.end())
            remove_component(_entity);
    }
private:
    std::array<T, c_max_entities>components;
    std::unordered_map<entity_id, size_t> id2component;
    std::unordered_map<size_t, entity_id> component2id;
    size_t current_idx = 0;
};


class component_manager
{
public:
    
    template<typename T>
    void register_component()
    {
        const char* component_name = typeid(T);
        component_ids[component_name] = get_new_id();
        component_arrays[component_name] = make_shared<component_array<T>>();
    }

    template<typename T>
    component_id get_component_id()
    {
        return component_ids[typeid(T)];
    }

    template<typename T>
    void add_instance(entity* _entity, T component)
    {
        get_component_array<T>()->insert_new_component(_entity, component);
    }

    template<typename T>
    void remove_instance(entity* _entity)
    {
        get_component_array<T>()->remove_component(_entity);
    }

    template<typename T>
    T& get_instance(entity* _entity)
    {
        return get_component_array<T>()->get_component(_entity);
    }

    void entity_destroyed(entity* _entity)
    {
        for (auto const& pair : component_arrays)
        {
            auto const& component = pair.second;
            component->entity_destroyed(_entity);
        }
    }

private:
    component_id get_new_id() { return next_id++; }
    std::unordered_map<const char*, component_id> component_ids;
    std::unordered_map<const char*, std::shared_ptr<component_array_base>> component_arrays;
    component_id next_id = 0;

    template<typename T>
    std::shared_ptr<component_array<T>> get_component_array()
    {
        const char* component_name = typeid(T);
        return static_pointer_cast<component_array<T>>(component_arrays[component_name]);
    }
};