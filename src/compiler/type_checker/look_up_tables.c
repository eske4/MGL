#include "look_up_tables.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//linear search for entries with matching ids 
const SymbolTableEntry* LookUpSymbolTable(const SymbolTable table, const char* id) {
    if (!table || !id) return NULL;  // Safety check (to see if there exist an table or id)

    for (size_t i = 0; i < table->count; i++) { //use for loop to go through each element in the SymbolTable table
        if (strcmp(table->entries[i].id, id) == 0) {  //use strcmp (string compare) to see if the id match (returns 0 if equal)
            return &table->entries[i];  //Return address of entry if match
        }
    }
    return NULL; // if not found function return null
}

//add entry to symbol table (identifier + location)
void AddSymbolTable(SymbolTable table, const char* id, const ASTNode* node) {
    // if statement to grow capasity of symboltable if needed
    if (table->count >= table->capacity) { //check if the number entries in symbol table have reached the capacity
        size_t new_cap = table->capacity * 2;   //if the count is more/equal to capacity, define new capsity double the table capacity
        SymbolTableEntry* new_entries = realloc(table->entries, new_cap * sizeof(SymbolTableEntry)); //uses the realloc (re allocate) function to reserve memory for the new cap (new_cap times the memory of one symboltable entry)
        table->entries = new_entries; //update the entris in symboltable to new momory block returned by the realloc function
        table->capacity = new_cap; //update the capacity of the symboltable to the new_cap
    }

    // add 1 more more entry to symboltable with the id and location
    table->entries[table->count++] = (SymbolTableEntry){
        .id = id,
        .ast_location = node,
    };
}

//allocates and initilizes a new symbol table
SymbolTable InitSymbolTable(size_t initial_capacity) { //function takes argument initial_capacity (how many entries the table should be able to hold)
    SymbolTable table = calloc(1, sizeof(*table)); //use calloc function (contiguous allocation) to create space for 1 entry with the size of the table
    table->entries = calloc(1, initial_capacity * sizeof(SymbolTableEntry)); 
    table->count = 0;
    table->capacity = initial_capacity;
    return table;
}

//clean up memory
void FreeSymbolTable(SymbolTable table) {
    free(table->entries);
    free(table);
}

//=====================Helper functions for constraint table)==========================

ConstrTable InitConstrTable(size_t initial_capacity) {
    ConstrTable table;
    table.map_count = 0;
    table.capacity = initial_capacity;
    table.maps = calloc(initial_capacity, sizeof(ConstrTableMap));
    printf("\nconstrain table has been initialized\n");
    return table;
}

void AddMap(ConstrTable* table, const char* map_id) {
    if (table->map_count>= table->capacity) {
        size_t new_cap = table->capacity * 2;
        ConstrTableMap* new_maps = realloc(table->maps, new_cap * sizeof(ConstrTableMap));
        table->maps = new_maps;
        table->capacity = new_cap;
    }

    table->maps[table->map_count++] = (ConstrTableMap){
        .map_id = map_id,
        .room_constr = 0,
        .connect_constr = 0,
        .room_constr_value = 0,
        .connect_constr_value = 0,
        .room_count = 0,
        .capacity = 4,  // initial room capacity
        .rooms = calloc(4, sizeof(ConstrTableRoom))
    };
    printf("\nMap: %s has been added to constraint table\n", map_id);
}

void AddRoomToMap(ConstrTable* table, const char* map_id, const char* room_id) {
    for (size_t i = 0; i < table->map_count; i++) {
        if (table->maps[i].map_id == map_id) {
            if (table->maps[i].room_count >= table->maps[i].capacity) {
                size_t new_cap = table->maps[i].capacity * 2;
                ConstrTableRoom* new_rooms = realloc(table->maps[i].rooms, new_cap * sizeof(ConstrTableRoom));
                table->maps[i].rooms = new_rooms;
                table->maps[i].capacity = new_cap;
            }
        
            table->maps[i].rooms[table->maps[i].room_count++] = (ConstrTableRoom){
                .room_id = room_id,
                .connect_count = 0
            };
        }
    }
    printf("\nRoom: %s has been added to map: %s in constraint table\n", room_id, map_id);
}

