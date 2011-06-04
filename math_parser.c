

/*  math_parser.c                     */  
/*  This parser is from pastebin.com  */ 

  

    #include <stdio.h>
    #include <stdlib.h>
    #include <ctype.h>
     
    char *p;
     
    int factor(void);
    int expression();
    int term(void);
     
     
    int isMulOp(char c)
    {
        return ((c == '*')|| (c == '/') || (c == '^'));
    }
     
     
    int isAddOp(char c)
    {
        return ((c == '+') || (c == '-'));
    }
     
    int getNumber()
    {
     
        int result = 0, value = 1;
       
        if( isAddOp(*p))
        {
            if(*p == '-')
            {
                value = -1;
            }
            p++;
        }
       
        if (isdigit (*p))
        {  
            while(isdigit(*p))
            {
                result = result * 10 + (*p - '0');
                p++;
            }
        }
        else
        {  
            printf("Error: expected digit\n");
            exit(EXIT_FAILURE);
        }
        result = result * value;
        return result;
    }
     
    int match(char c)
    {  
        if(*p == c)
        {
            p++;
        }
        else
        {  
            printf("Error: expected %c\n", *p);
            exit(EXIT_FAILURE);
        }
        return 0;
     }
     
    int parse (char *string)
    {
     
        int result;
       
        p = string;
     
        result = expression();
       
        if (*p != '\0')
        {
            printf("Unecpected token %c\n", *p);
            exit(EXIT_FAILURE);
        }
        return result;
    }
     
    int expression()
    {
        int result;
     
        result = term();
     
        while (isAddOp(*p))
        {  
            switch(*p)
            {
            case '-':
                match('-');
     
                result = result - term();
                break;
     
            case '+':
                match('+');
     
                result = result + term();
                break;
            }
        }
        return result;
    }
     
    int factor(void)
    {
        int result;
     
        if (*p == '(')
        {
            match('(');
     
            result = expression();
     
            match(')');
        }
        else
        {
            result = getNumber();
        }
        return result;
    }
     
    int term(void)
    {
        int result;
        result = factor();
     
        while (isMulOp(*p))
        {  
            switch(*p)
            {
            case '*':
                match('*');
     
                result = result*factor();
                break;
     
            case '/':
                match('/');
     
                result = result/factor();
                break;
            }
        }
        return result;
    }
     
     
    int main(int argc, char* argv[])
    {
     
        int result;
       
        if(argc == 2)
        {  
            result = parse(argv[1]);
            printf("Result is: %d\n", result);
        }
        else
        {  
            printf("Usage: %s <expression>",argv[0]);
            exit(EXIT_FAILURE);
     
        }
        return (EXIT_SUCCESS);
    }


