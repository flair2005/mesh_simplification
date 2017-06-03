#version 330 core
// This is a sample fragment shader.
uniform vec3 view_pos;
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 v_normal;
in vec3 frag_pos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    //Wireframe lighting
    color = vec4(1.0f,.25f,1.0f,1.0f);
    //Use normal lighting
    //color = vec4(v_normal, 1.0f);
    
    vec3 norm = normalize(v_normal);
    
    //Use phong lighting
    vec3 light_pos = vec3(1.0f, 1.0f, 1.0f);
    vec3 light_pos2 = vec3(-1.0f,-1.0f,-1.0f);
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
    vec3 object_color = vec3(1.0f, .5f, .31f);
    
    vec3 light_dir = light_pos - frag_pos;
    vec3 light_dir2= light_pos2- frag_pos;
    
    //Ambient Lighting
    float ambient_strength = .1f;
    vec3 ambient = ambient_strength * light_color;
    //Final ambient lighting color
    ambient = ambient * object_color;
    
    //Diffuse
    float diff = max(dot(norm, light_dir),0.0f);
    vec3 diffuse = diff * light_color;
    float diff2 =max(dot(norm, light_dir2),0.0f);
    vec3 diffuse2= diff2* light_color;
    
    //Specular
    float spec_strength = 0.001f;
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    
    float spec = pow(max(dot(view_dir,reflect_dir), 0.0), 32);
    vec3 specular = spec_strength * spec * light_color;
    
    vec3 result = (ambient + diffuse + diffuse2 + specular) * object_color;
    color = vec4(result, 1.0f);
}