void IncrementConnectCount(ConstrTable* table, const char* map_id, const char* room_id) {
    for (size_t i = 0; i < table->map_count; i++) {
        // First, check if this is the correct map
        if (strcmp(table->maps[i].map_id, map_id) == 0) {
            // Now search for the room in this map
            for (size_t j = 0; j < table->maps[i].room_count; j++) {
                if (strcmp(table->maps[i].rooms[j].room_id, room_id) == 0) {
                    table->maps[i].rooms[j].connect_count += 1;
                    printf("\nRoom: %s in map: %s now has %d connections\n", 
                           room_id, map_id, table->maps[i].rooms[j].connect_count);
                    return;
                }
            }
        }
    }
    printf("\nError: Room %s not found in map %s\n", room_id, map_id);
}

void SetRoomConstr(ConstrTable* table, const char*  map_id, int value) {
    for (size_t i = 0; i < table->map_count; i++) {
        if (table->maps[i].map_id == map_id) {
            table->maps[i].room_constr = 1;
            table->maps[i].room_constr_value = value;
            printf("\nRoom constraint has been activated in map: %s with value: %d\n", map_id, value);
            return;
        }
    }
}

void SetConnectConstr(ConstrTable* table, const char*  map_id, int value) {
    for (size_t i = 0; i < table->map_count; i++) {
        if (table->maps[i].map_id == map_id) {
            table->maps[i].connect_constr = 1;
            table->maps[i].connect_constr_value = value;
            printf("\nConnect constraint has been activated in map: %s connect_consr is : %d with value: %d\n", map_id, table->maps[i].connect_constr, table->maps[i].connect_constr_value);
            return;
        }
    }
}

int CheckRoomConstr (ConstrTable* table) {
    for (size_t i = 0; i < table->map_count; i++) {
        ConstrTableMap map = table->maps[i];

        if (map.room_constr == 1) {
            if ((int)map.room_count - 1 >= map.room_constr_value) {
                printf("\nconstraint check False");
                return 0;
            }
        }
    }
    printf("\n constraint check True");
    return 1; 
}
int CheckConnectConstr(ConstrTable* table) {
    int all_valid = 1;  // Start assuming all constraints are satisfied
    
    for (size_t i = 0; i < table->map_count; i++) {
        ConstrTableMap* map = &table->maps[i];

        if (map->connect_constr == 1) {
            printf("\nChecking connection constraints in map %s (max %d connections per room):",
                   map->map_id, map->connect_constr_value);
            
            for (size_t j = 0; j < map->room_count; j++) {
                printf("\n- Room %s: %d connections", 
                       map->rooms[j].room_id, map->rooms[j].connect_count);
                
                if (map->rooms[j].connect_count > map->connect_constr_value) {
                    printf(" [VIOLATION]");
                    all_valid = 0;  // Mark constraint as failed
                }
            }
        }
    }
    
    if (all_valid) {
        printf("\nAll connection constraints satisfied");
    } else {
        printf("\nConnection constraints violated!");
    }
    
    return all_valid;
}

#include <stdio.h>
#include <stdint.h>  // For uint32_t

int FindMaxConnectionCount(const ConstrTable* table) {
    if (table == NULL || table->map_count == 0) {
        printf("\nError: Empty constraint table\n");
        return -1; 
    }

    int max_connections = -1;

    // Find the actual maximum connection count
    for (size_t i = 0; i < table->map_count; i++) {
        const ConstrTableMap* map = &table->maps[i];
        
        for (size_t j = 0; j < map->room_count; j++) {
            const ConstrTableRoom* room = &map->rooms[j];
            
            if (room->connect_count > max_connections) {
                max_connections = room->connect_count;
            }
        }
    }

    printf("\nMaximum connections found: %d\n", max_connections);

    if (max_connections <= 64) {
        return 64;
    }

    uint32_t v = max_connections;
    v--;                   
    v |= v >> 1;           
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;                  
    
    if (v < max_connections) {
        v <<= 1;  
    }

    printf("Next power-of-two boundary: %d\n", v);
    return v;
}

void FreeConstrTable(ConstrTable* table) {
    for (size_t i = 0; i < table->map_count; i++) {
        free(table->maps[i].rooms);
    }
    free(table->maps);
    printf("\nConstraint table has been freed");
}