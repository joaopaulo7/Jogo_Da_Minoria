/*
 * Perceptron.hpp
 * 
 * Copyright 2019 João Paulo Paiva Lima <joao.lima1@estudante.ufla.br>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include <ctime>
#include <cmath>

struct matriz{
	
	int linhas;
	int colunas;
		
	float **m;
	float *bias;
};
	

class Perceptron{
	public:
	
		int numInputs;
		float *inputs;
		
		int numOutputs;
		float *outputs;
		
		int numCamadas;
		matriz *camadas;
		
		static void gerarCamadas(matriz *camadas, int numCamadas, int *disposicaoCamadas, float pesoBase){
			int linhas, colunas;
			
			for(int k = 0; k < numCamadas; k++)
			{
				
				linhas = disposicaoCamadas[k + 1];
				colunas = disposicaoCamadas[k];
				
				camadas[k].colunas = colunas;
				camadas[k].linhas = linhas;
				
				camadas[k].bias = new float[linhas];
				camadas[k].m = new float *[linhas];
				for(int i = 0; i < linhas; i++)
				{
					
					if(pesoBase > -0.000001 and pesoBase < 0.000001)
						camadas[k].bias[i] = (std::rand() % 1000000)/1000000.0;
					else
						camadas[k].bias[i] = pesoBase;
					
					camadas[k].m[i] = new float[colunas];
					for(int j = 0; j < colunas; j++)
					{
						if(pesoBase > -0.000001 and pesoBase < 0.000001)
						{
							camadas[k].m[i][j] = (std::rand() % 1000000)/1000000.0;
						}
						else
							camadas[k].m[i][j] = pesoBase;
					}
				}
			}
		}
		
		Perceptron(int numCamadas, int *disposicaoCamadas, float pesoBase){
			
			this->numInputs = disposicaoCamadas[0];
			this->inputs = new float[this->numInputs];
			
			this->numOutputs = disposicaoCamadas[numCamadas];
			
			this->numCamadas = numCamadas;
			this->camadas = new matriz[numCamadas];
			Perceptron::gerarCamadas(this->camadas, numCamadas, disposicaoCamadas,pesoBase);
				
		}
		
		float* multiplicar(int iCamada, float *vetor){
			
			int linhas = this->camadas[iCamada].linhas;
			int colunas = this->camadas[iCamada].colunas;
			
			float sum = 0;
			float *saida;
			saida = new float[linhas];
			 
			for(int i = 0; i < linhas; i++)
			{
				for(int j = 0; j < colunas; j++)
					sum += this->camadas[iCamada].m[i][j]*vetor[j];
				saida[i] = sum;
			}
			return saida;
		}
		
		void setInputs (float *inputs){
			this->inputs = inputs;
		}
		
		void getOutputs(){
			for(int i = 0; i < this->numOutputs; i++)
				std::cout << this->outputs[i];
		}
		
		void ativar(){
			float *saida;
			saida = this->multiplicar(0, this->inputs);
			
			for(int i = 0; i < this->camadas[0].linhas; i++)
				saida[i] = tanh(saida[i] + this->camadas[0].bias[i]);
			
			for(int i = 1; i < this->numCamadas -1; i++)
			{
				saida = this->multiplicar(i, saida);
				
				for(int j = 0; j < this->camadas[i].linhas; j++)
					saida[j] = tanh(saida[j] + this->camadas[i].bias[j]);
			}
			
			saida = this->multiplicar(this->numCamadas, saida);
			for(int i = 0; i < this->numOutputs; i++)
					saida[i] = saida[i] + this->camadas[this->numCamadas - 1].bias[i];
					
			this->outputs = saida;
		}
		
		void mostrar(){
			
			for(int k = 0; k < this->numCamadas; k++)
			{
				for(int i = 0; i< this->camadas[k].linhas; i++)
				{
					for(int j = 0; j< this->camadas[k].colunas; j++)
						std::cout<< this->camadas[k].m[i][j] << "|";
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
};

