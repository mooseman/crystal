

#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"
#define initFO 2
#define ofpFO 0
#define retFO 1
//static int mainPointer=0;
static int Isarg=0;
static int frameoffsetflag=0;
static int frameoffset=0;
static int frame[10];
static int DefNum=0;
static int ArgNum=0;
static int tmpOffset = -1;
static int arrayflag=0;
static int arraysoleflag=2;
static int anotherpos=0;
static int savedMain;
static int fppointer=0;
static int fpflag=0;
static int counter[10];
//static int offset;
static void cGen (TreeNode * tree);
struct my_functableRec
{
char * p;
int type;
};
//struct my_functableRec my_functablesole[20];
struct my_functable
{
struct my_functableRec my_functablesole[20];
};

struct my_functable mytable[10];


void initmytable (TreeNode * tree)
{
//printf("In initmytable()\n");
mytable[0].my_functablesole[0].p="1";
mytable[0].my_functablesole[0].type=0;
mytable[0].my_functablesole[1].p="1";
mytable[0].my_functablesole[1].type=0;
while(tree!=NULL)
{
if((tree->nodekind==ExpK)&&(tree->kind.exp==IntK))
{
mytable[arrayflag].my_functablesole[arraysoleflag].p=tree->attr.name;
mytable[arrayflag].my_functablesole[arraysoleflag].type=0;
counter[arrayflag]++;
arraysoleflag++;
}
else if((tree->nodekind==ExpK)&&(tree->kind.exp==IntarrayK))
{
mytable[arrayflag].my_functablesole[arraysoleflag].p=tree->attr.name;
mytable[arrayflag].my_functablesole[arraysoleflag].type=1;
counter[arrayflag]++;
arraysoleflag++;	
}
else if((tree->nodekind==ExpK)&&(tree->kind.exp==DefuncK))
{
//fpflag++;
//if(fpflag==)
//{
//fppointer=arraysoleflag;
//fpflag=1;
//}
++arrayflag;
arraysoleflag=2;
mytable[arrayflag].my_functablesole[0].p=tree->attr.name;
mytable[arrayflag].my_functablesole[0].type=arrayflag;
mytable[arrayflag].my_functablesole[1].p="1";
mytable[arrayflag].my_functablesole[1].type=0;
initmytable(tree->child[0]);
initmytable(tree->child[1]);
}
tree=tree->sibling;
}
}

static int pos_lookup(char * name,int pos)
{
int i=1;
for(i=1;i<20;i++)
{
if(!strcmp(mytable[pos].my_functablesole[i].p,name))
return i-2;
}
return -1;
}



static void genStmt( TreeNode * tree,int another)
{ 
//printf("In genStmt()\n");
int pos=another;
TreeNode * p1, * p2, * p3;
  int savedLoc1,savedLoc2,currentLoc;
  int loc;
  switch (tree->kind.stmt) {

      case IfK :
        // printf("I am in the IfK()\n");
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         p3 = tree->child[2] ;
       
         cGen(p1);
         savedLoc1 = emitSkip(1) ;
     
         cGen(p2);
         savedLoc2 = emitSkip(1) ;
   
         currentLoc = emitSkip(0) ;
         emitBackup(savedLoc1) ;
         emitRM_Abs("JEQ",ac,currentLoc,"if: jmp to else");
         emitRestore() ;
     
         cGen(p3);
         currentLoc = emitSkip(0) ;
         emitBackup(savedLoc2) ;
         emitRM_Abs("LDA",pc,currentLoc,"jmp to end") ;
         emitRestore() ;
     
         break;

      case WhileK:
     
         p1 = tree->child[0] ;
         p2 = tree->child[1] ;
         savedLoc1 = emitSkip(0);
         
         cGen(p1);
        
         emitRM("JGT",ac,1,pc," ");
         savedLoc2=emitSkip(1);
         cGen(p2);
         //currentLoc=emitSkip(0);
    
// emitRM("LD",ac,10,gp," ");    
// emitRO("OUT",ac,0,0," ");
         emitRM("LDC",pc,savedLoc1,0," ");
        
         //emitRM_Abs("LDA",pc,currentLoc," ");
        // emitRestore();
         currentLoc=emitSkip(0);
         emitBackup(savedLoc2);
         emitRM_Abs("LDA",pc,currentLoc," ");
         emitRestore();
         

         break; 

    
      case InputK:
         emitRO("IN",ac,0,0,"read integer value");
        
         break;
      case OutputK:
       
         cGen(tree->child[0]);
     
         emitRO("OUT",ac,0,0,"write ac");
         break;
      case ReturnK:
         //printf("I am in the ReturnK()\n");
         cGen(tree->child[0]);
         emitRM("ST",ac,0,mp," ");
         //emitRM("ST",ac,tmpOffset--,mp," ");
      default:
         break;
    }
}


