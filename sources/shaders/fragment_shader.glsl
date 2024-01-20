uniform sampler2D texture;
uniform vec2 fieldResolution;
uniform vec2 cellsResolution;
uniform int cellSize;

void main()
{
    vec2 pixelPosition = gl_TexCoord[0].xy * fieldResolution;
    vec2 cellPosition = pixelPosition / cellSize;
    vec2 cellUV = cellPosition / cellsResolution;
    gl_FragColor = texture2D(texture, cellUV);
}