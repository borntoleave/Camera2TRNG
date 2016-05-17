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
#define FRAME_L WIDTH*HEIGHT*CHANNEL
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
	short b;double total_time=0,t_read=0;
	dim=floor(sqrt(n_req));
	n_mat=dim*dim;
	short **array=new short *[dim];
	for(int i=0;i<dim;i++)
		array[i]=new short[dim];
	short *frame=new short [FRAME_L];//the array to store brightness in each frame

	cerr<<"number requested:	10^"<<power<<endl
		<<"matrix dimension:	"<<dim<<"x"<<dim<<endl
		<<"matrix size:		"<<n_mat<<endl;
	int pi,pj;//pixel in frame
	clock_t start,end,read_start,read_end;
	int f=first-1;pi=FRAME_L;t_read=0;
	start=clock();
	for (w=0;w<dim;w++)
		for (h=0;h<dim;h++)//traverse the row number for each column number
			{
			if(pi==FRAME_L)
				{
				read_start=clock();	
				f++;
				sprintf(fname,"%d.dat\0",f);
				ifstream in(fname,ios::in);
				pi=0;
				while (in.good())
					{
					in>>frame[pi++];
					}//the while loop plays the role of the snapshot, which can be very fast when run on camera chip 
				read_end=clock();
				double diff=double(read_end-read_start)/CLOCKS_PER_SEC;
				t_read+=diff;
				pi=0;
				printf("frame %d\n",f-first);
				}
			while(frame[pi]<left||frame[pi]>right)pi++;
			if(f%2)//odd frame
				{
				array[h][w]=(frame[pi]+1)&1;		
				}	
			else		//even frame
				{
				array[h][w]=frame[pi]&1;		
				}
			pi++;
//			cerr <<pi<<endl;
			}
		//----------------------------------
		end=clock();
		double diff=double(end-start)/CLOCKS_PER_SEC;
		total_time=diff-t_read;
		
		ofstream out("./seq.dat",ios::out);
		for(int h=0;h<dim;h++)
			for(int w=0;w<dim;w++)
				out<<array[h][w];
		for(;pi<FRAME_L;pi++)
			if(frame[pi]>=left&&frame[pi]<=right)
				{
				b = frame[pi]&1;
				out<<b;
				n_got++;}
	cerr <<"total time:\t"<<total_time<<"\t"<<double(n_mat/total_time)/pow(10,6)<<" Mbps"<<endl;
	delete [] array;
	delete [] frame;

}

