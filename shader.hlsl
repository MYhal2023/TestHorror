

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte���E�p
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ���C�g�p�o�b�t�@
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte���E�p
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte���E�p
};

// �t�H�O�p�o�b�t�@
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};

// �����p�o�b�t�@
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
{
	float4 Camera;
}

cbuffer LightViewBuffer : register(b8)
{
	matrix LightView;
}

cbuffer LightProjectionBuffer : register(b9)
{
	matrix LightProjection;
}


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos	: POSITION0,
						  out float4 outSMPosition  : POSITION1)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;

	//���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�
	matrix mat;
	mat = mul(World, LightView);	//���[���h�ϊ��s��A���C�g�r���[�ϊ��s��
	mat = mul(mat, LightProjection);	//���C�g�r���[�ł̎ˉe�ϊ��s��
	outSMPosition = mul(inPosition, mat);
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
Texture2D<float>	g_SMDepth : register(t1);

SamplerState	g_SamplerState : register( s0 );
SamplerState	g_SMSamplerState : register(s1);

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,
						 in  float4 inSMPosition    : POSITION1,

						 out float4 outDiffuse		: SV_Target )
{
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	// �e�̏���
	float ZValue = inSMPosition.z / inSMPosition.w;

	float2 SMTexture;
	SMTexture.x = (1.0f + inSMPosition.x / inSMPosition.w) * 0.5f;
	SMTexture.y = (1.0f - inSMPosition.y / inSMPosition.w) * 0.5f;

	float SMZValue = g_SMDepth.Sample(g_SMSamplerState, SMTexture).r + 0.001f;	//�e�ɂ������̈�

	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse;
	}
	else if(SMZValue < ZValue) // ���C�gPOS����v�Z�@SM�����C�g����̏��@Z�����_����̏��
	{
		color = float4(clamp(color.rgb - 0.85f, 0.0f, 1.0f), 1.0f);   // �e�̐F
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 5; i++)
		{
			float3 lightDir;
			float light;

			if (Light.Flags[i].y == 1)
			{
				if (Light.Flags[i].x == 1)
				{
					lightDir = normalize(Light.Direction[i].xyz);
					light = dot(lightDir, inNormal.xyz);

					light = 0.5 - 0.5 * light;
					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
				}
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}

				outColor += tempColor;
			}
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;
	}

	//�t�H�O
	if (Fog.Enable == 1)
	{
		float z = inPosition.z*inPosition.w;
		float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
		f = saturate(f);
		outDiffuse = f * color + (1 - f)*Fog.FogColor;
		outDiffuse.a = color.a;
	}
	else
	{
		outDiffuse = color;
	}

	////�����
	//if (fuchi == 1)
	//{
	//	float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
	//	//if ((angle < 0.5f)&&(angle > -0.5f))
	//	if (angle > -0.4f)
	//	{
	//		outDiffuse.r  = 1.0f;
	//		outDiffuse.g = 1.0f;		
	//		outDiffuse.b = 1.0f;
	//	}
	//}
}

//=============================================================================
// ���m�N���t�B���^�p���_�V�F�[�_
//=============================================================================
void VertexShaderMonoPolygon(in  float4 inPosition		: POSITION0,	//�ˉe�ϊ����W
	in	float4 inNormal : NORMAL0,
	in  float4 inDiffuse : COLOR0,											//�f�B�t���[�Y�F
	in  float2 inTexCoord : TEXCOORD0,										//Z�l�Z�o�p�e�N�X�`��
	out float4 outPosition : SV_POSITION,
	out float4 outNormal : NORMAL0,
	out float2 outTexCoord : TEXCOORD0,
	out float4 outDiffuse : COLOR0)
{
	//���[���h�r���[�ˉe�ϊ�
	matrix mat;
	mat = mul(World, View);		//���[���h�ϊ��s��
	mat = mul(mat, Projection);	//�ˉe�ϊ��s��
	outPosition = mul(inPosition, mat);
	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));
	outTexCoord = inTexCoord;
	outDiffuse = inDiffuse;
}

