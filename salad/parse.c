

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"
static TokenType token; /* holds current token */
static TreeNode * declaration_list(void);
static TreeNode * declaration(void);
static TreeNode * var_declaration(void);
static TreeNode * type_specifier(void);
static TreeNode * fun_declaration(void);
static TreeNode * params(void);
static TreeNode * param_list(void);
static TreeNode * param(void);
static TreeNode * compound_stmt(void);
static TreeNode * local_declarations(void);
static TreeNode * statement_list(void);
static TreeNode * statement(void);
static TreeNode * expression_stmt(void);
static TreeNode * selection_stmt(void);
static TreeNode * iteration_stmt(void);
static TreeNode * return_stmt(void);
static TreeNode * expression(void);
static TreeNode * var(void);
static TreeNode * simple_expression(void);
static TreeNode * relop(void);
static TreeNode * additive_expression(void);
static TreeNode * addop(void);
static TreeNode * term(void);
static TreeNode * mulop(void);
static TreeNode * factor(void);
static TreeNode * call(void);
static TreeNode * args(void);
static TreeNode * arg_list(void);
static int compareFunc(char *);
static void initfunc(void);
static int FuncFlag=1;

static void initfunc(void)
{
int i=0;
for(i=0;i<18;i++)
func[i]="5555";
func[18]="input";
func[19]="output";
}
static int compareFunc(char * a)
{
int j=1;
int i=0;
int flag=1;
for(i=0;i<20;i++)
{
//printf("before strcmp %d\n",i);
j=strcmp(a,func[i]);
//printf("strcmp %d\n",i);
if(j==0)
flag=0;
}
if(flag==0)
{
return FALSE;
}
else
{
return TRUE;
}
}


