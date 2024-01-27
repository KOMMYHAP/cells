uniform sampler2D texture;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture2D(texture, gl_TexCoord[0].xy);
}