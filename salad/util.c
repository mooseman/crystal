/****************************************************/
/* Author: SaladJonk                                */
/* School: UESTC                                    */
/* Email:  saladjonk@gmail.com                      */
/* Note:   This file I copyed many things from      */
/*         Kneeth C.Loudn                           */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case OUTPUT:
    case ELSE:
    case INPUT:
    case WHILE:
    case VOID:
    case INT:
    case RETURN:
    case MAIN:
      fprintf(listing,
         "reserved word: %s\n",tokenString);
      break;
    case ASSIGN: fprintf(listing,"assignment sign:  =\n"); break;
    case LT: fprintf(listing,"comparation sign:  <\n"); break;
    case LTEQ: fprintf(listing,"comparation sign:  <=\n");break;
    case NEQ: fprintf(listing,"comparation sign:  !=\n");break;
    case EQ: fprintf(listing,"comparation sign:  ==\n"); break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case COMOS: fprintf(listing,",\n");break;
    case LQUATA: fprintf(listing,"[\n");break;
    case RQUATA: fprintf(listing,"]\n");break;
    case LCIR: fprintf(listing,"{\n");break;
    case RCIR: fprintf(listing,"}\n");break;
    case BG: fprintf(listing,"comparation sign:  >\n");break;
    case BGEQ: fprintf(listing,"comparation sign:  >=\n");break;
    case SEMI: fprintf(listing,";\n"); break;
    case PLUS: fprintf(listing,"operation sign:  +\n"); break;
    case MINUS: fprintf(listing,"operation sign:  -\n"); break;
    case TIMES: fprintf(listing,"operation sign:  *\n"); break;
    case OVER: fprintf(listing,"operation sign:  /\n"); break;
    case ENDFILE: fprintf(listing,"END_OF_THE_FILE\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=4
#define UNINDENT indentno-=4

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */

void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case WhileK:
          fprintf(listing,"While\n");
          break;
       // case AssignK:
        //  fprintf(listing,"Assign to: %s\n",tree->attr.name);
        //  break;
        case InputK:
          fprintf(listing,"Read: %s\n", "Just input the data");
          break;
        case OutputK:
          fprintf(listing,"Output\n");
          break;
        case ReturnK:
          fprintf(listing,"Return\n");
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)          
    { switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s     %s\n",tree->attr.name,"variable that has been defined");
          break;
        case IntarrayK:
          fprintf(listing,"Id: %s     %s\n",tree->attr.name,"Define a array");
          break;    
        case ArrayK:
          fprintf(listing,"Id: %s     %s\n",tree->attr.name,"array type");
          break;        
        case FuncK:
          fprintf(listing,"Id: %s     %s\n",tree->attr.name,"function type");
          break;
        case DefuncK:
          fprintf(listing,"Id: %s     %s\n",tree->attr.name,"Define a function");
          break;
        case IntK:
	  fprintf(listing,"Id: %s     %s\n",tree->attr.name,"data definition or parameter,type of int");
	  break;
	case VoidK:
 	  fprintf(listing,"Id: %s     %s\n","void","the function is of type void");
	  break;
	case EmptyK:
	  fprintf(listing,"%s\n","/** this block do not have local data or parameter**/");
	  break;       
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
