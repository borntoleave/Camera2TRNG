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
		nGot[4]={0,0,0,0},frameN=0,right=255-left,w,h,flip;
	short b;double total_time=0;
		
	short *READ=new short [n_req];//the array to store seq
	short *frame=new short [WIDTH*HEIGHT*CHANNEL];//the array to store brightness in each frame
	cerr<<"number requested:	10^"<<power<<endl;

	int pi,pj,pc;//pixel in frame; pc is the channel
	clock_t start,end;
	for(int f=first;f<=last;f++)
		{
		if(nGot[0]>=n_req){cerr<<"enough!"<<endl;break;}
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
		{
			for(pj=0;pj<pi&&nGot[0]<n_req;pj+=3)
				for(pc=0;pc<3;pc++)
				if(frame[pj+pc]>=left&&frame[pj+pc]<=right)
				{
				if(nGot[pc+1]%2&&(frame[pj+pc]<85||frame[pj+pc]>170))
					READ[nGot[0]]=(frame[pj+pc]+1)&1;	
				else		
					READ[nGot[0]]=frame[pj+pc]&1;
				nGot[0]++;nGot[pc+1]++;
				}	
			}
		
		//----------------------------------
		end=clock();
		double dif=double(end-start)/CLOCKS_PER_SEC;
		total_time+=dif;
		printf("frame %d: time: %.5f\t\tnum_got: %d\n",frameN,dif,nGot[0]);
		}
		ofstream out("./seq.dat",ios::out);
		for(int i=0;i<n_req;i++)
			out<<READ[i];
		
	cerr <<"total time:\t"<<total_time<<"\t"<<double(n_req/total_time)/pow(10,6)<<" Mbps"<<endl;
	delete [] READ;
	delete [] frame;

}

