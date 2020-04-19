/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Main Header
#include "main.hpp"
//Local
#include <StdXX.hpp>

int main(int argc, char **argv)
{
	argc--; //program name
	FixedArray<String> args(argc);

	for(int32 i = 0; i < argc; i++)
		args[i] = String(argv[i+1]);

	return _StdPlusPlusMain(String(argv[0]), args);
}