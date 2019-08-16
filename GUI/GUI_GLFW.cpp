#include "GUI_GLFW.h"

int GUI_GLFW::mX = 0;
int GUI_GLFW::mY = 0;

bool GUI_GLFW::mClosef = false;

bool GUI_GLFW::mLeftClickf = false;
bool GUI_GLFW::mCancelf = false;

bool GUI_GLFW::mSpacePressf = false;

GUI_GLFW::GUI_GLFW() :
    mScreenWidth(1280), mScreenHeight(900), mCellWidth(100), mCellHeight(360),
    mTokenRadius(40), mBarWidth(80)
{
    srand(time(0));
        
    mColors[HIGHLIGHTED] = "#B4D5FF";
    mColors[IDLE] = "#381704";
    mColors[SELECTED] = "#00FF00";
    mColors[BLACK] = 0x000000;
    mColors[WHITE] = 0xFFFFFF;
    
    // Initialize the library
    glfwInit();

    /*if (!glfwInit())
      return -1;*/

    // Create a windowed mode window and its OpenGL context
    mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, "Backgammon!", NULL, NULL);

    if (!mWindow)
    {
        glfwTerminate();
        /*return -1;*/
    }

    glfwSetCursorPosCallback(mWindow, CursorPositionCallback);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);
    glfwSetInputMode(mWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    glfwSetKeyCallback(mWindow, KeyboardKeyCallback);
    glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // Make the window's context current
    glfwMakeContextCurrent(mWindow);

    // Apply standard coordinate system
    glViewport(0.0f, 0.0f, mScreenWidth, mScreenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, mScreenWidth, mScreenHeight, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GUI_GLFW::PollEvents()
{
    glfwPollEvents();
}

void GUI_GLFW::UpdateInteface(PROGRAM_MODE CURRENT_MODE, Cell *board)
{
    mLeftClickf = false;
    mCancelf = false;
    mSpacePressf = false;

    DrawBoard(CURRENT_MODE, board);
    glfwSwapBuffers(mWindow);
}

bool GUI_GLFW::Clicked()
{
    return mLeftClickf;
}

bool GUI_GLFW::Cancelled()
{
    return mCancelf;
}

bool GUI_GLFW::Rolled()
{
    return mSpacePressf;
}

bool GUI_GLFW::Closed()
{
    return glfwWindowShouldClose(mWindow);
}

void GUI_GLFW::CursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    mX = xpos, mY = ypos;
    //std::cout << " (" << xpos << ", " << ypos << ")\n";
}

void GUI_GLFW::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        if (action == GLFW_PRESS)
            mLeftClickf = true;

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        if (action == GLFW_PRESS)
            mCancelf = true;
}

void GUI_GLFW::KeyboardKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE)
        if (action == GLFW_PRESS)
            mSpacePressf = true;

    if (key == GLFW_KEY_ESCAPE)
        if (action == GLFW_PRESS)
            mCancelf = true;
}

GUI_GLFW::~GUI_GLFW()
{
    glfwTerminate();
}

void GUI_GLFW::DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell *board)
{
    switch (CURRENT_MODE)
    {
    case STANDBY:
        SetColor("#F1C38E", true);
        break;

    case PIECE_SELECTION:
        SetColor(0xD26AD9, true);
        break;

    case MOVE_SELECTION:
        SetColor("#0BA494", true);
        break;
        
    case BO_MOVE_SELECTION:
        SetColor(0xFF0000, true);
        break;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3);

    for (int i = 0; i < 24; i++)
        DrawCell(i, board[i]);

    // Middle bar:
    SetColor(mColors[IDLE]);
    DrawRectangle({ (mScreenWidth - mBarWidth + 16) / 2, 0 }, { (mScreenWidth + mBarWidth - 16) / 2, mScreenHeight });

    glLineWidth(4);
    SetColor("#000000");
    DrawLine({ (mScreenWidth - mBarWidth + 16) / 2, 0 }, { (mScreenWidth - mBarWidth + 16) / 2, mScreenHeight }); //Left
    DrawLine({ (mScreenWidth + mBarWidth - 16) / 2, 0 }, { (mScreenWidth + mBarWidth - 16) / 2, mScreenHeight }); //Right
    DrawLine({ (mScreenWidth) / 2, 0 }, { (mScreenWidth) / 2, mScreenHeight }); //Center

}

void GUI_GLFW::DrawCell(int i, const Cell &c)
{
    int quad = i / 6;
    Color cellColor = mColors[c.status];

    i %= 6;

    Point p1 = { mCellWidth * i, 0 };
    Point p2 = { mCellWidth * (1 + i), 0 };
    Point p3 = { mCellWidth * (0.5 + i), mCellHeight };


    switch (quad)
    {
    case 0:
        //Top Left:
        break;

    case 1:
        //Top Right:
        p1.x += 6 * mCellWidth + mBarWidth;
        p2.x += 6 * mCellWidth + mBarWidth;
        p3.x += 6 * mCellWidth + mBarWidth;
        break;

    case 2:
        //Bottom Right
        i = 5 - i;

        p1.x = mCellWidth * i;
        p2.x = mCellWidth * (1 + i);
        p3.x = mCellWidth * (0.5 + i);

        p1.x += 6 * mCellWidth + mBarWidth;
        p2.x += 6 * mCellWidth + mBarWidth;
        p3.x += 6 * mCellWidth + mBarWidth;

        p1.y = p2.y = mScreenHeight;
        p3.y = mScreenHeight - mCellHeight;
        break;

    case 3:
        //Bottom Left
        i = 5 - i;

        p1.x = mCellWidth * i;
        p2.x = mCellWidth * (1 + i);
        p3.x = mCellWidth * (0.5 + i);

        p1.y = p2.y = mScreenHeight;
        p3.y = mScreenHeight - mCellHeight;
        break;
    }

    SetColor(cellColor);
    DrawTriangle(p1, p2, p3);

    SetColor("#000000");
    DrawLine(p1, p3), DrawLine(p2, p3);
}

