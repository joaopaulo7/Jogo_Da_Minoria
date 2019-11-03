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
	std::srand(time(NULL));
	float entradas[3] = {1, 1, 3};
	int forma[4] = {3, 2, 5, 1};
	
	Perceptron p(3, forma, 1);
	
	p.mostrar();
	
	p.setInputs(entradas);
	
	p.ativar();
	
	p.getOutputs();
}

