#include <iostream>

using namespace std;


/**
 * @brief Abstract base class / interface for automobiles
 */
class Auto
{
public:
    Auto() : leistungPS(0) { }

    virtual void starten() = 0;
    virtual void beschleunigen() = 0;
    virtual void bremsen() = 0;

protected:
    int leistungPS;
};


/**
 * VW automobiles
 */
class VW : public Auto
{
public:
    virtual void starten() { cout << "VW gestartet!" << endl; }
    virtual void beschleunigen() { }
    virtual void bremsen() { }
};

/**
 * BMW automobiles
 */
class Audi : public Auto
{
public:
    virtual void starten() { cout << "Audi gestartet!" << endl; }
    virtual void beschleunigen() { }
    virtual void bremsen() { }
};

/**
 * BMW automobiles
 */
class BMW : public Auto
{
public:
    virtual void starten() { cout << "BMW gestartet!" << endl; }
    virtual void beschleunigen() { }
    virtual void bremsen() { }
};
