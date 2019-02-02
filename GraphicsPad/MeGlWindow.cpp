#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <MeGlWindow.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <Primitives\Vertex.h>
#include <Primitives\ShapeGenerator.h>
#include <Camera.h>
#include <MyModel.h>
#include <iostream> 
#include <list> 
#include <iterator> 


using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::vec2;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint programID;
GLuint passThroughProgramID;

Camera camera;
GLuint theBufferID;

GLuint cubeNumIndices;
GLuint arrowNumIndices;
GLuint red_planeNumIndices;
GLuint green_planeNumIndices;
GLuint teapotNumIndices;
GLuint sphereNumIndices;
GLuint torusNumIndices;

GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint red_planeVertexArrayObjectID;
GLuint green_planeVertexArrayObjectID;
GLuint teapotVertexArrayObjectID;
GLuint sphereVertexArrayObjectID;
GLuint torusVertexArrayObjectID;

GLuint cubeIndexByteOffset;
GLuint arrowIndexByteOffset;
GLuint red_planeIndexByteOffset;
GLuint green_planeIndexByteOffset;
GLuint teapotIndexByteOffset;
GLuint sphereIndexByteOffset;
GLuint torusIndexByteOffset;
 

// uniforms
GLint dominateColorUniformLocation;
GLuint fullTransformationUniformLocation;
GLuint modelToWorldMatrixUniformLocation;
GLint ambientLightUniformLocation;
GLint eyePositionWorldUniformLocation;
GLint lightPositionUniformLocation;
GLint lightIntensityUniformLocation;
GLint chekpoint_tilesUniformLocation;
GLint shading_TypeUniformLocation;

float lightIntensity;
int pickedObject;

int useDominateColor = 1; // initially, yes use it

vec3 checkpoint_LocationWorld = vec3(3.5f, 0.4f, -5.5f);
vec3 endpoint_LocationWorld = vec3(-13.1f, 0.4f, -14.3f);

// initially
vec3 sphereLocationWorld = vec3(13.5f, 0.4f, 13.5f); // 13.5f, 0.4f, 13.5f
vec4 arrowLocationWorld = vec4(0.0f, 2.4f, 6.0f, 1.0f);
vec4 teapotLocationWorld = vec4(9.0f, 2.4f, 8.0f, 1.0f);
vec4 lightPositionWorld = vec4(13.5f, 3.0f, 13.5f, 1.0f);
vec4 torusPositionWorld = vec4(-2.0f, 0.4f, 1.8f, 1.0f);

mat4 sphereRotationMatrix;

mat4 modelToProjectionMatrix;
mat4 viewToProjectionMatrix;
mat4 worldToViewMatrix;
mat4 worldToProjectionMatrix;

// translation matrices
mat4 arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
mat4 teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
mat4 sphereModelToWorldMatrix_translate = glm::translate(vec3(sphereLocationWorld));
mat4 planeModelToWorldMatrix_translate = glm::translate(vec3(0, 0, 0));
mat4 lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
mat4 torusModelToWorldMatrix_translate = glm::translate(vec3(torusPositionWorld));

// scale matrices
mat4 arrowModelToWorldMatrix_scale = glm::scale(0.5f, 0.5f, 0.5f);
mat4 teapotModelToWorldMatrix_scale = glm::scale(0.25f, 0.25f, 0.25f);
mat4 sphereModelToWorldMatrix_scale = glm::scale(0.4f, 0.4f, 0.4f);
mat4 torusModelToWorldMatrix_scale = glm::scale(0.5f, 0.5f, 0.5f);
mat4 lightModelToWorldMatrix_scale = glm::scale(0.1f, 0.1f, 0.1f);


// rotation matrices
float arrow_rotation_angle_x = 0.0f;
float teapot_rotation_angle_x = 270.0f;
float sphere_rotation_angle_x = 0.0f;
float torus_rotation_angle_x = 0.0f;
float light_rotation_angle_x = 0.0f;

float arrow_rotation_angle_y = 0.0f;
float teapot_rotation_angle_y = 0.0f;
float sphere_rotation_angle_y = 0.0f;
float torus_rotation_angle_y = 0.0f;
float light_rotation_angle_y = 0.0f;

