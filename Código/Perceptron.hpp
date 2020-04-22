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
    std::string funcao = "relu";
    
	float **m = NULL; //Matriz dos pesos
	float *bias = NULL;//O valores bases dos nodulos sao guardados junto aos pesos pois ficilita o processo de treino.
    float *valNodulos = NULL; //Valores que deixaram cada nodulo.
};
	

class Perceptron{
	private:
	
		int numInputs; //Numero de entradas
		float *inputs; //Vetor de entrada
		
		int numCamadas; //Numero de camadas de conexoes(que e uma unidade menor que as camadas de nodulos)
		camada *camadas;//As camadas (pesos e bias da rede)
		
		
		//Procedimento estatico(nao muda entre os objetos) para organizar a criaçao dos objetos.
		
		/*Ele e usado pelo construtor para gerar as camadas de conexoes e bias.
		 *Recebe a localizaçao na memoria de um vetor de camadas(de um objeto), o tamanho desse vetor,
		 *o vetor que representa a topologia da rede e os pesos basicos das conexoes(se for zero,
		 *o peso dado sera aleatorio).*/
		static void gerarCamadas(camada *camadas, int numCamadas, int *disposicaoCamadas, float pesoBase, std::string *funcoes){
			
			int linhas, colunas; //variaveis auxiliares para organizaçao do codigo.
			
			for(int k = 0; k < numCamadas-1; k++) //laço para as N camadas da rede.
			{
				
				linhas = disposicaoCamadas[k + 1]; //[k + 1] pois as linhas representam o proximo vetor de nodulos 
				colunas = disposicaoCamadas[k];    //colunas representam o vetor de nodulos anterior
                
				camadas[k].colunas = colunas;
				camadas[k].linhas = linhas;
				
                if(funcoes != NULL)
                    camadas[k].funcao = funcoes[k];
                
				camadas[k].bias = new float[linhas]; //criaçao dos valores basicos (bias) dos vetores de nodulos.
                camadas[k].valNodulos = new float[linhas]; //criaçao dos vetores de nodulos.
				camadas[k].m = new float *[linhas];  //a liguagem exige a criaçao de uma dimensao de cada vez.
				
				for(int i = 0; i < linhas; i++) //laço para as linhas da matriz de peso
				{
                    //verar os valores dos nodulos
                    camadas[k].valNodulos[i] = 0;
                    
					//Os bias pertecem aos nodulos posteriores, entao sao criados nesse laço
					if(pesoBase > -0.000001 and pesoBase < 0.000001)
						camadas[k].bias[i] = (std::rand() % 100000)/100000.0 - 0.5;
					else
						camadas[k].bias[i] = pesoBase;
					//fim da definiçao de bias
					
					camadas[k].m[i] = new float[colunas]; //criaçao da segunda dimensao.
					
					for(int j = 0; j < colunas; j++)//Laço para as colunas da matriz de peso.
					{
                        //definiçao de pesos. Se pesoBase == 0, entao e um numero aleatorio
						if(pesoBase > -0.000001 and pesoBase < 0.000001)
							camadas[k].m[i][j] = (std::rand() % 100000)/100000.0 - 0.5;
						else
							camadas[k].m[i][j] = pesoBase;
					}
				}
			}
		}
		
        static camada copiarCamada(camada c){
            camada c1;
            c1.linhas = c.linhas;
            c1.colunas = c.colunas;
            c1.funcao = c.funcao;
            c1.bias = new float[c1.linhas];
            c1.valNodulos = new float[c1.linhas];
            c1.m = new float *[c1.linhas];
            for(int i = 0; i < c1.linhas; i++)
			{
                c1.bias[i] = c.bias[i];
                c1.valNodulos[i] = c.valNodulos[i];
                
                c1.m[i] = new float [c1.colunas];
                for(int j = 0; j < c1.colunas; j++)
                    c1.m[i][j] = c.m[i][j];
            }
            return c1;
        }
        
    public:
		//Construtor.
		
		/*Ele recebe o tamanho da rede, sua disposiçao em um vetor de inteiros,
		 *cada valor do vetor representa o numero de nodulos da camada do indice,
		 *e o peso base das conexoes.*/
		Perceptron(int numCamadas, int *disposicaoCamadas, float pesoBase, std::string *funcoes = NULL, int semente = 0){
			
            if(semente ==0)
                std::srand(time(NULL));
            else
                std::srand(semente);
            
            this->numCamadas--;
            
			//definiçao de inputs
			this->numInputs = disposicaoCamadas[0];
			this->inputs = new float[this->numInputs];
			
			//definiçao do numero de camadas
			this->numCamadas = numCamadas;
			this->camadas = new camada [numCamadas];
			
			//Chama um procedimento estatico para definir as camadas da rede. 
			Perceptron::gerarCamadas(this->camadas, numCamadas, disposicaoCamadas, pesoBase, funcoes);
		}
		
        //GETS E SETS
        void setInputs(float *inputs){
            this->inputs = inputs;
        }
        
        int getNumCamadas(){
            return this->numCamadas;
        }
        
        camada getCamada(int i){
            return this->copiarCamada(this->camadas[i]);
        }
        
