include<stdio.h>

// cai dat giai thuat sap xep vun dong cho cac cay vi du trong slide
int adjust(int i, int n){
int key=A[i];
int j =2*i;
While(j<=n){
if(j<n&&A[j]<A[j+1]) j+=1;
If(key>A[j]) A[j/2] = key;
else A[j/2]=A[j];

}
} 


int main(){
// cay bieu dien duoi dang vector
int n = 10
int a[n]={32,51,27,83,96,11,45,75,66};
for(int i=n/2;i<1,i--){
adjust(i,n);
}
}
