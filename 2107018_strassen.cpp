#include<bits/stdc++.h>
#include<chrono>
using namespace std;

/*Throughout the program,whenever it is needed the vectors are passed as const reference
to remove the vector copying time. And to ensure that temporary vectors can be passed as reference
const keyword is used */

// Function to add two matrices
vector<vector<int>> add(const vector<vector<int>>&a,const vector<vector<int>>&b, int n){
    vector<vector<int>> c(n, vector<int>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            c[i][j]=a[i][j]+b[i][j];
        }
    }
    return c;
}

// Function to subtract two matrices
vector<vector<int>> sub(const vector<vector<int>> &a,const vector<vector<int>> &b, int n){
    vector<vector<int>> c(n,vector<int>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            c[i][j]=a[i][j]-b[i][j];
        }
    }
    return c;
}

//strassen's multiplication function
vector<vector<int>> strassen(const vector<vector<int>> &A,const vector<vector<int>> &B,int n)
{
    if(n==1){
        vector<vector<int>>C(1,vector<int>(1));
        C[0][0]=A[0][0]*B[0][0];
        return C;
    }

    int newsize=n/2;

    vector<vector<int>>A11(newsize,vector<int>(newsize,0));
    vector<vector<int>>A12(newsize,vector<int>(newsize,0));
    vector<vector<int>>A21(newsize,vector<int>(newsize,0));
    vector<vector<int>>A22(newsize,vector<int>(newsize,0));

    vector<vector<int>>B11(newsize,vector<int>(newsize,0));
    vector<vector<int>>B12(newsize,vector<int>(newsize,0));
    vector<vector<int>>B21(newsize,vector<int>(newsize,0));
    vector<vector<int>>B22(newsize,vector<int>(newsize,0));

    for(int i=0; i<newsize; i++){
        for(int j=0; j<newsize; j++){
            A11[i][i]=A[i][j];
            A12[i][j]=A[i][j+newsize];
            A21[i][j]=A[i+newsize][j];
            A22[i][j]=A[i+newsize][i+newsize];

            B11[i][i]=B[i][j];
            B12[i][j]=B[i][j+newsize];
            B21[i][j]=B[i+newsize][j];
            B22[i][j]=B[i+newsize][i+newsize];
        }
    }

    vector<vector<int>> P=strassen(add(A11,A22,newsize),add(B11,B22,newsize),newsize);
    vector<vector<int>> Q=strassen(add(A21,A22,newsize),B11,newsize);
    vector<vector<int>> R=strassen(A11,sub(B12,B22,newsize),newsize);
    vector<vector<int>> S=strassen(A22,sub(B21,B11,newsize),newsize);
    vector<vector<int>> T=strassen(add(A11,A12,newsize),B22,newsize);
    vector<vector<int>> U=strassen(sub(A21,A11,newsize),add(B11,B12,newsize),newsize);
    vector<vector<int>> V=strassen(sub(A12,A22,newsize),add(B21,B22,newsize),newsize);

    vector<vector<int>> C(n,vector<int>(n,0));

    for(int i=0; i<newsize; i++){
        for(int j=0; j<newsize; j++){
            C[i][j]=P[i][j]+S[i][j]-T[i][j]+V[i][j];
            C[i][j+newsize]=R[i][j]+T[i][j];
            C[i+newsize][j]=Q[i][j]+S[i][j];
            C[i+newsize][j+newsize]=P[i][j]+R[i][j]-Q[i][j]+U[i][j];
        }
    }

    return C;
}

//normal multiplication (O^3)
vector<vector<int>> mul(vector<vector<int>> &a,vector<vector<int>> &b,int n)
{
    vector<vector<int>> c(n,vector<int>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<n; k++){
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
    return c;
}

//this function generates a random matrix which contains elements from 0 to 999
vector<vector<int>> genmat(int n)
{
    vector<vector<int>>a(n,vector<int>(n,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            a[i][j]=rand()%1000;
        }
    }
    return a;
}
/*This function is implemented because the assignment has asked so,it likely has no use in our time comparison
since we are never needing to call it.Because we are creating functions as powers of 2 from main function ,via the generate matrix function*/
vector<vector<int>> padd(vector<vector<int>>& a,int n)
{
    int newsize=pow(2,ceil(log2(n)));
    vector<vector<int>> c(newsize,vector<int>(newsize,0));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            c[i][j]=a[i][j];
        }
    }
    return c;
}

int main()
{
    srand(time(0));
    for(int i=1; i<=9; i++) { //2^10 is the highest sized matrix
        int n=pow(2,i);
        vector<vector<int>>a=genmat(n);
        vector<vector<int>>b=genmat(n);
        //time  calculation for normal multiplication
        auto start=chrono::high_resolution_clock::now();
        vector<vector<int>> c1 = mul(a,b,n);
        auto finish=chrono::high_resolution_clock::now();
        double time1=chrono::duration<double>(finish-start).count();

        start=chrono::high_resolution_clock::now();
        vector<vector<int>>c2=strassen(a,b,n);
        finish=chrono::high_resolution_clock::now();
        double time2=chrono::duration<double>(finish-start).count();

        cout<<"Size:"<<n<<"  Strassen = "<<time2 <<"s   Normal = "<<time1<<"s\n";
    }
}