mat4 arrowModelToWorldMatrix_rotate = glm::rotate(arrow_rotation_angle_y, vec3(0.0f, 1.0f, 0.0f)) *
									glm::rotate(arrow_rotation_angle_x, vec3(1.0f, 0.0f, 0.0f));

mat4 teapotModelToWorldMatrix_rotate = glm::rotate(teapot_rotation_angle_y, vec3(0.0f, 1.0f, 0.0f)) *
										glm::rotate(teapot_rotation_angle_x, vec3(1.0f, 0.0f, 0.0f));

mat4 sphereModelToWorldMatrix_rotate = glm::rotate(sphere_rotation_angle_y, vec3(0.0f, 1.0f, 0.0f)) *
										glm::rotate(sphere_rotation_angle_x, vec3(1.0f, 0.0f, 0.0f));

mat4 torusModelToWorldMatrix_rotate = glm::rotate(torus_rotation_angle_y, vec3(0.0f, 1.0f, 0.0f)) *
										glm::rotate(torus_rotation_angle_x, vec3(1.0f, 0.0f, 0.0f));

mat4 lightModelToWorldMatrix_rotate = glm::rotate(light_rotation_angle_y, vec3(0.0f, 1.0f, 0.0f)) *
										glm::rotate(light_rotation_angle_x, vec3(1.0f, 0.0f, 0.0f));


// to world matrices 
mat4 arrowModelToWorldMatrix ;
mat4 teapotModelToWorldMatrix;
mat4 sphereModelToWorldMatrix;
mat4 planeModelToWorldMatrix;
mat4 torusModelToWorldMatrix;
mat4 lightModelToWorldMatrix ;


vec3 sphereLocationWorldStart = sphereLocationWorld;
vec4 torusPositionWorldStart = torusPositionWorld;

vec4 red = vec4(1, 0, 0, 0);
vec4 green = vec4(0, 1, 0, 0);
vec4 ballColor = vec4(1, 1, 0, 0);
vec4 white = vec4(1, 1, 1, 0);
vec4 prupule = vec4(1, 0, 0.5, 0);

int ball_on_green = 1; // at start it is 
int ball_touched_viechle = 0; // at start it's not
int ball_in_viechle = 0; // at start it's not

const int green_tiles_number = 200;

float* green_tiles_x = new float[green_tiles_number];
float* green_tiles_z = new float[green_tiles_number];

glm::vec2 oldMousePosition_objects;


void draw_green_tiles(const float* green_tiles_x,const float* green_tiles_z) {
	for (int i = 0; i < 81; i++) {
		// start and ending points
		if (i == 0 || i == 79) {
			glUniform4fv(dominateColorUniformLocation, 1, &white[0]);
			glUniform1i(chekpoint_tilesUniformLocation, 1);

		}
		// check point
		else if (i == 57 || i == 43) {
			glUniform4fv(dominateColorUniformLocation, 1, &prupule[0]);
			glUniform1i(chekpoint_tilesUniformLocation, 1);
		}
		// the green path
		else {
			glUniform4fv(dominateColorUniformLocation, 1, &green[0]);
			glUniform1i(chekpoint_tilesUniformLocation, 0);
		}

		planeModelToWorldMatrix = glm::translate(green_tiles_x[i], 0.001f, green_tiles_z[i]);
		modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
		glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, green_planeNumIndices, GL_UNSIGNED_SHORT, (void*)green_planeIndexByteOffset);
	}

}

