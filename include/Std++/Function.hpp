/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include "Debug.hpp"
#include "Memory.hpp"
#include "Utility.hpp"

namespace _stdxx_
{
    template<typename ReturnType, typename... ArgumentTypes>
    class Callable
    {
    public:
    	//Destructor
    	virtual ~Callable() = default;

		//Abstract
        virtual ReturnType Call(ArgumentTypes&&... args) = 0;
        virtual Callable* Move(void* destination) = 0;
    };

    template<typename ReturnType, typename... ArgumentTypes>
    class CallableFunction : public Callable<ReturnType, ArgumentTypes...>
    {
    public:
        //Constructor
        inline CallableFunction(ReturnType(*const pFunc)(ArgumentTypes...)) : funcPtr(pFunc)
        {
        }

        //Methods
        ReturnType Call(ArgumentTypes&&... args)
        {
            return this->funcPtr(StdXX::Forward<ArgumentTypes>(args)...);
        }

		CallableFunction* Move(void *destination) override
		{
			return pnew (destination)CallableFunction<ReturnType, ArgumentTypes...>(this->funcPtr);
		}

	private:
		//Members
		ReturnType(*const funcPtr)(ArgumentTypes...);
    };

    template<typename ReturnType, typename ClassType, typename... ArgumentTypes>
    class CallableMethod : public Callable<ReturnType, ArgumentTypes...>
    {
    public:
        //Constructor
        inline CallableMethod(ReturnType(ClassType::* const pMethod)(ArgumentTypes...), ClassType *pObject) : method(pMethod)
        {
            this->object = pObject;
        }

        //Methods
        ReturnType Call(ArgumentTypes&&... args) override
        {
            return (object->*(this->method))(args...);
        }

		CallableMethod* Move(void *destination) override
		{
			return pnew (destination)CallableMethod<ReturnType, ClassType, ArgumentTypes...>(this->method, this->object);
		}

	private:
		//Members
		ReturnType(ClassType::* const method)(ArgumentTypes...);
		ClassType *object;
    };

    template<typename LambdaType, typename ReturnType, typename... ArgumentTypes>
    class CallableLambda : public Callable<ReturnType, ArgumentTypes...>
    {
    public:
        //Constructor
        inline CallableLambda(LambdaType&& lambda) : lambda(StdXX::Move(lambda))
        {
        }

        //Methods
        ReturnType Call(ArgumentTypes&&... args) override
        {
            return this->lambda(args...);
        }

		CallableLambda* Move(void *destination) override
		{
			return pnew(destination)CallableLambda<LambdaType, ReturnType, ArgumentTypes...>(StdXX::Move(this->lambda));
		}

	private:
		//Members
		LambdaType lambda;
    };

    //The functor class (in terms of function signature)
    template<typename ReturnType, typename... ArgumentTypes>
    class FunctionBase
    {
    public:
        //Constructor
        inline FunctionBase()
        {
            this->callObj = nullptr;
        }

        //Destructor
		~FunctionBase()
		{
			this->Release();
		}

        //Inline operators
        inline ReturnType operator()(ArgumentTypes... args) const
        {
            return this->callObj->Call(StdXX::Forward<ArgumentTypes>(args)...);
        }

        //Inline
        inline bool IsBound() const
        {
            return this->callObj != nullptr;
        }

	protected:
		//Methods
		template<typename ReturnTypeInner, typename... ArgumentTypesInner>
		void Store(ReturnTypeInner(*const pFunc)(ArgumentTypesInner... args))
		{
			void *pMem;

			ASSERT(sizeof(CallableFunction<ReturnTypeInner, ArgumentTypesInner...>) <= sizeof(this->storage), "Storage too small. Report this as a StdXX bug please");

			pMem = (void *)this->storage; //we dont want to create mem on heap...
			this->callObj = new (pMem)CallableFunction<ReturnTypeInner, ArgumentTypesInner...>(pFunc);
		}

