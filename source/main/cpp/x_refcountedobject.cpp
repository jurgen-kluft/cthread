#include "xmthread\x_RefCountedObject.h"

namespace xcore 
{
	xrefobject::xrefobject()
		: _counter(1)
	{
	}

	xrefobject::~xrefobject()
	{
	}

} // namespace xcore
