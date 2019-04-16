/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Streams/StdIn.hpp>
//Global
#include <sys/termios.h>
#include <stdio.h>
//Local
#include <Std++/Streams/Readers/TextReader.hpp>
//Namespaces
using namespace StdXX;

String StdIn::ReadUnechoedLine()
{
	struct termios oflags, nflags;

	//disable echo
	tcgetattr(fileno(stdin), &oflags);
	nflags = oflags;
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag |= ECHONL;

	tcsetattr(fileno(stdin), TCSANOW, &nflags);

	TextReader textReader(*this, TextCodecType::UTF8);
	String secret = textReader.ReadLine();

	//restore original mode
	tcsetattr(fileno(stdin), TCSANOW, &oflags);

	return secret;
}