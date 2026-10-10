#pragma once

namespace Spark::STL::Math{

    double wrap(double value);
    double clamp(double value, double min, double max);
    double squareRoot(double value);
    double power(double value, double power);
    double abs(double value);
    double min(double a, double b);
    double max(double a, double b);
    double floor(double value);
    double ceil(double value);
    double round(double value);
    double sin(double value);
    double cos(double value);
    double tan(double value);
    double atan2(double a, double b);
    double degToRad(double value);
    double radToDeg(double value);
    double lerp(double a, double b, double t);
    double sign(double value);
    double pi();
    double e();
    double phi();
    double sqrt2();
    double sqrt3();
    double invPi();
    double invSqrtPi();
    double ln2();
    double ln10();
    double log2e();
    double log10e();
    double egamma();

}