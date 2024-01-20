uniform sampler2D texture;
uniform vec2 cellsResolution;

out vec4 color;

void main()
{
    vec2 uv = floor(gl_TexCoord[0].xy * cellsResolution) / cellsResolution;
    color = texture2D(texture, uv);
}