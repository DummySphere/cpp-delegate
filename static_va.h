// static_va.h, 2013-02-10 18:30:00
// STATIC_VA - C++ macro utilities to statically expand a code with variable argument count
// Copyright (c) 2012-2013 Julien Duminil
// This file is released under the MIT License (http://opensource.org/licenses/MIT)

#ifndef __STATIC_VA_H__
#define __STATIC_VA_H__

// VA Expand

#define STATIC_VA_PARAMS_0(_IT, _FIRST, _NEXT)
#define STATIC_VA_PARAMS_1(_IT, _FIRST, _NEXT) _FIRST() _IT(T_va1)
#define STATIC_VA_PARAMS_2(_IT, _FIRST, _NEXT) STATIC_VA_PARAMS_1(_IT, _FIRST, _NEXT) _NEXT() _IT(T_va2)
#define STATIC_VA_PARAMS_3(_IT, _FIRST, _NEXT) STATIC_VA_PARAMS_2(_IT, _FIRST, _NEXT) _NEXT() _IT(T_va3)
#define STATIC_VA_PARAMS_4(_IT, _FIRST, _NEXT) STATIC_VA_PARAMS_3(_IT, _FIRST, _NEXT) _NEXT() _IT(T_va4)
#define STATIC_VA_PARAMS_5(_IT, _FIRST, _NEXT) STATIC_VA_PARAMS_4(_IT, _FIRST, _NEXT) _NEXT() _IT(T_va5)
#define STATIC_VA_PARAMS_6(_IT, _FIRST, _NEXT) STATIC_VA_PARAMS_5(_IT, _FIRST, _NEXT) _NEXT() _IT(T_va6)
#define STATIC_VA_PARAMS_7(_IT, _FIRST, _NEXT) STATIC_VA_PARAMS_6(_IT, _FIRST, _NEXT) _NEXT() _IT(T_va7)

#define STATIC_VA_EXPAND(_CODE)\
	_CODE(STATIC_VA_PARAMS_7, STATIC_VA_MAIN)\
	_CODE(STATIC_VA_PARAMS_6, STATIC_VA_SUB)\
	_CODE(STATIC_VA_PARAMS_5, STATIC_VA_SUB)\
	_CODE(STATIC_VA_PARAMS_4, STATIC_VA_SUB)\
	_CODE(STATIC_VA_PARAMS_3, STATIC_VA_SUB)\
	_CODE(STATIC_VA_PARAMS_2, STATIC_VA_SUB)\
	_CODE(STATIC_VA_PARAMS_1, STATIC_VA_SUB)\
	_CODE(STATIC_VA_PARAMS_0, STATIC_VA_SUB)

// VA Tools

#define STATIC_VA_SEPARATOR_NONE()
#define STATIC_VA_SEPARATOR_COMMA() ,
#define STATIC_VA_SEPARATOR_COLON() :

#define STATIC_VA_EMPTY_IT(_PARAM)
#define STATIC_VA_IF_PARAMS(_PARAMS, _MACRO) _PARAMS(STATIC_VA_EMPTY_IT, _MACRO, STATIC_VA_SEPARATOR_NONE)

#define STATIC_VA_SEPARATE(_PARAMS, _SEP) STATIC_VA_IF_PARAMS(_PARAMS, _SEP)

#define STATIC_VA_FOR_EACH(_PARAMS, _IT, _FIRST_SEP) _PARAMS(_IT, _FIRST_SEP, STATIC_VA_SEPARATOR_COMMA)

// VA Common

#define STATIC_VA_ASK_VALUES_IT(_PARAM) _PARAM _##_PARAM
#define STATIC_VA_ASK_VALUES(_PARAMS, _FIRST_SEP) STATIC_VA_FOR_EACH(_PARAMS, STATIC_VA_ASK_VALUES_IT, _FIRST_SEP)

#define STATIC_VA_GIVE_VALUES_IT(_PARAM) _##_PARAM
#define STATIC_VA_GIVE_VALUES(_PARAMS, _FIRST_SEP) STATIC_VA_FOR_EACH(_PARAMS, STATIC_VA_GIVE_VALUES_IT, _FIRST_SEP)

#define STATIC_VA_GIVE_TYPES_IT(_PARAM) _PARAM
#define STATIC_VA_GIVE_TYPES(_PARAMS, _FIRST_SEP) STATIC_VA_FOR_EACH(_PARAMS, STATIC_VA_GIVE_TYPES_IT, _FIRST_SEP)

// VA Template

class static_va_empty;

#define STATIC_VA_ASK_TEMPLATE_TYPES_IT(_PARAM) typename _PARAM
#define STATIC_VA_ASK_TEMPLATE_TYPES(_PARAMS, _FIRST_SEP) STATIC_VA_FOR_EACH(_PARAMS, STATIC_VA_ASK_TEMPLATE_TYPES_IT, _FIRST_SEP)

#define STATIC_VA_ASK_TEMPLATE_TYPES_OPT_IT(_PARAM) typename _PARAM = static_va_empty
#define STATIC_VA_ASK_TEMPLATE_TYPES_OPT(_PARAMS, _FIRST_SEP) STATIC_VA_FOR_EACH(_PARAMS, STATIC_VA_ASK_TEMPLATE_TYPES_OPT_IT, _FIRST_SEP)

#define STATIC_VA_MAIN_TEMPLATE_TYPE(_TYPE, _TEMPLATE_PARAMS, _SEP, _PARAMS)\
	template<STATIC_VA_ASK_TEMPLATE_TYPES(_TEMPLATE_PARAMS, STATIC_VA_SEPARATOR_NONE) STATIC_VA_ASK_TEMPLATE_TYPES_OPT(_PARAMS, _SEP)>\
		_TYPE
#define STATIC_VA_SUB_TEMPLATE_TYPE(_TYPE, _TEMPLATE_PARAMS, _SEP, _PARAMS)\
	template<STATIC_VA_ASK_TEMPLATE_TYPES(_TEMPLATE_PARAMS, STATIC_VA_SEPARATOR_NONE) STATIC_VA_ASK_TEMPLATE_TYPES(_PARAMS, _SEP)>\
		_TYPE<STATIC_VA_GIVE_TYPES(_TEMPLATE_PARAMS, STATIC_VA_SEPARATOR_NONE) _SEP() STATIC_VA_GIVE_TYPES(_PARAMS, STATIC_VA_SEPARATOR_NONE) STATIC_VA_SEPARATE(_PARAMS, STATIC_VA_SEPARATOR_COMMA) static_va_empty>

#define STATIC_VA_TEMPLATE_NO_PARAMS_FIXED(_IT, _FIRST, _NEXT)

#define STATIC_VA_TEMPLATE_TYPE(_TYPE, _PARAMS, _STEP)\
	_STEP##_TEMPLATE_TYPE(_TYPE, STATIC_VA_TEMPLATE_NO_PARAMS_FIXED, STATIC_VA_SEPARATOR_NONE, _PARAMS)

#define STATIC_VA_TEMPLATE_TYPE_EX(_TYPE, _TEMPLATE_PARAMS, _PARAMS, _STEP)\
	_STEP##_TEMPLATE_TYPE(_TYPE, _TEMPLATE_PARAMS, STATIC_VA_SEPARATOR_COMMA, _PARAMS)

#endif // __STATIC_VA_H__
