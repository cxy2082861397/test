#include <cstdint>
//#include <cstdio>
#include <iostream>

using namespace std;

// ��ʾָ��
#define AUIPC 0x17

#define FENCE 0x0f 
#define ECSR 0x73
//#define CSRRWI 0x1110011

// �ѷ���ָ��
#define LUI //WANGYANG
#define JAL
#define JALR
#define BEQ
#define BNE
#define BLT
#define BGE//WANGYANG
#define BLTU
#define BGEU
#define LB
#define LH
#define LW
#define LBU//WANGYANG
#define SB
#define SH
#define SW
#define ADDI 0x08 
#define SLTI
#define SLTIU//WANGYANG
#define XORI
#define ORI
#define ANDI
#define SLLI
#define SRLI
#define SRAI//WANGYANG
#define ADD
#define SUB
#define SLL
#define SLT
#define SLTU

// ����
#define LHU 0x03
#define XOR
#define SRL

// ���ɶ���
#define SRA
#define OR
#define AND

// δ�����ָ��

#define FENCE_I
#define ECALL
#define CSRRW
#define CSRRS
#define CSRRC
#define CSRRSI
#define CSRRCI



// �ڴ�ģ����
// �ڴ�ģ�����й�����
const int WORDSIZE = sizeof(uint32_t);
unsigned int MSIZE = 4096;
char* M;

// �ڴ�ģ�����йغ���
int allocMem(uint32_t s) {
		M = new char[s];
		MSIZE = s;

		return s;
}

void freeMem() {
		delete[] M;
		MSIZE = 0;
}

char readByte(unsigned int address) {
	if(address >= MSIZE) {
		cout << "ERROR: address out of range in readByte" << endl;
		return 0;
	}

	return M[address];
}

void writeByte(unsigned int address, char data) {
	if(address >= MSIZE) {
		cout << "ERROR: address out of range in writeByte" << endl;
		return;
	}

	M[address] = data;
}

uint32_t readWord(unsigned int address) {
	if(address >= MSIZE-WORDSIZE) {
		cout << "ERROR: address out of range in readWord" << endl;
		return 0;
	}

	return *((uint32_t*)&(M[address]));
}

void writeWord(unsigned int address, uint32_t data) {
	if(address >= MSIZE-WORDSIZE) {
		cout << "ERROR: address out of range in writeWord" << endl;
		return;
	}

	*((uint32_t*)&(M[address])) = data;
}

// �������ֱ��д��Ҫ���Ե�ָ��
void progMem() {
	// �ӵ�ַ0��ʼд�����ָ��
	writeWord(0, (1 << 12) | (5 << 7) | (AUIPC));
}


// CPUģ�����й�����
uint32_t PC;
uint32_t R[32];
uint32_t IR;

unsigned int opcode;
unsigned int rs1, rs2, rd;
unsigned int funct7, funct3;
// immediate values for I-type, S-type, B-type, U-type, J-type
unsigned int imm11_0i;
unsigned int imm11_5s, imm4_0s;
unsigned int imm12b, imm10_5b, imm4_1b, imm11b;
unsigned int imm31_12u;
unsigned int imm20j, imm10_1j, imm11j, imm19_12j;

// CPUģ�����йغ���
void decode(uint32_t instruction) {
	opcode = instruction & 0x7f;
	rd = (instruction & 0x0f80) >> 7;
	rs1 = (instruction & 0xf8000) >> 15;
	rs2 = (instruction & 0x1f00000) >> 20;
	funct3 = (instruction & 0x7000) >> 12;
	funct7 = instruction >> 25;
	imm11_0i = instruction >> 20;
	imm11_5s = instruction >> 25;
	imm4_0s = (instruction & 0x0f80) >> 7;
	imm12b = instruction >> 31;
	imm10_5b = (instruction >> 25) & 0x3f;
	imm4_1b = (instruction & 0x0f00) >> 8;
	imm11b = (instruction & 0x080) >> 7;
	imm31_12u = instruction >> 12;
	imm20j = instruction >> 31;
	imm10_1j = (instruction >> 21) & 0x3ff;
	imm11j = (instruction >> 20) & 1;
	imm19_12j = (instruction >> 12) & 0x0ff;
}

void showRegs() {
	cout << "PC=" << PC << endl;
	cout << "IR=" << IR << endl;

	for(int i=0; i<32; i++) {
		cout << "R[" << i << "]=" << R[i] << endl;
	}
}

int main(int argc, char const *argv[]) {
	/* code */
	allocMem(4096);
	progMem();

	PC = 0;

	char c = 'Y';

	while(c != 'n') {
		cout << "ִ�е�ǰָ��֮ǰ�Ĵ���������" << endl;
		showRegs();

		IR = readWord(PC);
		PC = PC + WORDSIZE;

		decode(IR);

		switch(opcode) {
			case AUIPC:
				cout << "Do AUIPC" << endl;
				R[rd] = PC + (imm31_12u << 12);
				break;
			case LHU:
				cout << "Do LHU" << endl;
				break;
			case FENCE:
				cout << "Do FENCE" << endl;
				break;
			case ECSR:
				cout << "Do EBREAK" << endl;
				break;
			case ADDI:
				cout<<	"Do ADDI" <<endl;
				break;
			default:
				cout << "Unkown instruction" << endl;
				break;
		}

		cout << "ִ�е�ǰָ��֮��Ĵ���������" << endl;
		showRegs();

		cout << "����ģ�⣿��Y/n)" << endl;
		cin.get(c);
	}

	freeMem();

	return 0;
}

