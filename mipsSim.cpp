#include <fstream>
using namespace std;


//opcode: 6 bits
//rs: 5 bits
//rt: 5 bits
//rd: 5 bits
//shift: 5 bits
//funct: 5 bits

int mar;
int pc;
int ir;
int rd;
int rs;
int rt;
int sign_ext;
int funct;
int ram[1024];


int numAlu = 0;

int numInstFetch = 0;
int numLoads = 0;
int numStores = 0;

int numJumps = 0;
int numJumpsAndLinks = 0;
int numTakenBranches = 0;
int numUnTakenBranches = 0;

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
  ram[rt] = ram[rs] + sgin_ext;
  numLoads+=2;
  numStores++;
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


int main
{

  return 0;
}

//000000 00001 00010 00011 00000 100011
//000000 00001 00010 00011 00000 10011
