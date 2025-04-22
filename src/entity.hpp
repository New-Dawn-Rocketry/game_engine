#include "ecs_globals.hpp"



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