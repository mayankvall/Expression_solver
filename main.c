

//note - interpolation is not that accurate
/*
 ps doubts:-1.why using exit is not the correct way for a developer to build
 2.using strcat check wheather the size of the initial array is sufficient or not
 
 */

/*
 known methods-
 string to float
 string to integer
 integer to string
 */
//ps notes:-remember not to change the order of the program in calculate
//remember popop is a not returnable function

//current limitation 0.123123

/*
 note:-strcpy cannot be used to remove data from string
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "function.h"
#include "parenthesis.h"
#include "interpolation.h"
#include "integration.h"
#include "search.h"
#define max 50
#define pi 3.141592654
#define rad90 1.570796327



typedef struct stack1
{
    char *exp;//stroring expression
    char function[max][20];//storing sub-functions
    char operations[max][max][max];//storing operations ps-->modification required
    char loading[max][max][max];//loading in postfix way
    float sub_results[max];//stores the sub result by sub-result
    int topf;//for function variable
    int topop[max];//for operation variable
    int toplo;//for loading variable
    int topsub;//for sub_results
    int topstore;//for store_increment2 variable working position
    int topbraces;//for no_of_opening_braces variable for current working position
    int store_increment2[max];//for storing the value at which it comes to next branch
    int no_of_opening_braces[max];//for storing number of opening braces at each branch
    
    
    //for only evaluation of resultant postfix expression
    float stack_op[max];
    int top_stack;
    
    
}solve;
//initialisation to prevent junk values and initiate everything from zero or from begining
void initial(solve *p)
{
    int i,j;
    for(i=0;i<max;i++)
        strcpy(p->function[i],"");
    
    for(i=0;i<max;i++)
    {
        for(j=0;j<max;j++)
        {
            strcpy(p->operations[i][j],"");
        }
    }
    
    for(i=0;i<max;i++)
    {
        for(j=0;j<max;j++)
        {
            strcpy(p->loading[i][j],"");
        }
    }
    
    for(i=0;i<max;i++)
        p->store_increment2[i]=0;
    
    for(i=0;i<max;i++)
        p->no_of_opening_braces[i]=0;
    
    p->exp="";
    p->topf=-1;
    
    for(i=0;i<max;i++)
        p->topop[i]=-1;
    
    p->toplo=-1;
    p->topsub=0;// special case as in zeroth value will be the result of f(x) should not be touched till end
    //confusion lets make it as a test case during dubugging
    p->topbraces=-1;
    p->topstore=-1;
    p->top_stack=-1;
}
//setting expression
void setexpr(solve *p,char *s)
{
    p->exp=s;
}
//push the given function but check before whether the function exists or not
void pushf(solve *p,char *s)
{
    
    p->topf++;
    strcpy(p->function[p->topf],s);
}
//pop up the function when no more braces remains to pop up and calculate the value stored in sub_results and send to evaluation
void popf(solve *p,char *s)
{
    if(p->topf==-1)
    {
        s=NULL;
    }
    else
    {
        strcpy(s,p->function[p->topf]);
        strcpy(p->function[p->topf],"");
        p->topf--;
    }
}

//push the operators like +,-,*,/ into the operation stack,remember operation performs stack operation in an array of stacks
void pushop(solve *p,char *op,int pos)
{
    p->topop[pos]++;
    strcpy(p->operations[pos][p->topop[pos]],op);
    
}
//pops out the operator from the particular stack present in array of stack
void popop(solve *p,int pos,char *s)
{
    
    if(p->topop[pos]==-1)
        strcpy(s,"NOT");   //ps note:why like that
    else
    {
        strcpy(s,p->operations[pos][p->topop[pos]]);
        strcpy(p->operations[pos][p->topop[pos]],"");
        p->topop[pos]--;
        
    }
    
}
//pushing in variable loading doesnt means to push the characters or string but too increment just count
void pushlo(solve *p)
{
    p->toplo++;
}
//poping decreases the count by 1
void poplo(solve *p)
{
    p->toplo--;
}

//stores the sub_results
void pushsub(solve *p,float res)
{
    p->topsub++;
    p->sub_results[p->topsub]=res;
    
}
//returns the floating value to the back function
float popsub(solve *p)
{
    float re=p->sub_results[p->topsub];
    p->topsub--;
    return re;
}

//setting up stack operation for number of brackets
void pushbraces(solve *p,int res)
{
    p->topbraces++;
    p->no_of_opening_braces[p->topbraces]=res;
    
}
//returning the previous number of opening braces
int popbraces(solve *p)
{
    int z;
    if(p->topbraces==-1)
        return 0;
    else
    {
        z=p->no_of_opening_braces[p->topbraces];
        p->topbraces--;
        return z;
    }
}
//setting up the increment2 variable storing array at each number of times it breaks into branches
void pushincre2(solve *p,int n)
{
    p->topstore++;
    p->store_increment2[p->topstore]=n;
}
//returning the previous number of strings entered
int popincre2(solve *p)
{
    int z;
    if(p->topstore==-1)
        return 0;
    else
    {
        z=p->store_increment2[p->topstore];
        p->topstore--;
        return z;
    }
}
void push_stack(solve *p,float item)
{
    p->top_stack++;
    p->stack_op[p->top_stack]=item;
}
float pop_stack(solve *p)
{
    float data;
    if(p->top_stack==-1)
        return 0;
    else
    {
        data=p->stack_op[p->top_stack];
        p->top_stack--;
        return data;
    }
}
int priority(char *s)
{
    if(strcmp(s,"+")==0||strcmp(s,"-")==0)
    {
        return 1;
    }
    else if(strcmp(s,"*")==0||strcmp(s,"/")==0||strcmp(s,"%")==0)
    {
        return 2;
    }
    else if(strcmp(s,"^")==0)
    {
        return 3;
    }
    else
    {
        return 0;
    }
    
}
float evaluate(solve *p,float result,int no)
{
    float sub;
    char fun[30];//stores the function
    strcpy(fun,p->function[no]);
    float t=result;
    if(strcmp(fun,"sin")==0)
    {
        sub=sin(t);
    }
    else if(strcmp(fun,"cos")==0)
    {
        sub=cos(result);
    }
    else if(strcmp(fun,"tan")==0)
    {
        
        sub=tan(result);
    }
    else if(strcmp(fun,"cot")==0)
    {
        if(fabs(tan(result))>0.000000)
            sub=1/tan(result);
        else
        {
            printf("The value of cot is randomly assigned\n");
            sub=100000000.000000;
        }
    }
    else if(strcmp(fun,"cosec")==0)
    {
        if(fabs(sin(result))>0.000000)
            sub=1/sin(result);
        else
        {
            printf("The value of cosec is randomly assigned\n");
            sub=100000000.000000;
        }
    }
    else if(strcmp(fun,"sec")==0)
    {
        if(fabs(cos(result))>0.000000)
            sub=1/cos(result);
        else
        {
            printf("The value of cot is randomly assigned\n");
            sub=100000000.000000;
        }
    }
    
    else if(strcmp(fun,"asin")==0)
    {
        if(result<=1&&result>=-1)
            sub=asin(result);
        else
        {
            printf("Wrong use of sin inverse it's domain is -1 to 1\n");
            exit(0);
        }
        
    }
    else if(strcmp(fun,"acos")==0)
    {
        if(result<=1&&result>=-1)
            sub=acos(result);
        else
        {
            printf("Wrong use of cos inverse it's domain is -1 to 1\n");
            exit(0);
        }
        
    }
    else if(strcmp(fun,"atan")==0)
    {
        sub=atan(result);
    }
    else if(strcmp(fun,"acot")==0)
    {
        sub=(pi/2)-atan(result);
    }
    else if(strcmp(fun,"acosec")==0)
    {
        if(result>=1||result<=-1)
            sub=asin(1/result);
        else
        {
            printf("Wrong use of cosec inverse it's domain is R-[1,-1]\n");
            exit(0);
        }
    }
    else if(strcmp(fun,"asec")==0)
    {
        if(result>=1||result<=-1)
            sub=acos(1/result);
        else
        {
            printf("Wrong use of sec inverse it's domain is R-[1,-1]\n");
            exit(0);
        }
    }
    else if(strcmp(fun,"sinh")==0)
    {
        sub=sinh(result);
    }
    else if(strcmp(fun,"cosh")==0)
    {
        sub=cosh(result);
    }
else if(strcmp(fun,"tanh")==0)
{
    sub=tanh(result);
}
else if(strcmp(fun,"cosech")==0)
{
    if(result!=0)
        sub=1/sinh(result);
        else
        {
            printf("Wrong use of cosech it's domain is R-[0]\n");
            exit(0);
        }
}
else if(strcmp(fun,"sech")==0)
{
    sub=1/cosh(result);
}
else if(strcmp(fun,"coth")==0)
{
    if(result!=0)
        sub=1/tanh(result);
        else
        {
            printf("Wrong use of coth it's domain is R-[0]\n");
            exit(0);
        }
}
    else if(strcmp(fun,"asinh")==0)
    {
        sub=asinh(result);
    }
    else if(strcmp(fun,"acosh")==0)
    {
        if(result>=1)
            sub=acosh(result);
        else
        {
            printf("Wrong use of cosh inverse it's domain is [1,inf) \n");
            exit(0);
        }
    }
    else if(strcmp(fun,"atanh")==0)
    {
        if(result<1&&result>-1)
            sub=atanh(result);
        else
        {
            printf("Wrong use of tanh inverse it's domain is (-1,1) \n");
            exit(0);
        }
    }
    else if(strcmp(fun,"acosech")==0)
    {
        sub=asinh(1/result);
    }
    else if(strcmp(fun,"asech")==0)
    {
        if(result>0&&result<=1)
            sub=acosh(1/result);
        else
        {
            printf("Wrong use of cosh inverse it's domain is (0,1] \n");
            exit(0);
        }
    }
    else if(strcmp(fun,"acoth")==0)
    {
        if(result<-1||result>1)
            sub=atanh(1/result);
        else
        {
            printf("Wrong use of coth it's domain is R-[0]\n");
            exit(0);
        }
    }
    else if(strcmp(fun,"log")==0)
    {
        if(result>0)
            sub=log(result);
        else
        {
            printf("Wrong use of log it's domain is (0,inf]\n");
            exit(0);
        }
    }
    else if(strcmp(fun,"e")==0)
    {
        sub=exp(result);
    }
    
    return sub;
    
    
}
void calculate(solve *p,float x)
{
    char ch[max];//temporary stores the characters and pass them as a single string to loading variable
    
    char fun[30];//temporary stores the sub-function if encounters
    int i=0;
    int increment3=0;//keeps in track the 3D value of the variable loading to only change its value when the a sub-function is encountors
    int increment2=0;//keeps in track the 2D value of the variable loading to only change its value when operands are encountored
    int len;//to store the length of strings if required
    int n=1;//conversion of integer to string for temporary replacement of the sub-function
    char opr[5];//stores the operator
    while(*(p->exp))
    {
        
        strcpy(ch,"");
        i=0;
        //seperation of blank spaces
        if(*(p->exp)==' '||*(p->exp)=='\t')
        {
            p->exp++;
            continue;
        }
        //seperation of integer including floating values
        if(isdigit(*(p->exp)))
        {
            while(isdigit(*(p->exp)))
            {
                ch[i]=*(p->exp);
                i++;
                p->exp++;
            }
            ch[i]='\0';// extremely important step as without declaring it program fails
            strcpy(p->loading[increment3][increment2],ch);
            //to eliminate when number spaced and . is added
            while(*(p->exp)==' '||*(p->exp)=='\t')
            {
                p->exp++;
            }
            if(*(p->exp)=='.')
            {
                len=(int)strlen(ch);
                p->loading[increment3][increment2][len]='.';
                p->loading[increment3][increment2][len+1]='\0';
                p->exp++;
                if(*(p->exp)=='.')
                {
                    printf("Invalid use of '.' ");
                    exit(0);
                }
            }
            //again initialisation of the same variable is must to get expected result
            i=0;
            len=(int)strlen(p->loading[increment3][increment2]);
            while(isdigit(*(p->exp)))
            {
                p->loading[increment3][increment2][len+i]=*(p->exp);
                i++;
                p->exp++;
                
            }
            p->loading[increment3][increment2][len+i]='\0';
            
            increment2++;
            strcpy(ch,"");
            i=0;
            
            
        }
        //to store the unknown variable in mathematical equation for which we are writing the program
        
        //ps note:->->->the error was displaying when else if was used in the next upcoming statement, know why it was
        
        //no need to display the error message now if any other variable encountors other than x or X, it will be done in sub-function designing
        if(*(p->exp)=='x'||*(p->exp)=='X')
        {
            ch[i]=*(p->exp);
            i++;
            ch[i]='\0';
            strcpy(p->loading[increment3][increment2],ch);
            p->exp++;
            if(*(p->exp)=='x'||*(p->exp)=='X')
            {
                printf("Invalid use of the unkown variable\n");
                printf("Absence of operator\n");
                exit(0);
            }
            
            increment2++;
            strcpy(ch,"");
            i=0;
            
        }
        
        //subfunction seperation
        int t;//for checking wheather there is correct continuation to the function
        if((isalpha(*(p->exp)))&&(*(p->exp)!='x')&&(*(p->exp)!='X'))
        {
            while((isalpha(*(p->exp)))&&(*(p->exp)!='x')&&(*(p->exp)!='X'))
            {
                ch[i]=*(p->exp);
                i++;
                p->exp++;
            }
            ch[i]='\0';
            strcpy(fun,ch);
            int wheather=check(fun);//to store wheather a given function exists or not and ps note: during the inclusion of header files the program makes it dynamic implementation of functions present in header
            
            //correct function
            if(wheather==1)
            {
                pushf(p,fun);//remember i have pushed the function:not forgot
            }
            
            if(*(p->exp)=='(')
            {
                t=1;
            }
            else
            {
                t=0;
            }
            
            
            if(t==0)
            {
                printf("missing opening bracket after the function '%s'",fun);
                exit(0);
            }
            strcpy(fun,"");//making the variable fun NULL to reuse again
            
            //as sub-function encountored we have to bring down to another 3D character array
            
            strcpy(p->loading[increment3][increment2],"f");
            char digit[3];//storing capability upto 999 floating numbers of branching function value
            sprintf(digit,"%d",n);
            strcat(p->loading[increment3][increment2],digit);
            increment2++;//its must error found
            p->store_increment2[increment3]=increment2;
            increment3++;
            
            increment2=0;//initialisising for the stack to come down(loading variable)
            n++;//increases the function number
            strcpy(ch,"");
            i=0;
            
            
        }
        if(*(p->exp)=='(')
        {
            ch[i]=*(p->exp);
            p->exp++;
            i++;
            ch[i]='\0';
            pushop(p,ch,increment3);
            p->no_of_opening_braces[increment3]++;
            strcpy(ch,"");
            i=0;
        }
        
        if(*(p->exp)=='+'||*(p->exp)=='-'||*(p->exp)=='*'||*(p->exp)=='/'||*(p->exp)=='^'||*(p->exp)=='%')
        {
            //breaking NOT
            int remove=0;
            
            ch[i]=*(p->exp);
            i++;
            ch[i]='\0';
            if(p->topop[increment3]!=-1)
            {
                popop(p,increment3,opr);
                
                while(priority(ch)<=priority(opr))
                {
                    
                    strcpy(p->loading[increment3][increment2],opr);
                    increment2++;
                    popop(p,increment3,opr);
                    //to resolve problem with null poping
                    if(strcmp(opr,"NOT")==0)
                    {
                        remove=1;
                    }
                    else
                    {
                        remove=0;
                    }
                }
                if(remove==0)
                {
                    pushop(p, opr,increment3);
                    pushop(p,ch,increment3);
                }
                if(remove==1)
                {
                    pushop(p,ch,increment3);
                }
            }
            else
                pushop(p,ch,increment3);
            
            p->exp++;
            if(*(p->exp)=='+'||*(p->exp)=='-'||*(p->exp)=='*'||*(p->exp)=='/'||*(p->exp)=='^'||*(p->exp)=='%')
            {
                printf("Two operators together without any operands");
                exit(0);
            }
            strcpy(ch,"");
            i=0;
        }
        
        
        if(*(p->exp)==')')
        {
            ch[i]='(';
            i++;
            ch[i]='\0';
            popop(p,increment3,opr);
            while(strcmp(opr,ch)!=0)
            {
                strcpy(p->loading[increment3][increment2],opr);
                increment2++;
                popop(p,increment3,opr);
            }
            p->no_of_opening_braces[increment3]--;
            p->exp++;
        }
        
        if((p->no_of_opening_braces[increment3]==0)&&(strcmp(p->loading[increment3+1][0],"")==0)&&strcmp((p->operations[increment3][0]),"")==0)
        {
            
            //evaluation of postfix expression
            {
                int i=0;
                float n1,n2,n3=0;
                int remember=0;//to remember the number of last number of functions to remove
                for(i=0;i<increment2;i++)
                {
                    if(strcmp(p->loading[increment3][i],"x")==0||strcmp(p->loading[increment3][i],"X")==0)
                    {
                        push_stack(p,x);
                    }
                    if(isdigit(p->loading[increment3][i][0]))
                    {
                        push_stack(p,atof(p->loading[increment3][i]));
                    }
                    if(p->loading[increment3][i][0]=='f')
                    {
                        int k=1;
                        char function_number[3];
                        remember++;
                        while(p->loading[increment3][i][k]!='\0')
                        {
                            function_number[k-1]=p->loading[increment3][i][k];
                            k++;
                        }
                        
                        int fun_number=atoi(function_number);//to convert into integer
                        float temp=evaluate(p,p->sub_results[fun_number],fun_number-1);//to get the value of function number
                        push_stack(p,temp);
                        
                        //manual removal of the function from stack function
                        /*
                         int count;
                         int con;
                         int t=0;
                         for(count=0;count<=p->topf;count++)
                         {
                         if(count==fun_number-1)
                         {
                         t=1;
                         for(con=count;con<=p->topf-1;con++)
                         {
                         strcpy(p->function[con],p->function[con+1]);
                         }
                         strcpy(p->function[p->topf],"");
                         p->topf--;
                         remove_function++;
                         }
                         if(t==1)
                         break;
                         }
                         */
                        
                        /*
                         //manual removal of the value from sub-result
                         
                         int counts;
                         int cons;
                         int f=0;
                         for(counts=0;counts<p->topsub;counts++)
                         {
                         if(counts==fun_number)
                         {
                         f=1;
                         for(cons=counts;cons<p->topsub-1;cons++)
                         {
                         p->sub_results[cons]=p->sub_results[cons+1];
                         }
                         p->topsub--;
                         }
                         if(f==1)
                         break;
                         }
                         */
                        
                    }
                    if(strcmp(p->loading[increment3][i],"+")==0||strcmp(p->loading[increment3][i],"-")==0||strcmp(p->loading[increment3][i],"*")==0||strcmp(p->loading[increment3][i],"/")==0||strcmp(p->loading[increment3][i],"%")==0||strcmp(p->loading[increment3][i],"^")==0)
                    {
                        //if operator encountered
                        n1=pop_stack(p);
                        n2=pop_stack(p);
                        
                        switch(p->loading[increment3][i][0])
                        {
                            case '+':
                                n3=n2+n1;
                                break;
                                
                            case '-':
                                n3=n2-n1;
                                break;
                                
                            case '*':
                                n3=n2*n1;
                                break;
                                
                            case '/':
                                n3=n2/n1;
                                break;
                                
                            case '^':
                                n3=pow(n2,n1);
                                break;
                            case '%':
                                n3=fmodf(n2,n1);
                                break;
                                
                        }
                        push_stack(p,n3);
                    }
                }
                char dump[50];
                while(remember>0)
                {
                    popsub(p);
                    n--;
                    popf(p,dump);
                    remember--;
                }
            }
            
            float value=pop_stack(p);
            p->sub_results[n-1]=value;
            
            //manual removal of elements
            for(i=0;i<increment2;i++)
            {
                strcpy(p->loading[increment3][i],"");
            }
            
            
            increment3--;
            increment2=p->store_increment2[increment3];
            
        }
        
        
    }
    
}

