#ifndef DEFAULT_SHAPES_H
#define DEFAULT_SHAPES_H
#include <vector>
#include "Vertex.hpp"

namespace life::kuma::default_shapes {

	// https://stackoverflow.com/questions/5046579/vertex-normals-for-a-cube#5046605
	const std::vector<Vertex> quad  = {
		{{-1.0f, 1.0f, 0.0f},{0.0f,0.0f,1.0f},{0.0f,1.0f}}, // Top-left
		{{ 1.0f, 1.0f, 0.0f},{0.0f,0.0f,1.0f},{1.0f,1.0f}}, // Top-right
		{{ 1.0f,-1.0f, 0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f}}, // Bottom-right
		{{-1.0f,-1.0f, 0.0f},{0.0f,0.0f,1.0f},{0.0f,0.0f}}  // Bottom-left
	};



    const std::vector<unsigned int> quad_indices = {
		0,1,2,
		3,0,2
	};

	const std::vector<float> screen_quad = { // We still use quad_indices ↑
		// Pos		 	// UV
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
    const std::vector<float> scr_quad_dbg = {
            // Pos		 	// UV
           -0.0f,  1.0f,  0.0f, 1.0f,
           -0.0f, -0.0f,  0.0f, 0.0f,
            1.0f, -0.0f,  1.0f, 0.0f,

           -0.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -0.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

	const std::vector<Vertex> box = {
		// POSITION				// NORMALS			  // UV
		// Back face
		{{-0.5f, -0.5f, -0.5f}, { 0.0f, 0.0f,-1.0f},  {0.0f, 0.0f}}, // Bottom-left
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f, 0.0f,-1.0f},  {1.0f, 1.0f}}, // top-right
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f, 0.0f,-1.0f},  {1.0f, 0.0f}}, // bottom-right         
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f, 0.0f,-1.0f},  {1.0f, 1.0f}}, // top-right
		{{-0.5f, -0.5f, -0.5f}, { 0.0f, 0.0f,-1.0f},  {0.0f, 0.0f}}, // bottom-left
		{{-0.5f,  0.5f, -0.5f}, { 0.0f, 0.0f,-1.0f},  {0.0f, 1.0f}}, // top-left
		// Front face
		{{-0.5f, -0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f},  {0.0f, 0.0f}}, // bottom-left
		{{ 0.5f, -0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f},  {1.0f, 0.0f}}, // bottom-right
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f}}, // top-right
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f}}, // top-right
		{{-0.5f,  0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f},  {0.0f, 1.0f}}, // top-left
		{{-0.5f, -0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f},  {0.0f, 0.0f}}, // bottom-left
		// Left face                                               //
		{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f},  {1.0f, 0.0f}}, // top-right
		{{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f},  {1.0f, 1.0f}}, // top-left
		{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f},  {0.0f, 1.0f}}, // bottom-left
		{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f},  {0.0f, 1.0f}}, // bottom-left
		{{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f},  {0.0f, 0.0f}}, // bottom-right
		{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f},  {1.0f, 0.0f}}, // top-right
		// Right face
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f},  {1.0f, 0.0f}}, // top-left
		{{ 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f},  {0.0f, 1.0f}}, // bottom-right
		{{ 0.5f,  0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f},  {1.0f, 1.0f}}, // top-right         
		{{ 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f},  {0.0f, 1.0f}}, // bottom-right
		{{ 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f},  {1.0f, 0.0f}}, // top-left
		{{ 0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f},  {0.0f, 0.0f}}, // bottom-left     
		// Bottom face
		{{-0.5f, -0.5f, -0.5f}, { 0.0f,-1.0f, 0.0f},  {0.0f, 1.0f}}, // top-right
		{{ 0.5f, -0.5f, -0.5f}, { 0.0f,-1.0f, 0.0f},  {1.0f, 1.0f}}, // top-left
		{{ 0.5f, -0.5f,  0.5f}, { 0.0f,-1.0f, 0.0f},  {1.0f, 0.0f}}, // bottom-left
		{{ 0.5f, -0.5f,  0.5f}, { 0.0f,-1.0f, 0.0f},  {1.0f, 0.0f}}, // bottom-left
		{{-0.5f, -0.5f,  0.5f}, { 0.0f,-1.0f, 0.0f},  {0.0f, 0.0f}}, // bottom-right
		{{-0.5f, -0.5f, -0.5f}, { 0.0f,-1.0f, 0.0f},  {0.0f, 1.0f}}, // top-right
		// Top face
		{{-0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f},  {0.0f, 1.0f}}, // top-left
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f, 0.0f},  {1.0f, 0.0f}}, // bottom-right
		{{ 0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f},  {1.0f, 1.0f}}, // top-right     
		{{ 0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f, 0.0f},  {1.0f, 0.0f}}, // bottom-right
		{{-0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f},  {0.0f, 1.0f}}, // top-left
		{{-0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f, 0.0f},  {0.0f, 0.0f}}  // bottom-left   
	};

	const std::vector<unsigned int> box_indices = {
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35
	};

	const glm::mat4 mat4_identity_matrix = {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
	};
    const glm::mat3 mat3_identity_matrix = {
            {1,0,0},
            {0,1,0},
            {0,0,1}
    };

}

#endif // DEFAULT_SHAPES_H
