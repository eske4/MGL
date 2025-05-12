// Included header files:
#include "look_up_tables.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//=======================Helper functions for symbol table)============================

// Function to look up entries in symbol table
const SymbolTableEntry* LookUpSymbolTable(const SymbolTable table, const char* id)
{
    if (!table || !id)
        return NULL; // Safety check (to see if there exist an table or id)

    for (size_t i = 0; i < table->count; i++)
    { //use for loop to go through each element in the SymbolTable table
        if (strcmp(table->entries[i].ast_location->children[0]->data, id) == 0)
        {                              //use strcmp (string compare) to see if the id match (returns 0 if equal)
            return &table->entries[i]; //Return address of entry if match
        }
    }
    return NULL; // if not found function return null
}

// Function to look up entries in symbol table
int LookUpConnectSymbolTable(const SymbolTable table, const char* first_room, const char* second_room)
{
    if (!table || !first_room || !second_room)
        return 0; // Safety check (to see if there exist an table or id)

    for (size_t i = 0; i < table->count; i++)
    { //use for loop to go through each element in the SymbolTable table
        if (table->entries[i].ast_location->type == AT_CONNECT){
            if (table->entries[i].ast_location->children[1]->type == AT_BIDIRECTIONAL_EDGE) 
            {
                if (strcmp(table->entries[i].ast_location->children[0]->data, first_room)     == 0 
                    && strcmp(table->entries[i].ast_location->children[2]->data, second_room) == 0 
                    || strcmp(table->entries[i].ast_location->children[2]->data, first_room)  == 0 
                    && strcmp(table->entries[i].ast_location->children[0]->data, second_room) == 0  
                ) return 1; 
            }
            else if (strcmp(table->entries[i].ast_location->children[0]->data, first_room)    == 0 
                    && strcmp(table->entries[i].ast_location->children[2]->data, second_room) == 0  
                ) return 1; 
        }
    }
    return 0; // if not found, return 0 (FALSE)
}

// Function to add entry to symbol table (identifier + location)
void AddSymbolTable(SymbolTable table, const ASTNode* node)
{
    // if statement to grow capasity of symboltable if needed
    if (table->count >= table->capacity)
    {                                                        //check if the number entries in symbol table have reached the capacity
        size_t new_cap                = table->capacity * 2; //if the count is more/equal to capacity, define new capsity double the table capacity
        SymbolTableEntry* new_entries = realloc(
            table->entries,
            new_cap * sizeof(SymbolTableEntry)); //uses the realloc (re allocate) function to reserve memory for the new cap (new_cap times the memory of one symboltable entry)
        table->entries  = new_entries;           //update the entris in symboltable to new momory block returned by the realloc function
        table->capacity = new_cap;               //update the capacity of the symboltable to the new_cap
    }

    // add 1 more more entry to symboltable with the id and location
    table->entries[table->count++] = (SymbolTableEntry){
        .ast_location = node,
    };
}

// Function to put on constraint for symbol table
void ConstrSymbolTable(SymbolTable table, const ASTNode* node) 
{
    if (!table) return; 
    if (node->type == AT_MAP_CONSTR_ROOMS) table->room_constr = atoi(node->children[0]->data); 
    if (node->type == AT_MAP_CONSTR_CONNECT) table->connect_constr = atoi(node->children[0]->data); 
}

// Function to count the number of rooms in the symboltable
int CountRoomsSymbolTable(SymbolTable table)
{ 
    int count = 0; 
    for (int i = 0; i < table->count; i++) if (table->entries[i].ast_location->type == AT_ROOM) count += 1; 
    return count; 
}

// Function to find the number of connection the room with the most connection have
int FindMaxConnectSymbolTable (SymbolTable table)
{
    int max = 0;
    for (int i = 0; i < table->count; i++)
    {
        if (table->entries[i].ast_location->type == AT_ROOM) 
        { 
           const char* current_room_id = table->entries[i].ast_location->children[0]->data;
           int current_max = 0; 
           for (int j = 0; j < table->count; j++)
           {
                if (table->entries[j].ast_location->type == AT_CONNECT){
                    if (table->entries[j].ast_location->children[1]->type == AT_BIDIRECTIONAL_EDGE) 
                    {
                        if (strcmp(table->entries[j].ast_location->children[0]->data, current_room_id) == 0) current_max += 1;
                        if (strcmp(table->entries[j].ast_location->children[2]->data, current_room_id) == 0) current_max += 1;
                    } else 
                        if (strcmp(table->entries[j].ast_location->children[0]->data, current_room_id) == 0) current_max += 1;
                }
           }
           if (current_max > max) max = current_max;
        }
        
    }
    return max;
}

// Function to check if the symbol table does not violate (eventuelle) constraints 
int CheckConstrSymbolTable (SymbolTable table)
{
    if (table->room_constr != -1 && table->room_constr < CountRoomsSymbolTable(table)) return -1;
    
    int max_connect = FindMaxConnectSymbolTable(table);
    if (table->connect_constr != -1 && table->connect_constr < max_connect) return -1;

    int i = 8;
    while (i < max_connect)
        i *= 2;
    return i;
}

// Function to initiate the symbol table
SymbolTable InitSymbolTable(size_t initial_capacity)
{                                                  //function takes argument initial_capacity (how many entries the table should be able to hold)
    SymbolTable table       = calloc(1, sizeof(*table)); //use calloc function (contiguous allocation) to create space for 1 entry with the size of the table
    table->entries          = calloc(1, initial_capacity * sizeof(SymbolTableEntry));
    table->count            = 0;
    table->capacity         = initial_capacity;
    table->room_constr      = -1;
    table->connect_constr   = -1;  
    return table;
}

