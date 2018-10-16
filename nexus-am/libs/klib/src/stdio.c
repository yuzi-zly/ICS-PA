#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static int cnt;

static void itos(char s[],int x)
{
    int a[20];
    int num=0;
    while(x/10!=0)
    {
        int y=x%10;
        a[num++]=y;
        x=x/10;
    }
    a[num]=x;
   
    for(int i=0;i<=num;i++)
    {
        s[i]=a[num-i]+'0';
    }
    s[num+1]='\0';
    return;
}

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}


int sprintf(char *out, const char *fmt,...){
    assert(fmt);
    va_list ap;
    va_start(ap,fmt);
    cnt=0;
    const char *str=fmt;
    char *tem=NULL;
    char temp[40];
    char s[40];
    int x;
    memset(out,0,sizeof(out));

    while((*str)!='\0')
    {
        if(*str=='%')
        {
            str++;
            switch(*str)
            {
                case 's':{
                            tem=va_arg(ap,char*);
                            strcat(out,tem);
                            cnt+=strlen(tem);
                            break;
                         }
                        
                case 'd': {
                               memset(s,0,sizeof(s));
                               memset(temp,0,sizeof(temp));
                               x=va_arg(ap,int);
                               if(x==-2147483648)
                               {
                                    strcpy(temp,"-2147483648");
                                    strcat(out,temp);
                                    cnt+=strlen(temp);
                               }
                               else if(x<0)
                               {
                                  x=-x;
                                  temp[0]='-';
                                  itos(s,x);
                                  strcat(temp,s);
                                  strcat(out,temp);
                                  cnt+=strlen(temp);
                                 
                               }
                               else 
                               {
                                   itos(s,x);
                                   strcat(out,s);
                                   cnt+=strlen(s);
                               }
                               break;
                          }
            }
            str++;
        }
        else
        {
           memset(temp,0,sizeof(temp));
           temp[0]=*str;
           temp[1]='\0';
           strcat(out,temp);
           cnt++;
           str++;
        }
            
    }
    return cnt;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
