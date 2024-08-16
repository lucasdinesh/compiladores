#include "tacs.h"
#include "hash.h"
#include "stdlib.h"
#include <string.h>

tac *newTac(int type, hash_node *res, hash_node *op1, hash_node *op2)
{
	tac *n = (tac *)calloc(1, sizeof(tac));
	n->type = type;
	n->res = res;
	n->op1 = op1;
	n->op2 = op2;
	n->prev = NULL;
	n->next = NULL;
	return n;
}

tac *tacJoin(tac *l1, tac *l2)
{
	tac *t;
	if (!l1)
		return l2;
	if (!l2)
		return l1;
	t = l2;
	while (t->prev)
	{
		t = t->prev;
	}
	t->prev = l1;
	return l2;
}

tac *reverseTacList(tac *head)
{
	tac *current;

	for (current = head; current->prev; current = current->prev)
	{
		current->prev->next = current;
	}
	return current;
}

void printAllTacs(tac *l)
{
	tac *t;
	for (t = l; t; t = t->next)
		printTac(t);
}

void printTac(tac *l)
{

	if (l->type == TAC_SYMBOL)
		return;

	fprintf(stderr, "TAC(");
	switch (l->type)
	{
	case TAC_PRINT:
		fprintf(stderr, "PRINT(");
		break;
	case TAC_BEGINFUN:
		fprintf(stderr, "BEGINFUN(");
		break;
	case TAC_DIV:
		fprintf(stderr, "DIV(");
		break;
	case TAC_LESS:
		fprintf(stderr, "LESS(");
		break;
	case TAC_AND:
		fprintf(stderr, "AND(");
		break;
	case TAC_VEC_CALL:
		fprintf(stderr, "VEC_CALL(");
		break;
	case TAC_GE:
		fprintf(stderr, "GE(");
		break;
	case TAC_ARGLREST:
		fprintf(stderr, "TAC_ARGLREST(");
		break;
	case TAC_CALL:
		fprintf(stderr, "CALL(");
		break;
	case TAC_LABEL:
		fprintf(stderr, "LABEL(");
		break;
	case TAC_ARG:
		fprintf(stderr, "ARG(");
		break;
	case TAC_SUB:
		fprintf(stderr, "SUB(");
		break;
	case TAC_LE:
		fprintf(stderr, "LE(");
		break;
	case TAC_MUL:
		fprintf(stderr, "MUL(");
		break;
	case TAC_RETURN:
		fprintf(stderr, "RETURN(");
		break;
	case TAC_OR:
		fprintf(stderr, "OR(");
		break;
	case TAC_ENDFUN:
		fprintf(stderr, "ENDFUN(");
		break;
	case TAC_VECATTR:
		fprintf(stderr, "VECATTR(");
		break;
	case TAC_IFZ:
		fprintf(stderr, "IFZ(");
		break;
	case TAC_READ:
		fprintf(stderr, "READ(");
		break;
	case TAC_COPY:
		fprintf(stderr, "COPY(");
		break;
	case TAC_JUMP:
		fprintf(stderr, "JUMP(");
		break;
	case TAC_EQ:
		fprintf(stderr, "EQ(");
		break;
	case TAC_NOT:
		fprintf(stderr, "NOT(");
		break;
	case TAC_DIF:
		fprintf(stderr, "DIF(");
		break;
	case TAC_GREAT:
		fprintf(stderr, "GREAT(");
		break;
	case TAC_ADD:
		fprintf(stderr, "ADD(");
		break;
	default:
		fprintf(stderr, "UNKNOWN(");
		break;
	}

	if (l->res)
		fprintf(stderr, "%s, ", l->res->text);
	else
		fprintf(stderr, "0, ");

	if (l->op1)
		fprintf(stderr, "%s, ", l->op1->text);
	else
		fprintf(stderr, "0, ");

	if (l->op2)
		fprintf(stderr, "%s", l->op2->text);
	else
		fprintf(stderr, "0");

	fprintf(stderr, "))\n");
}