//=============================================================================
// ���m�N���t�B���^�p�s�N�Z���V�F�[�_
//=============================================================================
void PixelShaderMonoPolygon(in  float4 inPosition		: SV_POSITION,
	in  float4 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,

	out float4 outDiffuse : SV_Target)
{
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	float Y = color.r * 0.29891f + color.g * 0.58661f + color.b * 0.11448f;	//�s�N�Z���̋P�x�l�𒊏o
	color.r = Y;
	color.g = Y;
	color.b = Y;


	outDiffuse = color;
}

//=============================================================================
// ���ʂ̒��_�V�F�[�_
//=============================================================================
void VertexShaderSpecularPolygon(in  float4 inPosition		: POSITION0,
	in  float4 inNormal : NORMAL0,
	in  float4 inDiffuse : COLOR0,
	in  float2 inTexCoord : TEXCOORD0,

	out float4 outPosition : SV_POSITION,
	out float4 outNormal : NORMAL0,
	out float2 outTexCoord : TEXCOORD0,
	out float4 outDiffuse : COLOR0,
	out float4 outWorldPos : POSITION0,
	out float4 outSMPosition : POSITION1)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;

	//���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�
	matrix mat;
	mat = mul(World, LightView);	//���[���h�ϊ��s��A���C�g�r���[�ϊ��s��
	mat = mul(mat, LightProjection);	//���C�g�r���[�ł̎ˉe�ϊ��s��
	outSMPosition = mul(inPosition, mat);
}

//=============================================================================
//���ʂ̃s�N�Z���V�F�[�_�[
//=============================================================================
void PixelShaderSpecularPolygon(	in  float4 inPosition	: SV_POSITION,
							in  float4 inNormal		: NORMAL0,
							in  float2 inTexCoord	: TEXCOORD0,
							in  float4 inDiffuse	: COLOR0,
							in  float4 inWorldPos	: POSITION0,
							in  float4 inSMPosition : POSITION1,

							out float4 outDiffuse	: SV_Target)
{
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	// �e�̏���
	float ZValue = inSMPosition.z / inSMPosition.w;

	float2 SMTexture;
	SMTexture.x = (1.0f + inSMPosition.x / inSMPosition.w) * 0.5f;
	SMTexture.y = (1.0f - inSMPosition.y / inSMPosition.w) * 0.5f;

	float SMZValue = g_SMDepth.Sample(g_SMSamplerState, SMTexture).r + 0.001f;	//�e�ɂ������̈�

	if (SMZValue < ZValue) // ���C�gPOS����v�Z�@SM�����C�g����̏��@Z�����_����̏��
	{
		color = float4(clamp(color.rgb - 0.2f, 0.0f, 1.0f), 1.0f);   // �e�̐F
	}
	else
	{
		float3 lightDir;
		float light;
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

		if (Light.Flags[0].y == 1)
		{
			if (Light.Flags[0].x == 1)
			{
				lightDir = normalize(Light.Direction[0].xyz);
				light = dot(lightDir, inNormal.xyz);

				light = 0.5 - 0.5 * light;
				tempColor = color * Material.Diffuse * light * Light.Diffuse[0];
			}
			else if (Light.Flags[0].x == 2)
			{
				lightDir = normalize(Light.Position[0].xyz - inWorldPos.xyz);
				light = dot(lightDir, inNormal.xyz);

				tempColor = color * Material.Diffuse * light * Light.Diffuse[0];

				float distance = length(inWorldPos - Light.Position[0]);

				float att = saturate((Light.Attenuation[0].x - distance) / Light.Attenuation[0].x);
				tempColor *= att;
			}
			else
			{
				tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
			}

			outColor += tempColor;
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;

	}

	color.a = inDiffuse.a * Material.Diffuse.a;
	outDiffuse = color;

}