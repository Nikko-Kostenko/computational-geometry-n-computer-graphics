// computational-geometry-n-computer-graphics.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define iPair pair<int, int> 

#include <iostream>
#include <vector>
#include <time.h>
#include<stack>
#include <algorithm>
#include <stdio.h>
#include <SDL.h>
#include<bits/stdc++.h> 

using namespace std;


vector<iPair> hull;

bool check(vector<iPair> p, iPair a)
{
    bool check = true;
    for (size_t i = 0; i < p.size(); i++)
    {
        if (p[i].first == a.first && p[i].second == a.second)
        {
            check = false;
            break;
        }
    }
    return check;
}

int findSide(iPair p1, iPair p2, iPair p)
{
    int val = (p.second - p1.second) * (p2.first - p1.first) -
        (p2.second - p1.second) * (p.first - p1.first);

    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

int lineDist(iPair p1, iPair p2, iPair p)
{
    return abs((p.second - p1.second) * (p2.first - p1.first) -
        (p2.second - p1.second) * (p.first - p1.first));
}

void quickHull(vector<iPair> a, int n, iPair p1, iPair p2, int side)
{
    int ind = -1;
    int max_dist = 0;

    
    for (int i = 0; i < n; i++)
    {
        int temp = lineDist(p1, p2, a[i]);
        if (findSide(p1, p2, a[i]) == side && temp > max_dist)
        {
            ind = i;
            max_dist = temp;
        }
    }

    
    if (ind == -1)
    {
        if(check(hull,p1)){ hull.push_back(p1); }
        if (check(hull, p2)) { hull.push_back(p2); }
        
        return;
    }

     
    quickHull(a, n, a[ind], p1, -findSide(a[ind], p1, p2));
    quickHull(a, n, a[ind], p2, -findSide(a[ind], p2, p1));
  
}

vector<iPair> pHull(vector<iPair> a, int n)
{
    
    if (n < 3)
    {
        cout << "Convex hull not possible\n";
        vector<iPair> res0;
        return res0;
    }

    
    int min_x = 0, max_x = 0;
    for (int i = 1; i < n; i++)
    {
        if (a[i].first < a[min_x].first)
            min_x = i;
        if (a[i].first > a[max_x].first)
            max_x = i;
    }

     
    quickHull(a, n, a[min_x], a[max_x], 1);
    
    quickHull(a, n, a[min_x], a[max_x], -1);

    cout << "The points in Convex Hull are:\n";

    
    vector<iPair> res(hull.size());
    copy(hull.begin(), hull.end(), res.begin());
    
    return res;
}

vector<iPair> init(int n)
{
    vector<iPair> ans;
    ans.push_back(make_pair(rand() % 40, rand() % 40));
    for (size_t i = 0; i < n-1; i++)
    {
        iPair a = make_pair(rand() % 40, rand() % 40);
        ans.push_back(a);
    }
    return ans;
}

void fix(vector<iPair> &p)
{
    vector<iPair> fix;
    for (size_t i = 0; i < p.size(); i++)
    {
        bool check = false;
        for (size_t j = i + 1; j < p.size(); j++)
        {
            if (p[i].first == p[j].first && p[i].second == p[j].second && (p[i].first- p[i].second)>30)
            {
                check = true;
            }
            
        }
        if (!check)
        {
            fix.push_back(p[i]);
        }
    }
    p.clear();
    p = fix;
}

void printp(vector<iPair> p)
{
    
    for (size_t i = 0; i < p.size(); i++)
    {
        cout << p[i].first << " " << p[i].second<< " | ";
    }
}

void sort(vector<iPair>& p)
{
    sort(p.begin(), p.end());
}

double get_clockwise_angle(const iPair& p, const iPair& p0)
{
    double angle = 0.0;

    angle = atan2(p.first - p0.first, p.second - p0.second);
    if (angle <= 0)
    {
        angle += 2 * M_PI;
    }
    return angle;
}

double get_dist(const iPair& p, const iPair& p0)
{
    double x = p.first - p0.first;
    double y = p.second - p0.second;
    return sqrt(x * x + y * y);
}

bool compare_points(const iPair& a, const iPair& b, const iPair& p0)
{
    double angle1 = get_clockwise_angle(a, p0);
    double angle2 = get_clockwise_angle(b, p0);
    if (angle1 < angle2)
    {
        return true;
    }
    double dist1 = get_dist(a, p0);
    double dist2 = get_dist(b, p0);
    if (angle1 == angle2 && dist1 < dist2)
    {
        return true;
    }
    return false;
}

void clocksort(vector<iPair> &p)
{
    iPair p0 = { 0,0 };
    for (size_t i = 0; i < p.size(); i++)
    {
        p0.first += p[i].first;
        p0.second += p[i].second;
    }
    p0.first = p0.first / p.size();
    p0.second = p0.second / p.size();

    for (size_t i = 0; i < p.size(); i++)
    {
        p[i].first -= p0.first;
        p[i].second -= p0.second;
    }
    for (size_t i = 0; i < p.size(); i++)
    {
        for (size_t j = 0; j < p.size(); j++)
        {
            if (!compare_points(p[i], p[j], p0))
            {
                swap(p[i], p[j]);
            }
        }
    }
    for (size_t i = 0; i < p.size(); i++)
    {
        p[i].first += p0.first;
        p[i].second += p0.second;
    }
}

vector<iPair> base(const vector<iPair>& p)
{
    vector<iPair> res;
    double max = 0.0;
    
    for (size_t i = 0; i < p.size(); i++)
    {
        for (size_t j = i+1; j < p.size(); j++)
        {
            double dist = get_dist(p[i], p[j]);
            if (max < dist)
            {
                res.clear();
                res.push_back(p[i]);
                res.push_back(p[j]);
                max = dist;
            }
        }
    }
    return res;
}

iPair cdot(const vector<iPair>& p, const vector<iPair>& np)
{
    iPair res;
    double max = 0.0;

    for (size_t i = 0; i < np.size(); i++)
    {
        double dist = lineDist(p[0],p[1],np[i]);
        if (max < dist)
        {
            res = np[i];
            max = dist;
        }
        
    }
    return res;
}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void findCircle(const iPair &p1, const iPair& p2, const iPair& p3, SDL_Renderer* renderer, int n)
{
    int x12 = p1.first - p2.first;
    int x13 = p1.first - p2.first;

    int y12 = p1.second - p2.second;
    int y13 = p1.second - p3.second;

    int y31 = p3.second - p1.second;
    int y21 = p2.second - p1.second;

    int x31 = p3.first - p1.first;
    int x21 = p2.first - p1.first;

    // x1^2 - x3^2
    int sx13 = pow(p1.first, 2) - pow(p3.first, 2);

    // y1^2 - y3^2
    int sy13 = pow(p1.second, 2) - pow(p3.second, 2);

    int sx21 = pow(p2.first, 2) - pow(p1.first, 2);
    int sy21 = pow(p2.second, 2) - pow(p1.second, 2);

    int f = ((sx13) * (x12)
        +(sy13) * (x12)
        +(sx21) * (x13)
        +(sy21) * (x13))
        / (2 * ((y31) * (x12)-(y21) * (x13)));
    int g = ((sx13) * (y12)
        +(sy13) * (y12)
        +(sx21) * (y13)
        +(sy21) * (y13))
        / (2 * ((x31) * (y12)-(x21) * (y13)));

    int c = -pow(p1.first, 2) - pow(p1.second, 2) - 2 * g * p1.first - 2 * f * p1.second;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    int h = -g;
    int k = -f;
    int sqr_of_r = h * h + k * k - c;

    // r is the radius
    float r = sqrt(sqr_of_r);
     h = (p1.first + p2.first)/2;
     k = (p1.second + p2.second) / 2;
     r = get_dist(p1, p2) / 2;
    cout << "Centre = (" << h << ", " << k << ")" << endl;
    cout << "Radius = " << r;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    if (n <= 30)
    {
        DrawCircle(renderer, h * 10 + 200, k * 10 + 210, r * 10);
    }
    else
    {
        DrawCircle(renderer, h * 10 + 218, k * 10 + 220, r * 10.2);
    }
}



int main(int argc, char* argv[])
{
    int n;
    cout << "enter number of values : ";
    cin >> n;
    vector<iPair> a = init(n);
    fix(a);
    n = a.size();
    vector<iPair> ch = pHull(a, n);
    cout << "\n";
    cout << "CH points "<<endl;
    printp(ch);
    cout << "\n";
    clocksort(ch);
    cout << "\n";
    cout << "base points "<<endl;
    vector<iPair> basep = base(ch);
    printp(basep);
    cout << "\n";
    cout << "Third point " << endl;
    iPair cdotp = cdot(basep, ch);
    cout << cdotp.first<<"  "<<cdotp.second << endl;
    cout << "\n";
    /* Starting SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    /* Create a Window */
    SDL_Window* window = SDL_CreateWindow("Minimal circle!", 100, 100, 10000, 10000, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    /* Create a Render */
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    /* Draw the render on window */
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(render);
    // Draw the render points
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    for (size_t i = 0; i < a.size(); i++)
    {
        SDL_Rect rectangle;
        rectangle.x =  a[i].first * 10+200;
        rectangle.y =  a[i].second * 10+200;
        rectangle.w = 4;
        rectangle.h = 4;
        SDL_RenderFillRect(render, &rectangle);
    }
   
    // Draw the render CH points
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    for (size_t i = 0; i < ch.size(); i++)
    {
        SDL_Rect rectangle;
        rectangle.x =  ch[i].first*10+200;
        rectangle.y =  ch[i].second * 10+200;
        rectangle.w = 6;
        rectangle.h = 6;
        if (i < ch.size() - 1)
        {
            SDL_RenderDrawLine(render,ch[i].first * 10+200,  ch[i].second * 10+200, ch[i + 1].first * 10+200,ch[i + 1].second * 10+200);
        }
        else
        {
            SDL_RenderDrawLine(render, ch[i].first * 10 + 200, ch[i].second * 10 + 200, ch[0].first * 10 + 200, ch[0].second * 10 + 200);
        }
        SDL_RenderFillRect(render, &rectangle);
    }
    std::cout << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "circle points" << std::endl;
    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
    findCircle(basep[0], basep[1], cdotp, render,n);
    SDL_RenderPresent(render); // Show render on window

     /* Wait 2 seconds */
    SDL_Delay(500000);

    /* Free all objects*/
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    /* Quit program */
    SDL_Quit();

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