int on_green_tile(float x, float z) {

	// x : 14,  z: 8 -> 14
	if (x > 13 && x < 14 && z > 7 && z < 14) {
		return 1;
	}
	
	// x : 12 -> 13,  z: 8
	if (x > 11 && x < 13 && z > 7 && z < 8) {
		return 1;
	}

	// x: 12,  z : 6->7
	if (x > 11 && x < 12 && z > 5 && z < 7) {
		return 1;
	}

	// x: 10 -> 13,  z: 14
	if (x > 9 && x < 13 && z > 13 && z < 14) {
		return 1;
	}

	// x: 10,   z: 11 -> 13
	if (x > 9 && x < 10 && z > 10 && z < 13) {
		return 1;
	}

	// x: 5 -> 9,  z: 11
	if (x > 4 && x < 9 && z > 10 && z < 11) {
		return 1;
	}

	// x: 5,  z: 5->10
	if (x > 4 && x < 5 && z > 4 && z < 10) {
		return 1;
	}

	//x : 5,  z: 0->4
	if (x > 4 && x < 5 && z > -1 && z <4) {
		return 1;
	}

	// x: 2->4,  z: 6
	if (x > 1 && x < 4 && z > 5 && z < 6) {
		return 1;
	}

	// x: 0 -> 2,  z:5
	if (x > -1 && x < 2 && z > 4 && z < 5) {
		return 1;
	}

	// x: 0,  z: 2 -> 4
	if (x > -1 && x < 0 && z > 1 && z < 4) {
		return 1;
	}

	// x : -1,   z: 2
	if (x > -2 && x < -1 && z > 1 && z < 2) {
		return 1;
	}

	// x: -1 -> 1, z: -1
	if (x > -2 && x < 1 && z > -2 && z < -1) {
		return 1;
	}

	// x : 1,  z : -2 -> -3
	if (x > 0 && x < 1 && z > -4 && z < -2) {
		return 1;
	}

	// x : 0, z : -3 -> -7
	if (x > -1 && x < 0 && z > -8 && z < -3) {
		return 1;
	}

	// x : 1 -> 4, z : -5
	if (x > 0 && x < 4 && z > -6 && z < -5) {
		return 1;
	}

	// x : -1 -> -3,  z: -7
	if (x > -4 && x < -1 && z > -8 && z < -7) {
		return 1;
	}

	// x : -3,  z : -8 -> -9
	if (x > -4 && x < -3 && z > -10 && z < -8) {
		return 1;
	}

	// x : -4 -> -5,  z  : -9
	if (x > -6 && x < -4 && z > -10 && z < -9) {
		return 1;
	}

	// x : -5, z: -10 -> -13
	if (x > -6 && x < -5 && z > -14 && z < -10) {
		return 1;
	}

	// x : -6 -> -8, z : -11
	if (x > -9 && x < -6 && z > -12 && z < -11){
		return 1;
	}

	// x : -8,  z: -12
	if (x > -9 && x < -8 && z > -13 && z < -12){
		return 1;
	}

	// x : -9 -> -10,  z: -12
	if (x > -11 && x < -9 && z > -13 && z < -12) {
		return 1;
	}

	// x : -10,  z : -13 -> -14
	if (x > -11 && x < -10 && z > -15 && z < -13) {
		return 1;
	}

	// x : -11 -> -15,  z : -14
	if (x > -15 && x < -11 && z > -16 && z < -14) {
		return 1;
	}

	return 0;

}

void create_green_tiles() {
	float tmp_x[] = {14.0, 14.0, 14.0, 14.0, 14.0, 14.0, 14.0, //1
		13.0, 12.0, // 2
		12.0, 12.0,  // 3
		13.0, 12.0, 11.0, 10.0, // 4
		10.0, 10.0, 10.0,  // 5
		9.0, 8.0, 7.0, 6.0, 5.0, // 6  
		5.0, 5.0, 5.0, 5.0, 5.0, 5.0,   // 7
		5.0, 5.0, 5.0, 5.0, 5.0, // 8
		4.0, 3.0, 2.0,   // 9
		2.0, 1.0, 0.0,  // 10 
		0.0, 0.0, 0.0,   // 11  
		-1.0,   // 12
		-1.0, 0.0, 1.0, // 13   
		1.0, 1.0,   // 14
		0.0, 0.0, 0.0, 0.0, 0.0,  // 15 
		1.0, 2.0, 3.0, 4.0,   // 16
		-1.0, -2.0, -3.0,    // 17
		-3.0, -3.0,   // 18
		-4.0, -5.0,  // 19
		-5.0, -5.0, -5.0, -5.0,    // 20
		-6.0, -7.0, -8.0,  // 21
		-8.0,   // 22
		-9.0, -10.0,  // 23
		-10.0, -10.0, // 24
		-11.0, -12.0, -13.0, -14.0, -15.0}; //25

	float tmp_z[] = {14.0, 13.0, 12.0, 11.0, 10.0, 9.0, 8.0,  // 1
		8.0, 8.0, // 2
		7.0, 6.0, // 3
		14.0, 14.0, 14.0, 14.0, // 4 
		13.0, 12.0, 11.0,  // 5
		11.0, 11.0, 11.0, 11.0, 11.0, //6   
		10.0, 9.0, 8.0, 7.0, 6.0, 5.0,  // 7
		4.0, 3.0, 2.0, 1.0, 0.0, //8 
		6.0, 6.0, 6.0, // 9
		5.0, 5.0, 5.0, // 10 
		4.0, 3.0, 2.0,   // 11
		2.0,  // 12
		-1.0, -1.0, -1.0, // 13   
		-2.0, -3.0,   // 14
		-3.0, -4.0, -5.0, -6.0, -7.0, // 15
		-5.0, -5.0, -5.0, -5.0,   // 16
		-7.0, -7.0, -7.0,  // 17
		-8.0, -9.0,  // 18
		-9.0, -9.0,   // 19
		-10.0, -11.0, -12.0, -13.0, //20    
		-11.0, -11.0, -11.0,   // 21
		-12.0,   // 22
		-12.0, -12.0,  // 23 
		-13.0, -14.0,  // 24
		-14.0, -14.0, -14.0, -14.0, -14.0}; //25

	memcpy(green_tiles_x, tmp_x, sizeof(tmp_x));
	memcpy(green_tiles_z, tmp_z, sizeof(tmp_z));

}