int main()
{
    //input of the function
    
    int choice;
    printf("Numerical Analysis\n");
    printf("Press 1 for solving equation\n");
    printf("Press 2 for Interpolating concepts\n");
    printf("Press 3 for integration\n");
    //printf("Press 4 for search\n");
    scanf("%d",&choice);
    
    char subject[50];
    char times[50];
    
    time_t clock = time(NULL);
    
    strcpy(times,ctime(&clock));
    
    /*
    FILE *fp;
    fp=fopen("/Users/Numerical Analysis.txt","a+");
    if(fp==NULL)
    {
        printf("File doesn't exit\n");
        printf("The carried details couldn't be inputed to file\n");
    }
     */
    if(choice==1)
    {
        printf("Enter the name of subject\n");
        fpurge(stdin);
        fgets(subject,50,stdin);
        solve p;
        char eq[2*max];
        printf("Enter the equation\n");
        fpurge(stdin);
        fgets(eq,100,stdin);
        check_para(eq);
        
        //removing /n as because it takes it as an extra character
        int d=(int)strlen(eq);
        eq[d-1]='\0';
        
        //initiating copied part of project file
        initial(&p);
        setexpr(&p,eq);
        
        //to file
        /*
        fputs("Name of subject : ",fp);
        fputs(subject,fp);
        fputs("\n",fp);
        fputs("Time : ",fp);
        fputs(times,fp);
        fputs("\n",fp);
        fputs("Equation : ",fp);
        fputs(eq,fp);
        fputs("\n",fp);
        fputs("Details of the operation performed\n",fp);
         */
        
        float upper,lower;//to store the upper limit and lower limit
        printf("Enter the upper bound\n");
        scanf("%f",&upper);
        printf("Enter the lower bound\n");
        scanf("%f",&lower);
        
        float stoping_point=0.000001;//used to stop the itteration to give the value upto the nearest correct answer
        
        int i=0;//stores number of itteration
        float f1,f2;//stores the value of the function at lower and upper bound
        
        calculate(&p,lower);
        f1=p.sub_results[0];//gets the value of f(x) at lower end
        initial(&p);
        setexpr(&p,eq);
        calculate(&p,upper);
        f2=p.sub_results[0];//gets the value of f(x) at upper end
        
        
        float result;//stores the result
        
        /*
         condition for bisection method to operate is that the value of f1*f2<0*/
        while(f1*f2>=0)
        {
            printf("RE-enter the upper bound and lower bound\n");
            printf("Upper:");
            scanf("%f",&upper);
            printf("Lower:");
            scanf("%f",&lower);
            initial(&p);
            setexpr(&p,eq);
            calculate(&p,lower);
            f1=p.sub_results[0];//gets the value of f(x) at lower end
            initial(&p);
            setexpr(&p,eq);
            calculate(&p,upper);
            f2=p.sub_results[0];//gets the value of f(x) at upper end
        }
        
        
        float results;//to store the value of f(result)
        char check;
        printf("Do you want to display the details of each itteration\n");
        printf("Press y to display or any other key to display just result\n");
        fpurge(stdin);
        scanf("%c",&check);
        
        int t=0;//checking wheather itteration crossed the level
        while(1)
        {
            result=(lower+upper)/2;
            initial(&p);
            setexpr(&p,eq);
            calculate(&p,result);
            results=p.sub_results[0];//gets the value of f(x) at lower end
            
            if((f1*results)<0)
            {
                
                upper=result;
            }
            if((f2*results)<0)
            {
                lower=result;
            }
            
            if(check=='y'||check=='Y')
            {
                printf("iteration %d:",i+1);
                printf("  %f\n",result);
                /*
                char concat[6];
                sprintf(concat,"%d",i+1);
                fputs("iteration ",fp);
                fputs(concat,fp);
                fputs(":  ",fp);
                char resultStr[100];
                sprintf(resultStr,"%f",result);
                fputs(resultStr,fp);
                fputs("\n",fp);
                 */
                
            }
            if(fabs(results)<=stoping_point)
            {
                break;
            }
            i++;
            
            if(i==20000)//if itteration crosses 20000 breaks
            {
                printf("overflow\n");
                t=1;
                break;
            }
        }
        if(t==0)
        {
            printf("Value of x: %f",result);
            /*
            fputs("The Value of x : ",fp);
            char resStr[100];
            sprintf(resStr,"%f",result);
            fputs(resStr,fp);
            fputs("\n\n",fp);
            fclose(fp);
             */
        }
    }
    else if(choice==2)
    {
        //limit of the interpolation maximum 100 points not more than that
        receive(times);
        cals();
    }
    else if(choice==3)
    {
        char subject[50];
        printf("Enter the name of the subject\n");
        fpurge(stdin);
        fgets(subject,49,stdin);
        
        char integEQ[2*max];
        printf("Enter the equation\n");
        fpurge(stdin);
        fgets(integEQ,100,stdin);
        check_para(integEQ);
        
        //removing /n as because it takes it as an extra character
        int dumy=(int)strlen(integEQ);
        integEQ[dumy-1]='\0';
        
        float uppbo,lowbo;
        
        fpurge(stdin);
        printf("Enter the lower bound for integration\n");
        scanf("%f",&lowbo);
        printf("Enter the upper bound for integration\n");
        scanf("%f",&uppbo);
        
        int no_points=4004;
        float stepSize=((uppbo-lowbo)/(no_points));
        if(no_points<2)
        {
            printf("Please change the boundary levels of integration\n");
            exit(0);
        }
        int counti;
        float X[no_points],Y[no_points];
        for(counti=0;counti<=no_points;counti++)
        {
            X[counti]=lowbo+counti*stepSize;
        }
        solve integ;
        for(counti=0;counti<=no_points;counti++)
        {
            //initiating copied part of project file
            initial(&integ);
            setexpr(&integ,integEQ);
            calculate(&integ,X[counti]);
            Y[counti]=integ.sub_results[0];
        }
        // to file
        /*
        {
            FILE *fp;
            fp=fopen("/Users/mayankvalluru/Desktop/Numerical Analysis/Numerical Analysis.txt","a+");
            printf("Enter the subject\n");
            
            fputs("Name of subject : ",fp);
            fputs(subject,fp);
            fputs("\n",fp);
            fputs("Time : ",fp);
            fputs(times,fp);
            fputs("\n",fp);
            
            fputs("Equation : ",fp);
            fputs(integEQ,fp);
            fputs("\n",fp);
            fclose(fp);
        }
         */
        integrate(Y,no_points,stepSize);
    }
    else if(choice==4)
    {
       // sear();
    }
    
}
