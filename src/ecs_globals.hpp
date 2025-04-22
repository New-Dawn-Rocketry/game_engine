#pragma once

#include <cstdint>
#include <array>
#include <bitset>
#include <set>
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

/// TODO: Continue to add methods so that the entity can have more functionality
/// TODO: Error check just in case we have more than um.. 2^32 - 1 objects (-1 since the value -1 will represent uninitialized)
struct entity
{
    entity_id id = -1;
    world* _world;

    entity& operator= (const entity_id& other) { id = other; return *this; }

};

extern class entity_manager;

extern class component_array_base;
extern class component_manager;

extern class system_base;
extern class system_manager;

/// manages inter_manager communications
extern class world;