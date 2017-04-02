/** Include Files **/
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

/** Namespaces **/
using namespace std;


/**
    macro for the random value
**/
#define RANDOM  ((float)randomValue/450.0)


/**
    Returns a random number between fMin and fMax
**/
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

/**
    Structure to store the coordinates of a point
**/
typedef struct Coordinate{
    float x;
    float y;

    Coordinate(float _x, float _y): x(_x), y(_y){}
    Coordinate(){}

}Coordinate;

/**
    Name of the file from which input points are to be taken

**/
char file[1000];


/**
    Total number of points taken as input

    * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *                 Format of the file                  *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    * <Total Number of points>                            *
    * <point.x>     <point.y>                             *
    * <point.x>     <point.y>                             *
    * <point.x>     <point.y>                             *
    * .             .                                     *
    * .             .                                     *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    The x and y Coordinates of the points must lie in the interval [-1,1].
**/
int totalPoints;


/**
    Coordinates of points taken as input from the input file.
**/
std::vector<Coordinate> inputCoordinates;


/**
    Time value for the final hand position while drawing a line between
    a pair of points.
    Empirically that finalTime = 2.0 provides satisfactory results.
**/
int finalTime = 2;


/**
    The value of randomness in pixels.
    Empirically found a range [-5,+5] worked regardless of length of the lines.
**/
int randomValue = 5;


/**

    The value of dt(delta t) that is going to be used in the program.

    * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *    Empirical Values for the time step dt(delta t)   *
    *         Given finaltime is taken to be 2.0          *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *   Approx. Line Length in pixels   *   Time step Dt  *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *        [0;200]                    *       0.5       *
    *       (200;400]                   *       0.3       *
    *        > 400                      *       0.2       *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * *

**/
float dt = 0.2;


/**
    The number of points to be plotted between pair of control verties while constructing the Catmull-Rom Splines.
**/
int splineCurveParameter = 50;

/**
    The number of control points we get for a pair of points while drawing a line between them.
**/
int pointsPerLine;

/** Total number of final vertices that are used to draw lines **/
int totalVerticesToDraw = 0;

/** Final vertices that are used to draw lines **/
float* verticesToDraw;



void initialize(){

    float quo = ((float)finalTime)/dt;
    float floatVal = quo - (int)quo;
    pointsPerLine = 2 + quo;
    if(floatVal == 0)   pointsPerLine -= 1;

    verticesToDraw = new float[totalPoints*(4*splineCurveParameter)*(pointsPerLine-3)]();

    Coordinate firstSplineCurveCoordinate;

    for(int j = 0; j < inputCoordinates.size(); j++){
        /**
            Vector to store all the control points between a pair of points.
        **/
        std::vector<Coordinate> tempCoordinates;

        float x0 = inputCoordinates[j].x;
        float y0 = inputCoordinates[j].y;
        float xf = inputCoordinates[(j+1)%(totalPoints)].x;
        float yf = inputCoordinates[(j+1)%(totalPoints)].y;

        float t = dt;

        tempCoordinates.push_back(Coordinate(x0,y0));

        while(t < (float)finalTime){
            float tao = (t/(float)finalTime);
            float v1 = (fRand(-1.0*RANDOM, 1.0*RANDOM));

            /*Fifth order polynomial to get the control points between a pair of points (x0, y0) and (xf, yf)
              Reference - Zainab Meraj, Brian Wyvill, Tobias Isenberg, Amy A. Gooch, Richard Guy: Mimicking Hand-Drawn Pencil Lines
                        Computational Aesthetics in Graphics, Visualization, and Imaging (2008)
            */
            float xt = x0 + (x0 - xf)*(15*pow(tao,4) - 6*pow(tao,5) - 10*pow(tao,3)) + v1;
            float yt = y0 + (y0 - yf)*(15*pow(tao,4) - 6*pow(tao,5) - 10*pow(tao,3)) + v1;

            tempCoordinates.push_back(Coordinate(xt,yt));
            t+=dt;
        }
        tempCoordinates.push_back(Coordinate(xf,yf));

        /**
            CATMULL-ROM Spline Implementation
        **/
        for(int i = 1; i < tempCoordinates.size()-2;i++)
        {
            for(int k = 0;  k < splineCurveParameter; k++){

                float t = k*(1.0/splineCurveParameter);

                float xcr = tempCoordinates[i].x + 0.5*t*(-tempCoordinates[i-1].x+tempCoordinates[i+1].x)
                + t*t*(tempCoordinates[i-1].x - 2.5*tempCoordinates[i].x + 2*tempCoordinates[i+1].x - 0.5*tempCoordinates[i+2].x)
                + t*t*t*(-0.5*tempCoordinates[i-1].x + 1.5*tempCoordinates[i].x - 1.5*tempCoordinates[i+1].x + 0.5*tempCoordinates[i+2].x);
                float ycr = tempCoordinates[i].y + 0.5*t*(-tempCoordinates[i-1].y+tempCoordinates[i+1].y)
                + t*t*(tempCoordinates[i-1].y - 2.5*tempCoordinates[i].y + 2*tempCoordinates[i+1].y - 0.5*tempCoordinates[i+2].y)
                + t*t*t*(-0.5*tempCoordinates[i-1].y + 1.5*tempCoordinates[i].y - 1.5*tempCoordinates[i+1].y + 0.5*tempCoordinates[i+2].y);

                if((k==splineCurveParameter-1 && i==tempCoordinates.size()-3)){
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                }
                else if(j!=0 && (i==1 && k==0)){
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                }
                else if((i==1 && k==0) || (k==splineCurveParameter-1 && i==tempCoordinates.size()-3)){
                    if((i==1 && k==0) && (j==0)){
                        firstSplineCurveCoordinate.x = xcr;
                        firstSplineCurveCoordinate.y = ycr;
                    }
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                }
                else{
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                    verticesToDraw[totalVerticesToDraw++] = xcr;
                    verticesToDraw[totalVerticesToDraw++] = ycr;
                }
            }
        }

    }
    verticesToDraw[totalVerticesToDraw++] = firstSplineCurveCoordinate.x;
    verticesToDraw[totalVerticesToDraw++] = firstSplineCurveCoordinate.y;
}


