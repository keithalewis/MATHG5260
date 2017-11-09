// fms_jordan.h - Jordan numbers for calculating derivatives.
#include "ensure.h"

namespace fms {

    template<size_t n, class X = double>
    class jordan {
        X x[n];
    public:
        jordan()
        { }
        jordan(const X& x0)
        {
            x[0] = x0;
        }
        jordan(size_t m, const X* xi)
        {
            for (size_t i = 0; i < m && i < n)
                x[i] = xi[i];
            for (size_t i = m; i < n; ++i)
                x[i] = 0;
        }
        jordan(std::initializer_list<X> xl)
        {
            size_t m = xl.size();

            if (m > n)
                m = n;
            else
                std::fill(x + m, x + n, X(0));

            std::copy(xl.begin(), xl.begin() + m, x);
        }
        jordan(const jordan& x_)
        {
            memcpy(x, x_.x, n*sizeof(X));
        }
        jordan& operator=(const jordan& x_)
        {
            if (this != & x_) {
                memcpy(x, x_.x, n*sizeof(X));
            }
            
            return *this;
        }
        ~jordan()
        { }

        jordan& operator+=(X a)
        {
            return *this;
        }
        jordan& operator*=(X a)
        {
            return *this;
        }
        //...
        jordan& operator+=(const jordan& y)
        {
            return *this;
        }
    };
#ifdef _DEBUG

    void fms_jordan_test()
    {
        {
            jordan<1> x;
            jordan<1> x1(x);
            x = x1;
        }
    }

#endif // _DEBUG

} // fms

template<size_t n, class X = double>
fms::jordan<n,X> operator+(fms::jordan<n,X> x, fms::jordan<n,X>& y)
{
    return x += y;
}