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

#include "StaticVertexBuffer.h"
#include "StaticIndexBuffer.h"
#include "BasicShapeCreator.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshStrongReferenceObject BasicShapeCreator::CreateCube(const Vector3f &i_center, const Vector3f &i_scale, const Vector3f &i_tex_scale)
{
	MeshStrongReferenceObject mesh;

	vec3 center = vec3(i_center.m_vec.x, i_center.m_vec.y, i_center.m_vec.z);
	vec3 scale = vec3(i_scale.m_vec.x, i_scale.m_vec.y, i_scale.m_vec.z);
	vec3 tex_scale = vec3(i_tex_scale.m_vec.x, i_tex_scale.m_vec.y, i_tex_scale.m_vec.z);

	std::vector<vec3> vertices = {
		vec3( 0.5f, 0.5f,-0.5f), //0
		vec3( 0.5f, 0.5f, 0.5f), //1
		vec3(-0.5f, 0.5f, 0.5f), //2
		vec3(-0.5f, 0.5f,-0.5f), //3
		vec3( 0.5f,-0.5f,-0.5f), //4
		vec3( 0.5f,-0.5f, 0.5f), //5
		vec3(-0.5f,-0.5f, 0.5f), //6
		vec3(-0.5f,-0.5f,-0.5f)  //7 
	};

	std::vector<vec3> normals = {
		vec3(0.0f,-1.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f),
		vec3(-1.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f,-1.0f)
	};

	std::vector<vec2> tex_coords = {
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f)
	};

	std::vector<uint32_t> v_indices = {
		5,7,4, //-y
		5,6,7,
		0,2,1, //+y
		0,3,2,
		1,5,0, //+x
		0,5,4,
		6,1,2, //+z
		6,5,1,
		7,2,3, //-x
		7,6,2,
		3,0,4, //-z
		3,4,7
	};

	std::vector<uint32_t> n_indices = {
		0,0,0,
		0,0,0,
		1,1,1,
		1,1,1,
		2,2,2,
		2,2,2,
		3,3,3,
		3,3,3,
		4,4,4,
		4,4,4,
		5,5,5,
		5,5,5
	};

	std::vector<uint32_t> t_indices = {
		0,2,1, //-y
		0,3,2,
		3,1,2, //+y
		3,0,1,
		0,1,3, //+x
		3,1,2,
		1,3,0, //+z
		1,2,3,
		1,3,0, //-x
		1,2,3,
		3,0,1, //-z
		3,1,2
	};

	std::vector<vec3> v_data;
	std::vector<vec3> n_data;
	std::vector<vec2> t_data;
	std::vector<vec3> tn_data;
	std::vector<vec3> bn_data;
	std::vector<uint32_t> i_data;
	//
	vec3 f_vertices[3];
	vec3 f_normals[3];
	vec2 f_textures[3];
	vec3 f_tangent;
	vec3 f_binormal;

	for (uint32_t face = 0, data_triangle = 0; face < 6; ++face) {
		for (uint32_t triangle = 0; triangle < 2; ++triangle) {
			for (uint32_t order = 0; order < 3; ++order) {
				uint32_t rv_order = face * 6 + triangle * 3 + order;
				f_vertices[order] = vertices[v_indices[rv_order]].scale(scale) + center;
				f_normals[order] = vec3::normalize(normals[n_indices[rv_order]].scale(scale));
				if (face == 2 || face == 4) {
					f_textures[order] = tex_coords[t_indices[rv_order]].scale(vec2(tex_scale.z, tex_scale.y));
				}
				else if (face == 0 || face == 1) {
					f_textures[order] = tex_coords[t_indices[rv_order]].scale(vec2(tex_scale.z, tex_scale.x));
				}
				else if (face == 3 || face == 5) {
					f_textures[order] = tex_coords[t_indices[rv_order]].scale(vec2(tex_scale.x, tex_scale.y));
				}
			}
			f_tangent = CalculateTangetVector(
				vec3::normalize(f_vertices[0] - f_vertices[2]), //v1
				vec3::normalize(f_vertices[1] - f_vertices[2]), //v2
				vec2::normalize(vec2((f_textures[0] - f_textures[2]).x, (f_textures[0] - f_textures[2]).y)), //st1
				vec2::normalize(vec2((f_textures[1] - f_textures[2]).x, (f_textures[1] - f_textures[2]).y))  //st2
			);
			f_binormal = vec3::normalize(f_normals[0].cross(f_tangent));
			v_data.push_back(f_vertices[0]);
			n_data.push_back(f_normals[0]);
			t_data.push_back(f_textures[0]);
			tn_data.push_back(f_tangent);
			bn_data.push_back(f_binormal);

			v_data.push_back(f_vertices[1]);
			n_data.push_back(f_normals[1]);
			t_data.push_back(f_textures[1]);
			tn_data.push_back(f_tangent);
			bn_data.push_back(f_binormal);

			v_data.push_back(f_vertices[2]);
			n_data.push_back(f_normals[2]);
			t_data.push_back(f_textures[2]);
			tn_data.push_back(f_tangent);
			bn_data.push_back(f_binormal);

			i_data.push_back(data_triangle * 3 + 0);
			i_data.push_back(data_triangle * 3 + 1);
			i_data.push_back(data_triangle * 3 + 2);

			data_triangle++;
		}
	}

	mesh = new Mesh("Cube");

	StaticVertexBufferStrongReferenceObject vbuf_sref = new StaticVertexBuffer("CubeVBuffer", VertexBufferUsage_VERTEX_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	vbuf_sref.GetRef().RefreshBufferData(v_data.data(), v_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject nbuf_sref = new StaticVertexBuffer("CubeNBuffer", VertexBufferUsage_NORMAL_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	nbuf_sref.GetRef().RefreshBufferData(n_data.data(), n_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject bnbuf_sref = new StaticVertexBuffer("CubeBNBuffer", VertexBufferUsage_BINORMAL_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	bnbuf_sref.GetRef().RefreshBufferData(bn_data.data(), bn_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject tnbuf_sref = new StaticVertexBuffer("CubeTNBuffer", VertexBufferUsage_TANGENT_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	tnbuf_sref.GetRef().RefreshBufferData(tn_data.data(), tn_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject tbuf_sref = new StaticVertexBuffer("CubeTBuffer", VertexBufferUsage_TEX_COORD_BUFFER, VertexBufferFormat_X32Y32_SFLOAT);
	tbuf_sref.GetRef().RefreshBufferData(t_data.data(), t_data.size() * sizeof(vec2));
	StaticIndexBufferStrongReferenceObject ibuf_sref = new StaticIndexBuffer("CubeIBuffer", IndexBufferFormat_X32_UINT);
	ibuf_sref.GetRef().RefreshBufferData(i_data.data(), i_data.size() * sizeof(uint32_t));

	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, vbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_NORMAL_BUFFER, nbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_BINORMAL_BUFFER, bnbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_TANGENT_BUFFER, tnbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, tbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterIndexBuffer(ibuf_sref.StaticCastTo<IndexBuffer>());

	return mesh;
}

______________SD_END_GRAPHICS_NAMESPACE______________