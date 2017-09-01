
//based on simpson's 3/8 rule


void integrate(float Y[],int size,float h)
{
    int i;//index variable
    
    float sum=0.00;
    for(i=0;i<size;i++)
    {
        if(i==0&&i==size-1)
        {
            sum=sum+Y[i];
        }
        else if(i%3==0)
        {
            sum=sum+2*Y[i];
        }
        else
        {
            sum=sum+3*Y[i];
        }
    }
    printf("Integrated result=%f ",(3*h/8)*sum);
    
    //to file
    /*
    {
        FILE *fp;
        fp=fopen("/Users/Numerical Analysis.txt","a+");
        
        fputs("The result of integration by simpson's method=",fp);
        char put[50];
        sprintf(put,"%f",(3*h/8)*sum);
        fputs(put,fp);
        fputs("\n\n",fp);
        fclose(fp);
    }
     */
    
}
