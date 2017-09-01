
int *cal;
void DD(float[],float [],int);
float returns(int,int);
float dd[100][100];
char s[100];
char when[100];
void receive(char *s)
{
    strcpy(when,s);
}

char* sending()
{
    return when;
}

void cals()
{
    int no_in;
    float result=0;
    char subject[50];
    
    printf("Enter the subject\n");
    fpurge(stdin);
    fgets(subject,50,stdin);
    printf("Enter the number of data points given\n");
    fpurge(stdin);
    scanf("%d",&no_in);
    float arrX[no_in],arrY[no_in];
    int count=0;
    printf("Enter the data points\n");
    printf("X\tY\n");
    
    //to file
    /*
    FILE *fp;
    fp=fopen("/Users/mayankvalluru/Desktop/Numerical Analysis/Numerical Analysis.txt","a+");
    
    fputs("Name of subject : ",fp);
    fputs(subject,fp);
    fputs("\n",fp);
    fputs("Time : ",fp);
    fputs(sending(),fp);
    fputs("\n",fp);
    

    fputs("Newtons Divided difference : \n",fp);
    
    */
    
    for(count=0;count<no_in;count++)
    {
        scanf("%f",&arrX[count]);
        scanf("%f",&arrY[count]);
    }
    DD(arrX,arrY,no_in);
    
    
    printf("Enter the point at which value is calculated, insure at the point lies in between the points given\n");
    float point;
    fpurge(stdin);
    scanf("%f",&point);
    
    
    
    {
        result=arrY[0];
        int count=0;
        int countj=0;
        for(count=0;count<no_in-1;count++)
        {
            float value=1;
            for(countj=0;countj<=count;countj++)
            {
                value=value*(point-arrX[countj]);
            }
            value=value*dd[count][0];
            result=result+value;
        }
        printf("Result = %f",result);
    }
    
    
    //to file
    /*
    {
        fputs("X                     ",fp);
        fputs("f(X)                  ",fp);
        int i,j,len,k;
        for(i=1;i<no_in;i++)
        {
            char concat[5];
            sprintf(concat,"%d",i);
            fputs("dd-",fp);
            fputs(concat,fp);
            len=(int)strlen(concat);
            for(j=0;j<19-len;j++)
            {
                fputs(" ",fp);
            }
        }
        fputs("\n",fp);
        
        for(i=0;i<no_in;i++)
        {
            char X[20];
            sprintf(X,"%f",arrX[i]);
            fputs(X,fp);
            len=(int)strlen(X);
            for(j=0;j<22-len;j++)
            {
                fputs(" ",fp);
            }
            strcpy(X,"");
            sprintf(X,"%f",arrY[i]);
            fputs(X,fp);
            len=(int)strlen(X);
            for(j=0;j<22-len;j++)
            {
                fputs(" ",fp);
            }
            
            for(k=0;k<no_in-1-i;k++)
            {
                strcpy(X,"");
                sprintf(X,"%f",dd[k][i]);
                fputs(X,fp);
                len=(int)strlen(X);
                for(j=0;j<22-len;j++)
                {
                    fputs(" ",fp);
                }
            }
            
            fputs("\n\n",fp);
        }
    }
    fclose(fp);
     */
}

void DD(float X[],float Y[],int size)
{
    int i,j;
    for(i=0;i<size-1;i++)
    {
        dd[0][i]=(Y[i+1]-Y[i])/(X[i+1]-X[i]);
    }
    
    for(i=1;i<size-1;i++)
    {
        for(j=0;j<size-1-i;j++)
        {
            dd[i][j]=(dd[i-1][j+1]-dd[i-1][j])/(X[i+1+j]-X[j]);
        }
    }
    
}
