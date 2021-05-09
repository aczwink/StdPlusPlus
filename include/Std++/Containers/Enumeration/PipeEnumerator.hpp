/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "EnumeratorBuilder.hpp"

namespace _stdxx_
{
	template<typename FirstEnumeratorType, typename SecondEnumeratorType>
	class PipeEnumeratorBuilder : public StdXX::EnumeratorBuilder<typename FirstEnumeratorType::inputType, typename SecondEnumeratorType::outputType>
	{
	public:
		//Constructor
		inline PipeEnumeratorBuilder(const FirstEnumeratorType& lhs, const SecondEnumeratorType& rhs) : lhs(lhs), rhs(rhs)
		{
		}

		//Methods
		StdXX::Enumerator <typename SecondEnumeratorType::outputType> *CreateEnumerator(StdXX::Enumerator <typename FirstEnumeratorType::inputType> *input) const override
		{
			return this->rhs.CreateEnumerator(this->lhs.CreateEnumerator(input));
		}

	private:
		//Members
		FirstEnumeratorType lhs;
		SecondEnumeratorType rhs;
	};
}

namespace StdXX
{
	template<typename FirstEnumeratorType, typename SecondEnumeratorType>
	_stdxx_::PipeEnumeratorBuilder<FirstEnumeratorType, SecondEnumeratorType> operator>>(const FirstEnumeratorType& lhs, const SecondEnumeratorType& rhs)
	{
		return _stdxx_::PipeEnumeratorBuilder<FirstEnumeratorType, SecondEnumeratorType>(lhs, rhs);
	}
}