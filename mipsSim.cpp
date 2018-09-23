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
					registerArray[32],
 			 ram[1024];


map<unsigned int, string> opcodeMap;



void fillMap()
{
  opcodeMap[0x00] = "r";
  opcodeMap[0x09] = "i";
	opcodeMap[0x04] = "i";
	opcodeMap[0x07] = "i";
	opcodeMap[0x06] = "i";
	opcodeMap[0x05] = "i";
	opcodeMap[0x02] = "i";
	opcodeMap[0x03] = "i";
	opcodeMap[0x0f] = "i";
	opcodeMap[0x23] = "i";
	opcodeMap[0x1c] = "r";
	opcodeMap[0x0a] = "i";
	opcodeMap[0x2b] = "i";
	opcodeMap[0x0e] = "i";
}


//Adds the number in rs to the number in rt, then stores in rd
void addu()
{
  registerArray[rd] = registerArray[rs] + registerArray[rt];
  numAlu++;
  //cout << pc << ": addu - r[" << rd << "] now contains " << std::hex << registerArray[rd] << endl;
}

//Adds the number in rs to the immediately given value, then stores in rt
void addiu()
{
  registerArray[rt] = registerArray[rs] + sign_ext;
  numAlu++;
  //  cout << pc << ": addu - r[" << rt << "] now contains " << std::hex << registerArray[rt] << std:dec << endl;
}

//Performs bitwise AND operation rs*rt, then stores in rd
void _and()
{
	registerArray[rd] = registerArray[rs] & registerArray[rt];
	numAlu++;
}


//Branch is rs is equal to rt. Branches to immediate value.
void beq(){
	if (registerArray[rs] == registerArray[rt])
	{
		pc += sign_ext;
		numTakenBranches++;
	}
	else
	{
		numUnTakenBranches++;
	}

}

//Branch if r[rs] > 0 to the pc + signed immediate.
void bgtz()
{
	if (int(registerArray[rs]) > 0)
	{
		pc += sign_ext;
		numTakenBranches++;
	}
	else
	{
		numUnTakenBranches++;
	}
}

//Branch if r[rs] <= 0 to the pc + signed immediate.
void blez()
{
	if (int(registerArray[rs]) <= 0)
	{
		pc += sign_ext;
		numTakenBranches++;
	}
	else
	{
		numUnTakenBranches++;
	}
}

//Branch if registerArray[rs] is not equal to registerArray[rt], branch to pc + signed immediate.
void bne()
{
	if (registerArray[rs] != registerArray[rt])
	{
		pc += sign_ext;
		numTakenBranches++;
	}
	else
	{
		numUnTakenBranches++;
	}
}

//Halts execution
void hlt()
{
  return;
}


//or's register rs and register rt and places the result into rd
void _or()
{
	registerArray[rd] = registerArray[rs] | registerArray[rt];
	numAlu++;
}

//Shifts register rt left logically by shift and stores the result in rd
/////////////////////////////////
//UNSURE ABOUT THE REGISTERS TO BE USED, ALSO UNSURE ABOUT IMPLEMENTATION
/////////////////////////////////
void sll()
{
	registerArray[rd] = registerArray[rt] << shift;
	numAlu++;
}

//If register rs < sign_ext, then set register rt to 1 else set to 0
void slti()
{
	if (registerArray[rs] < sign_ext)
	{
		registerArray[rd] = 1;
	}
	else
	{
		registerArray[rd] = 0;
	}
	numAlu++;
}


//Logically shifts register rt right by shift and stores the result in rd, fills with ones or zeroes depending on sign
/////////////////////////////////
//UNSURE ABOUT THE REGISTERS TO BE USED, ALSO UNSURE ABOUT IMPLEMENTATION
/////////////////////////////////
void sra()
{
	registerArray[rd] = registerArray[rt] >> shift;
	numAlu++;
}


//Logically shifts register rt right by shift and stores the result in rd, fills with zeroes
/////////////////////////////////
//UNSURE ABOUT THE REGISTERS TO BE USED
/////////////////////////////////
void srl()
{
	registerArray[rd] = (unsigned int )(registerArray[rt]) >> shift;
	numAlu++;
}

//Subtract register rt from register rs and save the result into rd
void subu()
{
	registerArray[rd] = registerArray[rs] + registerArray[rt];
	numAlu++;
}

//Stores the word in r[t] at registerArray[registerArray[rs] + sign_imm
void sw()
{
	registerArray[registerArray[rs] + sign_ext] = registerArray[rt];
	numStores++;
}

//Exclusive or's registerArray[rs] and registerArray[rt] then stores the result in registerArray[rd]
void _xor()
{
	registerArray[rd] = registerArray[rs] ^ registerArray[rt];
	numAlu++;
}


//Exclusive or's registerArray[rs] with sign_ext and stores the result in registerArray[rt]
void xori()
{
	registerArray[rt] = registerArray[rs] ^ sign_ext;
	numAlu++;
}




//Fetches the next instruction.
void fetch()
{
  mar = pc;
  mdr = registerArray[mar];
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
	if(opcode == 0x04)
	{
		return beq;
	}
	if(opcode == 0x07)
	{
		return bgtz;
	}
	if(opcode == 0x06)
	{
		return blez;
	}
	if(opcode == 0x05)
	{
		return bne;
	}
	if(opcode == 0x02)
	{
		return j;
	}
	if(opcode == 0x03)
	{
		return jal;
	}
	if(opcode == 0x0f)
	{
		return lui;
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
		if(funct == 0x24)
		{
			return _and;
		}
		if(funct == 0x09)
		{
			return jalr;
		}
		if(funct == 0x08)
		{
			return jr;
		}
		if(funct == 0x27)
		{
			return nor;
		}
		if(funct == 0x25)
		{
			return _or;
		}
		if(funct == 0x00)
		{
			return sll;
		}
		if(funct == 0x03)
		{
			return sra;
		}
		if(funct == 0x02)
		{
			return srl;
		}
		if(funct == 0x23)
		{
			return subu;
		}
		if(funct == 0x26)
		{
			return _xor;
		}
  }
	if(opcode == 0x1c)
	{
		return mul;
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
  registerArray[0] = 1;
  registerArray[2] = 3;
  registerArray[3] = 4;
  registerArray[1] = 2;
  fillMap();
  void (* inst)();
  inst = decode();
  (*inst)();

  return 0;
}

//000000 00001 00010 00011 00000 100011
//000000 00001 00010 00011 00000 10011
