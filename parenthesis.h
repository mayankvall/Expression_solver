
void check_para(char *s)
{
    int count=0;
    while(*s)
    {
        if(*s=='(')
        {
            count++;
        }
        if(*s==')')
        {
            count--;
        }
        s++;
    }
    
    if(count!=0)
    {
        printf("Unbalanced paraenthesis\n");
        exit(0);
    }
}

