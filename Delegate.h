// Delegate.h, 2013-05-08 20:40:00
// Delegate C++ class
// Copyright (c) 2007-2013 Julien Duminil
// This file is released under the MIT License (http://opensource.org/licenses/MIT)

#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#include "static_va.h"
#include <new> // placement new
#include <cstring> // memcmp

#ifndef NULL
	#define DELEGATE_NULL
	#define NULL 0
#endif

#ifndef static_assert
	#define DELEGATE_STATIC_ASSERT
	#define static_assert(_cond, _msg) typedef int compile_time_error[(_cond) ? 1 : -1]
#endif

namespace Delegate
{
	
	#define DELEGATE_TEMPLATE_PARAMS_FIXED(_IT, _FIRST, _NEXT) _FIRST() _IT(T_out) // T_out: return type
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Empty template class definition
	
	#define DELEGATE_CLASS_EMPTY(_VA_PARAMS, _VA_STEP)\
		STATIC_VA_TEMPLATE_TYPE_EX(class Empty, DELEGATE_TEMPLATE_PARAMS_FIXED, _VA_PARAMS, _VA_STEP)\
		{\
		public:\
			inline bool IsEmpty() const { return true; }\
			inline int Compare(const Empty &_other) const { return 0; }\
		};
	
	STATIC_VA_EXPAND(DELEGATE_CLASS_EMPTY)
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function template class definition
	
