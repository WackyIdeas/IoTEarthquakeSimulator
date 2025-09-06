#include <iostream>
#include <cmath>

// Gaussova funkcija f(x)
double f(double x, double s) {
    return 0.5 * std::exp(-(x*x)/(2*s*s));
}

// Proširena funkcija g(x)
double g(double x, double s) {
    return 0.2*f(5*x - 14, s)
         + 0.09*f(6*x - 2, s)
         + 0.08*f(7*x - 7, s)
         + 0.40*f(6*x - 19, s)
         + 0.15*f(4*x - 19, s)
         + 0.05*f(6*x - 24, s);
}

// Semplovanje sa pragom
void sampleG(int N, double s, double x_min, double x_max, double threshold=0.0001) {
    for(int i = 0; i < N; i++) {
        double x = x_min + i*(x_max - x_min)/(N-1);
        double y = g(x, s);

        if (y < threshold) {
            y = 0.0;
        }

        std::cout << "{ \"acceleration_g\": " << y << "}," << std::endl;
        //std::cout << "x=" << x << ", g(x)=" << y << "\n";
    }
}

int main() {
    int N = 100;      
    double s = 1.0;   
    double x_min = 0.0;
    double x_max = 6.0;

    sampleG(N, s, x_min, x_max);

    return 0;
}
