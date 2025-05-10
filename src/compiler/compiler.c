#include "astree.h"
#include "compiler_state.h"
#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "il.h"
#include "code_gen.h"

void compile(const char* input)
{
    // Initialize compiler state
    csInit();
    csOpenFile(input);
    Token currentToken;

    // --- STAGE 1: Lexing ---
    scan(&currentToken); // Initialize the Lexer

    // --- STAGE 2: Parsing ---
    ASTree tree = parse(&currentToken);
    if (tree && tree->head)
        ASTreePrint(tree); // Parser's built-in debug print

    // --- STAGE 3: Semantic check ---
    TypeCheck(tree); // TypeChecker prints its own debug info
    // Move print out to here

    // --- STAGE 4: IL Generation ---
    InstructionTable ilTable = compile_to_il(tree);
    instr_print(ilTable); // IL module's debug print

    // Cleanup ASTreee
    if (tree)
        ASTFree(tree);
        
    // --- STAGE 5: Codegen ---
    generate_assembly(ilTable); // Outputs assembly directly

    // Cleanup IL table
    if (ilTable)
        instr_free(ilTable);
}
