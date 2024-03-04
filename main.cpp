#include "header.h"

std::vector<coord3D> points{ // Defining the points of the cube
    coord3D{-1.0, -1.0, -1.0}, coord3D{ 1.0, -1.0, -1.0},
    coord3D{-1.0,  1.0, -1.0}, coord3D{-1.0, -1.0,  1.0},
    coord3D{ 1.0,  1.0, -1.0}, coord3D{-1.0,  1.0,  1.0},
    coord3D{ 1.0, -1.0,  1.0}, coord3D{ 1.0,  1.0,  1.0}
};

std::vector<line> lines{ // Defining the lines that need to be drawn. A line has a start and end point.
    line{0, 1}, line{0, 2}, line{0, 3}, // 3 lines from (-1, -1, -1)
    line{7, 5}, line{7, 4}, line{7, 6}, // 3 lines from (1, 1, 1)
    line{5, 2}, line{5, 3}, // 2 lines from (-1, 1, 1)
    line{4, 2}, line{4, 1}, // 2 lines from (1, 1, -1)
    line{6, 3}, line{6, 1} // 2 lines from (1, -1, 1)
};

std::vector<coord3D> tetrahedronPoints{ // Defining the points of the cube
    coord3D{ 1.0,  1.0,  1.0}, coord3D{ 1.0, -1.0, -1.0},
    coord3D{-1.0,  1.0, -1.0}, coord3D{-1.0, -1.0,  1.0}
};

std::vector<line> tetrahedronLines{
    line{0, 1}, line{0, 2}, line{0, 3},
    line{1, 2}, line{1, 3}, line{2, 3}

};


// matrix multiplication: http://matrixmultiplication.xyz/
coord2D projection(coord3D point){
    // {1, 0, 0},
    // {0, 1, 0} is the projection matrix
    return coord2D{point.x, point.y}; // Orthographic projection
}

coord3D rotateX(coord3D point, float angle){
    return coord3D{
        1 * point.x + 0 * point.y          + 0 * point.z,
        0 * point.x + cos(angle) * point.y - sin(angle) * point.z,
        0 * point.x + sin(angle) * point.y + cos(angle) * point.z
    };
}

coord3D rotateY(coord3D point, float angle){
    return coord3D{
         cos(angle) * point.x + 0 * point.y + sin(angle) * point.z,
         0 * point.x          + 1 * point.y + 0 * point.z,
        -sin(angle) * point.x + 0 * point.y + cos(angle) * point.z
    };
}

coord3D rotateZ(coord3D point, float angle){
    return coord3D{
        cos(angle) * point.x - sin(angle) * point.y + 0 * point.z,
        sin(angle) * point.x + cos(angle) * point.y + 0 * point.z,
        0 * point.x          + 0 * point.y          + 1 * point.z
    };
}

coord3D rotate(coord3D point, float angleX, float angleY, float angleZ){
    point = rotateX(point, angleX);
    point = rotateY(point, angleY);
    point = rotateZ(point, angleZ);
    return point;
}

void drawCube(SDL_Renderer* renderer, float &x_rot, float &y_rot, float &z_rot){
    for(auto line : lines){ // Loop through all the lines
        coord3D start3DPoint = rotate(points[line.start], x_rot, y_rot, z_rot);
        coord3D end3DPoint = rotate(points[line.end], x_rot, y_rot, z_rot);

        coord2D startPoint = projection(start3DPoint);
        coord2D endPoint = projection(end3DPoint);

        int startX = MID_X + CUBE_LENGTH * startPoint.x;
        int startY = MID_Y + CUBE_LENGTH * startPoint.y;
        int endX = MID_X + CUBE_LENGTH * endPoint.x;
        int endY = MID_Y + CUBE_LENGTH * endPoint.y;

        SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
    }
}

void drawTetrahedron(SDL_Renderer* renderer, float &x_rot, float &y_rot, float &z_rot){
    for(auto line : tetrahedronLines){ // Loop through all the lines of tetrahedron
        coord3D start3DPoint = rotate(tetrahedronPoints[line.start], x_rot, y_rot, z_rot);
        coord3D end3DPoint = rotate(tetrahedronPoints[line.end], x_rot, y_rot, z_rot);

        coord2D startPoint = projection(start3DPoint);
        coord2D endPoint = projection(end3DPoint);

        int startX = MID_X + CUBE_LENGTH * startPoint.x;
        int startY = MID_Y + CUBE_LENGTH * startPoint.y;
        int endX = MID_X + CUBE_LENGTH * endPoint.x;
        int endY = MID_Y + CUBE_LENGTH * endPoint.y;

        SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
    }
}

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    float x_rot = 1.2;
    float y_rot = 6.4;
    float z_rot = -20.1;

    // Create the window
    window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool isRunning = true;

    while(isRunning){ // Main loop & event handler
        Uint64 start = SDL_GetPerformanceCounter();
        
        // Event loop
        if (SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) isRunning = false;
        }

        // Render loop
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Background colour
        SDL_RenderClear(renderer); // Clear everything & draw background

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Render colour to whatever

        drawCube(renderer, x_rot, y_rot, z_rot);

        SDL_RenderPresent(renderer); // Update screen

        // Update rotations
        x_rot += 0.01;
        y_rot += 0.05;
        z_rot += 0.03;

        Uint64 end = SDL_GetPerformanceCounter();
        
        // Cap to 60fps
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS));
        
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}