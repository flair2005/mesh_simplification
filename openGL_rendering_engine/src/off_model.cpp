#include "../include/off_model.h"

off_model::off_model(){
    toWorld = glm::mat4(1.0f);
}

off_model::off_model(const char* path){
    parse_file(path);
    toWorld = glm::mat4(1.0f);

    //Create the buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces_flat.size()*sizeof(int), &faces_flat[0], GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(glm::vec3), &faces_flat[0], GL_STATIC_DRAW);
    
    //Bind the normals
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
        
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

//Tokenizer helper function to delimit spaces and other tokens
//Credit to http://oopweb.com/CPP/Documents/CPPHOWTO/Volume/C++Programming-HOWTO-7.html
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " "){
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos){
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void off_model::parse_file(const char* path){
    //Create the file pointer
    ifstream file(path);

    //Create the string
    string line;

    //Wrong file path probably
    if(!file.is_open()){
        printf("Lul something broke\n");
        exit(-1);
    }
    
    //Vector for all our tokens 
    vector<string> tokens;
    
    //Use to start the object in the center
    float maxX, maxY, maxZ, minX, minY, minZ, avgX, avgY, avgZ;
    maxX = maxY = maxZ = numeric_limits<float>::min();
    minX = minY = minZ = numeric_limits<float>::max();

    //Loop while lines exists
    int lines_read = 0, nvert = 0, nfaces = 0;
    while(getline(file,line)){
        lines_read++;                //increment the number of lines read
        Tokenize(line, tokens, " "); //Tokenize our string and fill the tokens vector

        if(lines_read == 1){         //First line read contains one word, OFF
            printf("%s", tokens[0].c_str());    //Debug print
        }
        else if(lines_read == 2){         //On the second line, VERT FACES EDGES
            nvert  = stoi(tokens[0]);
            nfaces = stoi(tokens[1]);

            vertex_face_adj.resize(nvert);  //Our vertex-face-adj list is nvert long

            printf("%d %d\n", nvert, nfaces);   //Debug print
        }
        else if(lines_read <= nvert + 2){ //On lines 2 to 2 + vert process vertices
            float x = stof(tokens[0]);
            float y = stof(tokens[1]);
            float z = stof(tokens[2]);

            printf("%f %f %f\n", x, y, z);  //Debug print

            glm::vec3 vector(x,y,z);
            vertices.push_back(vector);

            // check mins and max
            if (x < minX ) minX = x;
            if (x > maxX ) maxX = x;
            if (y < minY ) minY = y;
            if (y > maxY ) maxY = y;
            if (z < minZ ) minZ = z;
            if (z > maxZ ) maxZ = z;
        }
        else{                                     //On lines 2+vert to the end we add to the faces
           unsigned int first  = stoi(tokens[1]); //Ignore the first number, we only deal with triangles
           unsigned int second = stoi(tokens[2]);
           unsigned int third  = stoi(tokens[3]);

           printf("%d %d %d\n", first, second, third); //Debug print

           glm::vec3 vector_f(first, second, third);
           faces.push_back(vector_f);         //Push back to the faces list
    
           //Offset to start at 0 for the face indices
           int off = 3+nvert;

           vertex_face_adj[first].push_back(lines_read - off); //Maintain our vertex to face adj
           vertex_face_adj[second].push_back(lines_read - off);
           vertex_face_adj[third].push_back(lines_read - off);

           faces_flat.push_back(first);     //Mantain a flat list also for drawing
           faces_flat.push_back(second);
           faces_flat.push_back(third);
        }

        tokens.clear();              //Remember to clear tokens every loop!
    }

    //This finally centers the object
    // Take the midrange average of each coordinate
    avgX = 0.5 * (maxX + minX);
    avgY = 0.5 * (maxY + minY);
    avgZ = 0.5 * (maxZ + minZ);
    cout << "avgs" <<  avgX << " " << avgY << " " << avgZ << endl;
    cout << "avgs" <<  maxX << " " << maxY << " " << maxZ << endl;
    
    float biggestAxis = max({maxX-minX, maxY-minY, maxZ-minZ});

    cout << "Updating vertices..." << endl;
    // 2nd Pass, update the vertices by subtracting avg - coord val
    // Center and scales!
    for (int i = 0; i < vertices.size(); ++i){
        vertices[i].x -= avgX;
        vertices[i].y -= avgY;
        vertices[i].z -= avgZ;
        vertices[i].x /= biggestAxis;
        vertices[i].y /= biggestAxis;
        vertices[i].z /= biggestAxis;
    }

    calculate_normals();
    
    file.close();
}


void off_model::calculate_normals(){
    for(int i = 0; i < vertices.size(); ++i){
        //Store all the shared faces
        glm::vec3 vertex_normal;
        for(auto face : faces){
            //If vertex is in this face
            if((int)face.x == i || (int)face.y == i || (int)face.z == i){
                glm::vec3 edge_1 = vertices[(int)face.y] - vertices[(int)face.x];
                glm::vec3 edge_2 = vertices[(int)face.z] - vertices[(int)face.x];

                vertex_normal += glm::normalize(glm::cross(edge_1, edge_2));
            }
        }
        //Now that the vertex normal is computed and added for each shared face
        //normalize the end result
        vertex_normal = glm::normalize(vertex_normal);
        normals.push_back(vertex_normal);
    }
}

void off_model::draw(GLuint shader){
    //Calculate the model and view (cam inverse) matrix
    glm::mat4 modelview = Window::V * toWorld;

    //Forward variables to the shader program
    uModel      = glGetUniformLocation(shader, "model");
    uProjection = glGetUniformLocation(shader, "projection");
    uModelview  = glGetUniformLocation(shader, "modelview");
    
    //Actually send those variables now
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &this->toWorld[0][0]);

    //Actually draw
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_LOOP, (unsigned int)faces_flat.size(), GL_UNSIGNED_INT, 0);
       
}

void off_model::scale(bool up){
    if(up){
        toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f,1.5f,1.5f));
    }
    else{
        toWorld = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f,0.5f,0.5f));
    }
}