// returns 1 if the ball is in the viechle
int touched_viechle(float x, float z) {
	if (x < -1.0f && x > -1.4f && z > 1.5f && z < 2.0f) {
		return 1;
	}
	return 0;
}

void put_ball_in_viechle() {
	sphereLocationWorld = vec3(-2.0f, 0.4f, 1.8f);
	sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);

}

void did_we_win(float x, float z) {
	if (x < endpoint_LocationWorld.x && z < endpoint_LocationWorld.z) {
		exit(0);
	}
}

void getUniformLocations() {
	lightPositionUniformLocation = glGetUniformLocation(programID, "light_Position_World");
	eyePositionWorldUniformLocation = glGetUniformLocation(programID, "eye_Position_World");
	dominateColorUniformLocation = glGetUniformLocation(programID, "dominate_Color");
	ambientLightUniformLocation = glGetUniformLocation(programID, "ambient_Light");
	shading_TypeUniformLocation = glGetUniformLocation(programID, "shading_Type");
	lightIntensityUniformLocation = glGetUniformLocation(programID, "light_Intensity");

	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");
	chekpoint_tilesUniformLocation = glGetUniformLocation(programID, "chekpoint_Tile");
}

// object: 0 if light, 1 if viechle ,  2 if teapot,  3 if arrow
// direction: 0 if +x, 1 if -x
//			  2 if +y, 3 if -y
//			  4 if +z, 5 if -z
void moveObject(int pickedObject, int direction) {
	
	if (pickedObject == 0) {
			if (direction == 0) {
				if (lightPositionWorld.x < 14){
					lightPositionWorld += vec4(0.1, 0, 0, 0);
					lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
				}

			}
			else if (direction == 1) {
				if (lightPositionWorld.x > -14) {
					lightPositionWorld += vec4(-0.1, 0, 0, 0);
					lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
				}
			}
			else if (direction == 2) {
				if (lightPositionWorld.y < 8) {
					lightPositionWorld += vec4(0.0, 0.1, 0, 0);
					lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
				}
			}
			else if (direction == 3) {
				if (lightPositionWorld.y > 3) {
					lightPositionWorld += vec4(0.0, -0.1, 0, 0);
					lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
				}
					
			}
			else if (direction == 4) {
				if (lightPositionWorld.z < 14) {
					lightPositionWorld += vec4(0.0, 0, 0.1, 0);
					lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
				}
			}
			else if (direction == 5) {
				if (lightPositionWorld.z > -14) {
					lightPositionWorld += vec4(0.0, 0, -0.1, 0);
					lightModelToWorldMatrix_translate = glm::translate(vec3(lightPositionWorld));
				}
					
			}
	}

	else if (pickedObject == 3) {
		
			if (direction == 0) {
				if (arrowLocationWorld.x < 14) {
					arrowLocationWorld += vec4(0.1, 0, 0, 0);
					arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
				}
			}
			else if (direction == 1) {
				if (arrowLocationWorld.x > -14) {
					arrowLocationWorld += vec4(-0.1, 0, 0, 0);
					arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
				}
			}

			else if (direction == 2) {
				if (arrowLocationWorld.y < 6) {
					arrowLocationWorld += vec4(0.0, 0.1, 0, 0);
					arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
				}
			}
			else if (direction == 3) {
				if (arrowLocationWorld.y > 2) {
					arrowLocationWorld += vec4(0.0, -0.1, 0, 0);
					arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
				}
					
			}
			else if (direction == 4) {
				if (arrowLocationWorld.z < 14) {
					arrowLocationWorld += vec4(0.0, 0, 0.1, 0);
					arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
				}
			}
			else if (direction == 5) {
				if (arrowLocationWorld.z > -14) {
					arrowLocationWorld += vec4(0.0, 0, -0.1, 0);
					arrowModelToWorldMatrix_translate = glm::translate(vec3(arrowLocationWorld));
				}
			}

	}
	
	else if (pickedObject == 2) {
		if (direction == 0) {
			if (teapotLocationWorld.x < 14) {
				teapotLocationWorld += vec4(0.1, 0, 0, 0);
				teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
			}
		}
		else if (direction == 1) {
			if (teapotLocationWorld.x > -14) {
				teapotLocationWorld += vec4(-0.1, 0, 0, 0);
				teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
			}
		}

		else if (direction == 2) {
			if (teapotLocationWorld.y < 6) {
				teapotLocationWorld += vec4(0.0, 0.1, 0, 0);
				teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
			}
		}
		else if (direction == 3) {
			if (teapotLocationWorld.y > 2) {
				teapotLocationWorld += vec4(0.0, -0.1, 0, 0);
				teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
			}
				
		}

		else if (direction == 4) {
			if (teapotLocationWorld.z < 14) {
				teapotLocationWorld += vec4(0.0, 0, 0.1, 0);
				teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
			}
				
		}
		else if (direction == 5) {
			if (teapotLocationWorld.z > -14) {
				teapotLocationWorld += vec4(0.0, 0, -0.1, 0);
				teapotModelToWorldMatrix_translate = glm::translate(vec3(teapotLocationWorld));
			}
				
		}
	}

	else if (pickedObject == 1) {
		if (direction == 0) {
			if (torusPositionWorld.x < 14) {
				torusPositionWorld += vec4(0.1, 0, 0, 0);
				torusModelToWorldMatrix_translate = glm::translate(vec3(torusPositionWorld));

			}
			if (ball_in_viechle == 1) {
				sphereLocationWorld += vec3(0.1, 0, 0);
				sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
				}
		}
		else if (direction == 1) {
			if (torusPositionWorld.x > -14) {
				torusPositionWorld += vec4(-0.1, 0, 0, 0);
				torusModelToWorldMatrix_translate = glm::translate(vec3(torusPositionWorld));
			}
			if (ball_in_viechle == 1) {
				sphereLocationWorld += vec3(-0.1, 0, 0);
				sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
			}
		}

		else if (direction == 4) {
			if (torusPositionWorld.z < 14) {
				torusPositionWorld += vec4(0.0, 0, 0.1, 0);
				torusModelToWorldMatrix_translate = glm::translate(vec3(torusPositionWorld));
			}
			if (ball_in_viechle == 1) {
				sphereLocationWorld += vec3(0.0, 0, 0.1);
				sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
			}
		}
		else if (direction == 5) {
			if (torusPositionWorld.z > -14) {
				torusPositionWorld += vec4(0.0, 0, -0.1, 0);
				torusModelToWorldMatrix_translate = glm::translate(vec3(torusPositionWorld));
			}
			if (ball_in_viechle == 1) {
				sphereLocationWorld += vec3(0.0, 0, -0.1);
				sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
			}
		}
	}	
}

