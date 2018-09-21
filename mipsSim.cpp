#include <fstream>
#include <iostream>
using namespace std;

// Authors: Andrew Weathers and Nicholas Muenchen
// Date: 21 September 2018
// Purpose: Simulate a simplified MIPS-like
//			instruction set 

//opcode: 6 bits
//rs: 5 bits
//rt: 5 bits
//rd: 5 bits
//shift: 5 bits
//funct: 6 bits
unsigned int mar,
			 mdr,
			 pc,
 			 ir,
 			 rd,
 			 rs,
 			 rt,
 			 shift,
			 funct,
			 numUnTakenBranches = 0;

int 	     sign_ext,
 			 ram[1024];

//Adds the number in rs to the number in rt, then stores in rd
void addu()
{
  ram[rd] = ram[rs] + ram[rt];
  numLoads+=2;
  numStores++;
}

//Adds the number in rs to the immediately given value, then stores in rt
void addiu()
{
  ram[rt] = ram[rs] + sign_ext;
  numLoads+=2;
  numStores++;
}

//Performs bitwise AND operation rs*rt, then stores in rd
void _and(){
	ram[rd] = ram[rs]&ram[rt];
	numLoads+=2;
	numStores++;
}

void beq(){
	if(r[rs]==r[rt])
		pc += sign_ext;
}

void bgtz(){
	if(r[rs]>0) pc+=sign_ext;
}

void fetch()
{
  mar = pc;
  mdr = ram[mar];
  ir = mdr;
  pc++;
  numInstFetch++;

}

//Right shift by 26 to isolate the opcode
void (*decode())()
{

}

int main()
{
  _and();
  return 0;
}

//000000 00001 00010 00011 00000 100011
//000000 00001 00010 00011 00000 10011
