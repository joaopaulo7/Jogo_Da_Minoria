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


//struct para organizar as informaçoes das camadas. A matriz m sao os pesos de cada conexao.
struct camada{
	
	int linhas;
	int colunas;
		
	float **m;
	float *bias;//O valores bases dos nodulos sao guardados junto aos pesos pois ficilita o processo de treino.
};
	

class Perceptron{
	public:
	
		int numInputs; //Numero de entradas
		float *inputs; //Vetor de entrada
		
		int numOutputs;//Numero de saidas
		float *outputs;//Vetor de saidas
		
		int numCamadas; //Numero de camadas de conexoes(que e uma unidade menor que as camadas de nodulos)
		camada *camadas;//As camadas (pesos e bias da rede)
		
		
		//Procedimento estatico(nao muda entre os objetos) para organizar a criaçao dos objetos.
		
		/*Ele e usado pelo construtor para gerar as camadas de conexoes e bias.
		 *Recebe a localizaçao na memoria de um vetor de camadas(de um objeto), o tamanho desse vetor,
		 *o vetor que representa a topologia da rede e os pesos basicos das conexoes(se for zero,
		 *o peso dado sera aleatorio).*/
		static void gerarCamadas(camada *camadas, int numCamadas, int *disposicaoCamadas, float pesoBase){
			
			int linhas, colunas; //variaveis auxiliares para organizaçao do codigo.
			
			for(int k = 0; k < numCamadas; k++) //laço para as N camadas da rede.
			{
				
				linhas = disposicaoCamadas[k + 1]; //[k + 1] pois as linhas representam o proximo vetor de nodulos 
				colunas = disposicaoCamadas[k];    //colunas representam o vetor de nodulos anterior
				
				camadas[k].colunas = colunas;
				camadas[k].linhas = linhas;
				
				camadas[k].bias = new float[linhas]; //criaçao dos valores basicos (bias) dos vetores de nodulos.
				camadas[k].m = new float *[linhas];  //a liguagem exige a criaçao de uma dimensao de cada vez.
				
				for(int i = 0; i < linhas; i++) //laço para as linhas da matriz de peso
				{
					
					//Os bias pertecem aos nodulos posteriores, entao sao criados nesse laço
					if(pesoBase > -0.000001 and pesoBase < 0.000001)
						camadas[k].bias[i] = (std::rand() % 1000000)/1000000.0;
					else
						camadas[k].bias[i] = pesoBase;
					//fim da definiçao de bias
					
					camadas[k].m[i] = new float[colunas]; //criaçao da segunda dimensao.
					
					for(int j = 0; j < colunas; j++)//Laço para as colunas da matriz de peso.
					{
						//definiçao de pesos. Se < 10, entao e um numero aleatorio
						if(pesoBase > -0.000001 and pesoBase < 0.000001)
							camadas[k].m[i][j] = (std::rand() % 1000000)/1000000.0;
						else
							camadas[k].m[i][j] = pesoBase;
					}
				}
			}
		}
		
		//Construtor.
		
		/*Ele recebe o tamanho da rede, sua disposiçao em um vetor de inteiros,
		 *cada valor do vetor representa o numero de nodulos da camada do indice,
		 *e o peso base das conexoes.*/
		Perceptron(int numCamadas, int *disposicaoCamadas, float pesoBase){
			
			//definiçao de inputs
			this->numInputs = disposicaoCamadas[0];
			this->inputs = new float[this->numInputs];
			
			//definiçao do numero de outputs
			this->numOutputs = disposicaoCamadas[numCamadas];
			
			//definiçao do numero de camadas
			this->numCamadas = numCamadas;
			this->camadas = new camada[numCamadas];
			
			//Chama um procedimento estatico para definir as camadas da rede. 
			Perceptron::gerarCamadas(this->camadas, numCamadas, disposicaoCamadas,pesoBase);
				
		}
		
		//GETS E SETS
		void setInputs (float *inputs){
			this->inputs = inputs;
		}
		
		void getOutputs(){
			for(int i = 0; i < this->numOutputs; i++)
				std::cout << this->outputs[i];
		}
		//[FIM]GETS E SETS
				
		//funçao que da o resultado da multiplicaçao de uma matriz por um vetor.
		float* multiplicar(int iCamada, float *vetor){
			
			//variaveis auxiliares
			int linhas = this->camadas[iCamada].linhas;
			int colunas = this->camadas[iCamada].colunas;
			
			float sum;
			
			float *saida; //Vetor resultante da multiplicaçao
			saida = new float[linhas];
			 
			for(int i = 0; i < linhas; i++)
			{
				sum = 0;
				
				for(int j = 0; j < colunas; j++)
					sum += this->camadas[iCamada].m[i][j]*vetor[j]; //Somatorio de linhas da matriz pela coluna do vetor
					
				saida[i] = sum;
			}
			
			return saida; //Retorna a posiçao na memoria do vetor resultante.
		}
		
		
		void ativar(){
			
			float *saida;
			//Multiplicaçao dos inputs
			saida = this->multiplicar(0, this->inputs); 
			
			//Aplicaçao da funçao de ativacao. Somando os bias.
			for(int i = 0; i < this->camadas[0].linhas; i++)
				saida[i] = reLU(saida[i] + this->camadas[0].bias[i]);
			
			//Ativaçao das camadas "ocultas".
			for(int i = 1; i < this->numCamadas -1; i++)
			{
				saida = this->multiplicar(i, saida);
				
				for(int j = 0; j < this->camadas[i].linhas; j++)
					saida[j] = reLU(saida[j] + this->camadas[i].bias[j]);
			}
			
			//Multiplicaçao dos outputs.
			saida = this->multiplicar(this->numCamadas-1, saida);
			
			//Ativaçao dos outputs. (falta a implementaçao de uma funçao de saida, provavelmente softmax)
			for(int i = 0; i < this->numOutputs; i++)
			{
				saida[i] = saida[i] + this->camadas[this->numCamadas - 1].bias[i];
			}
					
			this->outputs = saida;
		}
		
		//FUNCOES DE ATIVAÇAO
		float reLU(float x){
			if(x >= 0)
				return x;
			else
				return 0;
		}
		
		float lReLU(float x){
			if(x >= 0)
				return x;
			else
				return 0.001*x;
		}
		//[FIM]FUNCOES DE ATIVAÇAO
		
		
		//FUNÇOES DE DEBUG
		
		//Mostra os pesos de cada camada.
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
		//[FIM]FUNÇOES DE DEBUG
};

