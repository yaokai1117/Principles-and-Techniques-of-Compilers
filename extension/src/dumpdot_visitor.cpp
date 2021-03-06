#include <cstdio>
#include <vector>
#include <sstream>
#include "dumpdot_visitor.h"
#include "dumpdot.h"
#include "node.h"

using namespace std;


void DumpDotVisitor::dumpList(int length, int nRoot, int pos)
{
	for (int i = 0; i < length; i++) {
		int nChild = pending.back();
		pending.pop_back();
		dumper->drawLine(nRoot, pos, nChild);
	}
}


DumpDotVisitor::DumpDotVisitor(FILE *file)
{
	orderChanged = false;
	dumper = new DumpDOT(file);
}


DumpDotVisitor::~DumpDotVisitor()
{
	delete dumper;
}


void DumpDotVisitor::visitNodeList(NodeList *node)
{
}


void DumpDotVisitor::visitNumNode(NumNode *node)
{
	std::stringstream ss;
	ss << node->val;
	int nThis = dumper->newNode(1, ss.str().c_str());
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitFNumNode(FNumNode *node)
{
	std::stringstream ss;
	ss << node->fval;
	int nThis = dumper->newNode(1, ss.str().c_str());
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitCharNode(CharNode *node)
{
	std::stringstream ss;
	ss << node->cval;
	int nThis = dumper->newNode(1, ss.str().c_str());
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitBinaryExpNode(BinaryExpNode *node)
{
	char st[2] = " ";
	st[0] = node->op;
	int nThis = dumper->newNode(3, " ", st, " ");

	int nrhs = pending.back();
	pending.pop_back();
	int nlhs = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 0, nlhs);
	dumper->drawLine(nThis, 2, nrhs);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitUnaryExpNode(UnaryExpNode *node)
{
	char st[2] = " ";
	st[0] = node->op;
	int nThis = dumper->newNode(2, st, " ");

	int nOperand = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 1, nOperand);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitIdNode(IdNode *node)
{
	int nThis = dumper->newNode(1, node->name->c_str());
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitArrayItemNode(ArrayItemNode *node)
{
	int nThis = dumper->newNode(4, " ", "\\[", " ", "\\]");

	int nArray = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 0, nArray);
	int length = node->index->nodes.size();
	dumpList(length, nThis, 2);

	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitStructItemNode(StructItemNode *node)
{
	int nThis;
	if (node->isPointer)
		nThis = dumper->newNode(3, " ", "-\\>", node->itemName->c_str());
	else
		nThis = dumper->newNode(3, " ", ".", node->itemName->c_str());
	int nStru = pending.back();
	pending.pop_back();
	dumper->drawLine(nThis, 0, nStru);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitFunCallNode(FunCallNode *node)
{
	int nThis = dumper->newNode(4, " ", "\\(", " ", "\\)");
	int nFunc = pending.back();
	pending.pop_back();
	dumper->drawLine(nThis, 0, nFunc);
	if (node->hasArgs) {
		int length = node->argv->nodes.size();
		dumpList(length, nThis, 2);
	}
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitIdVarDefNode(IdVarDefNode *node)
{
	int nThis = 0;
	if (node->isAssigned) {
		nThis = dumper->newNode(3, node->name->c_str(), "=", " ");
		int nValue = pending.back();
		pending.pop_back();
		dumper->drawLine(nThis, 2, nValue);
	}
	else {
		nThis = dumper->newNode(1, node->name->c_str());
	}
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitArrayVarDefNode(ArrayVarDefNode *node)
{
	int nThis = 0;
	if (node->isAssigned) {
		nThis = dumper->newNode(8, node->name->c_str(), "\\[", " ", "\\]", "=", "\\{", " ", "\\}");

		int length = node->values->nodes.size();
		dumpList(length, nThis, 6);
	}
	else {
		nThis = dumper->newNode(4, node->name->c_str(), "\\[", " ", "\\]");
	}
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitEmptyNode(EmptyNode *node)
{
	int nThis = dumper->newNode(1, "empty");
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitBlockNode(BlockNode *node)
{
	int nThis = dumper->newNode(3, "\\{", " ", "\\}");
	int length = node->blockItems->nodes.size();
	dumpList(length, nThis, 1);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitVarDeclNode(VarDeclNode *node)
{
	string typeStr;
	switch (node->valueTy.type) {
	case INT_TYPE:
		typeStr = "int";
		break;
	case FLOAT_TYPE:
		typeStr = "float";
		break;
	case CHAR_AST:
		typeStr = "char";
		break;
	case STRUCT_TYPE:
		typeStr = string("struct ") + *(node->valueTy.structName);
		break;
	default:
		typeStr = "unknown type";
		break;
	}
	int nThis = dumper->newNode(2, typeStr.c_str(), " ");
	int length = node->defList->nodes.size();
	dumpList(length, nThis, 1);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitAssignStmtNode(AssignStmtNode *node)
{
	int nThis = dumper->newNode(3, " ", "=", " ");

	int nExp = pending.back();
	pending.pop_back();
	int nLVal = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 0, nLVal);
	dumper->drawLine(nThis, 2, nExp);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitFunCallStmtNode(FunCallStmtNode *node)
{
}


void DumpDotVisitor::visitBlockStmtNode(BlockStmtNode *node)
{
}


void DumpDotVisitor::visitCondNode(CondNode *node)
{
	char op_str[4] = "   ";
	char op = node->op;
	switch (op) {
	case AND_OP:
		op_str[0] = 'a';
		op_str[1] = 'n';
		op_str[2] = 'd';
		break;
	case OR_OP:
		op_str[0] = 'o';
		op_str[1] = 'r';
		break;
	case NOT_OP:
		op_str[0] = 'n';
		op_str[1] = 'o';
		op_str[2] = 't';
		break;
	case LT_OP:
		op_str[0] = '\\';
		op_str[1] = '<';
		break;
	case LTE_OP:
		op_str[0] = '\\';
		op_str[1] = '<';
		op_str[2] = '=';
		break;
	case GT_OP:
		op_str[0] = '\\';
		op_str[1] = '>';
		break;
	case GTE_OP:
		op_str[0] = '\\';
		op_str[1] = '>';
		op_str[2] = '=';
		break;
	case EQ_OP:
		op_str[1] = '=';
		op_str[2] = '=';
		break;
	case NEQ_OP:
		op_str[1] = '!';
		op_str[2] = '=';
		break;
	default:
		break;
	}
	int nThis = dumper->newNode(3, " ", op_str, " ");

	int nRhs = pending.back();
	pending.pop_back();
	dumper->drawLine(nThis, 2, nRhs);

	if (op != NOT_OP) {
		int nLhs = pending.back();
		pending.pop_back();
		dumper->drawLine(nThis, 0, nLhs);
	}

	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitIfStmtNode(IfStmtNode *node)
{
	int nThis = 0;
		if (node->hasElse) {
			nThis = dumper->newNode(6, "if", " ", "then", " ", "else", " ");

			int nElse = pending.back();
			pending.pop_back();
			int nThen = pending.back();
			pending.pop_back();
			int nCond = pending.back();
			pending.pop_back();

			dumper->drawLine(nThis, 1, nCond);
			dumper->drawLine(nThis, 3, nThen);
			dumper->drawLine(nThis, 5, nElse);
		}
		else {
			nThis = dumper->newNode(4, "if", " ", "then", " ");

			int nThen = pending.back();
			pending.pop_back();
			int nCond = pending.back();
			pending.pop_back();

			dumper->drawLine(nThis, 1, nCond);
			dumper->drawLine(nThis, 3, nThen);
		}
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitWhileStmtNdoe(WhileStmtNode *node)
{
	int nThis = dumper->newNode(3, "while", " ", " ");

	int nDo = pending.back();
	pending.pop_back();
	int nCond = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 1, nCond);
	dumper->drawLine(nThis, 2, nDo);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitReturnStmtNdoe(ReturnStmtNode *node)
{
	int nThis = dumper->newNode(2, "return", " ");

	int nExp  = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 1, nExp);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitBreakStmtNode(BreakStmtNode *node)
{
	int nThis = dumper->newNode(1, "break");
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitContinueStmtNode(ContinueStmtNode *node)
{
	int nThis = dumper->newNode(1, "continue");
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitFuncDeclNode(FuncDeclNode *node)
{
	int nThis = dumper->newNode(5, "void", node->name->c_str(), "\\(", " ", "\\)");
	if (node->hasArgs) {
		NodeList *argv = node->valueTy.argv;
		argv->accept(*this);
		int length = argv->nodes.size();
		dumpList(length, nThis, 3);
	}
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitFuncDefNode(FuncDefNode *node)
{
	int nThis = dumper->newNode(2, "definition", "body");

	int nBlock = pending.back();
	pending.pop_back();
	int nDecl = pending.back();
	pending.pop_back();

	dumper->drawLine(nThis, 0, nDecl);
	dumper->drawLine(nThis, 1, nBlock);

	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitStructDefNode(StructDefNode *node)
{
	int nThis = dumper->newNode(3, "struct", node->name->c_str(), "\\{ \\}");
	int length = node->decls->nodes.size();
	dumpList(length, nThis, 2);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::visitCompUnitNode(CompUnitNode *node)
{
	int nThis = dumper->newNode(1, "CompUnit");
	int length = node->nodes.size();
	dumpList(length, nThis, 0);
	pending.insert(pending.end(), nThis);
}


void DumpDotVisitor::enterBlockNode(BlockNode *node)
{
}


void DumpDotVisitor::enterIfStmtNode(IfStmtNode *node)
{
}


void DumpDotVisitor::enterWhileStmtNode(WhileStmtNode *node)
{
}


void DumpDotVisitor::enterFuncDefNode(FuncDefNode *node)
{
}


void DumpDotVisitor::enterStructDefNode(StructDefNode *node)
{
}



