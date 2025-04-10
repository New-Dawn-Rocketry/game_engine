#include <cstdint>
#include <array>
#include <bitset>
#include <queue>
#include <unordered_map>
#include <memory>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

using entity_id = std::uint32_t;

const entity_id c_max_entities = UINT32_MAX - 1;

using component_id = std::uint8_t;

const component_id c_max_types = UINT8_MAX;

using entity_signature = std::bitset<c_max_types>;

class entity_manager;
template <typename T> class component_array;

inline entity_manager _entity_manager;

/// TODO: Continue to add methods so that the entity can have more functionality
/// TODO: Error check just in case we have more than um.. 2^32 - 1 objects (-1 since the value -1 will represent uninitialized)
struct entity
{
    entity_id id = -1;
    /*
    /// is it worth it? we shall see...
    struct proxy
    {
        const entity* parent = nullptr;
        proxy(entity* parent) : parent(parent) {}
        operator const entity_signature() { return _entity_manager.get_entity_signature((entity*)parent);}
    }signature = proxy(this);   
    /*/  
    const entity_signature& signature() { return _entity_manager.get_entity_signature(this); }
    //*/

    void create() { _entity_manager.create_entity(this); }
    void destroy() { _entity_manager.destroy_entity(this);}
};



/// We have seperate arrays for the entity and each component type

class entity_manager 
{
public:
entity_manager()
{
    /// I will keep this the same for now but I eventually want to accomplish this in a different way
    for (entity_id id = 0; id < c_max_entities; id++)
    {
        unused_ids.push(id);
    }
}
void create_entity(entity* _entity)
{
    _entity->id = unused_ids.front();
    unused_ids.pop();
}
void destroy_entity(entity* _entity)
{
    unused_ids.push(_entity->id);
    signatures[_entity->id].reset();
}
void add_component_id(entity* _entity, component_id _component_id)
{
    signatures[_entity->id].set(_component_id);
}
void remove_component_id(entity* _entity, component_id _component_id)
{
    signatures[_entity->id].reset(_component_id);
}
const entity_signature get_entity_signature(entity* _entity)
{
    return signatures[_entity->id];
}
private:
    std::queue<entity_id>unused_ids;
    std::array<entity_signature, c_max_entities> signatures;
};

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
        
    }

    template<typename T>
    T& get_instance(entity* _entity)
    {
        
    }

    template<typename T>
    void entity_destroyed(entity* _entity)
    {
        
    }

private:
    component_id get_new_id() { return next_id++; }
    unordered_map<const char*, component_id> component_ids;
    unordered_map<const char*, shared_ptr<component_array_base>> component_arrays;
    component_id next_id = 0;

    template<typename T>
    shared_ptr<component_array<T>> get_component_array()
    {
        const char* component_name = typeid(T);
        return static_pointer_cast<component_array<T>>(component_arrays[component_name]);
    }
};