        camada* getCamadas(){
            camada *rede = new camada[this->numCamadas];
            for (int i = 0; i < this->numCamadas; i++)
                rede[i] = this->copiarCamada(this->camadas[i]);
            return rede;
        }
        
		//[FIM]GETS E SETS
				
		//funçao que da o resultado da multiplicaçao de uma matriz por um vetor.
		void multiplicar(int iCamada, float *vetor, float *saida){
			
			//variaveis auxiliares
			int linhas = this->camadas[iCamada].linhas;
			int colunas = this->camadas[iCamada].colunas;
			
			float sum;
			 
			for(int i = 0; i < linhas; i++)
			{
				sum = 0;
				
				for(int j = 0; j < colunas; j++)
					sum += this->camadas[iCamada].m[i][j]*vetor[j]; //Somatorio de linhas da matriz pela coluna do vetor
					
				saida[i] = sum;
			}
		}
		
		
		float* ativar(int camadaFim = 0){
			
            if(camadaFim == 0)
                camadaFim = this->numCamadas-1;
            
            float saida[1000];
            
			//Multiplicaçao dos inputs
			this->multiplicar(0, this->inputs, this->camadas[0].valNodulos); 
			
			//Aplicaçao da funçao de ativacao. Somando os bias.
			for(int i = 0; i < this->camadas[0].linhas; i++)
				saida[i] = aplicarFuncao(this->camadas[0].valNodulos[i] + this->camadas[0].bias[i], this->camadas[0].funcao);
            
			//Ativaçao das camadas "ocultas".
			for(int i = 1; i < camadaFim; i++)
			{
				this->multiplicar(i, saida, this->camadas[i].valNodulos);
				
                if(this->camadas[i].funcao != "softmax")
                {
                    for(int j = 0; j < this->camadas[i].linhas; j++)
                        saida[j] = aplicarFuncao(this->camadas[i].valNodulos[j] + this->camadas[i].bias[j], this->camadas[i].funcao);
                }
                else
                {
                    for(int j = 0; j < this->camadas[i].linhas; j++)
                    {
                        this->camadas[i].valNodulos[j] = this->camadas[i].valNodulos[j] + this->camadas[i].bias[j];
                        saida[i] = this->camadas[i].valNodulos[j];
                    }
                    softmax(this->camadas[i].linhas, saida);
                }
            }
            float *saida0 = new float[this->camadas[camadaFim - 1].linhas];
            for(int i = 0; i < this->camadas[camadaFim-1].linhas; i++)
                saida0[i] = saida[i];
            return saida0;
		}
		
		//FUNCOES DE ATIVAÇAO
        
        float aplicarFuncao(float x , std::string funcao){
            if(funcao == "relu")
                return relu(x);
            else if(funcao == "lRelu" or funcao == "lrelu" or funcao == "leakyRelu")
                return lRelu(x);
            else if(funcao == "sigmoid" or funcao == "Sigmoid" or funcao == "sig")
                return sigmoid(x);
            else if(funcao == "tanH" or funcao == "tanh" or funcao == "tanhx")
                return tanh(x);
            else 
                throw std::invalid_argument("Funcao de ativacao invalida: " + funcao);
        }
        
        static const float sigmoid(int x){
            return 1/(1 + exp(x));
        }
        
		static const float relu(float x){
			if(x >= 0)
				return x;
			else
				return 0;
		}
		
		static const float lRelu(float x){
			if(x >= 0)
				return x;
			else
				return 0.001*x;
		}
        
        void softmax(int tam, float *v){
            float sum = 0.0;
            float max = v[0];
            for(int i = 0; i < tam; i++)
                max = std::max(max, v[i]);
            for(int i = 0; i < tam; i++)
                sum += exp(v[i] - max);
            for(int i = 0; i < tam; i++)
                v[i] = exp(v[i] - max)/sum;
        }
        
        float derivEntropySoftmax(float x, float y){
            return x - y;
        }
		//[FIM]FUNCOES DE ATIVAÇAO
		
        //APRENDIZADO
        void aprender(float *correto, int camadaAprnd = 0){
            if(camadaAprnd)
            {
                
            }
        }
        
		
		//FUNÇOES DE DEBUG
		
		//Mostra os pesos de cada camada.
		void mostrar(){
			
			for(int k = 0; k < this->numCamadas; k++)
			{
                for(int i = 0; i < this->camadas[k].linhas; i++)
                    std::cout <<" "<<this->camadas[k].bias[i] <<"/";
                std::cout << std::endl;
				for(int i = 0; i < this->camadas[k].linhas; i++)
				{
					for(int j = 0; j< this->camadas[k].colunas; j++)
						std::cout<< this->camadas[k].m[i][j] << "|";
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
        
        void mostrarNodes(){
            for(int k = 0; k < this->numCamadas; k++)
			{
                for(int i = 0; i < this->camadas[k].linhas; i++)
                {
                    std::cout << this->camadas[k].valNodulos[i] << "/ ";
                }
                std::cout << std::endl << std::endl;
            }
        }
		//[FIM]FUNÇOES DE DEBUG
};

