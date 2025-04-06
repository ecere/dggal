public import IMPORT_STATIC "ecrt"
private:

// Authalic / Geodetic latitude conversion as defined by Charles Karney's "On auxiliary latitudes"
// https://arxiv.org/pdf/2212.05818

private /*static */define AUTH_ORDER = 6;

/*private inline */Radians latGeodeticToAuthalic(const double cp[2][AUTH_ORDER], Radians phi)
{
   return applyCoefficients(cp[0], phi);
}

/*static inline */Radians latAuthalicToGeodetic(const double cp[2][AUTH_ORDER], Radians phi)
{
   return applyCoefficients(cp[1], phi);
}

static const double Cxiphi[21] = // Cξφ (A19) - coefficients to convert geodetic latitude to authalic latitude
{
   -4/3.0,    -4/45.0,    88/ 315.0,       538/ 4725.0,     20824/467775.0,      -44732/   2837835.0,
              34/45.0,     8/ 105.0,     -2482/14175.0,    -37192/467775.0,   -12467764/ 212837625.0,
                       -1532/2835.0,      -898/14175.0,     54968/467775.0,   100320856/1915538625.0,
                                          6007/14175.0,     24496/467775.0,    -5884124/  70945875.0,
                                                           -23356/ 66825.0,     -839792/  19348875.0,
                                                                              570284222/1915538625.0
};

static const double Cphixi[21] = // Cφξ (A20) - coefficients to convert authalic latitude to geodetic latitude
{
   4 / 3.0,  4 / 45.0,   -16/35.0,  -2582 /14175.0,  60136 /467775.0,    28112932/ 212837625.0,
            46 / 45.0,  152/945.0, -11966 /14175.0, -21016 / 51975.0,   251310128/ 638512875.0,
                      3044/2835.0,   3802 /14175.0, -94388 / 66825.0,    -8797648/  10945935.0,
                                     6059 / 4725.0,  41072 / 93555.0, -1472637812/ 638512875.0,
                                                    768272 /467775.0,  -455935736/ 638512875.0,
                                                                       4210684958/1915538625.0
};

// ∆η(ζ) = S^(L)(ζ) · Cηζ · P^(M) (n) + O(n^L+1)    -- (20)
static void precomputeCoefficients(double a, double b, const double C[21], double cp[AUTH_ORDER])
{
   // Precomputing coefficients based on Horner's method
   double n = (a - b) / (a + b);  // Third flattening
   double d = n;

   cp[0] = (((((C[ 5] * n + C[ 4]) * n + C[ 3]) * n + C[ 2]) * n + C[ 1]) * n + C[ 0]) * d, d *= n;
   cp[1] = ((((             C[10]  * n + C[ 9]) * n + C[ 8]) * n + C[ 7]) * n + C[ 6]) * d, d *= n;
   cp[2] = (((                           C[14]  * n + C[13]) * n + C[12]) * n + C[11]) * d, d *= n;
   cp[3] = ((                                         C[17]  * n + C[16]) * n + C[15]) * d, d *= n;
   cp[4] = (                                                       C[19]  * n + C[18]) * d, d *= n;
   cp[5] =                                                                      C[20]  * d;
}

/*static inline */Radians applyCoefficients(const double * cp, Radians phi)
{
   // Using Clenshaw summation algorithm (order 6)
   double szeta = sin(phi), czeta = cos(phi);
   double X = 2 * (czeta - szeta) * (czeta + szeta); // 2 * cos(2*zeta)
   double u0, u1; // accumulators for sum

   u0 = X * cp[5]   + cp[4];
   u1 = X * u0      + cp[3];
   u0 = X * u1 - u0 + cp[2];
   u1 = X * u0 - u1 + cp[1];
   u0 = X * u1 - u0 + cp[0];

   return phi + /* sin(2*zeta) * u0 */ 2 * szeta * czeta * u0;
}

/*static */void authalicSetup(double a, double b, double cp[2][AUTH_ORDER])
{
   precomputeCoefficients(a, b, Cxiphi, cp[0]); // geodetic -> authalic
   precomputeCoefficients(a, b, Cphixi, cp[1]); // authalic -> geodetic
}