tac *createTacs(AST *node, hash_node *currentLoopLabel)
{
	if (!node)
		return NULL;

	tac *son[MAX_SONS];
	if (node->type == AST_WHILE)
	{
		currentLoopLabel = makeLabel();
	}
	for (int i = 0; i < MAX_SONS; i++)
		son[i] = createTacs(node->son[i], currentLoopLabel);

	switch (node->type)
	{
	case AST_SYMBOL:
		return newTac(TAC_SYMBOL, node->symbol, 0, 0);
	case AST_SUB:
		return generateBinaryOpTac(TAC_SUB, son);
	case AST_ADD:
		return generateBinaryOpTac(TAC_ADD, son);
	case AST_DIV:
		return generateBinaryOpTac(TAC_DIV, son);
	case AST_MULT:
		return generateBinaryOpTac(TAC_MUL, son);
	case AST_GREATHER:
		return generateBinaryOpTac(TAC_GREAT, son);
	case AST_LESS:
		return generateBinaryOpTac(TAC_LESS, son);
	case AST_GE:
		return generateBinaryOpTac(TAC_GE, son);
	case AST_LE:
		return generateBinaryOpTac(TAC_LE, son);
	case AST_EQ:
		return generateBinaryOpTac(TAC_EQ, son);
	case AST_DIF:
		return generateBinaryOpTac(TAC_DIF, son);
	case AST_NOT:
		return generateBinaryOpTac(TAC_NOT, son);
	case AST_OR:
		return generateBinaryOpTac(TAC_OR, son);
	case AST_AND:
		return generateBinaryOpTac(TAC_AND, son);
	case AST_VECATTR:
		return tacJoin(son[0], tacJoin(son[1], newTac(TAC_VECATTR, node->symbol, son[0] ? son[0]->res : 0, son[1] ? son[1]->res : 0)));
	case AST_ATTR:
		return tacJoin(son[0], newTac(TAC_COPY, node->symbol, son[0] ? son[0]->res : 0, 0));
	case AST_READ:
		return newTac(TAC_READ, node->symbol, 0, 0);
	case AST_PRINT:
		return tacJoin(tacJoin(son[0], newTac(TAC_PRINT, son[0] ? son[0]->res : 0, 0, 0)), son[1]);
	case AST_RETURN:
		return tacJoin(son[0], newTac(TAC_RETURN, son[0] ? son[0]->res : 0, 0, 0));
	case AST_IFELSE:
	case AST_IF:
		return createIf(son);
	case AST_WHILE:
		return createWhileLoop(son, currentLoopLabel);
	case AST_FUNC_CALL:
		return tacJoin(son[0], newTac(TAC_CALL, makeTemp(), node->symbol, 0));
	case AST_ARGL:
	case AST_ARGLREST:
		return tacJoin(son[1], tacJoin(son[0], newTac(TAC_ARGLREST, son[0] ? son[0]->res : 0, 0, 0)));
	case AST_VEC_CALL:
		return tacJoin(son[0], newTac(TAC_VEC_CALL, makeTemp(), node->symbol, son[0] ? son[0]->res : 0));
	case AST_DECFUNC:
		return createFunction(newTac(TAC_SYMBOL, node->symbol, 0, 0), son[1], son[2]);
	default:
		return tacJoin(tacJoin(tacJoin(son[0], son[1]), son[2]), son[3]);
	}
}

tac *generateBinaryOpTac(int type, tac *operands[])
{
	hash_node *firstOperand = 0;
	hash_node *secondOperand = 0;

	if (operands[0])
	{
		firstOperand = operands[0]->res;
	}

	if (operands[1])
	{
		secondOperand = operands[1]->res;
	}

	return tacJoin(operands[0], tacJoin(operands[1], newTac(type, makeTemp(), firstOperand, secondOperand)));
}

tac *createIf(tac *son[])
{
	hash_node *ifLabel = makeLabel();
	tac *ifTac = tacJoin(son[0], newTac(TAC_IFZ, ifLabel, son[0] ? son[0]->res : 0, 0));
	tac *ifLabelTac = newTac(TAC_LABEL, ifLabel, 0, 0);

	if (son[2])
	{
		hash_node *elseLabel = makeLabel();
		tac *elseLabelTac = newTac(TAC_LABEL, elseLabel, 0, 0);
		tac *elseJumpTac = newTac(TAC_JUMP, elseLabel, 0, 0);
		tac *ifElseTac = tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(ifTac, son[1]), elseJumpTac), ifLabelTac), son[2]), elseLabelTac);
		return ifElseTac;
	}
	else
	{
		return tacJoin(tacJoin(ifTac, son[1]), ifLabelTac);
	}
}

