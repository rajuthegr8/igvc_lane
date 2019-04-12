#include "opencv2/opencv.hpp"
#include<stdlib.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat frame;//storing image on which computations are performed
Mat roi;//for storing the region of interest
Mat grass;//applying the 2b-r filterng single channel image 
Mat filter;//applying the gaussian filter
void ROI();
void grass_rm();
bool isvalid(Mat,int,int);
//apply_kernel(Mat,double **);
//int **getTemplate(int,int,double,double);//generating the gaussian matrix

int m=41,n=41;
int m1=(m-1)/2;int n1=(n-1)/2;
int theta=90;
double sigma=9;


double gaussian(double sigma,int x)
{
	double r=(x/sigma)*(x/sigma);
	double value= (r-1)*exp(-r*r/2)/(sigma*sigma);
	return value;
}

double **getTemplate(int m,int n,double sigma,double theta)
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
			//sum+=matrix[i][j];
		}
	}
    return matrix;
}

double sum_of_kernel(double **matrix)
{
	int i,j;
	double sum=0;
	for(i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			sum+=matrix[j][i];
		}
	}

	return sum;
}

bool isvalid(Mat img,int x,int y)
{
	if(x-m1<0 || x+m1>=img.cols-1 || y-n1<0 || y+n1>=img.rows-1)
		return false;
	else
		return true;
}

void apply_kernel(Mat filter,double **matrix)
{
	int i,j,k,l;
	double sum=0,sum1=sum_of_kernel(matrix);

	//cout<<sum1<<endl;

	for(i=0;i<roi.cols;i++)
	{
		for(j=0;j<roi.rows;j++)
		{
			//cout<<"a"<<endl;
			if(isvalid(grass,i,j))
			{
				//cout<<j<<"  "<<i<<endl;
				for(k=-m1;k<=m1;k++)
				{
					for(l=-n1;l<=n1;l++)
					{
						//cout<<l<<endl;
						//cout<<k<<endl;
						//cout<<j<<"  "<<i<<endl;
						sum+=grass.at<uchar>(j+l,i+k)*matrix[l+n1][k+m1];

					}
				}
                if (sum<0)
                {
                	filter.at<uchar>(j,i)=0;
                }
                else
                	filter.at<uchar>(j,i)=255;

				//filter.at<uchar>(j,i)=sum/sum1;
				sum=0;
			}
		}
	}
}



void ROI()
{
    Mat temp(((9*frame.rows)/13), (frame.cols), CV_8UC3, Scalar(0,0,0));
    for (int i = 0; i < (9*frame.rows)/13; ++i)
    {
    	for (int j = 0; j < (frame.cols); ++j)
    	{


    		temp.at<Vec3b>(i,j)[0]=frame.at<Vec3b>(((2.8*frame.rows)/13)+i,j)[0];
    		temp.at<Vec3b>(i,j)[1]=frame.at<Vec3b>(((2.8*frame.rows)/13)+i,j)[1];
    		temp.at<Vec3b>(i,j)[2]=frame.at<Vec3b>(((2.8*frame.rows)/13)+i,j)[2];
    		
    	}

    }
    roi=temp.clone();
    
}
void grass_rm()
{
	Mat temp(roi.rows,roi.cols,CV_8UC1,Scalar(0));
	int i,j;
	filter=temp.clone();

	for (i = 0; i < roi.rows; ++i)
	{
		for (j=0; j < roi.cols; ++j)
		{
			int pixel=2*roi.at<Vec3b>(i,j)[0]-roi.at<Vec3b>(i,j)[2];
			if (pixel<=0)
			{
				temp.at<uchar>(i,j)=0;
			}
			else if (pixel>=255)
			{
				temp.at<uchar>(i,j)=255;
			}
			else
			{
				temp.at<uchar>(i,j)=pixel;
			}
		}
	}
	grass=temp.clone();
}
int main()
{
	double **matrix;
	int i,j;

	matrix =new double*[n];
	for(i=0;i<n;i++)
	{
		matrix[i]=new double[m];
	}

	matrix=getTemplate(m,n,sigma,theta);

	VideoCapture vid("vid.mp4");
	namedWindow("ROI",WINDOW_NORMAL);
	//namedWindow("Original",WINDOW_NORMAL);
	namedWindow("grass_rm",WINDOW_NORMAL);
    namedWindow("filtered",WINDOW_NORMAL);
	
	

	while(true)
	{
		vid>>frame;
		//imshow("Original",frame);
		//apply_kernel(filter,matrix);
	    
		ROI();
		grass_rm();
		apply_kernel(filter,matrix);
		imshow("ROI",roi);
	    imshow("grass_rm",grass);
	    imshow("filtered",filter);
	    waitKey(2);
	}
	waitKey(0);
	return 0;
}
