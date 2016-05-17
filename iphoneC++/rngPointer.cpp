//uses pointer array for transposing
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
	short **WRITE=new short *[n_mat];//the pointer array stores READ's elements in column-major order
	short *READ=new short [n_mat];//the array to store seq
	short *frame=new short [WIDTH*HEIGHT*CHANNEL];//the array to store brightness in each frame
	for(int pi=0;pi<dim*dim;pi++)
		{
		w=pi/dim;
		h=pi%dim;
		WRITE[pi]=READ+h*dim+w;
		}
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
				*WRITE[n_got] = 	frame[pj]&1;		
				n_got++;
				}	
			}
		else		//even frame
			{
			for(pj=0;pj<pi&&n_got<n_mat;pj++)
			if(frame[pj]>=left&&frame[pj]<=right)
				{
				*WRITE[n_got] = 	(frame[pj]+1)&1;		//flip bit
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
		for(int i=0;i<n_mat;i++)
			out<<READ[i];
		for(;pj<pi&&n_got<n_req;pj++)
			if(frame[pj]>=left&&frame[pj]<=right)
				{b = frame[pi]&1;
				out<<b;
				n_got++;}
	cerr <<"total time:\t"<<total_time<<"\t"<<double(n_got/total_time)/pow(10,6)<<" Mbps"<<endl;
	delete [] READ;
	delete [] frame;
	//for(pi=0;pi<dim*dim;pi++)
//		delete [] WRITE[pi];
	delete [] WRITE;
}

