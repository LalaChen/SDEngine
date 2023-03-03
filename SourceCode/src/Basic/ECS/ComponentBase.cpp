/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "ComponentBase.h"
#include "LogManager.h"

______________SD_START_BASIC_NAMESPACE_______________

ComponentBase::ComponentBase(const ObjectName& i_object_name)
: EventObject(i_object_name)
, m_initialized(false)
{
}

ComponentBase::~ComponentBase()
{
}

void ComponentBase::Initialize()
{
	if (m_initialized == false) {
		InitializeImpl();
		m_initialized = true;
	}
	else {
		SDLOGW("Component had been initialized.");
	}
}

void ComponentBase::Update()
{
	if (m_initialized == true) {
		UpdateImpl();
	}
}

void ComponentBase::Resize()
{
	if (m_initialized == true) {
		ResizeImpl();
	}
}


_______________SD_END_BASIC_NAMESPACE________________