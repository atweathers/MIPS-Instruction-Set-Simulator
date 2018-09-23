#include <fstream>
#include <map>
#include <ios>
#include <iostream>
#include <string>
#include <iomanip>
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
			 numUnTakenBranches = 0,
			 numAlu = 0,
			 numInstFetch = 0,
			 numLoads = 0,
			 numStores = 0,

			 numJumps = 0,
			 numJumpsAndLinks = 0,
			 numTakenBranches = 0,
			 numUnTakenBranches = 0;

int 	     sign_ext,
 			 ram[1024];


map<unsigned int, string> opcodeMap;



void fillMap()
{
  opcodeMap[0x00] = "r";
  opcodeMap[0x09] = "i";
}


//Adds the number in rs to the number in rt, then stores in rd
void addu()
{
  ram[rd] = ram[rs] + ram[rt];
  numLoads+=2;
  numStores++;
  cout << setw(8) << pc << ": addu - r[" << rd << "] now contains " << setbase(16) << ram[rd] << endl;
}

//Adds the number in rs to the immediately given value, then stores in rt
void addiu()
{
  ram[rt] = ram[rs] + sign_ext;
  numLoads+=1;
  numStores++;
  //  cout << pc << ": addu - r[" << rt << "] now contains " << std::hex << ram[rt] << std:dec << endl;
}

//Performs bitwise AND operation rs*rt, then stores in rd
void _and(){
	ram[rd] = ram[rs]&ram[rt];
	numLoads+=2;
	numStores++;
void hlt()
{
  return;
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

/*
rs = (ir >> 21) & 0x1f; // clamps to the 5 bit rs
rt = (ir >> 16) & 0x1f; // clamps to the 5 bit rt
rd = (ir >> 11) & 0x1f; // clamps to the 5 bit rd
shift = (ir >> 6) & 0x1f; // clamps to the 5 bit shift
rs = ir & 0x2f; // clamps to the 6 bit funct
*/


void (*imm_func())()
{
  unsigned int opcode = (ir >> 26) & 0x3f; // clamp to 6-bit opcode field
  rs = (ir >> 21) & 0x1f; // clamp to the 5 bit rs
  rt = (ir >> 16) & 0x1f; // clamp to the 5 bit rt
  sign_ext = (ir) & 0xffff; // clamp to 16 bit immediate value

  if(opcode == 0x09)
  {
    return addiu;
  }


}

void (*other_func())()
{
  unsigned int opcode = (ir >> 26) & 0x3f; // clamp to 6-bit opcode field
  rs = (ir >> 21) & 0x1f; // clamps to the 5 bit rs
  rt = (ir >> 16) & 0x1f; // clamps to the 5 bit rt
  rd = (ir >> 11) & 0x1f; // clamps to the 5 bit rd
  shift = (ir >> 6) & 0x1f; // clamps to the 5 bit shift
  funct = ir & 0x2f; // clamps to the 6 bit funct

  if(opcode == 0x00)
  {
    if(funct == 0x21)
    {
      return addu;
    }
  }

}




//Right shift by 26 to isolate the opcode

void ( *decode() )()
{
  unsigned int opcode = (ir >> 26) & 0x3f; // clamp to 6-bit opcode field
  cout << opcode << " fff" << endl;
  if(opcodeMap.find(opcode) != opcodeMap.end())
  {
    //Finds if the function is immediate
    if(opcodeMap.find(opcode)->second.compare("r"))
    {
      cout << "imm" << endl;
      return imm_func();
    }
    else
    {
      cout << "rrr" << endl;
      return other_func();
    }
  }
  else
  {
    return hlt;
  }
}

int main()
{
  ir = 2234401;
  ram[0] = 1;
  ram[2] = 3;
  ram[3] = 4;
  ram[1] = 2;
  fillMap();
  void (* inst)();
  inst = decode();
  (*inst)();

  return 0;
}

//000000 00001 00010 00011 00000 100011
//000000 00001 00010 00011 00000 10011