		template<typename ReturnTypeInner, typename ClassType, typename... ArgumentTypesInner>
		void Store(ReturnTypeInner(ClassType::* const pMethod)(ArgumentTypesInner...), ClassType *pObject)
		{
			void *pMem;

			ASSERT(sizeof(CallableMethod<ReturnTypeInner, ClassType, ArgumentTypesInner...>) <= sizeof(this->storage), "Storage too small. Report this as a StdXX bug please");

			pMem = (void *)this->storage; //we dont want to create mem on heap...
			this->callObj = new (pMem)CallableMethod<ReturnTypeInner, ClassType, ArgumentTypesInner...>(pMethod, pObject);
		}

		template<typename LambdaType>
		void Store(LambdaType&& lambda)
		{
			if(sizeof(CallableLambda<LambdaType, ReturnType, ArgumentTypes...>) <= sizeof(this->storage))
			{
				void* mem = (void*)this->storage; //we dont want to create mem on heap...
				this->callObj = pnew(mem) CallableLambda<LambdaType, ReturnType, ArgumentTypes...>(StdXX::Forward<LambdaType>(lambda));
			}
			else
			{
				this->callObj = new CallableLambda<LambdaType, ReturnType, ArgumentTypes...>(StdXX::Forward<LambdaType>(lambda));
			}
		}

		//Inline
		inline void CopyCallObj(const FunctionBase<ReturnType, ArgumentTypes...> &refOther)
		{
			StdXX::MemCopy(this->storage, refOther.storage, sizeof(this->storage));
			this->callObj = (Callable<ReturnType, ArgumentTypes...> *)this->storage;
		}

		inline void MoveCallObj(FunctionBase<ReturnType, ArgumentTypes...>&& other)
		{
			if(other.HasAllocatedStorage())
			{
				this->callObj = other.callObj;
				other.callObj = nullptr;
			}
			else
			{
				this->callObj = other.callObj->Move((void*)this->storage);
				other.Release(); //call destructor
			}
		}

    private:
        //Members
        Callable<ReturnType, ArgumentTypes...> *callObj;
        int64 storage[8]; //we put the implementation here for small objects, so that there is no need for heap alloc

		//Inline
		inline bool HasAllocatedStorage() const
		{
			return (void*)this->callObj != (void*)&this->storage;
		}

		inline void Release()
		{
			if(this->HasAllocatedStorage())
				delete this->callObj;
			else
				this->callObj->~Callable();
			this->callObj = nullptr;
		}
    };

    //evaluates "ret (args)" pattern to ("ret", "args")
    template<typename ReturnType, typename... ArgumentTypes>
    class ExtractedFunctionBaseClass;
    template<typename ReturnType, typename... ArgumentTypes>
    class ExtractedFunctionBaseClass<ReturnType(ArgumentTypes...)>
    {
    public:
        typedef FunctionBase<ReturnType, ArgumentTypes...> type;
    };
}

namespace StdXX
{
    template<typename FunctionType>
    class Function : public _stdxx_::ExtractedFunctionBaseClass<FunctionType>::type
    {
    public:
        //Constructors
        inline Function()
        {
        }

        inline Function(const Function<FunctionType> &refOther) //copy ctor
        {
            *this = refOther;
        }

        inline Function(Function<FunctionType>&& other) //move ctor
        {
            *this = Move(other);
        }

        template<typename ReturnType, typename... ArgumentTypes>
        Function(ReturnType(*const pFunc)(ArgumentTypes...))
        {
            this->template Store<ReturnType, ArgumentTypes...>(pFunc);
        }

        template<typename ReturnType, typename ClassType, typename... ArgumentTypes>
        Function(ReturnType(ClassType::* const pMethod)(ArgumentTypes...), ClassType *pObject)
        {
            this->template Store<ReturnType, ClassType, ArgumentTypes...>(pMethod, pObject);
        }

        template<typename LambdaType>
        Function(LambdaType &&refLambda)
        {
            this->template Store<LambdaType>((LambdaType &&)refLambda);
        }

        /*template<typename LambdaType>
        Function(LambdaType lambda)
        {
            this->Store<LambdaType>((LambdaType &&)lambda);
        }*/

        //Inline operators
        Function<FunctionType> &operator=(const Function<FunctionType> &refOther) //copy assign
        {
            this->CopyCallObj(refOther);

            return *this;
        }

        Function<FunctionType> &operator=(Function<FunctionType>&& other) //move assign
		{
			this->MoveCallObj(Move(other));

			return *this;
		}
    };
}