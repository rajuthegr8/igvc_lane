#include <iostream>

#include <math.h>
using namespace std;

double **getTemplate(int,int,double,double);//generating the gaussian matrix
double gaussian(double ,int );//second derivative of the gaussian


int m=17,n=13;
int m1=(m-1)/2;int n1=(n-1)/2;
int theta=0;
double sigma=6;
double gaussian(double sigma,int x)
{
	double r=(x/sigma)*(x/sigma);
	double value= (r-1)*exp(-r*r/2)/(sigma*sigma);
	return value;
}

double **getTemplate(int n,int m,double sigma,int theta)
{
	double **matrix,sum=0;
	int i,j;
    matrix=new double*[n];
	for ( i = 0; i < n; ++i)
	{
		matrix[i]=new double[m];
	}

	for ( i = 0; i < n; ++i)
	{
		for (j = 0; j < m; ++j)
		{
			matrix[i][j]=gaussian(sigma,j-n1);//cos(3.14159*theta/180)*gaussian(sigma,m1-i)+sin(3.14159*theta/180)*gaussian(sigma,j-n1);
			sum+=matrix[i][j];
		}
	}
    return matrix;
}
void print_template(double **matrix,int n,int m)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;

	}

}
int main()
{
	double **matrix;
	matrix=getTemplate(n,m,sigma,theta);
	print_template(matrix,n,m);
	return 0;
}
