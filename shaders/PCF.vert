#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in float vertexExpandDirection;


//�Œ���̍��W�ϊ�
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;
flat out vec3 sendCameraPosition;

//ShadowCoord�v�Z�p
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

//����
uniform mat4 normalMatrix;

//kajiya-kay�֌W
out vec3 globalPosition;
out vec3 mvPos;
out vec3 binormal;
out vec3 normal;
out vec3 tangent;

//phong�V�F�[�f�B���O
out vec3 localPosition;
out vec3 localNormal;
out mat4 mMatrix;

//�e�N�X�`��, �V���h�E�}�b�v�p
out vec4 shadowCoord;

//���̏��
uniform float thickness;

//flag
uniform float hairFlag;
flat out float sendHairFlag;
uniform float cylinderFlag;

void main(void)
{
	sendCameraPosition = cameraPosition;
	sendHairFlag = hairFlag;

	if (hairFlag == 1.0) {
		if (cylinderFlag == 1.0) {
			//kajiya-kay
			globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
			mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
			normal = (normalMatrix * vec4(vertexNormal, 1.0)).xyz;
			binormal = cross(normal, vertexTangent);
			tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;

			//phong
			localPosition = vertexPosition;
			mMatrix = modelMatrix;

			//�e�N�X�`���֌W
			shadowCoord = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

			//�`��ʒu
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
		} else {
			tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;//global

			vec4 nowGlobalPos = modelMatrix * vec4(vertexPosition, 1.0);
			vec3 viewVec = cameraPosition - nowGlobalPos.xyz;
			viewVec = normalize(viewVec);

			
			binormal = cross(viewVec, tangent);
			normal = cross(tangent, binormal);
			
			vec3 offset = binormal * vertexExpandDirection * thickness;

			vec4 movedPosition = vec4(offset, 0.0) + nowGlobalPos;
			
			//kajiya-kay
			globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
			mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
				
			//phong
			localPosition = (inverse(modelMatrix) * movedPosition).xyz;
			mMatrix = modelMatrix;
	

			shadowCoord = lightProjectionMatrix * lightViewMatrix * movedPosition;
			gl_Position = projectionMatrix * viewMatrix * movedPosition;
		}
	} else {
		//kajiya-kay
		globalPosition = (modelMatrix * vec4(vertexPosition, 1.0)).xyz;
		mvPos = (viewMatrix * vec4(globalPosition, 1.0)).xyz;
		normal = (normalMatrix * vec4(vertexNormal, 1.0)).xyz;

		//phong
		localPosition = vertexPosition;
		mMatrix = modelMatrix;

		//�e�N�X�`���֌W
		shadowCoord = lightProjectionMatrix * lightViewMatrix * modelMatrix * vec4(vertexPosition, 1.0);

		//�`��ʒu
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	}
}