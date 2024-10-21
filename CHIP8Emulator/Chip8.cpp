#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string.h>
#include "RandomByte.cpp"

namespace CHIP8{


	uint8_t registers[16]{};
	uint8_t memory[4096]{};
	uint16_t indexRegister{};
	uint16_t programCounter{};
	uint16_t stack[16]{};
	uint8_t stackPointer{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint8_t input[16]{};
	uint32_t display[64 * 32]{};
	uint16_t opcode;
	uint8_t fontset[80] ={
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	
	int InitDefaultData(){
		programCounter = 0x200;
		for (unsigned int i = 0; i < 80; i++)
		{
			memory[0x50 + i] = fontset[i];
		}
		return 0;
	}
	
	int LoadROM(const char* filename){ //load rom from memory and put it in the emulator memory
		FILE* rom = fopen(filename, "rb");
		fseek(rom, 0, SEEK_END);
		int size = ftell(rom);
		fseek(rom, 0, SEEK_SET);
		uint8_t buffer[size];
		fread(buffer, 1, size, rom);
		
		for (int i = 0; i < size; i++)
		{
			memory[0x200 + i] = buffer[i];
		}
		
		return 0;
	}
	
	
	//CPU
	
	//1
	int CLS(){ //set entire display to black. OPCODE: 00E0
		memset(display, 0, sizeof(display));
		return 1;
	}
	
	//2
	int RET(){ //return from subroutine in stack. OPCODE: 00EE
		stackPointer--;
		programCounter = stack[stackPointer];

		return 2;
	}
	
	//3
	int JPaddr(){ //jump to xxx adress. OPCODE: 1xxx
		uint16_t adress = opcode & 0x0FFFu;
		programCounter = adress;
		return 3;
	}
	
	//4
	int CALLaddr(){ //call subrouting at xxx, and store current spot in memory. OPCODE: 2xxx
		uint16_t adress = opcode & 0x0FFFu;
		stack[stackPointer] = programCounter;
		stackPointer++;
		programCounter = adress;
		return 4;
	}
	
	//5
	int SEVxByte(){ //skip next instruction if register[x] = byte at yy. OPCODE: 3xyy
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t byte = opcode & 0x00FFu;
		
		if (registers[Vx] == byte){
			programCounter += 2;
		}
		return 5;
	}
	
	//6
	int SNEVxByte(){ //skip next instruction if register[x] != byte at yy. OPCODE: 4xyy
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t byte = opcode & 0x00FFu;
		
		if (registers[Vx] != byte){
			programCounter += 2;
		}
		return 6;
	}
	
	//7
	int SEVxVy(){ //skip next instruction if register[x] = register[y]. OPCODE: 5xy0
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		
		if (registers[Vx] == registers[Vy]){
			programCounter += 2;
		}
		return 7;
	}
	
	//8
	int LDVxByte(){ //load byte at yy into register[x]. OPCODE: 6xyy
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t byte = opcode & 0x00FFu;
		registers[Vx] = byte;
		return 8;
	}
	
	//9
	int ADDVxByte(){ //add byte at yy to register[x]. OPCODE: 7xyy
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t byte = opcode & 0x00FFu;
		registers[Vx] += byte;
		return 9;
	}
	
	//10
	int LDVxVy(){ //load register[y] into register[x]. OPCODE: 8xy0
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		registers[Vx] = registers[Vy];
		return 10;
	}
	
	//11
	int ORVxVy(){ //set register[x] to register[x] or register[y]. OPCODE: 8xy1
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		registers[Vx] |= registers[Vy];
		return 11;
	}
	
	//12
	int ANDVxVy(){ //set register[x] to register[x] and register[y]. OPCODE: 8xy2
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		registers[Vx] &= registers[Vy];
		return 12;
	}
	
	//13
	int XORVxVy(){ //set register[x] to register[x] xor register[y]. OPCODE: 8xy3
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		registers[Vx] ^= registers[Vy];
		return 13;
	}
	
	//14
	int ADDVxVy(){ //set register[x] to register[x] + register[y], and store the carry bit in register[F]. OPCODE: 8xy4
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		uint16_t sum = registers[Vx] +  registers[Vy];
		
		if(sum > 255u){
			registers[0xF] = 1;
		}
		else{
			registers[0xF] = 0;
		}
		registers[Vx] = sum;
		
		
		return 14;
	}
	
	//15
	int SUBVxVy(){ //set register[x] to register[x] - register[y], and set register[F] to 0 if register[y] > register[X]. OPCODE 8xy5
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		
		if(registers[Vx] > registers[Vy]){
			registers[0xF] = 1;
		}
		else{
			registers[0xF] = 0;
		}
		registers[Vx] -= registers[Vy];
		
		
		return 15;
	}
	
	//16
	int SHRVx(){ //bitshift register[x] right 1 and store the least significant bit in register[F], y is unused. OPCODE: 8xy6
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		registers[0xF] = registers[Vx] & 0x1u;
		registers[Vx] >>= 1;
		return 16;
	}
	
	//17
	int SUBNVxVy(){ //set register[x] to register[y] - register[x], and set register[F] to 0 if register[x] > register[y]. OPCODE 8xy7
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		
		if(registers[Vx] < registers[Vy]){
			registers[0xF] = 1;
		}
		else{
			registers[0xF] = 0;
		}
		registers[Vx] = registers[Vy] - registers[Vx];
		
		
		return 17;
	}
	
	//18
	int SHLVx(){ // bitshift register[x] left 1 and store the most significant bit in register[F], y is unused. OPCODE: 8xyE
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		registers[0xF] = (registers[Vx] & 0x80u) >> 7u;
		registers[Vx] <<= 1;
		return 18;
	}
	
	//19
	int SNEVxVy(){ //skip next instruction if register[x] != register[y]. OPCODE: 9xy0
		uint8_t Vx = (opcode & 0x0F00u) >> 8;
		uint8_t Vy = (opcode & 0x00F0u) >> 4;
		
		if (registers[Vx] != registers[Vy]){
			programCounter += 2;
		}
		
		return 19;
	}
	
	//20
	int LDIaddr(){ //set index register as xxx. OPCODE: Axxx
		indexRegister = opcode & 0x0FFFu;
		return 20;
	}
	
	//21
	int JPV0addr(){ //jump to location xxx + register[0]. OPCODE: Bxxx
		programCounter = registers[0] + (opcode & 0x0FFFu);
		return 21;
	}
	
	//22
	int RNDVxByte(){ //set register[x] to random byte & yy. OPCODE: Cxyy
		registers[(opcode & 0x0F00) >> 8u] = randByte() & (opcode & 0x00FFu);
		return 22;
	}
	
	//23
	int DRWVxVyHeight(){ //Draw sprite at register[x], register[y] from memory index location with height z, and set register[F] to 1 if collision. OOPCODE: Dxyz
		
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		uint8_t Vy = (opcode & 0x00F0u) >> 4u;
		uint8_t height = opcode & 0x000Fu;
		
		uint8_t xPos = registers[Vx] % 64;
		uint8_t yPos = registers[Vy] % 32;
		registers[0xF] = 0;
		
		for(int row = 0; row < height; row++){
			
			uint8_t spriteRow = memory[indexRegister + row];
			for(int col = 0; col < 8; col++){
				
				uint8_t spritePixel = spriteRow & (0x80u >> col);
				uint32_t *displayPixel = &display[(yPos + row) * 64 + (xPos + col)];
				
				if(spritePixel > 0){
					
					if(*displayPixel == 0xFFFFFFFF){
						registers[0xF] = 1;
					}
					
					*displayPixel ^= 0xFFFFFFFF;
				}
			}
		}

		return 23;
	}
	
	//24
	int SKPVx(){ //Skip next instruction if key with value of register[x] is pressed. OPCODE: Ex9E
		uint8_t key = registers[(opcode & 0x0F00u) >> 8];
		if(input[key]){
			programCounter += 2;
		}
		
		return 24;
	}
	
	//25
	int SKPNVx(){ //Skip next instruction if key with value of register[x] is not pressed. OPCODE: ExA1
		uint8_t key = registers[(opcode & 0x0F00u) >> 8];
		if(!input[key]){
			programCounter += 2;
		}
		
		return 25;
	}
	
	//26
	int LDVxDT(){ //set register[x] to delay timer value. OPCODE: Fx07;
		registers[(opcode & 0x0F00u) >> 8u] = delayTimer;
		return 26;
	}
	
	//27
	int LDVxK(){ //wait for key press and store value in register[x]. OPCODE Fx0A
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		
		bool inputPressed = false;
		for(int i = 0; i < 16; i++){
			if(input[i]){
				inputPressed = true;
				registers[Vx] = i;
			}
		}
		if(!inputPressed){
			programCounter -= 2;
		}
		
		return 27;
	}
	
	//28
	int LDDTVx(){ // set delay timer to register[x]. OPCODE: Fx15
		delayTimer = registers[(opcode & 0x0F00u) >> 8u];
		
		return 28;
	}
	
	//29
	int LDSTVx(){ //set sound timer to register[x]. OPCODE Fx18
		soundTimer = registers[(opcode & 0x0F00u) >> 8u];
		
		return 29;
	}
	
	//30
	int ADDIVx(){ //set index register to index register + register[x]. OPCODE Fx1E
		indexRegister += registers[(opcode & 0x0F00u) >> 8u];
		
		return 30;
	}
	
	//31
	int LDFVx(){ //set I to font sprite at memory location register[x]. OPCODE Fx29
		uint8_t hexDigit = registers[(opcode & 0x0F00u) >> 8u];
		indexRegister = 0x50u + (5 * hexDigit);
		
		return 31;
	}
	
	//32
	int LDDVx(){ //store decimal representation of register[x] in memory at indexRegister and the next 2 bytes. OOPCODE: Fx33
		uint8_t Vx = registers[(opcode & 0x0F00u) >> 8u];
		
		memory[indexRegister + 2] = Vx % 10;
		Vx /= 10;
		memory[indexRegister + 1] = Vx % 10;
		Vx /= 10;
		memory[indexRegister] = Vx % 10;
		

		return 32;
	}
	
	//33
	int LDIVx(){ //OPCODE: Fx55
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		for (int i = 0; i <= Vx; i++)
		{
			memory[indexRegister + i] = registers[i];
		}
		
		return 33;
	}
	
	//34
	int LDVxI(){ //OPCODE: Fx65
		uint8_t Vx = (opcode & 0x0F00u) >> 8u;
		for (int i = 0; i <= Vx; i++)
		{
			registers[i] = memory[indexRegister + i];
		}
		
		return 34;
	}
	
	int Table0(){
		int returnValue = 0;
		switch(opcode & 0x000Fu){
			case 0x0:
				returnValue = CLS();
				break;
			case 0xE:
				returnValue = RET();
				break;
		}
		return returnValue;
	}
	
	int Table8(){
		int returnValue = 0;
		switch(opcode & 0x000Fu){
			case 0x0:
				returnValue = LDVxVy();
				break;
			case 0x1:
				returnValue = ORVxVy();
				break;
			case 0x2:
				returnValue = ANDVxVy();
				break;
			case 0x3:
				returnValue = XORVxVy();
				break;
			case 0x4:
				returnValue = ADDVxVy();
				break;
			case 0x5:
				returnValue = SUBVxVy();
				break;
			case 0x6:
				returnValue = SHRVx();
				break;
			case 0x7:
				returnValue = SUBNVxVy();
				break;
			case 0xE:
				returnValue = SHLVx();
				break;
		}
		return returnValue;
	}
	
	int TableE(){
		int returnValue = 0;
		switch(opcode & 0x000Fu){
			case 0xE:
				returnValue = SKPVx();
				break;
			case 0x1:
				returnValue = SKPNVx();
				break;
		}
		return returnValue;
	}
	
	int TableF(){
		int returnValue = 0;
		switch(opcode & 0x00FFu){
			case 0x07:
				returnValue = LDVxDT();
				break;
			case 0x0A:
				returnValue = LDVxK();
				break;
			case 0x15:
				returnValue = LDDTVx();
				break;
			case 0x18:
				returnValue = LDSTVx();
				break;
			case 0x1E:
				returnValue = ADDIVx();
				break;
			case 0x29:
				returnValue = LDFVx();
				break;
			case 0x33:
				returnValue = LDDVx();
				break;
			case 0x55:
				returnValue = LDIVx();
				break;
			case 0x65:
				returnValue = LDVxI();
				break;
		}
		return returnValue;
	}
	
	int CallFunc(){
		int returnValue = 0;
		
		switch ((opcode & 0xF000u) >> 12u){
			case 0x0:
				returnValue = Table0();
				break;
			case 0x1:
				returnValue = JPaddr();
				break;
			case 0x2:
				returnValue = CALLaddr();
				break;
			case 0x3:
				returnValue = SEVxByte();
				break;
			case 0x4:
				returnValue = SNEVxByte();
				break;
			case 0x5:
				returnValue = SEVxVy();
				break;
			case 0x6:
				returnValue = LDVxByte();
				break;
			case 0x7:
				returnValue = ADDVxByte();
				break;
			case 0x8:
				returnValue = Table8();
				break;
			case 0x9:
				returnValue = SNEVxVy();
				break;
			case 0xA:
				returnValue = LDIaddr();
				break;
			case 0xB:
				returnValue = JPV0addr();
				break;
			case 0xC:
				returnValue = RNDVxByte();
				break;
			case 0xD:
				returnValue = DRWVxVyHeight();
				break;
			case 0xE:
				returnValue = TableE();
				break;
			case 0xF:
				returnValue = TableF();
				break;
		}
		
		return returnValue;
	}
	
	int Cycle(){
		int returnValue = 0;
		opcode = (memory[programCounter]) << 8u | memory[programCounter + 1];
		programCounter += 2;
		returnValue = CallFunc();
		if (delayTimer > 0){delayTimer--;}
		if (soundTimer > 0){soundTimer--;}
		
		return returnValue;
	}
	//EndCPU
	
	
	int update(){
		printf("-%d-", uint8_t(Cycle()));
		return 0;
	}
}
