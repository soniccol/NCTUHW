#include <iostream>
#include <string>
#include <vector>
#include "InternalFun.h"
#include <algorithm>
#define ROTL(x,n) ( (x) << (n) | ((x) >> (64 - (n))))  // Circular shift "x" left by "n" bits
using namespace std;
bitset<64> RC[24] = 
{ 
	0x0000000000000001,
	0x0000000000008082,
	0x800000000000808A,
	0x8000000080008000,
	0x000000000000808B,
	0x0000000080000001,
	0x8000000080008081,
	0x8000000000008009,
	0x000000000000008A,
	0x0000000000000088,
	0x0000000080008009,
	0x000000008000000A,
	0x000000008000808B,
	0x800000000000008B,
	0x8000000000008089,
	0x8000000000008003,
	0x8000000000008002,
	0x8000000000000080,
	0x000000000000800A,
	0x800000008000000A,
	0x8000000080008081,
	0x8000000000008080,
	0x0000000080000001,
	0x8000000080008008
};

	vector<vector<bitset<64>>> newStateArray;
	vector<vector<bitset<64>>> piValue;
	vector<vector<bitset<64>>> xiValue;
 	vector<bitset<64>> stateArray;
	Binary origin;
	vector<vector<bitset<64>>> rhoValue;


bitset<64> shiftRight(bitset<64> b, unsigned int m)
{
	b = b << m | b >> (64-m);
	return b;
}

bitset<64> shiftLeft(bitset<64> b, unsigned int m)
{
	b = b >> m | b << (64-m);
	return b;
}
Binary internalFun(Binary stateVar) 
{
	vector<bitset<64>> tempV;
	bitset<64> temp;
	int j=0;
	for(int i=0;i<1600;i++){
		temp[j]=stateVar[i];
		j++;
		if(i!=0&&(i+1)%64==0){
			tempV.push_back(temp);
			temp.reset();
			j=0;
		}
	}
	vector<bitset<64>> state1;
	vector<bitset<64>> state2;
	vector<bitset<64>> state3;
	vector<bitset<64>> state4;
	vector<bitset<64>> state5;
	for(int i=0;i<25;i++){
		if(i%5==0){
			state1.push_back(tempV[i]);
		}else if(i%5==1){
			state2.push_back(tempV[i]);
		}else if(i%5==2){
			state3.push_back(tempV[i]);
		}else if(i%5==3){
			state4.push_back(tempV[i]);
		}else if(i%5==4){
			state5.push_back(tempV[i]);
		}
	}
	newStateArray.push_back(state1);
	newStateArray.push_back(state2);
	newStateArray.push_back(state3);
	newStateArray.push_back(state4);
	newStateArray.push_back(state5);

	for(int i=0;i<24;i++){
		stateVar=roundFun(stateVar,RC[i]);
	}
	newStateArray.clear();
	
	return stateVar;
}


/*** Round function in SHA-3 ***/
Binary roundFun(Binary stateVar, bitset<64> singleRC)
{

	int counter=0;
	/*** TODO: Implement your SHA3's 5 functions as follows ***/
	/*** Theta ***/
	vector<bitset<64>> C(5);
	for(int x=0;x<5;x++){
		C[x] = newStateArray[x][0] ^ newStateArray[x][1] ^ newStateArray[x][2] ^ newStateArray[x][3] ^ newStateArray[x][4];
	}
	vector<bitset<64>> D(5);
	D[0] = C[4] ^ shiftRight(C[1],1);
	D[1] = C[0] ^ shiftRight(C[2],1);
	D[2] = C[1] ^ shiftRight(C[3],1);
	D[3] = C[2] ^ shiftRight(C[4],1);
	D[4] = C[3] ^ shiftRight(C[0],1);

	for(int x=0;x<5;x++){
		newStateArray[x][0] = newStateArray[x][0]  ^ D[x];
		newStateArray[x][1] = newStateArray[x][1]  ^ D[x];
		newStateArray[x][2] = newStateArray[x][2]  ^ D[x];
		newStateArray[x][3] = newStateArray[x][3]  ^ D[x];
		newStateArray[x][4] = newStateArray[x][4]  ^ D[x];
	}

	/*** Rho  ***/
	rhoValue = newStateArray;
	const int rho_constant [24] = {
		1 ,3 ,6 ,10,15,21,
		28,36,45,55, 2,
	        14,27,41,56, 8,25,
	        43,62,18,39,61,20,44
	};
	int xx=0;
	int yy=0;
	for(int t=0;t<24;t++){
		int rho_value=0;
		if(t==0){
			xx=1;
			yy=0;
			rho_value=rho_constant[t];
			for(int z=0;z<64;z++){
				newStateArray[xx][yy][z]=rhoValue[xx][yy][(z+64-rho_value)%64];
			}
		}else{
			int tempxx=xx%5;
			int tempyy=yy%5;
			xx=tempyy%5;		
			yy=(2*tempxx+3*tempyy)%5;
			rho_value=rho_constant[t];
			for(int z=0;z<64;z++){
				newStateArray[xx][yy][z]=rhoValue[xx][yy][(z+64-rho_value)%64];
			}
		}
	}	

	/*** Pi ***/
	piValue=newStateArray;
	for(int y=0;y<5;y++){
		for(int x=0;x<5;x++){
			int yp=((2*x+3*y)%5);
			newStateArray[y][yp]=piValue[x][y];
		}
	}


	/*** Xi ***/
	xiValue=newStateArray;
	for(int y=0;y<5;y++){
		for(int x=0;x<5;x++){
			newStateArray[x][y]= xiValue[x][y] ^ ((~xiValue[(x+1)%5][y]) & xiValue[(x+2)%5][y]);
		}
		xiValue=newStateArray;
	}

	/*** Iota start ***/
	bitset<64> rSingleRC;
	for(int i=0,j=63;i<64,j>-1;i++,j--){
	    rSingleRC[i]=singleRC[j];
	}
	newStateArray[0][0] =newStateArray[0][0] ^ rSingleRC;	

	/*** Iota End ***/
	counter=0;
	for(int y=0;y<5;y++){
		for(int x=0;x<5;x++){
			for(int z=0;z<64;z++){
				stateVar[counter]=newStateArray[x][y][z];
				counter++;
			}
		}
	}
	counter=0;
	return stateVar;
}
