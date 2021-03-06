#include <cstdio>
#include <string>
#include "msgfactory.h"
#include "tok.h"
// comment the next line to hide debug info
#define DEBUG
#include "util.h"
#include "global.h"

extern FILE *yyin;
extern int yylex();     // lexer.cc provides yylex()
extern int yyparse();   // parser.cc provides yyparse()
extern MsgFactory msgFactory;
//extern InputNode *root; // AST's root, shared with yyparse()

int main(int argc, char** argv)
{
    if (handle_opt(argc, argv) == false)
        return 0;
    yyin = infp;        // infp is initialized in handle_opt()
    yyparse();
    /*if (dumpfp != NULL) {
        DumpDOT *dumper = new DumpDOT(dumpfp);
        root->dumpdot(dumper);
        delete dumper;
        fclose(dumpfp);
    }*/
    /* root->printast(stdout, 0); */

	msgFactory.summary();

    return 0;
}
