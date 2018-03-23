/*
* Lens flare feature generation shader
* Input: HDR color (bright passed) texture
* Output: Lens flare texture
*/

struct Uniforms
{
	float scale;
};

ConstantBuffer<Uniforms> uniforms : register(b0);

Texture2D inputTex : register(t0); //HDR texture
Texture2D lensColorTex : register(t1); //lens color texture
SamplerState samp0 : register(s0);

struct PS_Input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

static const int samples = 8;
static const float dispersal = 0.3;
static const float halo_width = 0.6;
static const float distortion_amount = 1.5;

//chromatic distortion
float4 distort(float2 tex_coord, float2 direction, float3 distortion)
{
	float3 distorted = float3(inputTex.Sample(samp0, tex_coord + direction * distortion.x).x,
							  inputTex.Sample(samp0, tex_coord + direction * distortion.y).y,
							  inputTex.Sample(samp0, tex_coord + direction * distortion.z).z);
	distorted *= uniforms.scale;
	return float4(distorted, 1);
}

PS_Input VSMain(uint vertexId : SV_VertexID)
{
	// Triangle strip based on vertex id
	// 3-----2
	// |   / |
	// | /   |
	// 1-----0
	// 0: (1, 0)
	// 1: (0, 0)
	// 2: (1, 1)
	// 3: (0, 1)
    PS_Input output;

    output.texCoord.x = (vertexId & 1) ^ 1; // 1 if bit0 is 0.
    output.texCoord.y = vertexId >> 1; // 1 if bit1 is 1.

    float2 posL = output.texCoord.xy * 2.0f - float2(1, 1);
    output.position = float4(posL, 0.5f, 1.0f);
    output.texCoord.y = 1.f - output.texCoord.y;

    return output;
}

float4 PSMain(PS_Input input) : SV_TARGET
{
	float2 texCoord = -input.texCoord + 1.0;

	uint3 inputTexSize;
	inputTex.GetDimensions(0, inputTexSize.x, inputTexSize.y, inputTexSize.z);
	float2 invTexSize = 1.0 / inputTexSize.xy;

	float2 ghost_vec = (0.5 - texCoord) * dispersal;
	float2 halo_vec = normalize(ghost_vec) * halo_width;

	float3 distortion = float3(-invTexSize.x * distortion_amount, 0, invTexSize.x * distortion_amount);

	//sample ghosts
	float4 result = float4(0,0,0,0);
	for (int c = 0; c < samples; ++c)
	{
		float2 offset = frac(texCoord + ghost_vec * float(c));

		float weight = length(0.5 - offset) / length(float2(0.5, 0.5));
		weight = pow(1.0 - weight, 10.0);

		result += distort(offset, normalize(ghost_vec), distortion) * weight;
	}

	
	result *= lensColorTex.Sample(samp0, float2(length(0.5 - texCoord) / length(float2(0.5, 0.5)), 0));

	//sample halo
	float weight = length(0.5 - frac(texCoord + halo_vec)) / length(float2(0.5, 0.5));
	weight = pow(1.0 - weight, 10.0);

	result += distort(frac(texCoord + halo_vec), normalize(ghost_vec), distortion) * weight;

	return result;
}
