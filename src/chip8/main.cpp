#include <iostream>
#include <stdio.h>
#include <stdint.h>


using namespace std;

#define Memsize 4096 //Es la cantidad de memoria de la maquina

struct machine_s{
	
	uint8_t mem[Memsize]; //Memoria de la maquina
	uint16_t pc; //The program counter (PC) should be 16-bit, and is used to store the currently executing address.
	
	uint8_t r[16]; //16 registros de proposito general
	//registro especial de direccion I
	uint16_t i; //There is also a 16-bit register called I. This register is generally used to store memory addresses
	//temporizadores
	uint8_t delay,soundTimer; //two special purpose 8-bit registers, for the delay and sound timers.

	//*puntero de la pila
	uint16_t sp; //The stack pointer (SP) can be 8-bit, it is used to point to the topmost level of the stack.
	//*Pila de registro de 16 bits
	uint16_t stack[16]; //The stack is an array of 16 16-bit values, used to store the address that the interpreter shoud return to when finished with a subroutine.
	
};


void load_rom(struct machine_s* machine){
	
	FILE* fp = fopen("PONG","r");
	
	
	if(fp == NULL){
		perror("Error open file");
		exit(1);
	}
	
		
	//Obtengo el tamaño del archivo
	fseek(fp, 0, SEEK_END);//Me ubico al final del archivo
	int longitd = ftell(fp);
	fseek(fp, 0 , SEEK_SET);
	
	//0x200 posicion en memoria donde se inicia a cargar las roms
	fread(machine->mem + 0x200, longitd, 1, fp); //0x200 = 512
		
		//Leer el opcode
	
	
	fclose(fp);
}

// Vuelve todos los valores a default
void init_machine(struct machine_s* machine)
{
	machine->sp = machine->i = machine->delay = machine->soundTimer = 0x00;
	machine->pc = 0x200;//direccion memoria
	
	for(int i=0; i< Memsize; i++){
		machine->mem[i] = 0x00;
	}
	for(int i=0; i< 16; i++){
		machine->stack[i] = 0x00;
		machine->r[i] = 0;
	}
	
}

int main(int argc, char** argv) {
	
	struct machine_s window;
	init_machine(&window);
	load_rom(&window);
	
	int mustQuits = 0;
	
	//Si mi valor es diferente del inicial dado detenerse
	while(!mustQuits){
		//Leer el opcode
		uint16_t opcode = (window.mem[window.pc] << 8) | window.mem[window.pc + 1]; //Operacion binaria para leer 2 bytes
		//Si llego al final de memoria vuelvo al principio
		window.pc += 2;
		if(window.pc == Memsize){
			window.pc = 0;
		}
		
		///nnn son los 12 bits menos significativos
		uint16_t nnn = opcode & 0x0FFF;//esto es una mascara de bits
		//kk son los 8 bits menos sifnificativos
				uint8_t kk = opcode & 0xFF;
		//4 bits menos significativos
			uint8_t n = opcode & 0xF;
		//4 bits menos significativos en el primer byte
		uint8_t x = (opcode >> 8) & 0xF;
		//4 bits menos significativos del segundo byte
			uint8_t y = (opcode >> 4) & 0xF;
		
			uint8_t p  = (opcode >> 12);
		
		switch(p){
			case 0:
				if(opcode == 0x00E0){
					//Limpiar pantalla
					cout<<"CLEAR"<<endl;
				}else if (n == 0x00EE){
					//Todo RET --> Vuelve a subrutina
					cout<<"RET"<<endl;
				}
				break;
			case 1:
				//JP
				printf("JP %x\n", nnn);
				break;
			case 2:
				//CALL
				printf("CALL %x\n", nnn);
				break;
			case 3:
				//SE
				printf("SE %x, %x\n", x,kk);
				break;
			case 4:
				//SNE
				printf("SNE %x, %x\n", x,kk);
				break;
			case 5:
				//
				printf("SE %x, %x\n", x, kk);
				break;
			case 6:
				//
				printf("LD %x, %x\n", x, kk);
				break;
			case 7:
				//
				printf("ADD %x, %x\n", x, kk);
				break;
			case 8:
				switch (n) {
					case 0:
						printf("LD %x, %x\n", x, y);
						break;
					case 1:
						printf("OR %x, %x\n", x, y);
						break;
					case 2:
						printf("AND %x, %x\n", x, y);
						break;
					case 3:
						printf("XOR %x, %x\n", x, y);
						break;
					case 4:
						printf("ADD %x, %x\n", x, y);
						break;
					case 5:
						printf("SUB %x, %x\n", x, y);
						break;
					case 6:
						printf("SHR %x\n", x);
						break;
					case 7:
						printf("SUBN %x, %x\n", x, y);
						break;
					case 0xE:
						printf("SHL %x\n", x);
						break;
				}
				break;
			case 9:
				//
				printf("SNE %x, %x\n", x, y);
				break;
			case 0xA:
				printf("LD I, %x\n", nnn);
				break;
			case 0xB:
				printf("JP V0, %x\n", nnn);
				break;
			case 0xC:
				printf("RND %x, %x\n", x,kk);
				break;
			case 0xD:
					printf("DRW %x,%x,%x\n", x,y,n);
				break;
			case 0xE:
				if(kk == 0x9E){
					printf("SKP %x\n", x);
				}else if(kk == 0xA1){
					printf("SKNP %x\n", x);
				}
				
				break;
			case 0xF:
				
				switch(kk) {
					case 0x07:
						printf("LD %x, DT\n", x);
					break;
					case 0x0A:
						printf("LD %x, K\n", x);
					break;
					case 0x15:
						printf("LD DT, %x\n", x);
					break;
					case 0x18:
						printf("LD ST, %x K\n", x);
					break;
					case 0x1E:
						printf("ADD I, %x\n", x);
					break;
					case 0x29:
						printf("LD F, %x\n", x);
					break;
					case 0x33:
						printf("LD B, %x\n", x);
					break;
					case 0x55:
						printf("LD [I], %x\n", x);
					break;
					case 0x65:
						printf("LD %x, [I]\n", x);
					break;
				}
				break;
		}
		
		
		//printf("%x",opcode);

		
	}
	
	system("pause");
	return 0;
}