// Function to free the symbol table from memory
void FreeSymbolTable(SymbolTable table)
{
    free(table->entries);
    free(table);
}






//=====================Helper functions for constraint table)==========================

// Function to initiate the constraint table
ConstrTable InitConstrTable(size_t initial_capacity)
{
    ConstrTable table;
    table.map_count = 0;
    table.capacity  = initial_capacity;
    table.maps      = calloc(initial_capacity, sizeof(ConstrTableMap));
    return table;
}

// Function to add a Map to the constraint table
void AddMap(ConstrTable* table, const char* map_id)
{
    if (table->map_count >= table->capacity)
    {
        size_t new_cap           = table->capacity * 2;
        ConstrTableMap* new_maps = realloc(table->maps, new_cap * sizeof(ConstrTableMap));
        table->maps              = new_maps;
        table->capacity          = new_cap;
    }

    table->maps[table->map_count++] = (ConstrTableMap){.map_id               = map_id,
                                                       .room_constr          = 0,
                                                       .connect_constr       = 0,
                                                       .room_constr_value    = 0,
                                                       .connect_constr_value = 0,
                                                       .room_count           = 0,
                                                       .capacity             = 4, // initial room capacity
                                                       .rooms                = calloc(4, sizeof(ConstrTableRoom))};
}

// Function to add a Room to the constraint table
void AddRoomConstrTable(ConstrTable* table, const char* map_id, const char* room_id)
{
    for (size_t i = 0; i < table->map_count; i++)
    {
        if (table->maps[i].map_id == map_id)
        {
            if (table->maps[i].room_count >= table->maps[i].capacity)
            {
                size_t new_cap             = table->maps[i].capacity * 2;
                ConstrTableRoom* new_rooms = realloc(table->maps[i].rooms, new_cap * sizeof(ConstrTableRoom));
                table->maps[i].rooms       = new_rooms;
                table->maps[i].capacity    = new_cap;
            }

            table->maps[i].rooms[table->maps[i].room_count++] = (ConstrTableRoom){.room_id = room_id, .connect_count = 0};
        }
    }
}

// Function to increment the connection count for a Room by 1
void IncrementConnectCount(ConstrTable* table, const char* map_id, const char* room_id)
{
    for (size_t i = 0; i < table->map_count; i++)
    {

        if (strcmp(table->maps[i].map_id, map_id) == 0)
        {
            for (size_t j = 0; j < table->maps[i].room_count; j++)
            {

                if (strcmp(table->maps[i].rooms[j].room_id, room_id) == 0)
                {
                    table->maps[i].rooms[j].connect_count += 1;
                    return;
                }
            }
        }
        return;
    }
}

// Function to activate room constraint for a Map
void SetRoomConstr(ConstrTable* table, const char* map_id, int value)
{
    for (size_t i = 0; i < table->map_count; i++)
    {
        if (table->maps[i].map_id == map_id)
        {
            table->maps[i].room_constr       = 1;
            table->maps[i].room_constr_value = value;
            return;
        }
    }
}

// Function to activate connection constraint for a Map
void SetConnectConstr(ConstrTable* table, const char* map_id, int value)
{
    for (size_t i = 0; i < table->map_count; i++)
    {
        if (table->maps[i].map_id == map_id)
        {
            table->maps[i].connect_constr       = 1;
            table->maps[i].connect_constr_value = value;
            return;
        }
    }
}

// Check if any maps violate the room constraint
int CheckRoomConstr(ConstrTable* table)
{
    for (size_t i = 0; i < table->map_count; i++)
    {
        ConstrTableMap map = table->maps[i];

        // Only check for maps where constraint is activated
        if (map.room_constr == 1)
        {
            if ((int)map.room_count - 1 >= map.room_constr_value)
            {
                return 0;
            }
        }
    }
    return 1;
}

// Check if any Room violate connection constraint
int CheckConnectConstr(ConstrTable* table)
{
    for (size_t i = 0; i < table->map_count; i++)
    {
        ConstrTableMap* map = &table->maps[i];

        // Only check for maps where constraint is activated
        if (map->connect_constr == 1)
        {
            for (size_t j = 0; j < map->room_count; j++)
            {
                if (map->rooms[j].connect_count > map->connect_constr_value)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Function to find the Room with most connection
int FindMaxConnectCount(const ConstrTable* table, int max_connect)
{
    // Go through all Maps to check what the max connection are for 1 room:
    for (size_t i = 0; i < table->map_count; i++)
    {
        const ConstrTableMap* map = &table->maps[i];
        // Check if connection constraint is activated for map (no need to check each Room)
        if (map->connect_constr == 1)
        {
            map->connect_constr_value > max_connect ? max_connect = map->connect_constr_value : max_connect;
        }
        else
        {
            for (size_t j = 0; j < map->room_count; j++)
            {
                const ConstrTableRoom* room = &map->rooms[j];

                if (room->connect_count > max_connect)
                {
                    max_connect = room->connect_count;
                }
            }
        }
    }

    // Round max_connect up to nearest powers of 2 number
    int i = 1;
    while (i < max_connect)
        i *= 2;
    return i;
}

// Function to free the constraint table from memory
void FreeConstrTable(ConstrTable* table)
{
    for (size_t i = 0; i < table->map_count; i++)
    {
        free(table->maps[i].rooms);
    }
    free(table->maps);
}