tac *createWhileLoop(tac *son[], hash_node *whileLabel)
{
	hash_node *jumpLabel = makeLabel();

	tac *whileTac = newTac(TAC_IFZ, jumpLabel, son[0] ? son[0]->res : 0, 0);
	tac *whileLabelTac = newTac(TAC_LABEL, whileLabel, 0, 0);
	tac *jumpTac = newTac(TAC_JUMP, whileLabel, 0, 0);
	tac *jumpLabelTac = newTac(TAC_LABEL, jumpLabel, 0, 0);

	return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(whileLabelTac, son[0]), whileTac), son[1]), jumpTac), jumpLabelTac);
}

tac *createFunction(tac *symbol, tac *params, tac *code)
{
	return tacJoin(tacJoin(tacJoin(newTac(TAC_BEGINFUN, symbol->res, 0, 0), params), code), newTac(TAC_ENDFUN, symbol->res, 0, 0));
}

void addData(FILE *fout, AST *node)
{
	if (!node)
		return;

	if (node->type == AST_DECVET)
	{
		char *variable = calloc(strlen(node->son[1]->symbol->text) + 1, sizeof(char));
		fprintf(fout, "_%s:\n", node->symbol->text);

		for (AST *aux = node->son[2]; aux; aux = aux->son[1])
		{
			char *value = calloc(strlen(aux->son[0]->symbol->text) + 1, sizeof(char));
			value = aux->son[0]->symbol->text;
			if (node->son[0]->type == AST_TYPEFLOAT)
			{
				fprintf(fout, "\t.float\t%s\n", value);
			}
			else
			{
				fprintf(fout, "\t.long\t%s\n", value);
			}
		}
	}

	for (int i = 0; i < MAX_SONS; i++)
	{
		addData(fout, node->son[i]);
	}
}

void addImmediates(FILE *fout)
{
	hash_node **hash = getHash();
	for (int i = 0; i < HASH_SIZE; i++)
	{
		for (hash_node *aux = hash[i]; aux; aux = aux->next)
		{
			if ((aux->type == SYMBOL_LIT_INTEGER || aux->type == SYMBOL_LIT_REAL) || aux->type == SYMBOL_LIT_CHAR)
			{
				fprintf(fout, "_%s:\n", aux->text);

				if (aux->type == SYMBOL_LIT_REAL)
				{
					// char *conversion = calloc(strlen(aux->text)+1, sizeof(char));
					fprintf(fout, "\t.float  %s\n", aux->text);
					// free(conversion);
				}
				else if (aux->type == SYMBOL_LIT_INTEGER)
				{
					// char *conversion = calloc(strlen(aux->text)+1, sizeof(char));

					fprintf(fout, "\t.long   %s\n", aux->text);
				}
				else if (aux->type == SYMBOL_LIT_CHAR)
				{
					fprintf(fout, "\t.long   %d\n", aux->text[1]); // 'x' will print ascii of x only
				}
			}
		}
	}
}

void addTemporaries(FILE *fout)
{
	hash_node **hash = getHash();

	for (int i = 0; i < HASH_SIZE; i++)
	{
		for (hash_node *aux = hash[i]; aux; aux = aux->next)
		{
			if (strncmp(aux->text, "mYWeeirT", 8) == 0)
			{
				fprintf(fout, "_%s:\n", aux->text);
				if (aux->datatype == DATATYPE_FLOAT)
				{
					fprintf(fout, "\t.float  0.0\n");
				}
				else
				{
					fprintf(fout, "\t.long  0\n");
				}
			}
		}
	}
}