	#define DELEGATE_CLASS_FUNCTION(_VA_PARAMS, _VA_STEP)\
		STATIC_VA_TEMPLATE_TYPE_EX(class Function, DELEGATE_TEMPLATE_PARAMS_FIXED, _VA_PARAMS, _VA_STEP)\
		{\
		public:\
			typedef T_out (*FuncPtr)(STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) );\
			\
		public:\
			Function(FuncPtr _funcPtr) : m_funcPtr(_funcPtr) {}\
			\
			inline T_out Call(STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ) { return (*m_funcPtr)(STATIC_VA_GIVE_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ); }\
			inline bool IsEmpty() const { return m_funcPtr == NULL; }\
			inline int Compare(const Function &_other) const { return memcmp(&_other.m_funcPtr, &m_funcPtr, sizeof(FuncPtr) ); }\
			\
		private:\
			FuncPtr m_funcPtr;\
		};
	
	STATIC_VA_EXPAND(DELEGATE_CLASS_FUNCTION)
	
	#define DELEGATE_AUTO_FUNCTION(_VA_PARAMS, _VA_STEP)\
		template<typename T_out STATIC_VA_ASK_TEMPLATE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)> Function<T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)> Auto(T_out (*_func)(STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ) )\
		{\
			return Function<T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)>(_func);\
		};
	
	STATIC_VA_EXPAND(DELEGATE_AUTO_FUNCTION)
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Method template class definition
	
	#define DELEGATE_METHOD_TEMPLATE_PARAMS_FIXED(_IT, _FIRST, _NEXT) _FIRST() _IT(T) _NEXT() _IT(T_out) // T: class, T_out: return type
	
	#define DELEGATE_CLASS_METHOD(_VA_PARAMS, _VA_STEP)\
		STATIC_VA_TEMPLATE_TYPE_EX(class Method, DELEGATE_METHOD_TEMPLATE_PARAMS_FIXED, _VA_PARAMS, _VA_STEP)\
		{\
		public:\
			typedef T_out (T::*MethodPtr)(STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) );\
			\
		public:\
			Method(T *_object, MethodPtr _methodPtr) : m_object(_object), m_methodPtr(_methodPtr) {}\
			\
			inline T_out Call(STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE)) { return (m_object->*m_methodPtr)(STATIC_VA_GIVE_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ); }\
			inline bool IsEmpty() const { return m_object == NULL; }\
			inline int Compare(const Method &_other) const\
			{\
				int diff = memcmp(&_other.m_object, &m_object, sizeof(T *) );\
				if(diff != 0)\
					return diff;\
				else\
					return memcmp(&_other.m_methodPtr, &m_methodPtr, sizeof(MethodPtr) );\
			}\
			\
		private:\
			T *m_object;\
			MethodPtr m_methodPtr;\
		};
	
	STATIC_VA_EXPAND(DELEGATE_CLASS_METHOD)
	
	#define DELEGATE_AUTO_METHOD(_VA_PARAMS, _VA_STEP)\
		template<typename T, typename T_out STATIC_VA_ASK_TEMPLATE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)> Method<T, T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)> Auto(T *_object, T_out (T::*_method)(STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ) )\
		{\
			return Method<T, T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)>(_object, _method);\
		};
	
	STATIC_VA_EXPAND(DELEGATE_AUTO_METHOD)
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Helpers
	
	namespace Helpers
	{
		class ClassNoInheritance1 {}; // no inheritance
		class ClassNoInheritance2 {}; // no inheritance
		class ClassSingleInheritance : ClassNoInheritance1 {}; // single inheritance
		class ClassMultipleInheritance : ClassNoInheritance1, ClassNoInheritance2 {}; // multiple inheritance
		// class ClassVirtualInheritance : virtual ClassNoInheritance1 {}; // virtual inheritance
		// class ClassUnknown; // unknown class
		
		struct DelegateUsualSize
		{
			void *m_object;
			union
			{
				void *m_ptr;
				void (*m_func)();
				union MethodPtr
				{
					void (ClassNoInheritance1::*m_b)();
					void (ClassSingleInheritance::*m_s)();
					void (ClassMultipleInheritance::*m_m)();
					// The two next inheritances are not commonly used, so ignore them for the default buffer size
					// void (ClassVirtualInheritance::*m_v)();
					// void (ClassUnknown::*m_u)();
				} m_method;
			} m_code;
		};
		
		template<size_t _BufferSize>
		struct BufferInPlace
		{
			static_assert(_BufferSize >= sizeof(void *), "Buffer size must be big enough to contain a pointer !");
			char m_buffer[_BufferSize];
			
			template<bool _InPlace>
			struct InPlace;

			// in place
			template<>
			struct InPlace<true>
			{
				template<typename T>
				static inline void construct(char *_buffer, const T &_value)
				{
					new (reinterpret_cast<void *>(_buffer)) T(_value);
				}

				template<typename T>
				static inline void destruct(char *_buffer)
				{
					reinterpret_cast<T *>(_buffer)->~T();
				}

				template<typename T>
				static inline T &reference(char *_buffer)
				{
					return *reinterpret_cast<T *>(_buffer);
				}
			};

			// new/delete
			template<>
			struct InPlace<false>
			{
				template<typename T>
				static inline void construct(char *_buffer, const T &_value)
				{
					*reinterpret_cast<T **>(_buffer) = new T(_value);
				}

				template<typename T>
				static inline void destruct(char *_buffer)
				{
					delete *reinterpret_cast<T **>(_buffer);
				}

				template<typename T>
				static inline T &reference(char *_buffer)
				{
					return **reinterpret_cast<T **>(_buffer);
				}
			};

			template<typename T>
			inline void construct(const T &_value)
			{
				InPlace<(sizeof(T) <= _BufferSize)>::construct<T>(m_buffer, _value);
			}
			
			template<typename T>
			inline void destruct()
			{
				InPlace<(sizeof(T) <= _BufferSize)>::destruct<T>(m_buffer);
			}
			
			template<typename T>
			inline T &reference()
			{
				return InPlace<(sizeof(T) <= _BufferSize)>::reference<T>(m_buffer);
			}
			
			template<typename T>
			inline const T &reference() const { return const_cast<BufferInPlace*>(this)->reference<T>(); }
		};
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Callback template class definition
	
	#define DELEGATE_CLASS_CALLBACK(_VA_PARAMS, _VA_STEP)\
		STATIC_VA_TEMPLATE_TYPE_EX(class Callback, DELEGATE_TEMPLATE_PARAMS_FIXED, _VA_PARAMS, _VA_STEP)\
		{\
		protected:\
			typedef Helpers::BufferInPlace<sizeof(Helpers::DelegateUsualSize)> Buffer;\
			\
			struct Behavior\
			{\
				typedef T_out (*CallFuncPtr)(Buffer &_data STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA));\
				typedef void (*CopyFuncPtr)(Buffer &_data, const Buffer &_otherData);\
				typedef void (*DestructFuncPtr)(Buffer &_data);\
				typedef bool (*IsEmptyFuncPtr)(const Buffer &_data);\
				typedef int (*CompareFuncPtr)(const Buffer &_data, const Buffer &_otherData);\
				\
				CallFuncPtr m_call;\
				CopyFuncPtr m_copy;\
				DestructFuncPtr m_destruct;\
				IsEmptyFuncPtr m_isEmpty;\
				CompareFuncPtr m_compare;\
				\
				Behavior(CallFuncPtr _call, CopyFuncPtr _copy, DestructFuncPtr _destruct, IsEmptyFuncPtr _isEmpty, CompareFuncPtr _compare) : m_call(_call), m_copy(_copy), m_destruct(_destruct), m_isEmpty(_isEmpty), m_compare(_compare) {}\
			};\
			\
			template<typename T> static inline T_out CallStub(Buffer &_data STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA) ) { return _data.reference<T>().Call(STATIC_VA_GIVE_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ); }\
			template<typename T> static inline void CopyStub(Buffer &_data, const Buffer &_otherData) { _data.construct<T>(_otherData.reference<T>() ); }\
			template<typename T> static inline void DestructStub(Buffer &_data) { _data.destruct<T>(); }\
			template<typename T> static inline bool IsEmptyStub(const Buffer &_data) { return _data.reference<T>().IsEmpty(); }\
			template<typename T> static inline int CompareStub(const Buffer &_data, const Buffer &_otherData) { return _data.reference<T>().Compare(_otherData.reference<T>() ); }\
			\
			template<typename T> static inline const Behavior *GenericBehavior(Buffer &_data, const T &_value)\
			{\
				_data.construct<T>(_value);\
				static const Behavior behavior(&CallStub<T>, &CopyStub<T>, &DestructStub<T>, &IsEmptyStub<T>, &CompareStub<T>);\
				return &behavior;\
			}\
			template<typename T> static inline const Behavior *EmptyBehavior(Buffer &_data, const T &_value)\
			{\
				_data.construct<T>(_value);\
				static const Behavior behavior(NULL, &CopyStub<T>, &DestructStub<T>, &IsEmptyStub<T>, &CompareStub<T>);\
				return &behavior;\
			}\
			\
		public:\
			template<typename T> inline Callback(const T &_value) { m_behavior = GenericBehavior(m_data, _value); }\
			inline Callback() { m_behavior = EmptyBehavior(m_data, Empty<T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)>() ); }\
			inline Callback(const Empty<T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)> &_value) { m_behavior = EmptyBehavior(m_data, Empty<T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)>() ); }\
			inline Callback(T_out (*_func)(STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ) ) { m_behavior = GenericBehavior(m_data, Function<T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)>(_func) ); }\
			template<typename T> inline Callback(T *_object, T_out (T::*_method)(STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) ) ) { m_behavior = GenericBehavior(m_data, Method<T, T_out STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)>(_object, _method) ); }\
			inline Callback(const Callback &_other) { m_behavior = _other.m_behavior; (*m_behavior->m_copy)(m_data, _other.m_data); }\
			inline ~Callback() { (*m_behavior->m_destruct)(m_data); }\
			\
			template<typename T> inline Callback &operator =(const T &_value)\
			{\
				(*m_behavior->m_destruct)(m_data);\
				m_behavior = GenericBehavior(m_data, _value);\
				return *this;\
			}\
			inline Callback &operator =(const Callback &_other)\
			{\
				(*m_behavior->m_destruct)(m_data);\
				m_behavior = _other.m_behavior;\
				(*m_behavior->m_copy)(m_data, _other.m_data);\
				return *this;\
			}\
			\
			inline T_out operator ()(STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE)) { return (*m_behavior->m_call)(m_data STATIC_VA_GIVE_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)); }\
			inline operator int() { return !(*m_behavior->m_isEmpty)(m_data); }\
			inline bool operator !() { return (*m_behavior->m_isEmpty)(m_data); }\
			inline bool operator ==(const Callback &_other) const { return memcmp(&m_behavior, &_other.m_behavior, sizeof(Behavior *)) == 0 && (*m_behavior->m_compare)(m_data, _other.m_data) == 0; }\
			inline bool operator <(const Callback &_other) const\
			{\
				int diff = memcmp(&m_behavior, &_other.m_behavior, sizeof(Behavior *));\
				if(diff != 0)\
					return diff > 0;\
				else\
					return (*m_behavior->m_compare)(m_data, _other.m_data) > 0;\
			}\
			\
		private:\
			Buffer m_data;\
			const Behavior *m_behavior;\
		};
	
	STATIC_VA_EXPAND(DELEGATE_CLASS_CALLBACK)
	
} // namespace Delegate

#ifdef DELEGATE_NULL
	#undef NULL
#endif

#ifdef DELEGATE_STATIC_ASSERT
	#undef static_assert
#endif

#endif // __DELEGATE_H__
