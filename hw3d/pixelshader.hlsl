cbuffer CBuf
{
    int index;
    float4 face_colors[6];
};

float4 main() : SV_TARGET
{
    return face_colors[index % 6];
}