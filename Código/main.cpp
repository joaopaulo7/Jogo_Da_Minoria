/*
 * main.cpp
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,Caroline Alves
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>
#include "Perceptron.hpp"


int main()
{
    float entradas[518400];
    
    for (int i = 0; i < 518400; i++)
    {
        entradas[i] = (std::rand()%16777216)/16777216;
    }
        
	int forma[5] = {518400, 720, 512, 256, 100};
    
    std::string s[5] = {"relu", "relu", "relu", "softmax"};
	
	Perceptron p(5, forma, 0, s);
	
	//p.mostrar();
	
	p.setInputs(entradas);
	
    //p.mostrarNodes();
    
    //std::cout << "-------------------------------" << std::endl;
    
    //p.mostrar();
    
	float* v = p.ativar(0);
	
    //p.mostrarNodes();
    
	for(int i = 0; i < 100; i++)
        printf("%.9f \n", v[i]);
    int n;
    std::cin >> n;
    
    v = p.ativar(0);
	
    //p.mostrarNodes();
    
	for(int i = 0; i < 100; i++)
        printf("%.9f \n", v[i]);
}
