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

#include "StaticIndexBuffer.h"
#include "StaticVertexBuffer.h"
#include "BasicShapeCreator.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshStrongReferenceObject BasicShapeCreator::CreateWorldGUI(float i_width, float i_height)
{
	float hw = i_width / 2.0f, hh = i_height / 2.0f;

	MeshStrongReferenceObject mesh;

	std::vector<vec3> v_data;
	std::vector<vec2> t_data;
	std::vector<uint32_t> i_data;

	v_data.push_back(vec3(-hw, -hh, 0.0f));
	v_data.push_back(vec3( hw, -hh, 0.0f));
	v_data.push_back(vec3( hw,  hh, 0.0f));
	v_data.push_back(vec3(-hw,  hh, 0.0f));
	t_data.push_back(vec2(0.0f, 1.0f));
	t_data.push_back(vec2(1.0f, 1.0f));
	t_data.push_back(vec2(1.0f, 0.0f));
	t_data.push_back(vec2(0.0f, 0.0f));
	i_data = { 0, 1, 2, 0, 2, 3 };


	mesh = new Mesh("WGUI");

	StaticVertexBufferStrongReferenceObject vbuf = new StaticVertexBuffer("WGUIVBuffer", VertexBufferUsage_VERTEX_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	SD_SREF(vbuf).RefreshBufferData(v_data.data(), v_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject tbuf = new StaticVertexBuffer("WGUITBuffer", VertexBufferUsage_TEX_COORD_BUFFER, VertexBufferFormat_X32Y32_SFLOAT);
	SD_SREF(tbuf).RefreshBufferData(t_data.data(), t_data.size() * sizeof(vec2));
	StaticIndexBufferStrongReferenceObject ibuf = new StaticIndexBuffer("WGUIIBuffer", IndexBufferFormat_X32_UINT);
	SD_SREF(ibuf).RefreshBufferData(i_data.data(), i_data.size() * sizeof(uint32_t));

	SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, vbuf.StaticCastTo<VertexBuffer>());
	SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, tbuf.StaticCastTo<VertexBuffer>());
	SD_SREF(mesh).RegisterIndexBuffer(ibuf.StaticCastTo<IndexBuffer>());

	return mesh;
}
______________SD_END_GRAPHICS_NAMESPACE______________