void rotateObject(int pickedObject, const glm::vec2& newMousePosition) {
	
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition_objects;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition_objects = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 1.3f;

	oldMousePosition_objects = newMousePosition;
	

	// arrow
	if (pickedObject == 3) {
		arrow_rotation_angle_x = mouseDelta.x * ROTATIONAL_SPEED;
		arrow_rotation_angle_y = mouseDelta.y * ROTATIONAL_SPEED;

		arrowModelToWorldMatrix_rotate = glm::rotate(arrow_rotation_angle_y, vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(arrow_rotation_angle_x, vec3(1.0f, 0.0f, 0.0f));
	}


}

// if direction 0 go UP
//				1 go Down
//				2 go right
//				3 go left
////////////////////////////////////////// TODO: rotation to be DONE
void moveBall(int direction) {
	if (ball_in_viechle != 1) {

		if (direction == 0) {
			sphereLocationWorld += vec3(0.1, 0.0, 0.0);
			sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);

		}
		else if (direction == 1) {
			sphereLocationWorld += vec3(-0.1, 0.0, 0.0);
			sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
		}
		else if (direction == 2) {
			sphereLocationWorld += vec3(0.0, 0.0, 0.1);
			sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
		}
		else if (direction == 3) {
			sphereLocationWorld += vec3(0.0, 0.0, -0.1);
			sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
		}
	}

}