static void genExp( TreeNode * tree,int another)
{ 
TreeNode * temp;
//printf("In genExp() ---> %d\n",tree->kind.exp);
int set=0;
int i=0;
int currentLoc=0;
int savedFunc=0;
int pos=another;
//printf("This is the value of anotherpos %d\n",pos);
int loc;
  TreeNode * p1, * p2;
  switch (tree->kind.exp) {

    case ConstK :
   
      emitRM("LDC",ac,tree->attr.val,0," ");

      break; 
    	
    case IdK :
    // printf("I am in the IdK \n");
      if((pos==0)||(pos==arrayflag)) 
      {
      //printf("I am in the globale field\n");
      loc = st_lookup(tree->attr.name,0);
     // printf("globale loc -------------------> %d\n",loc);
      emitRM("LD",ac,loc,gp,"load id value");
      }
      else
      {
     // printf("I am in the local field\n");
      loc = 2+(pos_lookup(tree->attr.name,pos));
     // printf("local loc ---------------------> %d\n",loc);
      emitRM("LD",ac,loc,fp," load id value");
      }
   
      break; 

    case OpK :
         if(tree->attr.op==ASSIGN)
         { 
//printf("I am in the ASSIGN()      %d     %d\n",arrayflag,pos);
          p2=tree->child[1];
          cGen(p2);
            if(tree->child[0]->kind.exp!=ArrayK)
{

           if((pos==0)||(pos==arrayflag)) 
               {
      			//printf("EEEEEEEEEEEDDDDDDDDDDDDDDDDDDDDDDDDD\n");
     			 loc = st_lookup(tree->child[0]->attr.name,0);
     			// printf("globale loc -------------------> %d\n",loc);
     			 emitRM("ST",ac,loc,gp,"load id value");
    	  }
     		 else
      			{
     				// printf("DDDDDDDDDDDDDDDDDDDDDDDDDD\n");
     				 loc = 2+(pos_lookup(tree->child[0]->attr.name,pos));
     				 //printf("local loc ---------------------> %d\n",loc);
     				 emitRM("ST",ac,loc,fp," load id value");
     			 }
}
else
{
if((pos==0)||(pos==arrayflag))
{
  emitRM("ST",ac,tmpOffset--,mp,"op: push left");
  loc = st_lookup(tree->child[0]->attr.name,0);
  //cGen(tree->child[0]->child[0]);
     	           emitRM("LDC",ac1,loc,0," ");
cGen(tree->child[0]->child[0]);
emitRO("ADD",ac,ac1,ac," ");
 emitRM("LD",ac1,++tmpOffset,mp,"op: load left");

  //loc=loc+(tree->child[0]->child[0]->attr.val);   	           
emitRM("ST",ac1,0,ac," ");
}
else
{
emitRM("ST",ac,tmpOffset--,mp,"op: push left");
loc =2+pos_lookup(tree->child[0]->attr.name,pos);
                   emitRM("LD",ac1,loc,fp," ");
cGen(tree->child[0]->child[0]);
emitRO("ADD",ac1,ac1,ac," ");
emitRM("LD",ac,++tmpOffset,mp,"op: load left");
emitRM("ST",ac,0,ac1," ");
}

}
}
          
else
{
      
         p1 = tree->child[0];
         p2 = tree->child[1];
      
         cGen(p1);
     
         emitRM("ST",ac,tmpOffset--,mp,"op: push left");
      
         cGen(p2);
       
         emitRM("LD",ac1,++tmpOffset,mp,"op: load left");
         switch (tree->attr.op) {
            case PLUS :
               emitRO("ADD",ac,ac1,ac,"op +");
               break;
            case MINUS :
               emitRO("SUB",ac,ac1,ac,"op -");
               break;
            case TIMES :
               //printf("I am in the times()");
               emitRO("MUL",ac,ac1,ac,"op *");
               break;
            case OVER :
               emitRO("DIV",ac,ac1,ac,"op /");
               break;
            case LT :
               emitRO("SUB",ac,ac1,ac,"op <") ;
               emitRM("JLT",ac,2,pc,"br if true") ;
               emitRM("LDC",ac,0,ac,"false case") ;
               emitRM("LDA",pc,1,pc,"unconditional jmp") ;
               emitRM("LDC",ac,1,ac,"true case") ;
               break;
            case EQ :
               emitRO("SUB",ac,ac1,ac,"op ==") ;
               emitRM("JEQ",ac,2,pc,"br if true");
               emitRM("LDC",ac,0,ac,"false case") ;
               emitRM("LDA",pc,1,pc,"unconditional jmp") ;
               emitRM("LDC",ac,1,ac,"true case") ;
               break;
            default:
               emitComment("BUG: Unknown operator");
               break;
         }
  }
             break; 
          case ArrayK:
                
               if((pos==0)||(pos==arrayflag)) 
                   {
                   loc = st_lookup(tree->attr.name,0);
                   if(Isarg)
{
emitRM("LDC",ac,loc,0," ");
}
else
{
cGen(tree->child[0]);
     	           emitRM("LDC",ac1,loc,0," ");
                   emitRO("ADD",ac,ac1,ac," ");
                   emitRM("LD",ac,0,ac," ");
}
                   //emitRM("LD",ac,loc+(tree->child[0]->attr.val)-1,gp,"load id value");
     		   }
    		  else
    		  {
if(Isarg)                   
emitRM("LD",ac,loc,fp," ");
else
{
                   //loc=mytable[pos].my_functablesole[pos_lookup(tree->attr.name.pos)+2].
    		   loc =2+pos_lookup(tree->attr.name,pos);
                   emitRM("LD",ac1,loc,fp," ");
                   cGen(tree->child[0]);
                   emitRO("ADD",ac1,ac,ac1," ");
                   emitRM("LD",ac,0,ac1," ");
                   //emitRM("LDC",ac,tree->child[0]->attr.val,0," ");
                   //emitRO("ADD",ac,ac1,ac," ");
       }            //emitRM("LD",ac,0,ac," load id value");
   		  
 }       
                 break; 
case FuncK:
if((pos!=0)&&(pos!=arrayflag))
{
emitRM("LDC",ac3,frame[pos]+2,0," ");
}
else
{
emitRM("LDC",ac3,0,0," ");
}
emitRO("ADD",ac2,ac3,fp," ");
emitRM("ST",fp,0,ac2," ");
//emitRO("ADD",fp,fp,ac3," ");

    //printf("I am in the FuncK()\n");
          //if((pos==0)||(pos==arrayflag))
          //  emitRM("LDC",ac3,0,0," ");
        ///  else
         //   emitRM("LD",ac3,0,mp," ");
 if(tree->child[0]->kind.exp!=EmptyK)
{
//printf("HEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
//=judge(tree->child[0]->attr.name,pos);
if((pos==0)||(pos==arrayflag))
{
if(tree->child[0]->kind.exp==ConstK)
{
emitRM("LDC",ac,tree->child[0]->attr.val,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
if(tree->child[0]->kind.exp==ArrayK)
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LDC",ac,loc,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LD",ac,loc,gp," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
}
}

else
{
if(tree->child[0]->kind.exp==ConstK)
{
emitRM("LDC",ac,tree->child[0]->attr.val,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
if(tree->child[0]->kind.exp==ArrayK)
{
loc=2+pos_lookup(tree->child[0]->attr.name,pos);
if(loc==1)
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LDC",ac,loc,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
emitRM("LD",ac,loc,fp," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
}
else
{
loc=2+pos_lookup(tree->child[0]->attr.name,pos);
if(loc==1)
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LD",ac,loc,gp," ");
}
else
{
emitRM("LD",ac,loc,fp," ");
}
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
}
}
temp=tree;
tree=tree->child[0]->sibling;
while(tree!=NULL)
{
Isarg=1;
genExp(tree,pos);
emitRM("LDC",ac1,initFO+(set++),0," ");
//emitRO("ADD",ac2,ac1,ac3," ");
emitRO("ADD",ac1,ac1,ac2," ");
emitRM("ST",ac,0,ac1," ");
Isarg=0;
tree=tree->sibling;
}
/*
printf("In the while %d\n",set);
//global=judge(tree->attr.name,pos);
if((pos==0)||(pos==arrayflag))
{
if(tree->child[0]->kind.exp==ConstK)
{
emitRM("LDC",ac,tree->child[0]->attr.val,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
if(tree->child[0]->kind.exp==ArrayK)
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LDC",ac,loc,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LD",ac,loc,gp," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
}
}

else
{
if(tree->child[0]->kind.exp==ConstK)
{
emitRM("LDC",ac,tree->child[0]->attr.val,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
if(tree->child[0]->kind.exp==ArrayK)
{
loc=2+pos_lookup(tree->child[0]->attr.name,pos);
if(loc==1)
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LDC",ac,loc,0," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
else
{
emitRM("LD",ac,loc,fp," ");
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
}
else
{
loc=2+pos_lookup(tree->child[0]->attr.name,pos);
if(loc==1)
{
loc=st_lookup(tree->child[0]->attr.name,0);
emitRM("LD",ac,loc,gp," ");
}
else
{
emitRM("LD",ac,loc,fp," ");
}
emitRM("LDC",ac1,initFO+(set++),0," ");
emitRO("ADD",ac3,ac1,ac3," ");
emitRO("ADD",ac3,ac3,fp," ");
emitRM("ST",ac,0,ac3," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
//emitRM("ST",ac,frameoffset+initFO+(set++),fp," ");
}
}
}
*/
//tree=tree->sibling;

//printf("I am in satay \n");
for(i=1;i<arrayflag;i++)
{
if(!strcmp(mytable[i].my_functablesole[0].p,temp->attr.name))
{
savedFunc=mytable[i].my_functablesole[1].type;
}
}

///emitRM("LDC",ac1,ofpFO,0," ");
//emitRO("ADD",ac2,ac1,fp," ");
//emitRO("ADD",ac2,ac2,fp," ");
//emitRM("ST",fp,0,ac2," ");

//emitRO("ADD",ac3,ac3,fp," ");
//emitRM("LDA",fp,0,ac3," ");	
//if((pos!=0)&&(pos!=arrayflag))
//{
//emitRM("LDC",ac1,frame[pos],0," ");
//emitRM("LD",ac2,0,mp," ");
//emitRO("ADD",ac2,ac2,ac1," ");
//emitRM("ST",ac2,0,mp," ");
//}

//emitRM("LDC",ac1,ofpFO,0," ");
//emitRO("ADD",ac2,ac1,fp," ");
//emitRO("ADD",ac2,ac2,fp," ");
//emitRM("ST",fp,0,ac2," ");



//emitRM("ST",fp,frameoffset+2+ofpFO,fp," ");
//emitRO("ADD",ac3,ac3,fp," ");
//emitRM("LDA",fp,0,ac3," ");	
emitRO("ADD",fp,fp,ac3," ");
emitRM("LDA",ac,1,pc," ");
emitRM("LDC",pc,savedFunc,0," ");           
emitRM("LD",fp,ofpFO,fp," ");    
}

break;
             

              
case DefuncK:
//printf("In the DefuncK()\n");
if(!strcmp(tree->attr.name,"main"))
 {
         currentLoc = emitSkip(0) ;
         emitBackup(savedMain) ;
         emitRM_Abs("LDA",pc,currentLoc,"jump to the main function");
         emitRestore() ;
}
savedFunc=emitSkip(0);
//printf("here\n");	
 for(i=1;i<=arrayflag;i++)
  {
//printf("here %d\n",i);
if(!strcmp(mytable[i].my_functablesole[0].p,tree->attr.name))
{
anotherpos=mytable[i].my_functablesole[0].type;
//printf("-------------------> %d\n",anotherpos);
mytable[i].my_functablesole[1].type=savedFunc;
//printf("-------------------> %d\n",savedFunc);
frame[anotherpos]=counter[anotherpos];
//printf("-------------------> %d\n",frameoffset);
//break;
}
}
//printf("here\n");
temp=tree;
tree=tree->child[1];
while((tree!=NULL)&&(tree->nodekind==ExpK)&&((tree->kind.exp==IntK)||(tree->kind.exp==IntarrayK)))
tree=tree->sibling;
//printf("%d...\n",tree->attr.op);
if(strcmp(temp->attr.name,"main"))
{
emitRM("ST",ac,retFO,fp," ");
}
cGen(tree);
if(strcmp(temp->attr.name,"main"))
emitRM("LD",pc,retFO,fp," ");
break;

 case EmptyK:
 case VoidK:
 case IntK:
             
 case IntarrayK:
            
default:
              
break;
  }
}


static void cGen( TreeNode * tree)
{
//printf("In cGen()\n");
 if (tree != NULL)
  { switch (tree->nodekind) {
      case StmtK:
        genStmt(tree,anotherpos);
        break;
      case ExpK:
        genExp(tree,anotherpos);
        break;
      default:
        break;
    }
    cGen(tree->sibling);
  }
}


void codeGen(TreeNode * syntaxTree, char * codefile)
{ 
printf("\nGenerating the assembly code of TM visual machine ......\n");
initmytable(syntaxTree);
//printf("Int codeGen()\n");
char * s = malloc(strlen(codefile)+7);
int k=counter[arrayflag];   
//printf("---------------------------> %d",k);
//printf("here\n");
strcpy(s,"File: ");
   strcat(s,codefile);
   emitComment("C- Compilation to TM Code");
   emitComment(s);
//printf("here\n")
//printf("%s\n",mytable[3].my_functablesole[k+1].p)
   //printf("--------------------------> %s",mytable[arrayflag].my_functablesole[k+1].p);
k=st_lookup(mytable[arrayflag].my_functablesole[k+1].p,0);
//printf("--------------------------> %d",k);
//printf("here\n");
emitComment("Standard prelude:");

   emitRM("LDC",fp,k+1,0,"load the initial value of stack frame");
   emitRM("LD",mp,0,ac,"load maxaddress from location 0");
   emitRM("ST",ac,0,mp," ");
   emitRM("ST",ac,0,ac,"clear location 0");
   
   emitComment("End of standard prelude.");

  
  savedMain = emitSkip(1) ;
// printf("I am here\n");
 cGen(syntaxTree);
   
   emitComment("End of execution.");
   emitRO("HALT",0,0,0,"");
}
