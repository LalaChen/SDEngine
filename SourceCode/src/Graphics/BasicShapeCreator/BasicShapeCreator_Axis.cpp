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
#include "BasicShapeCreator.h"

#include "StaticIndexBuffer.h"
#include "StaticVertexBuffer.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshStrongReferenceObject BasicShapeCreator::CreateAxis(float i_width, float i_length)
{
	MeshStrongReferenceObject mesh;

	std::vector<vec3> v_data;
	std::vector<Color4f> c_data;
	std::vector<uint32_t> i_data;

	//x axis
	v_data.push_back(vec3(0.0f, 0.0f, 0.0f));
	v_data.push_back(vec3(0.0f, 0.0f, i_width));
	v_data.push_back(vec3(i_length, 0.0f, 0.0f));
	
	v_data.push_back(vec3(0.0f, 0.0f, 0.0f));
	v_data.push_back(vec3(0.0f, 0.0f, -0.5f * i_width));
	v_data.push_back(vec3(- 0.5 * i_length, 0.0f, 0.0f));
	
	c_data.push_back(Color4f::Red());
	c_data.push_back(Color4f::Red());
	c_data.push_back(Color4f::Red());
	
	c_data.push_back(Color4f::Red(0.3f));
	c_data.push_back(Color4f::Red(0.3f));
	c_data.push_back(Color4f::Red(0.3f));
	
	i_data.push_back(0);
	i_data.push_back(1);
	i_data.push_back(2);
	i_data.push_back(0);
	i_data.push_back(2);
	i_data.push_back(1);

	i_data.push_back(3);
	i_data.push_back(4);
	i_data.push_back(5);
	i_data.push_back(3);
	i_data.push_back(5);
	i_data.push_back(4);

	//y axis
	v_data.push_back(vec3(0.0f, 0.0f, 0.0f));
	v_data.push_back(vec3(0.0f, i_length, 0.0f));
	v_data.push_back(vec3(0.0f, 0.0f, i_width));

	v_data.push_back(vec3(0.0f, 0.0f, 0.0f));
	v_data.push_back(vec3(0.0f, -0.5f * i_length, 0.0f));
	v_data.push_back(vec3(0.0f, 0.0f, -0.5f * i_width));

	c_data.push_back(Color4f::Green());
	c_data.push_back(Color4f::Green());
	c_data.push_back(Color4f::Green());

	c_data.push_back(Color4f::Green(0.3f));
	c_data.push_back(Color4f::Green(0.3f));
	c_data.push_back(Color4f::Green(0.3f));

	i_data.push_back(6);
	i_data.push_back(7);
	i_data.push_back(8);

	i_data.push_back(6);
	i_data.push_back(8);
	i_data.push_back(7);

	i_data.push_back(9);
	i_data.push_back(10);
	i_data.push_back(11);

	i_data.push_back(9);
	i_data.push_back(11);
	i_data.push_back(10);

	//z axis
	v_data.push_back(vec3(0.0f, 0.0f, 0.0f));
	v_data.push_back(vec3(0.0f, 0.0f, i_length));
	v_data.push_back(vec3(i_width, 0.0f, 0.0f));

	v_data.push_back(vec3(0.0f, 0.0f, 0.0f));
	v_data.push_back(vec3(0.0f, 0.0f, -0.5f * i_length));
	v_data.push_back(vec3(-0.5f * i_width, 0.0f, 0.0f));

	c_data.push_back(Color4f::Blue());
	c_data.push_back(Color4f::Blue());
	c_data.push_back(Color4f::Blue());

	c_data.push_back(Color4f::Blue(0.3f));
	c_data.push_back(Color4f::Blue(0.3f));
	c_data.push_back(Color4f::Blue(0.3f));

	i_data.push_back(12);
	i_data.push_back(13);
	i_data.push_back(14);

	i_data.push_back(12);
	i_data.push_back(14);
	i_data.push_back(13);

	i_data.push_back(15);
	i_data.push_back(16);
	i_data.push_back(17);

	i_data.push_back(15);
	i_data.push_back(17);
	i_data.push_back(16);

	mesh = new Mesh("Axis");

	StaticVertexBufferStrongReferenceObject vbuf = new StaticVertexBuffer("AxisVBuffer", VertexBufferUsage_VERTEX_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	SD_SREF(vbuf).RefreshBufferData(v_data.data(), v_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject cbuf = new StaticVertexBuffer("AxisCBuffer", VertexBufferUsage_COLOR_BUFFER, VertexBufferFormat_X32Y32Z32W32_SFLOAT);
	SD_SREF(cbuf).RefreshBufferData(c_data.data(), c_data.size() * sizeof(Color4f));
	StaticIndexBufferStrongReferenceObject ibuf = new StaticIndexBuffer("AxisIBuffer", IndexBufferFormat_X32_UINT);
	SD_SREF(ibuf).RefreshBufferData(i_data.data(), i_data.size() * sizeof(uint32_t));

	SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, vbuf.StaticCastTo<VertexBuffer>());
	SD_SREF(mesh).RegisterVertexBuffer(VertexBufferUsage_COLOR_BUFFER, cbuf.StaticCastTo<VertexBuffer>());
	SD_SREF(mesh).RegisterIndexBuffer(ibuf.StaticCastTo<IndexBuffer>());

	return mesh;
}
______________SD_END_GRAPHICS_NAMESPACE______________