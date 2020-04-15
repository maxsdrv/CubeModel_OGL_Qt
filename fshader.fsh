uniform sampler2D u_texture;
uniform highp vec4 u_lightPosition; //light source position
uniform highp float u_lightPower; //power source position
varying highp vec4 v_position;
varying highp vec3 v_normal;
varying highp vec2 v_texcoord;



void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0); //result color
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 diffMatColor = texture2D(u_texture, v_texcoord); //save diff color materrial
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz); //gaze direction vector
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition); // lights vector
    vec3 reflectLight = normalize(reflect(lightVect, v_normal)); //reflection vector
    float len = length(v_position.xyz - eyePosition.xyz);
    float specularFactor = 10.0;
    float ambientFactor = 0.1;

    vec4 diffColor = diffMatColor * u_lightPower *
         dot(v_normal, -lightVect) / (1.0 + 0.25 * pow(len, 2.0)); //calculate of diffuse
                                                                    //color

    resultColor += diffColor;

    gl_FragColor = resultColor;

}
