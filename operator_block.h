#ifndef OPERATOR_BLOCK_H
#define OPERATOR_BLOCK_H

#include <bits/stdc++.h>

double opAdd (double a, double b) { return a + b; }
double opSub (double a, double b) { return a - b; }
double opMul (double a, double b) { return a * b; }
double opDiv (double a, double b) { if (b == 0) return 0; return a / b; }
double opMod (double a, double b) { if (b == 0) return 0; return fmod (a, b); }

double opRandom (double lo, double hi)
{
    if (lo > hi) swap (lo, hi);
    return lo + ((double)rand () / RAND_MAX) * (hi - lo);
}

int opRandomInt (int lo, int hi)
{
    if (lo > hi) swap (lo, hi);
    return lo + rand () % (hi - lo + 1);
}

bool opGreater (double a, double b) { return a > b; }
bool opLess (double a, double b)    { return a < b; }
bool opEqual (double a, double b)   { return fabs (a - b) < 1e-9; }
bool opAnd (bool a, bool b)         { return a and b; }
bool opOr (bool a, bool b)          { return a or b; }
bool opNot (bool a)                 { return !a; }

double opAbs (double a)             { return fabs (a); }
double opFloor (double a)           { return floor (a); }
double opCeil (double a)            { return ceil (a); }
double opSqrt (double a)            { return a >= 0 ? sqrt (a) : 0; }
double opSin (double a)             { return sin (a * M_PI / 180.0); }
double opCos (double a)             { return cos (a * M_PI / 180.0); }
double opTan (double a)             { return tan (a * M_PI / 180.0); }
double opLog (double a)             { return a > 0 ? log10 (a) : 0; }
double opLn (double a)              { return a > 0 ? log (a) : 0; }
double opPow10 (double a)           { return pow (10, a); }
double opPowE (double a)            { return exp (a); }
double opRound (double a)           { return round (a); }

string opJoin (string a, string b)           { return a + b; }
string opLetterOf (string s, int i)          { if (i < 1 or i > (int)s.size ()) return ""; return string (1, s [i - 1]); }
int opLengthOf (string s)                    { return (int)s.size (); }
bool opContains (string s, string sub)       { return s.find (sub) != string::npos; }

bool isValidNumber (string s)
{
    if (s.empty ()) return false;
    int i = 0;
    if (s [0] == '-' or s [0] == '+') i++;
    bool hasDot = false;
    for (; i < (int)s.size (); i++)
    {
        if (s [i] == '.')
        {
            if (hasDot) return false;
            hasDot = true;
        }
        else if (!isdigit (s [i]))
            return false;
    }
    return true;
}

double toNumber (string s)
{
    if (!isValidNumber (s)) return 0;
    return stod (s);
}

#endif //OPERATOR_BLOCK_H