# MIPS-Instruction-Set-Simulator
This c program emulates a MIPS-like instruction set with the following simplifications: 
  32-bit memory words with word addressability;
  a limited memory size of 1024 words;
  branch offsets and targets are not shifted before use;
  no jump or branch delay slots (i.e., jumps and branches have immediate effect);
  the program starts execution at address zero; and,
  no traps/exceptions/interrupts.
