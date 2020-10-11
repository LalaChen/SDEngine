/* ==============  SD Engine License ==============
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

#include "GraphicsManager.h"
#include "LogManager.h"
#include "MeshRenderComponent.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_COMPONENT_POOL_TYPE_IMPLEMENTATION(MeshRenderComponent, MeshRenderComponent);

MeshRenderComponent::MeshRenderComponent(const ObjectName &i_object_name)
: Component(i_object_name)
{
}

MeshRenderComponent::~MeshRenderComponent()
{
}

bool MeshRenderComponent::AddMesh(const MeshWeakReferenceObject &i_mesh_wref, const MaterialWeakReferenceObject &i_mat_wref)
{
    if (i_mat_wref.IsNull() == false && i_mesh_wref.IsNull() == false) {
        std::map<MeshWeakReferenceObject, MaterialWeakReferenceObject>::iterator iter = m_mesh_groups.begin();
        if (iter == m_mesh_groups.end()) {
            m_mesh_groups[i_mesh_wref] = i_mat_wref;
            return true;
        }
        else {
            SDLOGE("Add existed mesh(%s).", i_mesh_wref.GetRef().GetObjectName().c_str());
            return false;
        }
    }
    else {
        SDLOGE("Add null data. (Mat, Mesh)(%d, %d)", i_mat_wref.IsNull(), i_mesh_wref.IsNull());
        return false;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________