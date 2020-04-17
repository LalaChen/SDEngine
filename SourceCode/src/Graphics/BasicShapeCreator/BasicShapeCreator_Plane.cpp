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
#include "BasicShapeCreator.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

MeshStrongReferenceObject BasicShapeCreator::CreatePlane(
    const Vector3f &i_center,
    const Vector3f &i_u, const Vector3f &i_v,
    float i_width, float i_length,
    float i_tessellated_u_unit_length,
    float i_tessellated_v_unit_length,
    float i_texture_wrap_x,
    float i_texture_wrap_y)
{
	MeshStrongReferenceObject mesh;

	std::vector<vec3> vertices = {
		vec3(0.5f, 0.5f,-0.5f), //0
		vec3(0.5f, 0.5f, 0.5f), //1
		vec3(-0.5f, 0.5f, 0.5f), //2
		vec3(-0.5f, 0.5f,-0.5f), //3
	};

	std::vector<vec2> tex_coords = {
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f)
	};

	vec3 center(vec3::normalize(vec3(i_center.m_vec.x, i_center.m_vec.y, i_center.m_vec.z)));
    vec3 u(vec3::normalize(vec3(i_u.m_vec.x, i_u.m_vec.y, i_u.m_vec.z)));
    vec3 v(vec3::normalize(vec3(i_v.m_vec.x, i_v.m_vec.y, i_v.m_vec.z)));

    vec3 normal = u.cross(v);

	//----- temporary storage of vertex data for each face.
	vec3 f_vertices[4];
	vec3 f_normals[4];
	vec2 f_textures[4];
	vec3 f_tangent;
	vec3 f_binormal;
	//----- count
	uint32_t i, j;
	//----- plane info
	vec3 half_u(i_width / 2.0f * u.x, i_width / 2.0f * u.y, i_width / 2.0f * u.z);
	vec3 half_v(i_length / 2.0f * v.x, i_length / 2.0f * v.y, i_length / 2.0f * v.z);
	vec3 left_back_pos = center - half_u - half_v;
	vec3 right_front_pos = center + half_u + half_v;
	vec3 u_add_size(i_tessellated_u_unit_length * u.x, i_tessellated_u_unit_length * u.y, i_tessellated_u_unit_length * u.z);
	vec3 v_add_size(i_tessellated_v_unit_length * v.x, i_tessellated_v_unit_length * v.y, i_tessellated_v_unit_length * v.z);
	uint32_t width_number = static_cast<uint32_t>((i_width / i_tessellated_u_unit_length) + 0.5f);
	uint32_t length_number = static_cast<uint32_t>((i_length / i_tessellated_v_unit_length) + 0.5f);
	float tex_wrap_u_unit_length = i_texture_wrap_x / static_cast<float>(width_number);
	float tex_wrap_v_unit_length = i_texture_wrap_y / static_cast<float>(length_number);

    std::vector<vec3> v_data;
    std::vector<vec3> n_data;
    std::vector<vec2> t_data;
    std::vector<vec3> tn_data;
    std::vector<vec3> bn_data;
    std::vector<uint32_t> i_data;

	uint32_t face_index_start_id = 0;
	for (i = 0; i < length_number; ++i) {
		for (j = 0; j < width_number; ++j) {
			//1. calculate this face
			f_vertices[0] = left_back_pos + v_add_size.scale(static_cast<float>(i    )) + u_add_size.scale(static_cast<float>(j    ));
			f_vertices[1] = left_back_pos + v_add_size.scale(static_cast<float>(i + 1)) + u_add_size.scale(static_cast<float>(j    ));
			f_vertices[2] = left_back_pos + v_add_size.scale(static_cast<float>(i + 1)) + u_add_size.scale(static_cast<float>(j + 1));
			f_vertices[3] = left_back_pos + v_add_size.scale(static_cast<float>(i    )) + u_add_size.scale(static_cast<float>(j + 1));
			f_normals[0] = normal;
			f_normals[1] = normal;
			f_normals[2] = normal;
			f_normals[3] = normal;
			f_textures[0] = vec2(tex_wrap_u_unit_length *  j     , tex_wrap_v_unit_length *  i     );
			f_textures[1] = vec2(tex_wrap_u_unit_length *  j     , tex_wrap_v_unit_length * (i + 1));
			f_textures[2] = vec2(tex_wrap_u_unit_length * (j + 1), tex_wrap_v_unit_length * (i + 1));
			f_textures[3] = vec2(tex_wrap_u_unit_length * (j + 1), tex_wrap_v_unit_length *  i     );
			f_tangent = CalculateTangetVector(
				vec3::normalize(f_vertices[0] - f_vertices[2]), //v1
				vec3::normalize(f_vertices[1] - f_vertices[2]), //v2
				vec2::normalize(vec2((f_textures[0] - f_textures[2]).x, (f_textures[0] - f_textures[2]).y)), //st1
				vec2::normalize(vec2((f_textures[1] - f_textures[2]).x, (f_textures[1] - f_textures[2]).y))  //st2
			);

			f_binormal = normal.cross(f_tangent).normalize();

			bn_data.push_back(f_binormal); tn_data.push_back(f_tangent); t_data.push_back(f_textures[0]); n_data.push_back(f_normals[0]); v_data.push_back(f_vertices[0]);
			bn_data.push_back(f_binormal); tn_data.push_back(f_tangent); t_data.push_back(f_textures[1]); n_data.push_back(f_normals[1]); v_data.push_back(f_vertices[1]);
			bn_data.push_back(f_binormal); tn_data.push_back(f_tangent); t_data.push_back(f_textures[2]); n_data.push_back(f_normals[2]); v_data.push_back(f_vertices[2]);
			bn_data.push_back(f_binormal); tn_data.push_back(f_tangent); t_data.push_back(f_textures[3]); n_data.push_back(f_normals[3]); v_data.push_back(f_vertices[3]);
			//2. combine up triangle (0,1,2)
			i_data.push_back(face_index_start_id + 0);
			i_data.push_back(face_index_start_id + 1);
			i_data.push_back(face_index_start_id + 2);
			//3. combine down triangle (0,2,3)
			i_data.push_back(face_index_start_id + 0);
			i_data.push_back(face_index_start_id + 2);
			i_data.push_back(face_index_start_id + 3);
			//
			face_index_start_id += 4;
		}
	}

	mesh = new Mesh("Plane");

	StaticVertexBufferStrongReferenceObject vbuf_sref = new StaticVertexBuffer("PlaneVBuffer", VertexBufferUsage_VERTEX_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	vbuf_sref.GetRef().RefreshBufferData(v_data.data(), v_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject nbuf_sref = new StaticVertexBuffer("PlaneNBuffer", VertexBufferUsage_NORMAL_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	nbuf_sref.GetRef().RefreshBufferData(n_data.data(), n_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject bnbuf_sref = new StaticVertexBuffer("PlaneBNBuffer", VertexBufferUsage_BINORMAL_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	nbuf_sref.GetRef().RefreshBufferData(bn_data.data(), bn_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject tnbuf_sref = new StaticVertexBuffer("PlaneTNBuffer", VertexBufferUsage_TANGENT_BUFFER, VertexBufferFormat_X32Y32Z32_SFLOAT);
	nbuf_sref.GetRef().RefreshBufferData(tn_data.data(), tn_data.size() * sizeof(vec3));
	StaticVertexBufferStrongReferenceObject tbuf_sref = new StaticVertexBuffer("PlaneTBuffer", VertexBufferUsage_TEX_COORD_BUFFER, VertexBufferFormat_X32Y32_SFLOAT);
	tbuf_sref.GetRef().RefreshBufferData(t_data.data(), t_data.size() * sizeof(vec2));
	StaticVertexBufferStrongReferenceObject ibuf_sref = new StaticVertexBuffer("PlaneIBuffer", VertexBufferUsage_ELEMENT_BUFFER, VertexBufferFormat_X32_UINT);
	ibuf_sref.GetRef().RefreshBufferData(i_data.data(), i_data.size() * sizeof(uint32_t));

	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_VERTEX_BUFFER, vbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_NORMAL_BUFFER, nbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_BINORMAL_BUFFER, bnbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_TANGENT_BUFFER, tnbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_TEX_COORD_BUFFER, tbuf_sref.StaticCastTo<VertexBuffer>());
	mesh.GetRef().RegisterVertexBuffer(VertexBufferUsage_ELEMENT_BUFFER, ibuf_sref.StaticCastTo<VertexBuffer>());

	return mesh;
}


______________SD_END_GRAPHICS_NAMESPACE______________