/**
 * @file compiler.c
 * @brief Main compilation pipeline with built-in debugging
 * 
 * Stages:
 *   1. Lexing    (debug output in lexer.c)
 *   2. Parsing   (debug AST print on success)
 *   3. Typecheck (debug output in type_checker.c)
 *   4. IL Gen    (debug table print)
 *   5. Codegen   (outputs assembly directly)
 */

#include "astree.h"
#include "compiler_state.h"
#include "lexer.h"
#include "parser.h"
#include "type_checker.h"
#include "il.h"
#include "code_gen.h"

/**
 * @brief Runs the full compiler pipeline with preserved debug outputs
 * @param input Source file path
 * 
 * @note Debug outputs are hardcoded in submodules (void functions).
 *       Cleanup happens regardless of stage failures.
 */
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