int main(int argc, char const *argv[]) {
    srand (time(NULL));


    cout << endl << endl << "*-*-*-*-*-*-*-Artistic Line Drawing-*-*-*-*-*-*-*" << endl << endl;

    cout << " Points File Path : ";
    cin  >> file ;
    cout << endl;

    /*** Reading the points file ***/
    ifstream myfile (file);
    string line;

    if (myfile.is_open())
    {
        myfile >> totalPoints;
        int n1 = totalPoints;
        float x,y;
        while(n1--){
            Coordinate c;
            myfile >> c.x >> c.y;
            inputCoordinates.push_back(c);
        }
        myfile.close();
    }
    else{
        cout << " Unable to open file, try again!" << endl << endl;
        cout << "*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        return -1;
    }
    /********************************/


    cout << " Final Time( <int> ideally 2) : ";
    cin  >> finalTime ;
    cout << endl;
    if( finalTime <= 0 )
    {
        cout << " Final time can't be less than or equal to zero, try again!" << endl << endl;
        cout << "*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        return -1;
    }

    cout << " Time increment( <float> ideally 0.2) : ";
    cin  >> dt ;
    cout << endl;
    if(dt <= 0 || dt >= finalTime || finalTime/dt <= 2.0){
        cout << " Time increment value of out of bounds, try again!" << endl << endl;
        cout << "*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        return -1;
    }


    cout << " Randomness( <int> ideally 5) : ";
    cin  >> randomValue ;
    cout << endl;
    if(randomValue <= 0){
        cout << " Randomness can't be less than or equal to zero, try again!" << endl << endl;
        cout << "*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        return -1;
    }


    cout << " Spline Curve Parameter( <int> ideally 50) : ";
    cin  >> splineCurveParameter ;
    cout << endl;

    if(splineCurveParameter <= 2){
        cout << " Spline Curve Parameter value out of range, try again!" << endl << endl;
        cout << "*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        return -1;
    }

    initialize();

    cout << "*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl << endl << endl;

    /** Initialization Code for Opengl  --Start-- **/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(1200, 900, "OpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    /** Initialization Code for Opengl --End--  **/

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, totalPoints*(4*splineCurveParameter)*(pointsPerLine-3)*sizeof(float), verticesToDraw, GL_STATIC_DRAW);

    char *vertexShaderString = "#version 150\nin vec2 position;\nvoid main(){\ngl_Position = vec4(position,0.0, 1.0);\n}";
    char *fragmentShaderString = "#version 150\nout vec4 outColor;\nvoid main(){\noutColor = vec4(1.0, 1.0, 1.0, 1.0);\n}";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
    glCompileShader(vertexShader);

    //Start Checking for errors
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE){
        printf("Error in compilation of vertex shader source.\n");
    }
    //End Checking errors

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);

    //Start Checking for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE){
        printf("Error in compilation of fragment shader source.\n");
    }
    //End Checking errors

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);


    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);


    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);


    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);

    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

        glDrawArrays(GL_LINES, 0, totalVerticesToDraw);
    }

    glfwTerminate();
    return 0;
}
