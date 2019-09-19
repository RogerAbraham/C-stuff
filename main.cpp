
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <math.h>
using namespace std;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static float SCREENX = GetSystemMetrics(SM_CXSCREEN);
static float SCREENY = GetSystemMetrics(SM_CYSCREEN);
static float APPX = SCREENX /6;
static float APPY = SCREENY / 2.42;
static float nResult = 0.0;
static vector <string> sResult;
static string temp = " ";
static float f = 0.0f;
static int counter = 0;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void calculate(string &str);
// Main code
int main(int, char**)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    FreeConsole();
    
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("COMP3141 Calculator Final Project (Rogers Abraham)"), WS_OVERLAPPEDWINDOW, 100, 100, APPX, APPY, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    { 
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //if (show_demo_window)
          //ImGui::ShowDemoWindow(&show_demo_window);

        // =============CALCULATOR====================       
        ImGui::Begin(" ",NULL,ImGuiWindowFlags_NoBringToFrontOnFocus);                          


            ImGui::SetWindowFontScale(1.6);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 0.5f), "%s", temp.c_str()); // CALCULATOR TOP OUTPUT RESULT SCREEN TEXT
            ImGui::Separator();

            //BUTTON CREATION AND VALUE ASSIGNMENT

                if (ImGui::Button("7", ImVec2(40.0f, 50.0f)))
                {
                    temp += "7";
                   // sResult.push_back(temp);
                }
                ImGui::SameLine();

                if (ImGui::Button("8", ImVec2(40.0f, 50.0f))) {

                    temp += "8";
                }

                ImGui::SameLine();

                if (ImGui::Button("9", ImVec2(40.0f, 50.0f)))
                    temp += "9";

                ImGui::SameLine();

                if (ImGui::Button("x", ImVec2(40.0f, 50.0f)))
                    temp += "x";

                if (ImGui::Button("4", ImVec2(40.0f, 50.0f)))
                    temp += "4";

                ImGui::SameLine();

                if (ImGui::Button("5", ImVec2(40.0f, 50.0f)))
                    temp += "5";
                ImGui::SameLine();

                if (ImGui::Button("6", ImVec2(40.0f, 50.0f)))
                    temp += "6";
                ImGui::SameLine();

                if(ImGui::Button("-", ImVec2(40.0f, 50.0f)))
                  temp += "-";

                if (ImGui::Button("1", ImVec2(40.0f, 50.0f)))
                    temp += "1";

                ImGui::SameLine();

                if (ImGui::Button("2", ImVec2(40.0f, 50.0f)))
                    temp += "2";

                ImGui::SameLine();

                if (ImGui::Button("3", ImVec2(40.0f, 50.0f)))
                    temp += "3";
                ImGui::SameLine();

                if (ImGui::Button("+", ImVec2(40.0f, 50.0f)))
                    temp += "+";

                if (ImGui::Button("0", ImVec2(40.0f, 50.0f)))
                    temp += "0";
                ImGui::SameLine();

                if (ImGui::Button("sqt", ImVec2(40.0f, 50.0f)))
                    temp += "sqt";
                ImGui::SameLine();

                if (ImGui::Button("sqr", ImVec2(40.0f, 50.0f)))
                    temp += "sqr";
                ImGui::SameLine();

                if (ImGui::Button("/", ImVec2(40.0f, 50.0f)))
                    temp += "/";

                if (ImGui::Button("=", ImVec2(40.0f, 50.0f)))
                   calculate(temp);
                    //BEDMAS > BRACKTS > EXPONENT > DIVISION > MULT > ADD > SUB
         
                ImGui::SameLine();

                if (ImGui::Button("CLR", ImVec2(40.0f, 50.0f)))
                    temp = " ";

                ImGui::SameLine();

                static bool check = false; // need this for checkbox state

                if (ImGui::Button("DEL", ImVec2(40.0f, 50.0f)))
                    if (temp.length() >1) {
                        temp.pop_back();
                    }
                ImGui::SameLine();
               
                ImGui::Checkbox("MODE", &check);

                ImGui::Separator();

                if (ImGui::CollapsingHeader("ADVANCED", &check))
                {
                    ImGui::SetWindowFontScale(1.0);

                    if(ImGui::Button("SIN", ImVec2(40.0f, 40.0f)))
                        temp += "SIN";
                    ImGui::SameLine();
                    if(ImGui::Button("COS", ImVec2(40.0f, 40.0f)))
                        temp += "COS";
                    ImGui::SameLine();
                    if(ImGui::Button("TAN", ImVec2(40.0f, 40.0f)))
                        temp += "TAN";


              //  }
                // ImGui::Text("counter = %d", counter);

            }
            ImGui::End();
        

  
        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

