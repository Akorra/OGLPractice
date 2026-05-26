#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3  lightDir = normalize(lightPos - fs_in.FragPos);
    vec3  normDir  = normalize(fs_in.Normal); 
    float diff     = max(dot(lightDir, normDir), 0.0);
    vec3  diffuse  = diff * color;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);    
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normDir, halfDir), 0.0), 32.0);
    }
    else
    {
        vec3 refDir = reflect(-lightDir, normDir);
        spec = pow(max(dot(refDir, viewDir), 0.0), 32.0);
    }
    
    vec3 specular = vec3(0.3) * spec; // assuming bright white light
    FragColor = vec4(ambient + diffuse + specular, 1.0);

}