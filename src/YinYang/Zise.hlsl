Texture2D Texture: register(t0);

SamplerState gTriLinearSam{
  Filter = ANISOTROPIC;
}; //SamplerState

float4 main(float4 posH: SV_POSITION,  float2 texo: TEXCOORD): SV_TARGET{
  float4 color = Texture.Sample(gTriLinearSam, texo); // get the color of the texture
 // float4 white = {1.0, 1.0, 1.0, 1.0}; // reminder
 // float4 black = { 0.0, 0.0, 0.0, 1.0 };
  float4 half_marker = { 0.5, 0.5, 0.5, 1.0 }; // this is a 50% gray

  // these are the pallete colors
  float4 primary_color = {0.5, 0.0, 0.5, 1.0}; // main color; violet
  float4 secondary_color = {1.0, 0.8, 0.0, 1.0};// secondary; yellow

  if (color.r > half_marker.r && color.b > half_marker.b && color.g > half_marker.g) // if color is marked as white
  {
	  color = (color * secondary_color); 
  }
  else // if color is marked as black
  {
	  color.rgb = 1.0 - color.rgb;
	  color = (color * primary_color);
  }

  return color;
} //main