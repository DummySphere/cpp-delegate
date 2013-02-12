// DelegateMulticast.h, 2013-02-10 19:05:00
// Multicast Delegate C++ class
// Copyright (c) 2012-2013 Julien Duminil
// This file is released under the MIT License (http://opensource.org/licenses/MIT)

#ifndef __DELEGATEMULTICAST_H__
#define __DELEGATEMULTICAST_H__

#include "Delegate.h"
#include <vector> // std::vector
#include <algorithm> // std::find

namespace Delegate
{
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Multicast template class definition

	#define DELEGATE_CALLBACKLIST_TYPENAME() typename

	#define DELEGATE_CLASS_CALLBACKLIST(_VA_PARAMS, _VA_STEP)\
		STATIC_VA_TEMPLATE_TYPE(class Multicast, _VA_PARAMS, _VA_STEP)\
		{\
			private:\
				typedef Callback<void STATIC_VA_GIVE_TYPES(_VA_PARAMS, STATIC_VA_SEPARATOR_COMMA)> Cb;\
				typedef std::vector<Cb> CbList;\
				typedef STATIC_VA_IF_PARAMS(_VA_PARAMS, DELEGATE_CALLBACKLIST_TYPENAME) CbList::iterator CbListIterator;\
				\
			public:\
				inline void operator +=(const Cb &_cb) { m_callbacks.push_back(_cb); }\
				inline void operator -=(const Cb &_cb)\
				{\
					CbListIterator it_end = m_callbacks.end();\
					CbListIterator it = std::find(m_callbacks.begin(), it_end, _cb);\
					if(it != it_end)\
						m_callbacks.erase(it);\
				}\
				\
				inline operator int() { return !m_callbacks.empty(); }\
				inline bool operator !() { return m_callbacks.empty(); }\
				inline void operator()(STATIC_VA_ASK_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) )\
				{\
					CbListIterator it_end = m_callbacks.end();\
					for(CbListIterator it = m_callbacks.begin(); it != it_end; ++it)\
					{\
						Cb &cb = *it;\
						if(cb)\
							cb(STATIC_VA_GIVE_VALUES(_VA_PARAMS, STATIC_VA_SEPARATOR_NONE) );\
					}\
				}\
				\
			private:\
				CbList m_callbacks;\
		};

	STATIC_VA_EXPAND(DELEGATE_CLASS_CALLBACKLIST)

} // namespace Delegate

#endif // __DELEGATEMULTICAST_H__