void calculate(string& str) {


    //So wee to calculate some string of operators and integers
    //BEDMAS
    // We have exponents SQR, SQT, SIN COS TAN, div, mul, add, sub
    // we find the highest operator, execute it on the term between it (index-1) and and last found operator(index+1)operator then recreate then remove the operator and operand fro mthe string and replace it with the calculated value
    string match = "nrSCTsct/x+- ";
    //string match = "+-/x";
    int first = str.find_first_of(match);
    int next = str.find_first_of(match, first + 1);

    while (next != -1) { // keep going until were down to just a number

       
        // ==========================SQUAAAAAAAAAAAAAAAAAAAAAAARREINGGGGG==============================
        int next_sqr = str.find("sqr");
        
        while (next_sqr != -1 && !isdigit(str[next_sqr - 1])) {

            int nextTok = str.find_first_of(match, next_sqr + 3); //The index of the next token: sqr, sqt, sin, cos, tan, /, x, +, -
            int nextnextTok = str.find_first_of(match, nextTok + 1); //for the negative case we need to check the next next token

            //No next token(sqr) OR a negative term with no Next token(term)
            if (nextTok == -1 || ((nextTok - (next_sqr+2)) == 1 && (nextnextTok == -1)))  { // if (nextTok == -1 || nextTok-next_sqr == 1 && nextnextTok == -1)) 

                if (str[str.length() - 1] == 'r') {//guard agaisnt unused square root

                    str = "ERROR:operator[value]";
                    return;
                }

                if (nextTok == -1) { //theres no next token we know its just positive term
                    string pinch = str.substr(next_sqr + 3, str.length());
                    float num = atof(pinch.c_str()); // this gets the value
                    num *= num; // perform the square
                    str.replace(next_sqr, str.length(), std::to_string(num));
                    next_sqr = str.find("sqr");
                    break;
                    
                }

                if ((nextTok - (next_sqr + 2)) == 1 && nextnextTok == -1) { //Negative case with no right side toekn

                    string pinch = str.substr(next_sqr + 4, str.length());
                    float num = (atof(pinch.c_str()))*-1; // this gets the value and negates it
                    num *= num; // perform the square
                    str.replace(next_sqr, str.length(), std::to_string(num)); // the same replace shoulkd work
                    //Update
                    next_sqr = str.find("sqr");
                    if (next_sqr != -1) {
                        nextTok = str.find_first_of(match, next_sqr + 3);
                        nextnextTok = str.find_first_of(match, nextTok + 1);
                    }

                }

                break;
            }

            //Negative case with right side token
            if ((nextTok - (next_sqr + 2)) == 1 && nextnextTok != -1) { //Negative case with right side token

                string pinch = str.substr(next_sqr + 4, (nextnextTok - nextTok)); // should be the value between negative signs
                float num = (atof(pinch.c_str())) * -1; // this gets the value and negates it
                num *= num; // perform the square
                str.replace(next_sqr, (nextnextTok - next_sqr) - 1, "");
                str.insert(next_sqr, std::to_string(num));

                //update
                next_sqr = str.find("sqr");
                if (next_sqr != -1) {
                    nextTok = str.find_first_of(match, next_sqr + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }

                break;

            }

            else { //So theres a token on the right and no negative sign after sqr > sqr46

                //Check to see that there is a digit on the right of the next operand otherwise we error

              
                string pinch = str.substr(next_sqr + 3, (nextTok - (next_sqr + 3)));
                float num = atof(pinch.c_str()); // this gets the value
                num *= num; // perform the square
               
                str.replace(next_sqr, nextTok - next_sqr, "");
                str.insert(next_sqr, std::to_string(num));
                //update
                next_sqr = str.find("sqr");
                if (next_sqr != -1) {
                    nextTok = str.find_first_of(match, next_sqr + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }
            }

            //update
            next_sqr = str.find("sqr");
            if (next_sqr != -1) {
                nextTok = str.find_first_of(match, next_sqr + 3);
                nextnextTok = str.find_first_of(match, nextTok + 1);
            }
        }

        int next_sqt = str.find("sqt");
        // ==========================ROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOT==============================
        while (next_sqt != -1 && !isdigit(str[next_sqt - 1])) {

            int nextTok = str.find_first_of(match, next_sqt + 3); //The index of the next token: sqr, sqt, sin, cos, tan, /, x, +, -
            int nextnextTok = str.find_first_of(match, nextTok + 1);
            if (nextTok == -1 || ((nextTok - (next_sqt + 2)) == 1 && (nextnextTok == -1))) { // Means there are no more tokens to the right, so we just calculate and replace

                if (str[str.length() - 1] == 't') {//guard agaisnt unused square root

                    str = "ERROR:operator[value]";
                    return;
                }
                if (nextTok == -1) {
                    string pinch = str.substr(next_sqt + 3, str.length());
                    float num = sqrt(atof(pinch.c_str())); // this gets the value               
                    str.replace(next_sqt, str.length(), std::to_string(num));
                }

                if ((nextTok - (next_sqt + 2)) == 1 && nextnextTok == -1) {

                    str = "ERROR:No Neg ROOT";
                    return;
                }
            }



            else { //So theres a token on the right and before that token is our term that we need to calc and replace

                //Check to see that there is a digit on the right of the next operand otherwise we error


                string pinch = str.substr(next_sqt + 3, (nextTok - (next_sqt + 3)));
                float num = sqrt(atof(pinch.c_str())); // this gets the value                                       
                str.replace(next_sqt, nextTok - next_sqt, "");
                str.insert(next_sqt, std::to_string(num));
            }

            next_sqt = str.find("sqt");
        }

        int next_sin = str.find("SIN");
        //====================================SIN==========================
        while (next_sin != -1 && !isdigit(str[next_sin - 1])) {

            int nextTok = str.find_first_of(match, next_sin + 3); //The index of the next token: SIN, sqt, sin, cos, tan, /, x, +, -
            int nextnextTok = str.find_first_of(match, nextTok + 1); //for the negative case we need to check the next next token

            //No next token(SIN) OR a negative term with no Next token(term)
            if (nextTok == -1 || ((nextTok - (next_sin + 2)) == 1 && (nextnextTok == -1))) { // if (nextTok == -1 || nextTok-next_sin == 1 && nextnextTok == -1)) 

                if (str[str.length() - 1] == 'r') {//guard agaisnt unused square root

                    str = "ERROR:operator[value]";
                    return;
                }

                if (nextTok == -1) { //theres no next token we know its just positive term
                    string pinch = str.substr(next_sin + 3, str.length());
                    float num = sin(atof(pinch.c_str())); // this gets the value
                  
                    str.replace(next_sin, str.length(), std::to_string(num));
                    next_sin = str.find("SIN");
                    break;

                }

                if ((nextTok - (next_sin + 2)) == 1 && nextnextTok == -1) { //Negative case with no right side toekn

                    string pinch = str.substr(next_sin + 4, str.length());
                    float num = sin((atof(pinch.c_str()))) * -1; // this gets the value and negates it
                    ////FORGOT LOL num; // perform the square
                    str.replace(next_sin, str.length(), std::to_string(num)); // the same replace shoulkd work
                    //Update
                    next_sin = str.find("SIN");
                    if (next_sin != -1) {
                        nextTok = str.find_first_of(match, next_sin + 3);
                        nextnextTok = str.find_first_of(match, nextTok + 1);
                    }

                }

                break;
            }

            //Negative case with right side token
            if ((nextTok - (next_sin + 2)) == 1 && nextnextTok != -1) { //Negative case with right side token

                string pinch = str.substr(next_sin + 4, (nextnextTok - nextTok)); // should be the value between negative signs
                float num = sin((atof(pinch.c_str()))) * -1; // this gets the value and negates it
                ////FORGOT LOL num; // perform the square
                str.replace(next_sin, (nextnextTok - next_sin) - 1, "");
                str.insert(next_sin, std::to_string(num));

                //update
                next_sin = str.find("SIN");
                if (next_sin != -1) {
                    nextTok = str.find_first_of(match, next_sin + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }

                break;

            }

            else { //So theres a token on the right and no negative sign after SIN > SIN46

                //Check to see that there is a digit on the right of the next operand otherwise we error


                string pinch = str.substr(next_sin + 3, (nextTok - (next_sin + 3)));
                float num = sin(atof(pinch.c_str())); // this gets the value
                ////FORGOT LOL num; // perform the square

                str.replace(next_sin, nextTok - next_sin, "");
                str.insert(next_sin, std::to_string(num));
                //update
                next_sin = str.find("SIN");
                if (next_sin != -1) {
                    nextTok = str.find_first_of(match, next_sin + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }
            }

            //update
            next_sin = str.find("SIN");
            if (next_sin != -1) {
                nextTok = str.find_first_of(match, next_sin + 3);
                nextnextTok = str.find_first_of(match, nextTok + 1);
            }
        }

        int next_tan = str.find("TAN");
        //====================================TAN==========================
        while (next_tan != -1 && !isdigit(str[next_tan - 1])) {

            int nextTok = str.find_first_of(match, next_tan + 3); //The index of the next token: TAN, sqt, sin, cos, tan, /, x, +, -
            int nextnextTok = str.find_first_of(match, nextTok + 1); //for the negative case we need to check the next next token

            //No next token(TAN) OR a negative term with no Next token(term)
            if (nextTok == -1 || ((nextTok - (next_tan + 2)) == 1 && (nextnextTok == -1))) { // if (nextTok == -1 || nextTok-next_tan == 1 && nextnextTok == -1)) 

                if (str[str.length() - 1] == 'r') {//guard agaisnt unused square root

                    str = "ERROR:operator[value]";
                    return;
                }

                if (nextTok == -1) { //theres no next token we know its just positive term
                    string pinch = str.substr(next_tan + 3, str.length());
                    float num = tan(atof(pinch.c_str())); // this gets the value

                    str.replace(next_tan, str.length(), std::to_string(num));
                    next_tan = str.find("TAN");
                    break;

                }

                if ((nextTok - (next_tan + 2)) == 1 && nextnextTok == -1) { //Negative case with no right side toekn

                    string pinch = str.substr(next_tan + 4, str.length());
                    float num = tan((atof(pinch.c_str()))) * -1; // this gets the value and negates it
                    ////FORGOT LOL num; // perform the square
                    str.replace(next_tan, str.length(), std::to_string(num)); // the same replace shoulkd work
                    //Update
                    next_tan = str.find("TAN");
                    if (next_tan != -1) {
                        nextTok = str.find_first_of(match, next_tan + 3);
                        nextnextTok = str.find_first_of(match, nextTok + 1);
                    }

                }

                break;
            }

            //Negative case with right side token
            if ((nextTok - (next_tan + 2)) == 1 && nextnextTok != -1) { //Negative case with right side token

                string pinch = str.substr(next_tan + 4, (nextnextTok - nextTok)); // should be the value between negative signs
                float num = tan((atof(pinch.c_str()))) * -1; // this gets the value and negates it
                ////FORGOT LOL num; // perform the square
                str.replace(next_tan, (nextnextTok - next_tan) - 1, "");
                str.insert(next_tan, std::to_string(num));

                //update
                next_tan = str.find("TAN");
                if (next_tan != -1) {
                    nextTok = str.find_first_of(match, next_tan + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }

                break;

            }

            else { //So theres a token on the right and no negative sign after TAN > TAN46

                //Check to see that there is a digit on the right of the next operand otherwise we error


                string pinch = str.substr(next_tan + 3, (nextTok - (next_tan + 3)));
                float num = tan(atof(pinch.c_str())); // this gets the value
                //FORGOT LOL num; // perform the square

                str.replace(next_tan, nextTok - next_tan, "");
                str.insert(next_tan, std::to_string(num));
                //update
                next_tan = str.find("TAN");
                if (next_tan != -1) {
                    nextTok = str.find_first_of(match, next_tan + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }
            }

            //update
            next_tan = str.find("TAN");
            if (next_tan != -1) {
                nextTok = str.find_first_of(match, next_tan + 3);
                nextnextTok = str.find_first_of(match, nextTok + 1);
            }
        }
        int next_cos = str.find("COS");
        //====================================COS==========================
        while (next_cos != -1 && !isdigit(str[next_cos - 1])) {

            int nextTok = str.find_first_of(match, next_cos + 3); //The index of the next token: COS, sqt, sin, cos, tan, /, x, +, -
            int nextnextTok = str.find_first_of(match, nextTok + 1); //for the negative case we need to check the next next token

            //No next token(COS) OR a negative term with no Next token(term)
            if (nextTok == -1 || ((nextTok - (next_cos + 2)) == 1 && (nextnextTok == -1))) { // if (nextTok == -1 || nextTok-next_cos == 1 && nextnextTok == -1)) 

                if (str[str.length() - 1] == 'r') {//guard agaisnt unused square root

                    str = "ERROR:operator[value]";
                    return;
                }

                if (nextTok == -1) { //theres no next token we know its just positive term
                    string pinch = str.substr(next_cos + 3, str.length());
                    float num = cos(atof(pinch.c_str())); // this gets the value

                    str.replace(next_cos, str.length(), std::to_string(num));
                    next_cos = str.find("COS");
                    break;

                }

                if ((nextTok - (next_cos + 2)) == 1 && nextnextTok == -1) { //Negative case with no right side toekn

                    string pinch = str.substr(next_cos + 4, str.length());
                    float num = cos((atof(pinch.c_str()))) * -1; // this gets the value and negates it
                    ////FORGOT LOL num; // perform the square
                    str.replace(next_cos, str.length(), std::to_string(num)); // the same replace shoulkd work
                    //Update
                    next_cos = str.find("COS");
                    if (next_cos != -1) {
                        nextTok = str.find_first_of(match, next_cos + 3);
                        nextnextTok = str.find_first_of(match, nextTok + 1);
                    }

                }

                break;
            }

            //Negative case with right side token
            if ((nextTok - (next_cos + 2)) == 1 && nextnextTok != -1) { //Negative case with right side token

                string pinch = str.substr(next_cos + 4, (nextnextTok - nextTok)); // should be the value between negative signs
                float num = cos((atof(pinch.c_str()))) * -1; // this gets the value and negates it
                ////FORGOT LOL num; // perform the square
                str.replace(next_cos, (nextnextTok - next_cos) - 1, "");
                str.insert(next_cos, std::to_string(num));

                //update
                next_cos = str.find("COS");
                if (next_cos != -1) {
                    nextTok = str.find_first_of(match, next_cos + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }

                break;

            }

            else { //So theres a token on the right and no negative sign after COS > COS46

                //Check to see that there is a digit on the right of the next operand otherwise we error


                string pinch = str.substr(next_cos + 3, (nextTok - (next_cos + 3)));
                float num = cos(atof(pinch.c_str())); // this gets the value
                //FORGOT LOL num; // perform the square

                str.replace(next_cos, nextTok - next_cos, "");
                str.insert(next_cos, std::to_string(num));
                //update
                next_cos = str.find("COS");
                if (next_cos != -1) {
                    nextTok = str.find_first_of(match, next_cos + 3);
                    nextnextTok = str.find_first_of(match, nextTok + 1);
                }
            }

            //update
            next_cos = str.find("COS");
            if (next_cos != -1) {
                nextTok = str.find_first_of(match, next_cos + 3);
                nextnextTok = str.find_first_of(match, nextTok + 1);
            }
        }

        // ==========================THE 4 match = - x / ==============================
        int next_add = str.find("+");
        int next_sub = str.find("-");
        int next_mul = str.find("x");
        int next_div = str.find("/");

        while (next_add != -1 || next_sub != -1 || next_mul != -1 || next_div != -1) {
            //D M A S
            next_div = str.find("/");
            while (next_div != -1) {
                int nextTok = str.find_first_of(match, next_div + 1); //The index of the next token: sqr, sqt, sin, cos, tan, /, x, +, -
                int prevTok = str.find_last_of(match, next_div - 1); // the index of the previous token
                int nextnextTok = str.find_first_of(match, nextTok + 1);
                int prevprevTok = str.find_last_of(match, prevTok - 1);

                if ((nextnextTok == -1 && str[nextTok] != '-') || (nextnextTok == -1) || (nextTok == -1)) { // Means there are no more tokens to the right, so we just calculate and replace

                    if (str[str.length() - 1] == '/') {//guard agaisnt unused square root

                        str = "ERROR:operator[value]";
                        return;
                    }

                    else {

                        if (str[next_div + 1] != '-') {
                        string pinch1 = str.substr(next_div + 1, str.length()); // this is the term from \ To the end of string
                        string pinch2 = str.substr(prevTok + 1, (next_div)-(prevTok + 1));
                        float num;
                        if (str[prevTok] == '-') { // check for negative

                           // this is the term from previos token to the 
                            num = -atof(pinch2.c_str()) / atof(pinch1.c_str()); // d o th e negative div

                        }
                        else {
                            num = atof(pinch2.c_str()) / atof(pinch1.c_str()); // do the postiive div
                        }

                        if (str[prevTok] == '-') {
                            str.replace(prevTok, str.length(), std::to_string(num));
                        }

                        else {
                            str.replace(prevTok + 1, str.length(), std::to_string(num));
                        }

                        next_div = str.find("/");
                        //  str.insert(next_div, std::to_string(num));
                    }

                        else {

                            string pinch1 = str.substr(next_div + 2, str.length()); // this is the term from \ To the end of string
                            string pinch2 = str.substr(prevTok + 1, (next_div)-(prevTok + 1));
                            float num;
                            if (str[prevTok] == '-') { // check for negative

                               // this is the term from previos token to the 
                                num = -atof(pinch2.c_str()) / -atof(pinch1.c_str()); // d o th e negative div

                            }
                            else {
                                num = atof(pinch2.c_str()) / -atof(pinch1.c_str()); // do the postiive div
                            }

                            if (prevTok == 0 || prevprevTok ==0) {//no term before the negative left div operand

                                    if(prevTok==0)
                                        str.replace(prevTok+1, str.length(), std::to_string(num));

                                    if(prevprevTok ==0)
                                        str.replace(prevprevTok + 1, str.length(), std::to_string(num));
                            }
                           
                            else { // term on the left
                                if (num < 0) {
                                    str.replace(prevTok, str.length(), std::to_string(num));
                                }

                                else {
                                    str.replace(prevTok + 1, str.length(), std::to_string(num));
                                }
                            }

                            next_div = str.find("/");


                        }

                    }
                }
                else { //so we have a token on the right of the divide

                    //check that negative sign isnt right after the / (no negative second operand)
                    if (str[next_div+1] != '-') {
                        string pinch1 = str.substr(next_div + 1, nextTok - (next_div + 1)); // this is the term from \ To the end of string
                        string pinch2 = str.substr(prevTok + 1, (next_div)-(prevTok + 1));
                       // prevprevTok = str.find_last_of(match, prevTok - 1);
                        float num;
                        if (str[prevTok] == '-') { // check for negative

                           // this is the term from previos token to the 
                            num = -atof(pinch2.c_str()) / atof(pinch1.c_str()); // d o th e negative div
                        }
                        else {
                            num = atof(pinch2.c_str()) / atof(pinch1.c_str()); // do the postiive div
                        }

                        if (prevTok == 0) {//no term before the negative left div operand       
                            str.replace(prevTok+1, nextTok - prevTok+1, std::to_string(num));
                        }

                        else { // term on the left
                            if (num < 0) {
                                str.replace(prevTok, nextTok - prevTok, "");
                                str.insert(prevTok, std::to_string(num));
                            }

                            else {
                                str.replace(prevTok + 1, nextTok - prevTok-1, "");
                                str.insert(prevTok+1, std::to_string(num));
                            }
                        }

                        
                    }

                    else { // so we have a negative second operand
                        string pinch1 = str.substr(next_div + 2, nextnextTok - (next_div + 2)); // this is the term from \ To the end of string
                        string pinch2 = str.substr(prevTok + 1, (next_div)-(prevTok + 1));
                        float num;
                        if (str[prevTok] == '-') { // check for negative

                           // this is the term from previos token to the 
                            num = -atof(pinch2.c_str()) / -atof(pinch1.c_str()); // d o th e negative div
                        }
                        else {
                            num = atof(pinch2.c_str()) / -atof(pinch1.c_str()); // do the postiive div
                        }

                        if (prevprevTok == 0 && str[prevprevTok] != ' ') {//no term before the negative left div operand       
                            str.replace(prevTok, nextnextTok - prevTok, std::to_string(num));
                        }

                        else { // term on the left
                            if (num < 0) {
                                str.replace(prevTok, nextnextTok - prevTok, "");
                                str.insert(prevTok, std::to_string(num));
                            }

                            else {
                                str.replace(prevTok, nextnextTok - prevTok, "");
                                str.insert(prevTok, '+'+std::to_string(num));
                            }
                        }

                        

                    }

                }
                next_div = str.find("/");

            }

            next_mul = str.find("x");
            while (next_mul != -1) {
                int nextTok = str.find_first_of(match, next_mul + 1); //The index of the next token: sqr, sqt, sin, cos, tan, /, x, +, -
                int prevTok = str.find_last_of(match, next_mul - 1); // the index of the previous token
                int nextnextTok = str.find_first_of(match, nextTok + 1);
                int prevprevTok = str.find_last_of(match, prevTok - 1);

                if ((nextnextTok == -1 && str[nextTok] != '-') || (nextnextTok == -1)||(nextTok == -1)) { // Means there are no more tokens to the right, so we just calculate and replace

                    if (str[str.length() - 1] == 'x') {//guard agaisnt unused square root

                        str = "ERROR:operator[value]";
                        return;
                    }

                    else {

                        if (str[next_mul + 1] != 'x') {
                            string pinch1 = str.substr(next_mul + 1, str.length()); // this is the term from \ To the end of string
                            string pinch2 = str.substr(prevTok + 1, (next_mul)-(prevTok + 1));
                            float num;
                            if (str[prevTok] == '-') { // check for negative

                               // this is the term from previos token to the 
                                num = -atof(pinch2.c_str()) * atof(pinch1.c_str()); // d o th e negative div

                            }
                            else {
                                num = atof(pinch2.c_str()) * atof(pinch1.c_str()); // do the postiive div
                            }

                            if (str[prevTok] == '-') {
                                str.replace(prevTok, str.length(), std::to_string(num));
                            }

                            else {
                                str.replace(prevTok + 1, str.length(), std::to_string(num));
                            }

                            next_mul = str.find("x");
                            //  str.insert(next_mul, std::to_string(num));
                        }

                        else {

                            string pinch1 = str.substr(next_mul + 2, str.length()); // this is the term from \ To the end of string
                            string pinch2 = str.substr(prevTok + 1, (next_mul)-(prevTok + 1));
                            float num;
                            if (str[prevTok] == '-') { // check for negative

                               // this is the term from previos token to the 
                                num = -atof(pinch2.c_str()) * -atof(pinch1.c_str()); // d o th e negative div

                            }
                            else {
                                num = atof(pinch2.c_str()) * -atof(pinch1.c_str()); // do the postiive div
                            }

                            if (prevTok == 0 || prevprevTok == 0) {//no term before the negative left div operand

                                if (prevTok == 0)
                                    str.replace(prevTok + 1, str.length(), std::to_string(num));

                                if (prevprevTok == 0)
                                    str.replace(prevprevTok + 1, str.length(), std::to_string(num));
                            }

                            else { // term on the left
                                if (num < 0) {
                                    str.replace(prevTok, str.length(), std::to_string(num));
                                }

                                else {
                                    str.replace(prevTok + 1, str.length(), std::to_string(num));
                                }
                            }

                            next_mul = str.find("x");


                        }

                    }
                }
                else { //so we have a token on the right of the divide

                    //check that negative sign isnt right after the / (no negative second operand)
                    if (str[next_mul + 1] != '-') {
                        string pinch1 = str.substr(next_mul + 1, nextTok - (next_mul + 1)); // this is the term from \ To the end of string
                        string pinch2 = str.substr(prevTok + 1, (next_mul)-(prevTok + 1));
                        // prevprevTok = str.find_last_of(match, prevTok - 1);
                        float num;
                        if (str[prevTok] == '-') { // check for negative

                           // this is the term from previos token to the 
                            num = -atof(pinch2.c_str()) * atof(pinch1.c_str()); // d o th e negative div
                        }
                        else {
                            num = atof(pinch2.c_str()) * atof(pinch1.c_str()); // do the postiive div
                        }

                        if (prevTok == 0) {//no term before the negative left div operand       
                            str.replace(prevTok+1, nextTok - (prevTok+1), std::to_string(num));
                        }

                        else { // term on the left
                            if (num < 0) {
                                str.replace(prevTok, nextTok - prevTok, "");
                                str.insert(prevTok, std::to_string(num));
                            }

                            else {
                                str.replace(prevTok + 1, nextTok - prevTok - 1, "");
                                str.insert(prevTok + 1, std::to_string(num));
                            }
                        }


                    }

                    else { // so we have a negative second operand
                        string pinch1 = str.substr(next_mul + 2, nextnextTok - (next_mul + 2)); // this is the term from \ To the end of string
                        string pinch2 = str.substr(prevTok + 1, (next_mul)-(prevTok + 1));
                        float num;
                        if (str[prevTok] == '-') { // check for negative

                           // this is the term from previos token to the 
                            num = -atof(pinch2.c_str()) * -atof(pinch1.c_str()); // d o th e negative div
                        }
                        else {
                            num = atof(pinch2.c_str()) * -atof(pinch1.c_str()); // do the postiive div
                        }

                        if (prevprevTok == 0 && str[prevprevTok] != ' ') {//no term before the negative left div operand       
                            str.replace(prevTok, nextnextTok - prevTok, std::to_string(num));
                        }

                        else { // term on the left
                            if (num < 0) {
                                str.replace(prevTok, nextnextTok - prevTok, "");
                                str.insert(prevTok, std::to_string(num));
                            }

                            else {
                                str.replace(prevTok, nextnextTok - prevTok, "");
                                str.insert(prevTok, '+' + std::to_string(num));
                            }
                        }



                    }

                }
                next_mul = str.find("x");

            }

            next_add = str.find("+");
            while (next_add != -1) {

                int nextTok = str.find_first_of(match, next_add + 1); //The index of the next token: sqr, sqt, sin, cos, tan, /, x, +, -
                int prevTok = str.find_last_of(match, next_add - 1); // the index of the previous token
                int nextnextTok = str.find_first_of(match, nextTok + 1);

                if (nextTok == -1) { // Means there are no more tokens to the right, so we just calculate and replace

                    if (str[str.length() - 1] == '+') {//guard agaisnt unused square root

                        str = "ERROR:operator[value]";
                        return;
                    }

                    else {
                        string pinch1 = str.substr(next_add + 1, str.length()); // this is the term from \ To the end of string
                        string pinch2 = str.substr(prevTok + 1, (next_add)-(prevTok + 1));
                        float num;
                        if(str[prevTok] == '-'){ // check for negative

                           // this is the term from previos token to the 
                             num = -atof(pinch2.c_str()) + atof(pinch1.c_str()); // d o th e negative add
                            }
                        else {
                             num = atof(pinch2.c_str()) + atof(pinch1.c_str()); // do the postiive add
                        }

                        if (str[prevTok] == '-') {
                            str.replace(prevTok, str.length(), std::to_string(num));
                        }

                        else {
                            str.replace(prevTok+1, str.length(), std::to_string(num));
                        }

                        
                        //  str.insert(next_add, std::to_string(num));
                    }
                }
                else { //So theres a token on the right and before that token is our term that we need to calc and replace

                    string pinch1 = str.substr(next_add + 1, nextTok -( next_add+1)); // this is the term from \ To the end of string
                    string pinch2 = str.substr(prevTok + 1, (next_add)-(prevTok + 1));
                    float num;
                    if (str[prevTok] == '-') { // check for negative

                       // this is the term from previos token to the 
                        num = -atof(pinch2.c_str()) + atof(pinch1.c_str()); // d o th e negative add
                    }
                    else {
                        num = atof(pinch2.c_str()) + atof(pinch1.c_str()); // do the postiive add
                    }

                    if (num <= 0) {

                        str.replace(prevTok, nextTok - (prevTok), ""); // should start at the begining of the string which is " "
                        str.insert(prevTok, std::to_string(num));
                       
                    }

                    else {
                        str.replace(prevTok+1, nextTok - (prevTok+1), ""); // should start at the begining of the string which is " "
                        
                        str.insert(prevTok+1, std::to_string(num));
                       
                        next_add = str.find("+");
                    }
                }
                next_add = str.find("+");
               
            }

            next_sub = str.find("-");
            while (next_sub != -1) {

                int nextTok = str.find_first_of(match, next_sub + 1); //The index of the next token: sqr, sqt, sin, cos, tan, /, x, +, -
                int prevTok = str.find_last_of(match, next_sub - 1); // the index of the previous token
                int nextnextTok = str.find_first_of(match, nextTok + 1);
                

                if (nextTok == -1 ) { // Means there are no more tokens to the right just return the string

                    
                    if (str[str.length() - 1] == '-') {//guard agaisnt unused square root

                        str = "ERROR:operator[value]";
                        return;
                    }

                    if (next_sub == 1) {

                        return;
                    }

                    else {

                        string pinch1 = str.substr(next_sub + 1, str.length() - next_sub+1); // the right of -
                        string pinch2 = str.substr(1, next_sub-1);
                        float num = (atof(pinch2.c_str())) - atof(pinch1.c_str());
                        str.replace(1, str.length() - 1, ""); // should start at the begining of the string which is " "
                        str.insert(1, std::to_string(num));
                        next_sub = str.find("-");
                        break;
                        //  str.insert(next_sub, std::to_string(num));
                    }
                }                               
                    //Leading negative case and leading term case
                    if (str[next_sub-1] == ' ') {//leading neg with neg tokens to the right

                        string pinch1 = str.substr(next_sub + 1, (nextTok)-(next_sub + 1));

                        if (nextnextTok == -1) { //We have a term after next token
                            string pinch2 = str.substr(nextTok+1, str.length());
                            float num = (-atof(pinch1.c_str())) - atof(pinch2.c_str()); // this gets the value
                            str.replace(next_sub, nextnextTok - next_sub, ""); // should start at the begining of the string which is " "
                            str.insert(next_sub, std::to_string(num));
                            next_sub = str.find("-");
                            return;
                        }

                        else { //The last item is a term
                            string pinch2 = str.substr(nextTok + 1, nextnextTok - (nextTok+1));
                            float num = (-atof(pinch2.c_str())) - atof(pinch1.c_str()); // this gets the value
                            str.replace(1, nextnextTok - str.find_last_of(match, next_sub+1),""); // should start at the begining of the string which is " "
                            next_sub = str.find("-");
                            str.insert(next_sub, std::to_string(num));
                            next_sub = str.find("-");
                            break;
                        }

                        

                    }

                    else { //must be a term
                 
                    string pinch1 = str.substr(next_sub + 1, (nextTok)-(next_sub + 1)); // this is the term from - To the end of string
                    string pinch2 = str.substr(prevTok + 1, (next_sub)-(prevTok + 1)); // this is the term from previos token to the 
                    float num = atof(pinch2.c_str()) - atof(pinch1.c_str()); // this gets the value

                    str.replace(str.find_last_of(match, next_sub - 1)+1, nextTok-(str.find_last_of(match, next_sub - 1)+1), ""); // should start at the begining of the string which is " "
                    next_sub = str.find("-");
                    str.insert(str.find_last_of(match, next_sub - 1)+1, std::to_string(num));
                    next_sub = str.find("-");
                    }
           
            }
                     
     }
     first = str.find_first_of(match);
     next = str.find_first_of(match, first + 1);
 }
        return;
}



bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
