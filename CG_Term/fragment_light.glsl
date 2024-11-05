#version 330 core
in vec3 FragPos; //--- 노멀값을 계산하기 위해 객체의 위치값을 버텍스 세이더에서 받아온다.
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;


uniform sampler2D outTexture;
uniform vec3 lightColor;	//빛 색
//uniform vec3 lightPos;	//빛 위치
uniform mat4 lightMatix;	//빛 위치

uniform vec3 viewPos;		//카메라 위치
uniform vec3 objectColor;   //물체 색
uniform int TextureN;		//텍스쳐 입히는지 안입히는지

uniform bool cal_dis;

void main ()
{	
	//--------------------------조명부분--------------------------------------
	vec3 ambientLight = vec3(0.3f); //--- 주변 조명 세기
	vec3 ambient = ambientLight * lightColor; //--- 주변 조명 값
	vec3 result;
	//광원 계산 => matrix * 0,0,0임.
	vec4 shLightPos = lightMatix * vec4(0,0,0, 1.0f);
	vec3 lightPos = vec3(shLightPos);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normalVector = normalize (Normal);
	float diffuseLight = max(dot(normalVector,lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;
	int shininess = 128;
	float dis = distance(lightPos,FragPos);
	vec3 viewDir = normalize (viewPos - FragPos);						 //--- 관찰자의 방향
	vec3 reflectDir = reflect (-lightDir, normalVector);				 //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
	float specularLight = max (dot (viewDir, reflectDir), 0.0);			 //--- V와 R의 내적값으로 강도 조절: 음수 방지
	specularLight = pow(specularLight, shininess);						 //--- shininess 승을 해주어 하이라이트를 만들어준다.
	vec3 specular = specularLight * lightColor;							 //--- 거울 반사 조명값: 거울반사값 * 조명색상값
	
	//---------------------------텍스쳐-------------------------
				
	if(cal_dis){

	result = ((ambient + diffuse + specular)/dis) * objectColor;	
	}
	else
	  result =  objectColor;	//--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상


	if(TextureN == 1) {
		FragColor = vec4 (result, 1.0) * texture(outTexture, TexCoord); //--- 텍스쳐 입힌 최종 색상
	}
	else if(TextureN == 0) {
			FragColor = vec4 (result, 1.0) ;							// --- 텍스처 안입힌 최종 색상
	}
}