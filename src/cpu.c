#include "cpu.h"
#include "mem.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

void init_cpu()
{
	for (int j = 0; j < 0xf; j++) {
		cpu.v[j] = 0;
		cpu.stack[j] = 0;
	}
	cpu.i = 0;
	cpu.dt = 0;
	cpu.st = 0;
	cpu.pc = 0;
	cpu.sp = 0;
}

void init_gfx() 
{
	gfx.draw = false;
	for (int j = 0; j < 32; j++) {
		for (int k = 0; k < 64; k++) {
			gfx.screen[j][k] = 0;
		}
	}
}

void emulate_cycle(uint16_t opcode)
{
	switch (opcode >> 12) {
		case 0x0:
			switch (opcode & 0xff) {
				case 0xe0: cls_0(); break;
				case 0xee: ret_0(); break;
				default: printf("UNKNOWN 0"); break;
			}
			break;
		case 0x1: jp_1(opcode & 0xfff); break;
		case 0x2: call_2(opcode & 0xfff); break;
		case 0x3: se_3((opcode >> 8) & 0xf, opcode & 0xff); break;
		case 0x4: sne_4((opcode >> 8) & 0xf, opcode & 0xff); break;
		case 0x5: se_5((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
		case 0x6: ld_6((opcode >> 8) & 0xf, opcode & 0xff); break;
		case 0x7: add_7((opcode >> 8) & 0xf, opcode & 0xff); break;
		case 0x8:
			  switch (opcode & 0xf) {
				  case 0x0: ld_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0x1: or_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0x2: and_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0x3: xor_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0x4: add_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0x5: sub_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0x6: shr_8((opcode >> 8) & 0xf); break;
				  case 0x7: subn_8((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
				  case 0xe: shl_8((opcode >> 8) & 0xf); break;
				  default: printf("UNKNOWN 8");
			  }
			  break;
		case 0x9: sne_9((opcode >> 8) & 0xf, (opcode >> 4) & 0xf); break;
		case 0xa: ld_a(opcode & 0xfff); break;
		case 0xb: jp_b(opcode & 0xfff); break;
		case 0xc: rnd_c((opcode >> 8) & 0xf, opcode & 0xff); break;
		case 0xd: drw_d((opcode >> 8) & 0xf, (opcode >> 4) & 0xf, opcode & 0xf); break;
		case 0xe:
			  switch (opcode & 0xff) {
				  case 0x9e: skp_e((opcode >> 8) & 0xf); break;
				  case 0xa1: sknp_e((opcode >> 8) & 0xf); break;
				  default: printf("UNKNOWN E"); break;
			  }
			  break;
		case 0xf:
			  switch (opcode & 0xff) {
				  case 0x07: ld_f07((opcode >> 8) & 0xf); break;
				  case 0x0a: ld_f0a((opcode >> 8) & 0xf); break;
				  case 0x15: ld_f15((opcode >> 8) & 0xf); break;
				  case 0x18: ld_f18((opcode >> 8) & 0xf); break;
				  case 0x1e: add_f((opcode >> 8) & 0xf); break;
				  case 0x29: ld_f29((opcode >> 8) & 0xf); break;
				  case 0x33: ld_f33((opcode >> 8) & 0xf); break;
				  case 0x55: ld_f55((opcode >> 8) & 0xf); break;
				  case 0x65: ld_f65((opcode >> 8) & 0xf); break;
				  default: printf("UNKNOWN F"); break;
			  }
		default: printf("UNKNOWN"); break;
	}
}

void cls_0() { gfx.draw = false; }

void ret_0() { cpu.pc = cpu.stack[--cpu.sp]; }

void jp_1(uint16_t address) { cpu.pc = address; }

void call_2(uint16_t address) { cpu.stack[cpu.sp--] = cpu.pc; cpu.pc = address; }

void se_3(uint8_t nib, uint8_t byte) { if (cpu.v[nib] == byte) { cpu.pc += 2; } }

void sne_4(uint8_t nib, uint8_t byte) { if (cpu.v[nib] != byte) { cpu.pc += 2; } }

void se_5(uint8_t nib1, uint8_t nib2) { if (cpu.v[nib1] == cpu.v[nib2]) { cpu.pc += 2; } }

void ld_6(uint8_t nib, uint8_t byte) { cpu.v[nib] = byte; }

void add_7(uint8_t nib, uint8_t byte) { cpu.v[nib] += byte; }

void ld_8(uint8_t nib1, uint8_t nib2) { cpu.v[nib1] = cpu.v[nib2]; }

void or_8(uint8_t nib1, uint8_t nib2) { cpu.v[nib1] |= cpu.v[nib2]; }

void and_8(uint8_t nib1, uint8_t nib2) { cpu.v[nib1] &= cpu.v[nib1]; }

void xor_8(uint8_t nib1, uint8_t nib2) { cpu.v[nib1] ^= cpu.v[nib2]; }

void add_8(uint8_t nib1, uint8_t nib2) { cpu.v[nib1] += cpu.v[nib2]; }

void sub_8(uint8_t nib1, uint8_t nib2)
{
	cpu.v[nib1] -= cpu.v[nib2];
	if (cpu.v[nib1] > cpu.v[nib2]) {
		cpu.v[0xf] = 1;
	} else {
		cpu.v[0xf] = 0;
	}
}

void shr_8(uint8_t nib)
{
	if ((cpu.v[nib] & 0x1) == 1) {
		cpu.v[0xf] = 1;
	} else {
		cpu.v[0xf] = 0;
	}
	cpu.v[nib] >>= 1;
}

void subn_8(uint8_t nib1, uint8_t nib2)
{
	cpu.v[nib1] = cpu.v[nib2] - cpu.v[nib1];
	if (cpu.v[nib2] > cpu.v[nib1]) {
		cpu.v[0xf] = 1;
	} else {
		cpu.v[0xf] = 0;
	}
}

void shl_8(uint8_t nib)
{
	if ((cpu.v[nib] & 0x80) == 1) {
		cpu.v[0xf] = 1;
	} else {
		cpu.v[0xf] = 0;
	}
	cpu.v[nib] = (cpu.v[nib] << 1) & 0xff;
}

void sne_9(uint8_t nib1, uint8_t nib2) { if (cpu.v[nib1] != cpu.v[nib2]) { cpu.pc += 2; }
}

void ld_a(uint16_t address) { cpu.i = address; }

void jp_b(uint16_t address) { cpu.pc = address + cpu.v[0x0]; }
 
void rnd_c(uint8_t nib, uint8_t byte) { cpu.v[nib] = (rand() % 0x100) & byte; }

void drw_d(uint8_t nib1, uint8_t nib2, uint8_t nib3)
{
	for (int j = 0; j < nib3; j++) {
	}
	gfx.draw = true;
}

void skp_e(uint8_t nib) { if (keys[cpu.v[nib]]) { cpu.pc += 2; } }

void sknp_e(uint8_t nib) { if (!keys[cpu.v[nib]]) { cpu.pc += 2; } }

void ld_f07(uint8_t nib) { cpu.v[nib] = cpu.dt; }

// incomplete
void ld_f0a(uint8_t nib)
{
	
}

void ld_f15(uint8_t nib) { cpu.dt = cpu.v[nib]; }

void ld_f18(uint8_t nib) { cpu.st = cpu.v[nib]; }

void add_f(uint8_t nib) { cpu.i += cpu.v[nib]; }

void ld_f29(uint8_t nib) { cpu.i = cpu.v[nib] * 5; }

void ld_f33(uint8_t nib)
{ 
	mem[cpu.i] = cpu.v[nib] / 100;
	mem[cpu.i + 1] = (cpu.v[nib] % 100) / 10;
	mem[cpu.i + 2] = cpu.v[nib] % 10;
}

void ld_f55(uint8_t nib) { for (int j = 0; j <= nib; j++) { mem[cpu.i + j] = cpu.v[j]; } }

void ld_f65(uint8_t nib) { for (int j = 0; j <= nib; j++) { cpu.v[j] = mem[cpu.i + j]; } }