void GUI_GLFW::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    float vertices[] =
        {
            x1, y1,     0.0,
            x2, y2,     0.0,
            x3, y3,     0.0
        };

    glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute

    glVertexPointer(3, GL_FLOAT, 0, vertices); // point to the vertices to be used
    glDrawArrays(GL_TRIANGLES, 0, 3); // draw the vertices

    glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex array attributes
}

void GUI_GLFW::DrawTriangle(Point p1, Point p2, Point p3)
{
    DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void GUI_GLFW::DrawRectangle(int x1, int y1, int x2, int y2)
{
    float vertices[] =
        {
            x1, y1, 0.0,
            x2, y1, 0.0,
            x2, y2, 0.0,
            x1, y2, 0.0,
        };

    glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute

    glVertexPointer(3, GL_FLOAT, 0, vertices); // point to the vertices to be used
    glDrawArrays(GL_QUADS, 0, 4); // draw the vertices

    glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex array attributes
}

void GUI_GLFW::DrawRectangle(Point p1, Point p2)
{
    DrawRectangle(p1.x, p1.y, p2.x, p2.y);
}

void GUI_GLFW::DrawLine(int x1, int y1, int x2, int y2)
{
    float vertices[] =
        {
            x1, y1, 0.0,
            x2, y2, 0.0,
        };

    glEnable(GL_LINE_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute

    glVertexPointer(3, GL_FLOAT, 0, vertices); // point to the vertices to be used
    glDrawArrays(GL_LINES, 0, 2); // draw the vertices

    glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex array attributes
    glDisable(GL_LINE_SMOOTH);
}

void GUI_GLFW::DrawLine(Point p1, Point p2)
{
    DrawLine(p1.x, p1.y, p2.x, p2.y);
}

void GUI_GLFW::DrawCircle(int x, int y, int radius, bool color)
{
    if (color)
        SetColor("#000000");
    else
        SetColor("#FFFFFF");

    const int numberOfSides = 360;
    const int numberOfVertices = numberOfSides + 2;
    const double myPI = 3.1415926535897;

    GLfloat twicePi = 2.0f * myPI;

    GLfloat circleVerticesX[numberOfVertices];
    GLfloat circleVerticesY[numberOfVertices];
    GLfloat circleVerticesZ[numberOfVertices];

    circleVerticesX[0] = x;
    circleVerticesY[0] = y;
    circleVerticesZ[0] = 0;

    for (int i = 1; i < numberOfVertices; i++)
    {
        circleVerticesX[i] = x + (radius * cos(i *  twicePi / numberOfSides));
        circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
        circleVerticesZ[i] = 0;
    }

    GLfloat allCircleVertices[(numberOfVertices) * 3];

    for (int i = 0; i < numberOfVertices; i++)
    {
        allCircleVertices[i * 3] = circleVerticesX[i];
        allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
        allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GUI_GLFW::DrawCircle(Point p, int radius, bool color)
{
    DrawCircle(p.x, p.y, radius, color);
}

void GUI_GLFW::SetColor(int r, int g, int b, bool background)
{
    if (background)
        glClearColor(r / 255.0, g / 255.0, b / 255.0, 1);

    else
        glColor3f(r / 255.0, g / 255.0, b / 255.0);
}

void GUI_GLFW::SetColor(Color c, bool background)
{
    SetColor(c.r, c.g, c.b, background);
}

float GUI_GLFW::TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

float GUI_GLFW::TriangleArea(Point p1, Point p2, Point p3)
{
    return TriangleArea(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

int GUI_GLFW::CellNumber()
{
    // P (mX, mY)
    Point P = { mX, mY };

    float At, A1, A2, A3;

    for (int i = 0; i < 6; i++)
    {
        //Top Left:
        Point A = { mCellWidth * i, 0 };
        Point B = { mCellWidth * (1 + i), 0 };
        Point C = { mCellWidth * (0.5 + i), mCellHeight };

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return i;

        //Top Right:
        A.x += 6 * mCellWidth + mBarWidth;
        B.x += 6 * mCellWidth + mBarWidth;
        C.x += 6 * mCellWidth + mBarWidth;

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return i + 6;

        //Bottom Right:
        A.y = B.y = mScreenHeight;
        C.y = mScreenHeight - mCellHeight;

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return 17 - i;

        //Bottom Left
        A.x -= 6 * mCellWidth + mBarWidth;
        B.x -= 6 * mCellWidth + mBarWidth;
        C.x -= 6 * mCellWidth + mBarWidth;

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return 23 - i;
    }

    return -1;
}
