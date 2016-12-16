Texture2D Texture: register(t0);

SamplerState gTriLinearSam {
	Filter = ANISOTROPIC;
}; //SamplerState

float4 main(float4 posH: SV_POSITION, float2 texo : TEXCOORD) : SV_TARGET{
	float4 color = Texture.Sample(gTriLinearSam, texo);
	float4 invisible = { 0.0, 0.0, 0.0, 0.0 };

	return invisible;
} //main