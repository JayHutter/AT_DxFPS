Texture2D tex;

SamplerState splr;

float4 main(float2 coord : TexCoord) : SV_Target
{
	return tex.Sample(splr,coord);
}