void generateASM(tac *first, AST *node)
{
	tac *tac;
	FILE *fout;
	fout = fopen("out.s", "w");
	int jumpAux = 0;

	// INIT
	fprintf(fout,
			"## FIXED INIT\n"
			"printNumber:.string	\"%%d\\n\"\n"
			"printSTR: .string \"%%s\\n\"\n"
			"printReal: .string \"%%f\\n\"\n"
			"printChar: .string \"%%c\\n\"\n"
			"\n");

	for (tac = first; tac; tac = tac->next)
	{

		switch (tac->type)
		{
		case TAC_BEGINFUN:
			fprintf(fout,
					"## TAC_BEGINFUN"
					"\n"
					"\t.globl %s\n"
					"%s:\n"
					"\tpushq %%rbp\n"
					"\n",
					tac->res->text, tac->res->text);

			break;
		case TAC_ENDFUN:
			fprintf(fout,
					"## TAC_ENDFUN"
					"\n"
					"\tpopq %%rbp"
					"\n"
					"\tret"
					"\n"
					"\n");
			break;
		case TAC_PRINT:

			if (tac->res->type == SYMBOL_LIT_STRING)
			{
				fprintf(fout,
						"## TAC_PRINT_STRING\n"
						"\tleaq _%s(%%rip), %%rax\n"
						"\tmovq	%%rax, %%rsi\n"
						"\tleaq printSTR(%%rip), %%rax\n"
						"\tmovq %%rax, %%rdi\n"
						"\tcall printf@PLT\n\n",
						tac->res->tempAssemblyName);
			}

			if (tac->res->type == SYMBOL_IDENTIFIER)
			{
				fprintf(fout, "## TAC_PRINT_INT\n"
							  "\tmovl\t_%s(%%rip), %%eax\n"
							  "\tmovl\t%%eax, %%esi\n"
							  "\tleaq\tprintNumber(%%rip), %%rax\n"
							  "\tmovq\t%%rax, %%rdi\n"
							  "\tcall\tprintf@PLT\n\n",
						tac->res->text);
			}

			else
			{
				switch (tac->res->datatype)
				{
				case DATATYPE_FLOAT:
					fprintf(fout,
							"## TAC_PRINT_FLOAT\n"
							"\tmovss\t_%s(%%rip), %%xmm0\n"
							"\tpxor\t%%xmm1, %%xmm1\n"
							"\tcvtss2sd\t%%xmm0, %%xmm1\n"
							"\tmovq\t %%xmm1, %%rax\n"
							"\tmovq\t%%rax, %%xmm0\n"
							"\tleaq\tprintReal(%%rip), %%rax\n"
							"\tmovq\t%%rax, %%rdi\n"
							"\tmovl\t$1, %%eax\n"
							"\tcall\tprintf@PLT\n"
							"\tmovl\t$0, %%eax\n\n",
							tac->res->text);
					break;
				case DATATYPE_CHAR:
					fprintf(fout, "## TAC_PRINT_CHAR\n"
								  "movzbl\t_%s(%%rip), %%eax\n"
								  "movsbl\t%%al, %%eax\n"
								  "movl\t%%eax, %%esi\n"
								  "leaq\tprintChar(%%rip), %%rax\n"
								  "movq\t%%rax, %%rdi\n"
								  "\tcall\tprintf@PLT\n\n",
							tac->res->text);
					break;
				case DATATYPE_INT:
					fprintf(fout, "## TAC_PRINT_INT\n"
								  "\tmovl\t_%s(%%rip), %%eax\n"
								  "\tmovl\t%%eax, %%esi\n"
								  "\tleaq\tprintNumber(%%rip), %%rax\n"
								  "\tmovq\t%%rax, %%rdi\n"
								  "\tcall\tprintf@PLT\n\n",
							tac->res->text);
					break;
				case DATATYPE_BOOL:
					fprintf(fout,
							"## TAC_PRINT_BOOL 1 is TRUE 0 is False\n"
							"\tmovl\t_%s(%%rip), %%eax\n"
							"\tmovl\t%%eax, %%esi\n"
							"\tleaq\tprintNumber(%%rip), %%rax\n"
							"\tmovq\t%%rax, %%rdi\n"
							"\tcall\tprintf@PLT\n\n",
							tac->res->text);
					break;
				}
			}
			break;
		case TAC_ADD:
			fprintf(fout, "## TAC_ADD\n"
						  "\tmovl\t_%s(%%rip), %%edx\n"
						  "\tmovl\t_%s(%%rip), %%eax\n"
						  "\taddl\t%%edx, %%eax\n",
					tac->op1->text, tac->op2->text);
			break;
		case TAC_SUB:
			fprintf(fout, "## TAC_SUB\n"
						  "\tmovl\t_%s(%%rip), %%eax\n"
						  "\tmovl\t_%s(%%rip), %%edx\n"
						  "\tsubl\t%%edx, %%eax\n\n",
					tac->op1->text, tac->op2->text);
			break;
		case TAC_MUL:
			fprintf(fout, "## TAC_MUL\n"
						  "\tmovl\t_%s(%%rip), %%edx\n"
						  "\tmovl\t_%s(%%rip), %%eax\n"
						  "\timull\t%%edx, %%eax\n\n",
					tac->op1->text, tac->op2->text);
			break;
		case TAC_DIV:
			fprintf(fout, "## TAC_DIV\n"
						  "\tmovl\t	_%s(%%rip), %%eax\n"
						  "\tmovl\t	_%s(%%rip), %%ecx\n"
						  "\tcltd\n"
						  "\tidivl\t	%%ecx\n\n",
					tac->op1->text,
					tac->op2->text);
			break;
		case TAC_COPY:
			fprintf(fout, "## TAC_COPY\n"
						  "\tmovl	%%eax, _%s(%%rip)\n\n",
					tac->res->text);
			break;
		case TAC_VEC_CALL:
			fprintf(fout, "## TAC_VEC_CALL\n"
						  "\tmovl	_%s+%d(%%rip), %%edx\n"
						  "\tmovl	%%edx, _%s(%%rip)\n\n",
					tac->op1->text, 4 * atoi(tac->op2->text), tac->res->text);
			break;
		case TAC_VECATTR:
			fprintf(fout,
					"## TAC_VECATTR\n"
					"\txorl	%%eax, %%eax\n"
					"\tmovl	_%s(%%rip), %%esi\n"
					"\tmovl	 %%esi, _%s+%d(%%rip)\n\n",
					tac->op2->text, tac->res->text, 4 * atoi(tac->op1->text));
			break;
		case TAC_GREAT:
			fprintf(fout,
					"## TAC_GREAT\n"
					"\tmovl\t_%s(%%rip), %%eax\n"
					"\tcmpl\t_%s(%%rip), %%eax\n"
					"\tsetg\t   %%al\n"
					"\tmovzbl\t  %%al, %%eax\n"
					"\tmovl\t %%eax,_%s(%%rip)\n\n",
					tac->op1->text, tac->op2->text, tac->res->text);
		case TAC_LESS:
			fprintf(fout,
					"## TAC_LESS\n"
					"\tmovl\t_%s(%%rip), %%eax\n"
					"\tcmpl\t_%s(%%rip), %%eax\n"
					"\tsetl\t    %%al\n"
					"\tmovzbl\t  %%al, %%eax\n"
					"\tmovl\t %%eax,_%s(%%rip)\n\n",
					tac->op1->text, tac->op2->text, tac->res->text);
			break;
		case TAC_EQ:
			fprintf(fout,
					"## TAC_EQ\n"
					"\tmovl\t_%s(%%rip), %%eax\n"
					"\tcmpl\t_%s(%%rip), %%eax\n"
					"\tseteq\t   %%al\n"
					"\tmovzbl\t  %%al, %%eax\n"
					"\tmovl\t %%eax,_%s(%%rip)\n\n",
					tac->op1->text, tac->op2->text, tac->res->text);
			break;
		case TAC_LE:
			fprintf(fout,
					"## TAC_LE\n"
					"\tmovl\t_%s(%%rip), %%eax\n"
					"\tcmpl\t_%s(%%rip), %%eax\n"
					"\tsetle\t   %%al\n"
					"\tmovzbl\t  %%al, %%eax\n"
					"\tmovl\t %%eax,_%s(%%rip)\n\n",
					tac->op1->text, tac->op2->text, tac->res->text);
			break;
		case TAC_GE:
			fprintf(fout,
					"## TAC_GE\n"
					"\tmovl\t_%s(%%rip), %%eax\n"
					"\tcmpl\t_%s(%%rip), %%eax\n"
					"\tsetge\t  %%al\n"
					"\tmovzbl\t  %%al, %%eax\n"
					"\tmovl\t %%eax,_%s(%%rip)\n\n",
					tac->op1->text, tac->op2->text, tac->res->text);
			break;
		case TAC_DIF:
			fprintf(fout,
					"## TAC_LE\n"
					"\tmovl\t_%s(%%rip), %%eax\n"
					"\tcmpl\t_%s(%%rip), %%eax\n"
					"\tsetne\t   %%al\n"
					"\tmovzbl\t  %%al, %%eax\n"
					"\tmovl\t $0, _%s(%%rip)\n\n",
					tac->op1->text, tac->op2->text, tac->res->text);
			break;
		case TAC_IFZ:
			fprintf(fout, "\tmovl _%s(%%rip), %%eax\n"
						  "\tmovl $1, %%edx\n"
						  "\tandl %%eax, %%edx\n"
						  "\tjz .%s\n",
					tac->op1->text, tac->res->text);
			break;
		case TAC_LABEL:
			fprintf(fout, ".%s:\n", tac->res->text);
			break;

		case TAC_JUMP:
			fprintf(fout, "\tjmp .%s\n", tac->res->text);
			break;
		}
	}

	// HASH TABLE

	printASM(fout);
	addData(fout, node);
	addImmediates(fout);
	addTemporaries(fout);

	fclose(fout);
}