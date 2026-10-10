#include "Spark/STL/Math.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>

namespace Novella::NScript::Modules::Math{

    double clamp(double value, double min, double max){

        return std::clamp(value, min, max);
    }

    double squareRoot(double value){

        return std::sqrt(value);
    }

    double power(double value, double power){

        return std::pow(value, power);
    }

    double abs(double value){

        return std::abs(value);
    }

    double min(double a, double b){

        return std::min(a, b);
    }

    double max(double a, double b){

        return std::max(a, b);
    }

    double floor(double value){

        return std::floor(value);
    }

    double ceil(double value){

        return std::ceil(value);
    }

    double round(double value){

        return std::round(value);
    }

    double sin(double value){

        return std::sin(value);
    }

    double cos(double value){

        return std::cos(value);
    }

    double tan(double value){

        return std::tan(value);
    }

    double atan2(double a, double b){

        return std::atan2(a,b);
    }

    double degToRad(double value){

        return value * (std::numbers::pi / 180.0);
    }
    
    double radToDeg(double value){

        return value * (180.0 / std::numbers::pi);
    }

    double lerp(double a, double b, double t){

        return std::lerp(a, b, t);
    }

    double sign(double value){

        if(value > 0) return 1.0;

        if(value < 0) return -1.0;

        return 0.0;
    }

    double pi(){

        return std::numbers::pi;
    }

    double e(){

        return std::numbers::e;
    }

    double phi(){

        return std::numbers::phi;
    }

    double sqrt2(){

        return std::numbers::sqrt2;
    }

    double sqrt3(){

        return std::numbers::sqrt3;
    }

    double invPi(){

        return std::numbers::inv_pi;
    }

    double invSqrtPi(){

        return std::numbers::inv_sqrtpi;
    }

    double ln2(){

        return std::numbers::ln2;
    }

    double ln10(){

        return std::numbers::ln10;
    }

    double log2e(){

        return std::numbers::log2e;
    }

    double log10e(){

        return std::numbers::log10e;
    }

    double egamma(){

        return std::numbers::egamma;
    }
}