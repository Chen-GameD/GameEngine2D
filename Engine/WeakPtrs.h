#pragma once
#include "SmartPtrs.h"

namespace Engine
{
	template<class T> class SmartPtrs;
	template<class T>class WeakPtrs
	{
	public:
		friend class SmartPtrs<T>;
		friend class WeakPtrs<T>;

		WeakPtrs() : _ptr(nullptr), _counter(nullptr) {}

		WeakPtrs(SmartPtrs<T>& other) : _ptr(other._ptr), _counter(other._counter)
		{
			if (_counter != nullptr)
			{
				++(_counter->weakCount);
			}
		}

		WeakPtrs(const WeakPtrs& i_owner) :
			_counter(i_owner._counter),
			_ptr(i_owner._ptr)
		{
			if (_counter != nullptr)
			{
				(_counter->weakCount)++;
			}
		}

		template<class U>
		WeakPtrs(const WeakPtrs<U>& i_owner) :
			_counter(i_owner._counter),
			_ptr(i_owner._ptr)
		{
			if (_counter != nullptr)
			{
				(_counter->weakCount)++;
			}
		}

		WeakPtrs(WeakPtrs<T>& other) : _ptr(other._ptr), _counter(other._counter)
		{
			if (_counter != nullptr)
			{
				(_counter->weakCount)++;
			}
		}

		WeakPtrs<T>& operator = (const WeakPtrs<T>& other)
		{
			if (this == &other) return *this;

			release();

			_counter = other._counter;
			_ptr = other._ptr;

			if (_counter != nullptr)
			{
				(_counter->weakCount)++;
			}

			return *this;
		}

		WeakPtrs<T>& operator =(const SmartPtrs<T>& other)
		{
			release();

			_counter = other._counter;
			_ptr = other._ptr;

			if (_counter != nullptr)
			{
				(_counter->weakCount)++;
			}

			return *this;
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

		~WeakPtrs()
		{
			release();
		}

		SmartPtrs<T> lock()
		{
			return SmartPtrs<T>(*this);
		}

		bool expired()
		{
			if (_counter != nullptr && _counter->shareCount > 0)
				return false;

			return true;
		}

		void release()
		{
			if (_counter == nullptr) return;

			(_counter->weakCount)--;
			if (_counter->weakCount == 0 && _counter->shareCount == 0)
			{
				delete _counter;
				_counter = nullptr;
			}
		}

	private:
		T* _ptr;
		Counter* _counter;
	};
}