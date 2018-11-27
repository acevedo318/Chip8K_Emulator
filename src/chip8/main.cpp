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
	
	bool mustQuits = 0;
	
	//Si mi valor es diferente del inicial dado detenerse
	while(!mustQuits){
		//Leer el opcode
		uint16_t opcode = (window.mem[window.pc] << 8) | window.mem[window.pc + 1]; //Operacion binaria para leer 2 bytes
		//Si llego al final de memoria vuelvo al principio
		if(++window.pc == Memsize){
			window.pc = 0;
			mustQuits = 1;
			
		}
		
		
		uint16_t nnn = opcode & 0x0FFF;//esto es una mascara de bits
		uint8_t kk = opcode & 0xFF;
		uint8_t n = opcode & 0xF;
		uint8_t x = (opcode >> 8) | 0xF;
		//uint8_t y = (opcode >> 4) | ;
		
		printf("%x",opcode);

		
	}
	
	system("pause");
	return 0;
}
