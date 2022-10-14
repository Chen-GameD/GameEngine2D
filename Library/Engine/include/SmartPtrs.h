#pragma once
#include "Counter.h"
#include "WeakPtrs.h"
#include "assert.h"

namespace Engine
{
	template<class T> class WeakPtrs;
	template<class T> class SmartPtrs
	{
	public:
		T* _ptr = nullptr;
		Counter* _counter = nullptr;

		friend class WeakPtrs<T>;

		//Constructor
		SmartPtrs():_ptr(nullptr), _counter(nullptr){}

		explicit SmartPtrs(T* ptr) :_ptr(ptr), _counter(nullptr)
		{
			if (_ptr)
			{
				_counter = new Counter;
				_counter->shareCount = 1;
			}
		}

		SmartPtrs(const WeakPtrs<T>& other) :_ptr(other._ptr), _counter(other._counter)
		{
			if (other._counter && other._counter->shareCount > 0)
			{
				_counter = other._counter;
				_ptr = other._ptr;
			}
			else
			{
				_counter = nullptr;
				_ptr = nullptr;
			}

			if (_counter)
			{
				(_counter->shareCount)++;
			}
		}

		SmartPtrs(const SmartPtrs<T>& other): _ptr(other._ptr), _counter(other._counter)
		{
			if (_counter != nullptr)
			{
				(_counter->shareCount)++;
			}
		}

		//Override operator
		SmartPtrs<T>& operator = (const SmartPtrs<T>& other)
		{
			if (this == &other) return *this;

			release();
			_counter = other._counter;
			_ptr = other._ptr;

			if (_counter != nullptr)
			{
				(_counter->shareCount)++;
			}

			return *this;
		}

		// Assignment Operator = null specific
		SmartPtrs& operator=(std::nullptr_t i_null)
		{
			release();

			_counter = nullptr;
			_ptr = nullptr;

			return *this;
		}

		T& operator*()
		{
			return *(this->_ptr);
		}

		T* operator->()
		{
			return this->_ptr;
		}

		const T* operator->() const
		{
			return _ptr;
		}

		template<class U>
		inline bool operator==(const SmartPtrs<U>& i_other)
		{
			return _ptr == i_other._ptr;
		}

		template<class U>
		inline bool operator==(const WeakPtrs<U>& i_other)
		{
			return _ptr == i_other._ptr;
		}

		template<class U>
		inline bool operator!=(const SmartPtrs<U>& i_other)
		{
			return _ptr != i_other._ptr;
		}

		template<class U>
		inline bool operator!=(const WeakPtrs<U>& i_other)
		{
			return _ptr != i_other._ptr;
		}

		operator bool()
		{
			return _ptr != nullptr;
		}

		void Reset(T* pOther = nullptr)
		{
			release();

			_ptr = pOther;
			if (_ptr != nullptr)
			{
				_counter = new Counter();
				_counter->shareCount = 1;
			}
		}

		T* get()
		{
			return _ptr;
		}

		size_t use_count()
		{
			return (_counter != nullptr) ? _counter->shareCount : 0;
		}

		//Destructor
		~SmartPtrs()
		{
			release();
		}

		void release()
		{
			if (_counter == nullptr) return;

			(_counter->shareCount)--;

			if (_counter->shareCount == 0)
			{
				delete _ptr;
				_ptr = nullptr;

				if (_counter->weakCount == 0)
				{
					delete _counter;
					_counter = nullptr;
				}
			}
		}
	};
}