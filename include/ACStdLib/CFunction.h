/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
//Local
#include "Debug.h"
#include "Memory.h"

namespace _Intern
{
    template<typename ReturnType, typename... ArgumentTypes>
    class ICallable
    {
    public:
        //Abstract
        virtual ReturnType Call(ArgumentTypes&&... args) = 0;
    };

    template<typename ReturnType, typename... ArgumentTypes>
    class CCallableFunction : public ICallable<ReturnType, ArgumentTypes...>
    {
    private:
        //Members
        ReturnType(*const pFunc)(ArgumentTypes...);

    public:
        //Constructor
        inline CCallableFunction(ReturnType(*const pFunc)(ArgumentTypes...)) : pFunc(pFunc)
        {
        }

        //Methods
        ReturnType Call(ArgumentTypes&&... args)
        {
            return this->pFunc(args...);
        }
    };

    template<typename ReturnType, typename ClassType, typename... ArgumentTypes>
    class CCallableMethod : public ICallable<ReturnType, ArgumentTypes...>
    {
    private:
        //Members
        ReturnType(ClassType::* const pMethod)(ArgumentTypes...);
        ClassType *pObject;

    public:
        //Constructor
        inline CCallableMethod(ReturnType(ClassType::* const pMethod)(ArgumentTypes...), ClassType *pObject) : pMethod(pMethod)
        {
            this->pObject = pObject;
        }

        //Methods
        ReturnType Call(ArgumentTypes&&... args)
        {
            return (pObject->*(this->pMethod))(args...);
        }
    };

    template<typename LambdaType, typename ReturnType, typename... ArgumentTypes>
    class CCallableLambda : public ICallable<ReturnType, ArgumentTypes...>
    {
    private:
        //Members
        LambdaType lambda;

    public:
        //Constructor
        inline CCallableLambda(LambdaType &&refLambda) : lambda(refLambda)
        {
        }

        //Methods
        ReturnType Call(ArgumentTypes&&... args)
        {
            return this->lambda(args...);
        }
    };

    //The functor class (in terms of function signature)
    template<typename ReturnType, typename... ArgumentTypes>
    class AFunctionClass
    {
    private:
        //Members
        ICallable<ReturnType, ArgumentTypes...> *pCallObj;
        int64 storage[8]; //we put the implementation here for small objects, so that there is no need for heap alloc

    protected:
        //Methods
        template<typename ReturnTypeInner, typename... ArgumentTypesInner>
        void Store(ReturnTypeInner(*const pFunc)(ArgumentTypesInner... args))
        {
            void *pMem;

            ASSERT(sizeof(CCallableFunction<ReturnTypeInner, ArgumentTypesInner...>) <= sizeof(this->storage));

            pMem = (void *)this->storage; //we dont want to create mem on heap...
            this->pCallObj = new (pMem)CCallableFunction<ReturnTypeInner, ArgumentTypesInner...>(pFunc);
        }

        template<typename ReturnTypeInner, typename ClassType, typename... ArgumentTypesInner>
        void Store(ReturnTypeInner(ClassType::* const pMethod)(ArgumentTypesInner...), ClassType *pObject)
        {
            void *pMem;

            ASSERT(sizeof(CCallableMethod<ReturnTypeInner, ClassType, ArgumentTypesInner...>) <= sizeof(this->storage));

            pMem = (void *)this->storage; //we dont want to create mem on heap...
            this->pCallObj = new (pMem)CCallableMethod<ReturnTypeInner, ClassType, ArgumentTypesInner...>(pMethod, pObject);
        }

        template<typename LambdaType>
        void Store(LambdaType &&refLambda)
        {
            void *pMem;

            ASSERT(sizeof(CCallableLambda<LambdaType, ReturnType, ArgumentTypes...>) <= sizeof(this->storage));

            pMem = (void *)this->storage; //we dont want to create mem on heap...
            this->pCallObj = pnew(pMem) CCallableLambda<LambdaType, ReturnType, ArgumentTypes...>((LambdaType &&)refLambda);
        }

        //Inline
        inline void CopyCallObj(const AFunctionClass<ReturnType, ArgumentTypes...> &refOther)
        {
            ACStdLib::MemCopy(this->storage, refOther.storage, sizeof(this->storage));
            this->pCallObj = (ICallable<ReturnType, ArgumentTypes...> *)this->storage;
        }

    public:
        //Constructor
        inline AFunctionClass()
        {
            this->pCallObj = nullptr;
        }

        //Inline operators
        inline ReturnType operator()(ArgumentTypes&&... args) const
        {
            return this->pCallObj->Call((ArgumentTypes&&)args...);
        }

        inline explicit operator bool() const
        {
            return this->pCallObj != nullptr;
        }
    };

    //evaluates "ret (args)" pattern to ("ret", "args")
    template<typename ReturnType, typename... ArgumentTypes>
    class ExtractedFunctionBaseClass;
    template<typename ReturnType, typename... ArgumentTypes>
    class ExtractedFunctionBaseClass<ReturnType(ArgumentTypes...)>
    {
    public:
        typedef AFunctionClass<ReturnType, ArgumentTypes...> type;
    };
}

namespace ACStdLib
{
    template<typename FunctionType>
    class CFunction : public _Intern::ExtractedFunctionBaseClass<FunctionType>::type
    {
    public:
        //Constructors
        inline CFunction()
        {
        }

        inline CFunction(const CFunction<FunctionType> &refOther) //copy ctor
        {
            *this = refOther;
        }

        inline CFunction(CFunction<FunctionType> &&refOther) //move ctor
        {
            *this = refOther; //do a copy here
        }

        template<typename ReturnType, typename... ArgumentTypes>
        CFunction(ReturnType(*const pFunc)(ArgumentTypes...))
        {
            this->template Store<ReturnType, ArgumentTypes...>(pFunc);
        }

        template<typename ReturnType, typename ClassType, typename... ArgumentTypes>
        CFunction(ReturnType(ClassType::* const pMethod)(ArgumentTypes...), ClassType *pObject)
        {
            this->template Store<ReturnType, ClassType, ArgumentTypes...>(pMethod, pObject);
        }

        template<typename LambdaType>
        CFunction(LambdaType &&refLambda)
        {
            this->template Store<LambdaType>((LambdaType &&)refLambda);
        }

        /*template<typename LambdaType>
        CFunction(LambdaType lambda)
        {
            this->Store<LambdaType>((LambdaType &&)lambda);
        }*/

        //Inline operators
        CFunction<FunctionType> &operator=(const CFunction<FunctionType> &refOther) //copy assign
        {
            this->CopyCallObj(refOther);

            return *this;
        }
    };
}