#include <stdio.h>
void main(){int i,N,R;scanf("%d%d",&N,&R);long long p=1;for(i=0;i<N-1;i++){printf("%ld ",p);p*=R;}printf("%ld\n",p);}