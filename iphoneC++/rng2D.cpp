#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <cmath>
#define WIDTH 3264
#define HEIGHT 2448
#define CHANNEL 3
#define left 2
#define DEPTH 1

using namespace std;
int main(int argc, char *argv[])
{
	if(argc!=4){cerr<<"format: <n_req> <start> <end>\n";exit(-1);}
	char fname[100];
	int power=atoi(argv[1]),
		n_req=pow(10,power),
		first=atoi(argv[2]),
		last=atoi(argv[3]),
		dim,n_mat,n_got=0,frameN=0,right=255-left,w,h,flip;
	short b;double total_time=0;
	dim=floor(sqrt(n_req));
	n_mat=dim*dim;
	short **array=new short *[dim];
	for(int i=0;i<dim;i++)
		array[i]=new short[dim];
	short *frame=new short [WIDTH*HEIGHT*CHANNEL];//the array to store brightness in each frame

	cerr<<"number requested:	10^"<<power<<endl
		<<"matrix dimension:	"<<dim<<"x"<<dim<<endl
		<<"matrix size:		"<<n_mat<<endl;
	int pi,pj;//pixel in frame
	clock_t start,end;
	for(int f=first;f<=last;f++)
		{
		if(n_got>=n_mat){cerr<<"enough!"<<endl;break;}
		frameN++;
		sprintf(fname,"%d.dat\0",f);
		ifstream in(fname,ios::in);
		pi=0;
		while (in.good())
			{
			in>>frame[pi++];
			
			}//the while loop plays the role of the snapshot, which can be very fast when run on camera chip 
		start=clock();
		//----------------------------------
		if(frameN%2)//odd frame
			{
			for(pj=0;pj<pi&&n_got<n_mat;pj++)
				if(frame[pj]>=left&&frame[pj]<=right)
				{
				h=n_got/dim;
				w=n_got%dim;
				array[w][h]=frame[pj]&1;		
				n_got++;
				}	
			}
		else		//even frame
			{
			for(pj=0;pj<pi&&n_got<n_mat;pj++)
			if(frame[pj]>=left&&frame[pj]<=right)
				{
				w=n_got/dim;
				h=n_got%dim;
				array[h][w]=frame[pj]&1;		
				n_got++;
				}
			}
		//----------------------------------
		end=clock();
		double dif=double(end-start)/CLOCKS_PER_SEC;
		total_time+=dif;
		printf("frame %d: time: %.5f\t\tnum_got: %d\n",frameN,dif,n_got);
		}
		ofstream out("./seq.dat",ios::out);
		for(int h=0;h<dim;h++)
			for(int w=0;w<dim;w++)
				out<<array[h][w];
		for(;pj<pi&&n_got<n_req;pj++)
			if(frame[pj]>=left&&frame[pj]<=right)
				{
				b = frame[pi]&1;
				out<<b;
				n_got++;}
	cerr <<"total time:\t"<<total_time<<"\t"<<double(n_req/total_time)/pow(10,6)<<" Mbps"<<endl;
	delete [] array;
	delete [] frame;

}

