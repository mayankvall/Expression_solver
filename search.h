/*
int compares(char *,char *,int);

void sear()
{
    FILE *fp;
    char line[1000];
    fp=fopen("/Users/Numerical Analysis.txt","r+");
    
    int secChoice;
    printf("Search on the basis of topic\n");
    printf("Press 1 on the basis of subject\n");
    printf("Press 2 on the basis of date\n");
    char toConcat[30];
    scanf("%d",&secChoice);
    char search[50];
    int flag=0;
    int breakF=0;
    int i;
    int count=0;
    char month[4];
    char dateD[3];
    switch(secChoice)
    {
        case 1:
            printf("Enter the part of subject name : \n");
            fpurge(stdin);
            fgets(search,50,stdin);
            
            strcpy(toConcat,"Name of subject : ");
            int lens=(int)strlen(toConcat);
            for(i=0;i<strlen(search);i++)
            {
                toConcat[lens+i]=search[i];
            }
            while(fgets(line,999,fp)!=NULL)
            {
                if(line[0]=='N')
                {
                    flag=compares(toConcat,line,count);
                }
                count++;
                if(flag==1)
                {
                    while(fgets(line,999,fp)!=NULL)
                    {
                        if(line[0]=='N'&&line[1]=='a'&&line[2]=='m'&&line[3]=='e')
                        {
                            breakF++;
                        }
                        if(breakF==1)
                        {
                            break;
                        }
                        
                        printf("%s\n",line);
                        
                    }
                    
                    break;
                }
            }
            if(flag==0)
            {
                printf("Required request doesn't exists\n");
            }
            break;
        case 2:
            printf("Enter the date of the project done : \n");
            fpurge(stdin);
            fgets(dateD,3,stdin);
            printf("Enter month in first three alph form :\n");
            fpurge(stdin);
            fgets(month,4,stdin);
            
            while(fgets(line,1000,fp)!=NULL)
            {
                if(line[0]=='T'&&line[1]=='i'&&line[2]=='m'&&line[3]=='e')
                {
                    if((line[15]==dateD[0]||line[15]==' ')&&line[16]==dateD[1]&&line[11]==month[0]&&line[12]==month[1]&&line[13]==month[2])
                    {
                        while(fgets(line,999,fp)!=NULL)
                        {
                            if(line[0]=='T'&&line[1]=='i'&&line[2]=='m'&&line[3]=='e')
                            {
                                if(line[15]==dateD[0]&&line[16]==dateD[1])
                                {
                                    breakF++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            
                            printf("%s\n",line);
                            
                        }
                    }
                }
                
            }
            
    }
}

int compares(char *given,char *fil,int counts)
{
    
    int lenG=(int)strlen(given);
    int count=0;
    
    while(1)
    {
        if(*given==*fil)
        {
            fil++;
            given++;
            count++;
        }
        else
            break;
    }
    
    if(counts==0)
    {
        if(count==lenG-1)
            return 1;
        else
            return 0;
    }
    if(count==lenG+1)
    {
        return 1;
    }
    else
        return 0;
    
}
 */
