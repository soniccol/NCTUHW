#include <iostream>
#include <string>
#include "Sponge.h"
#include "InternalFun.h"
#include <algorithm>
using namespace std;
#define bitRate   576 
#define capacity  1024

void  SpongeConstruction(string inputString, int outputLen) 
{
	// Transform the input string into binary bits
	BinaryTransfer(inputString);			
	vector<Binary> Message = Padding(inputString);
	//Initialize the state variable, there are 1600 bits set default as zero
	Binary stateVar ;
	// Absorbing phase
	/*** TODO: Implement your SHA3's absorbing phase here ***/
	for(int i=0;i<Message.size();i++){
        	Binary t = Message[i];
		stateVar^=t;
		stateVar=internalFun(stateVar);
	}

	// Squeezing phase
	string hashVal=""; // The final output value 
	/*** TODO: Implement your SHA3's squeezing phase here ***/
	string stateStr = stateVar.to_string();
	reverse(stateStr.begin(),stateStr.end());
	hashVal+=stateStr.substr(0,576);
	int squeezeTime=outputLen/576;
	if(squeezeTime > 0){
		for(int i=0;i<squeezeTime;i++){
			stateVar=internalFun(stateVar);
			string result_s=stateVar.to_string();
			reverse(result_s.begin(),result_s.end());
			hashVal+=result_s.substr(0,576);
		}
	}
	// Print the hash value to the stdout
	PrintHex(hashVal.substr(0,outputLen));
}

int mod(int a, int b)
{ return (a%b+b)%b; }

void BinaryTransfer(string& inputString)
{
	string binary = "" ;
	for(int i=0; i<inputString.length(); i++)
	{
		for(int k=7; k>=0; k--)
			if( ((inputString[i] >> k) & 0x01) )
				binary += "1" ;
			else
				binary += "0" ;
	}
	inputString = binary ;
}

vector< Binary > Padding(string inputString) 
{
	/*** TODO: Implement with Multirate padding ***/
	/*** Multirate padding: Appends 10*1 to the message M such that resulting length is r*k bits
         10*1={11,101,1001,......}
        ***/
	int zeroLength= mod(-inputString.length()-2,576);
	inputString+="1";
	for(int i=0;i<zeroLength;i++){
		inputString+="0";
	}
	inputString+="1";
	//store padding message into result vector
	vector<Binary> PaddingMessage;
	Binary tempMessage;
	int j=0;
	for(int i=0;i<inputString.length();i++){
		char c = inputString[i];
		if(c=='1'){
			tempMessage[j]=1;
		}else{
			tempMessage[j]=0;
		}
		j++;
		if((i+1)%576==0){
			PaddingMessage.push_back(tempMessage);	
			tempMessage.reset();
			j=0;
		}		
	}
	return PaddingMessage;	
}

/*** Print the hash value in hex ***/
void PrintHex(string hashVal) 
{
	for(int i=0; i<hashVal.length(); i+=4)
	{
		string Ahex = hashVal.substr(i, 4) ; 
		int Val = 0 ;
		for(int k=0; k<4; k++)
			if( Ahex[k]-'0' )	
				Val += (1 << (3-k)) ;
		
		if( Val < 10 )	cout << Val ;
		else	cout << (char)(Val+55) ;			
	}		
}