MeGlWindow::MeGlWindow(MyModel* theModel):theModel(theModel)
{
	create_green_tiles();
}

void MeGlWindow::sendDataToOpenGL()
{
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData red_plane = ShapeGenerator::makePlane(30);
	ShapeData green_plane = ShapeGenerator::makePlane(2);
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData sphere = ShapeGenerator::makeSphere();
	ShapeData torus = ShapeGenerator::makeTorus();

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		cube.vertexBufferSize() + cube.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		red_plane.vertexBufferSize() + red_plane.indexBufferSize() +
		green_plane.vertexBufferSize() + green_plane.indexBufferSize() +
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize() +
		torus.vertexBufferSize() + torus.indexBufferSize(), 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	arrowIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, red_plane.vertexBufferSize(), red_plane.vertices);
	currentOffset += red_plane.vertexBufferSize();
	red_planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, red_plane.indexBufferSize(), red_plane.indices);
	currentOffset += red_plane.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, green_plane.vertexBufferSize(), green_plane.vertices);
	currentOffset += green_plane.vertexBufferSize();
	green_planeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, green_plane.indexBufferSize(), green_plane.indices);
	currentOffset += green_plane.indexBufferSize();

	
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	teapotIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);
	currentOffset += teapot.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	currentOffset += sphere.vertexBufferSize();
	sphereIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	currentOffset += sphere.indexBufferSize();

	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.vertexBufferSize(), torus.vertices);
	currentOffset += torus.vertexBufferSize();
	torusIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, torus.indexBufferSize(), torus.indices);
	currentOffset += torus.indexBufferSize();

	cubeNumIndices = cube.numIndices;
	arrowNumIndices = arrow.numIndices;
	red_planeNumIndices = red_plane.numIndices;
	
	green_planeNumIndices = green_plane.numIndices;

	teapotNumIndices = teapot.numIndices;
	sphereNumIndices = sphere.numIndices;
	torusNumIndices = torus.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);
	glGenVertexArrays(1, &red_planeVertexArrayObjectID);
	glGenVertexArrays(1, &teapotVertexArrayObjectID);
	glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &torusVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint arrowByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(red_planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint red_planeByteOffset = arrowByteOffset + arrow.vertexBufferSize() + arrow.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)red_planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(red_planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(red_planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);


	glBindVertexArray(green_planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint green_planeByteOffset = red_planeByteOffset + red_plane.vertexBufferSize() + red_plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)green_planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(green_planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(green_planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(teapotVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint teapotByteOffset = green_planeByteOffset + green_plane.vertexBufferSize() + green_plane.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)teapotByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(teapotByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(teapotByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(sphereVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint sphereByteOffset =  teapotByteOffset + teapot.vertexBufferSize() + teapot.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)sphereByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sphereByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sphereByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	glBindVertexArray(torusVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint torusByteOffset = sphereByteOffset + sphere.vertexBufferSize() + sphere.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)torusByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(torusByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(torusByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	cube.cleanup();
	arrow.cleanup();
	red_plane.cleanup();
	green_plane.cleanup();
	teapot.cleanup();
	sphere.cleanup();
	torus.cleanup();
}

void MeGlWindow::paintGL()
{
	// preparations
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, width(), height());

		lightIntensity = theModel->lightIntensity;
		pickedObject = theModel->pickedObject;

		viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
		worldToViewMatrix = camera.getWorldToViewMatrix();
		worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
		useDominateColor = 1;

		glUseProgram(programID);

		getUniformLocations();

		glUniform1i(shading_TypeUniformLocation, theModel->shadingType);

		vec4 ambientLight(0.2f, 0.2f, 0.2f, 1.0f);
		glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);
		glm::vec3 eyePosition = camera.getPosition();
		glUniform3fv(eyePositionWorldUniformLocation, 1, &eyePosition[0]);
		glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);
		glUniform1f(lightIntensityUniformLocation, lightIntensity);
		glUniform1i(dominateColorUniformLocation, useDominateColor);
	}
	
	// Plane
	{
		glUniform4fv(dominateColorUniformLocation, 1, &red[0]);

		glBindVertexArray(red_planeVertexArrayObjectID);
		planeModelToWorldMatrix = planeModelToWorldMatrix_translate;
		modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
		glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, red_planeNumIndices, GL_UNSIGNED_SHORT, (void*)red_planeIndexByteOffset);

		// draw the green tiles	
		glBindVertexArray(green_planeVertexArrayObjectID);
		draw_green_tiles(green_tiles_x, green_tiles_z);

		ball_on_green = on_green_tile(sphereLocationWorld.x, sphereLocationWorld.z);
		ball_touched_viechle = touched_viechle(sphereLocationWorld.x, sphereLocationWorld.z);
		did_we_win(sphereLocationWorld.x, sphereLocationWorld.z);
	}
	
	// Sphere
	{
		// if ball not on path
		if (ball_on_green == 0 && ball_in_viechle != 1) {
			// if it crossed the checkpoint
			if (sphereLocationWorld.x < 3.6 && sphereLocationWorld.z < -1.6f) {
				sphereLocationWorld = vec3(checkpoint_LocationWorld);
				sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
			}
			else {
				sphereLocationWorld = vec3(sphereLocationWorldStart); // restart
				sphereModelToWorldMatrix_translate = glm::translate(sphereLocationWorld);
			}
		}

		// if ball touched the viechle,  put it inside the viechle
		if (ball_touched_viechle == 1) {
			// ball not inside viechle
			if (ball_in_viechle == 0) {
				// put it inside it.
				ball_in_viechle = 1;
				put_ball_in_viechle();
			}
		}
		glUniform4fv(dominateColorUniformLocation, 1, &ballColor[0]);
		glBindVertexArray(sphereVertexArrayObjectID);
		sphereModelToWorldMatrix = sphereModelToWorldMatrix_translate * sphereModelToWorldMatrix_rotate * sphereModelToWorldMatrix_scale;
		modelToProjectionMatrix = worldToProjectionMatrix * sphereModelToWorldMatrix;
		glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &sphereModelToWorldMatrix[0][0]);
		glUniform1i(dominateColorUniformLocation, useDominateColor);
		glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);
	}
	
	
	// arrow
	{
		glUniform4fv(dominateColorUniformLocation, 1, &ballColor[0]);
		glBindVertexArray(arrowVertexArrayObjectID);
		arrowModelToWorldMatrix = arrowModelToWorldMatrix_translate * arrowModelToWorldMatrix_scale * arrowModelToWorldMatrix_rotate;
		modelToProjectionMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
		glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &arrowModelToWorldMatrix[0][0]);
		glUniform1i(dominateColorUniformLocation, useDominateColor);
		glDrawElements(GL_TRIANGLES, arrowNumIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);
	}
	
	
	// teapot
	{glUniform4fv(dominateColorUniformLocation, 1, &ballColor[0]);

	glBindVertexArray(teapotVertexArrayObjectID);
	teapotModelToWorldMatrix = teapotModelToWorldMatrix_translate * teapotModelToWorldMatrix_scale * teapotModelToWorldMatrix_rotate;
	modelToProjectionMatrix = worldToProjectionMatrix * teapotModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &teapotModelToWorldMatrix[0][0]);
	glUniform1i(dominateColorUniformLocation, useDominateColor);
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexByteOffset);}
	
	
	// torus
	{
		glUniform4fv(dominateColorUniformLocation, 1, &ballColor[0]);

		// did the torus touch the second checkpoint
		if (torusPositionWorld.x > 3.1f && torusPositionWorld.x < 3.8f &&  torusPositionWorld.z > -4.7f && torusPositionWorld.z < -4.5f) {
			sphereLocationWorld = vec3(3.3f, 0.4f, -5.3f);
			torusPositionWorld = torusPositionWorldStart;
			ball_in_viechle = 0;
		}
		// is the viechle on red tiles ?
		if (on_green_tile(torusPositionWorld.x, torusPositionWorld.z) != 0) {
			torusPositionWorld = torusPositionWorldStart;
			if (ball_in_viechle == 1) {
				sphereLocationWorld = vec3(-2.0f, 0.4f, 1.8f);
			}
		}

		glBindVertexArray(torusVertexArrayObjectID);
		torusModelToWorldMatrix = torusModelToWorldMatrix_translate * torusModelToWorldMatrix_rotate * torusModelToWorldMatrix_scale;
		modelToProjectionMatrix = worldToProjectionMatrix * torusModelToWorldMatrix;
		glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
		glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &torusModelToWorldMatrix[0][0]);
		glUniform1i(dominateColorUniformLocation, useDominateColor);
		glDrawElements(GL_TRIANGLES, torusNumIndices, GL_UNSIGNED_SHORT, (void*)torusIndexByteOffset);
	}

	//  light
	{glBindVertexArray(sphereVertexArrayObjectID);
	lightModelToWorldMatrix = lightModelToWorldMatrix_translate * lightModelToWorldMatrix_scale * lightModelToWorldMatrix_rotate;
	modelToProjectionMatrix = worldToProjectionMatrix * lightModelToWorldMatrix;
	glUseProgram(passThroughProgramID);
	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	modelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "modelToWorldMatrix");
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&lightModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, sphereNumIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);
	}
	
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	setFocus();
	if (pickedObject == 4) {
		camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	}

	else{
		rotateObject(pickedObject, glm::vec2(e->x(), e->y()));
	}
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{	
	switch (e->key()) {
		case Qt::Key::Key_Up:
			moveBall(0);
			break;
		case Qt::Key::Key_Down:
			moveBall(1);
			break;
		case Qt::Key::Key_Right:
			moveBall(2);
			break;
		case Qt::Key::Key_Left:
			moveBall(3);
			break;
	}

	// the camera is picked
	if (pickedObject == 4) {
		switch (e->key())
		{
		case Qt::Key::Key_W:
			camera.moveForward();
			break;
		case Qt::Key::Key_S:
			camera.moveBackward();
			break;
		case Qt::Key::Key_A:
			camera.strafeLeft();
			break;
		case Qt::Key::Key_D:
			camera.strafeRight();
			break;
		case Qt::Key::Key_R:
			camera.moveUp();
			break;
		case Qt::Key::Key_F:
			camera.moveDown();
			break;
		}
	}
	else{
		switch (e->key())
		{
			case Qt::Key::Key_W:
				moveObject(pickedObject, 0);
				break;
			case Qt::Key::Key_S:
				moveObject(pickedObject, 1);
				break;
			case Qt::Key::Key_A:
				moveObject(pickedObject, 5);
				break;
			case Qt::Key::Key_D:
				moveObject(pickedObject, 4);
				break;
			case Qt::Key::Key_R:
				moveObject(pickedObject, 2);
				break;
			case Qt::Key::Key_F:
				moveObject(pickedObject, 3);
				break;
			default:
				break;
		}

	}
	repaint();
}

bool MeGlWindow::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MeGlWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeGlWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string MeGlWindow::readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void MeGlWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = readShaderCode("VertexShaderPassThroughCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderPassThroughCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	passThroughProgramID = glCreateProgram();
	glAttachShader(passThroughProgramID, vertexShaderID);
	glAttachShader(passThroughProgramID, fragmentShaderID);

	glLinkProgram(passThroughProgramID);

	if (!checkProgramStatus(passThroughProgramID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void MeGlWindow::initializeGL()
{
	setMinimumSize(1200, 600);
	setMouseTracking(false);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sendDataToOpenGL();
	installShaders();

}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
	glDeleteProgram(passThroughProgramID);
}