static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected)
{ if (token == expected) token = getToken();
  else {
    syntaxError("unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"      ");
  }
}





TreeNode * declaration_list(void)
{
//printf("In the declartaion_list\n");
TreeNode * t = declaration();
TreeNode * p = t;
while(token!=ENDFILE)
{
TreeNode * q=declaration();
if(q!=NULL)
p->sibling=q;
p=q;
}
//printf("pass the declaration_list()\n");
return t;
}

TreeNode * declaration(void)
{
//printf("In the declartaion\n");
TreeNode * t=type_specifier();
if((t!=NULL)&&((token == ID)||(token==MAIN)))
{
//printf("in declariton\n");
t->attr.name=copyString(tokenString);
match(token);
}
switch(token)
{
case SEMI:
match(SEMI);
break;
case LQUATA:
match(LQUATA);
TreeNode * p=newExpNode(IntarrayK);
p->type=t->type;;
p->attr.name=t->attr.name;
if(token==NUM) 
{
TreeNode * q=newExpNode(ConstK);
q->attr.val = atoi(tokenString);
p->child[0]=q;
match(NUM);
}
match(RQUATA);
match(SEMI);
t=p;
break;
case LPAREN:
match(LPAREN);
TreeNode * k=newExpNode(DefuncK);
k->attr.name=t->attr.name;
if(strcmp("main",k->attr.name))
func[FuncFlag++]=t->attr.name;
else
func[0]=t->attr.name;
k->type=t->type;
TreeNode * m=params();	
TreeNode * l=m;
while(m!=NULL)
{
if(m->kind.exp==ArrayK)
m->kind.exp=IntarrayK;
m=m->sibling;
}
k->child[0] = l;
match(RPAREN);
TreeNode * n=compound_stmt();
k->child[1] = n;
t=k;
break;
default:
break;
}
//printf("pass the declartion()\n");
return t;
}

TreeNode * type_specifier(void)
{
//printf("In the type_specifier\n");
TreeNode * t = NULL;
if(token == INT)
{
t = newExpNode(IntK);
t->type=Integer;
match(INT);
}
else
{
t=newExpNode(VoidK);
t->type=Void;
match(VOID);
}
//printf("pass the type_specifier()\n");
return t;
}

TreeNode * params(void)
{
TreeNode * t=NULL;
if(token==VOID)
{
t=newExpNode(VoidK);
t->type=Void;
match(VOID);
}
else
t=param_list();
//printf("pass the params()\n");
return t;
}

TreeNode * param_list(void)
{
TreeNode * t=param();
TreeNode * q=t;
while(token==COMOS)
{
match(COMOS);
TreeNode * p=param();
q->sibling=p;
q=p;
}
//printf("pass the param_list()\n");
return t;
}

TreeNode * param(void)
{
TreeNode * t=type_specifier();
if(token==ID)
{
t->attr.name=copyString(tokenString);
match(ID);
}
if(token==LQUATA)
{
t->kind.exp=ArrayK;
match(LQUATA);
match(RQUATA);
}
//printf("pass the param()\n");
return t;
}


TreeNode * compound_stmt(void)
{
//printf("In the compound stmt\n");
match(LCIR);
TreeNode * t=local_declarations();
TreeNode * q=statement_list();
TreeNode * p = t;
while(p->sibling!=NULL)
p=p->sibling;
p->sibling=q;
match(RCIR);
//printf("pass the compound_stmt()\n");
return t;
}

TreeNode * local_declarations(void)
{
//printf("In the local declaration\n");
TreeNode * t=newExpNode(EmptyK);
TreeNode * q=t;

if((token==INT)||(token==VOID))
{
t=var_declaration();
q=t;
}
//printf("outside while");
while((token==INT)||(token==VOID))
{
//printf("Inside while");
TreeNode * p=var_declaration();
q->sibling=p;
q=p;
}

//printf("pass the local_declaration()\n");
return t;
}

TreeNode * var_declaration(void)
{
//printf("In the var declartion\n");
TreeNode * t=type_specifier();
if(token==ID)
{
t->attr.name=copyString(tokenString);
match(ID);
}
if(token==LQUATA)
{
match(LQUATA);
t->kind.exp=IntarrayK;
TreeNode * n=newExpNode(ConstK);
n->attr.val=atoi(tokenString);
t->child[0]=n;
//t->attr.val=atoi(tokenString);
match(token);
match(RQUATA);
}
match(SEMI);
//printf("successfule\n");
//printf("pass the var_declartion()\n");
return t;
}

TreeNode * statement_list(void)
{
//printf("I am in the statement_list\n");
TreeNode * t = NULL;
if((token==IF)||(token==WHILE)||(token==RETURN)||(token==ID)||(token==LPAREN)||(token==NUM)||(token==OUTPUT))
{
t=statement();
}
TreeNode * p = t;
while((token==IF)||(token==WHILE)||(token==RETURN)||(token==ID)||(token==LPAREN)||(token==NUM)||(token==OUTPUT))
{
//printf("I am in the while of statement_list\n");
TreeNode * q = statement();
p->sibling=q;
p=q;
}
//printf("pass the statement_list()\n");
return t;
}

TreeNode * statement(void)
{
TreeNode * t=NULL;
switch(token)
{
case IF:
t=selection_stmt();
break;
case WHILE:
t=iteration_stmt();
break;
case RETURN:
t=return_stmt();
break;
case LPAREN:
case NUM:
case ID:
case OUTPUT:
t=expression_stmt();
break;
case LCIR:
t=compound_stmt();
break;
default:
break;
}
//printf("pass the statement()\n");
return t;
}

TreeNode * expression_stmt(void)
{
TreeNode * t=NULL;
if(token!=SEMI)
t=expression();
match(SEMI);
//printf("pass the expression_stmt()\n");
return t;
}


TreeNode * selection_stmt(void)	
{
TreeNode * t = newStmtNode(IfK);
  match(IF);
  match(LPAREN);
   t->child[0] = expression();
match(RPAREN);  
 t->child[1] = statement();
  if (token==ELSE) {
    match(ELSE);
   t->child[2] = statement();
  }
//match(SEMI);
//printf("pass the selection_stmt()\n");
return t;
}

TreeNode * iteration_stmt(void)
{
TreeNode * t = newStmtNode(WhileK);
 match(WHILE);
match(LPAREN);

  if (t!=NULL) t->child[0] =expression();
  match(RPAREN);
  if (t!=NULL) t->child[1] =statement();
//match(SEMI);
//printf("pass the iteration_stmt()\n");  
return t;
}


TreeNode * return_stmt(void)
{
TreeNode * t = newStmtNode(ReturnK);
match(RETURN);
if(token!=SEMI)
t->child[0]=expression();
match(SEMI);
//printf("pass the return_stmt()\n");
return t;
}

TreeNode * expression(void)
{
//printf("I am in expression()\n");
TreeNode * t = NULL;
if((token==ID)&&compareFunc(tokenString))
{
//printf("I am in the if\n");
t=var();
if(token==ASSIGN)
{
match(ASSIGN);
TreeNode * p=newExpNode(OpK);
p->attr.op=ASSIGN;
p->child[0]=t;
TreeNode * q=expression();
p->child[1]=q;
t=p;
}
else
{
TreeNode * p =NULL;
while((token==LTEQ)||(token==LT)||(token==BG)||(token==BGEQ)||(token==EQ)||(token==NEQ))
{
p=newExpNode(OpK);
p->attr.op=token;
p->child[0]=t;
match(token);
TreeNode * q=additive_expression();
p->child[1]=q;
t=p;
}

while((token==PLUS)||(token==MINUS))
{
p = newExpNode(OpK);
p->attr.op=token;
p->child[0]=t;
match(token);
TreeNode * q = term();
p->child[1]=q;
t=p;
}

while((token==TIMES)||(token==OVER))
{
TreeNode * p = newExpNode(OpK);
p->attr.op=token;
p->child[0]=t;
match(token);
TreeNode * q=factor();
p->child[1]=q;
t=p;
}
}
}
else
{
t=simple_expression();
}
//printf("pass the expression()\n");
return t;
}

TreeNode * var(void)
{
//printf("am in\n");
TreeNode * t = NULL;

//printf("am in\n");
t = newExpNode(IdK);
t->attr.name=copyString(tokenString);
match(token);
if(token==LQUATA)
{
match(LQUATA);
t->kind.exp=ArrayK;
if(token!=RQUATA)
{
TreeNode * q = expression();
t->child[0]=q;
}
else
{
t->child[0]=NULL;
}
match(RQUATA);
}
//printf("pass the var()\n");
return t;
}


TreeNode * simple_expression(void)
{
//printf("I am in simple_expression\n");
TreeNode * t = additive_expression();
TreeNode * p = NULL;
while((token==LTEQ)||(token==LT)||(token==BG)||(token==BGEQ)||(token==EQ)||(token==NEQ))
{
p=newExpNode(OpK);
p->attr.op=token;
p->child[0]=t;
match(token);
TreeNode * q=additive_expression();
p->child[1]=q;
t=p;
}
//printf("pass the simple_expression()\n");
return t;
}

TreeNode * additive_expression(void)
{
TreeNode * t = term();
TreeNode * p = NULL;
while((token==PLUS)||(token==MINUS))
{
p = newExpNode(OpK);
p->attr.op=token;
p->child[0]=t;
match(token);
TreeNode * q = term();
p->child[1]=q;
t=p;
}
//printf("pass the additive_expression()\n");
return t;
} 

TreeNode * term(void)
{
TreeNode * t=factor();
while((token==TIMES)||(token==OVER))
{
TreeNode * p = newExpNode(OpK);
p->attr.op=token;
p->child[0]=t;
match(token);
TreeNode * q=factor();
p->child[1]=q;
t=p;
}
//printf("pass the term()\n");
return t;
}

TreeNode * factor(void)
{
//printf("I am in factor\n");
TreeNode * t=NULL;
if(token==LPAREN)
{
match(LPAREN);
t=expression();
match(RPAREN);
}
else if((token==ID)||(token==INPUT)||(token==OUTPUT))
{
if(!compareFunc(tokenString))
t=call();
else
t=var();
}
else
{
t=newExpNode(ConstK);
t->attr.val=atoi(tokenString);
match(NUM);
}
//printf("pass the factor()\n");
return t;
}

TreeNode * call(void)
{
TreeNode * t= NULL;
if(token==INPUT)
{
t=newStmtNode(InputK);
t->attr.name=copyString(tokenString);
}
else if(token==OUTPUT)
{
t=newStmtNode(OutputK);
t->attr.name=copyString(tokenString);
}
else
{
//printf("In call\n");
t=newExpNode(FuncK);
t->attr.name=copyString(tokenString);
}
match(token);
match(LPAREN);
TreeNode * p=args();
t->child[0]=p;
//printf("pass the call()\n");
return t;
}

TreeNode * args(void)
{
TreeNode * t=newExpNode(EmptyK);
if(token!=RPAREN)
t=arg_list();
match(RPAREN);
//match(SEMI);
//printf("pass the args()\n");
return t;
}

TreeNode * arg_list(void)
{
TreeNode * t=expression();
TreeNode * p=t;
while(token==COMOS)
{
match(COMOS);
TreeNode * q=expression();
p->sibling=q;
p=q;
}
//printf("pass the arg_list()\n");
return t;
}



/****************************************/
TreeNode * parse(void)
{ 
initfunc();
TreeNode * t;
  token = getToken();
  t = declaration_list();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}
