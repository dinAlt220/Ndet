#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include "ThreeDLookup.hh"

ThreeDLookup::ThreeDLookup(std::vector<double> &x,
                           std::vector<double> &y,
                           std::vector<double> &z,
                           std::vector< std::vector< std::vector<double> > > &a_dataTable) {
    xvec.insert(xvec.end(), x.begin(), x.end());
    yvec.insert(yvec.end(), y.begin(), y.end());
    zvec.insert(zvec.end(), z.begin(), z.end());
    dataTable = a_dataTable;


    auto xbounds = std::minmax_element(x.begin(), x.end());
    minx = *xbounds.first; maxx = *xbounds.second;

    auto ybounds = std::minmax_element(y.begin(), y.end());
    miny = *ybounds.first; maxy = *ybounds.second;

    auto zbounds = std::minmax_element(z.begin(), z.end());
    minz = *zbounds.first; maxz = *zbounds.second;
}

ThreeDLookup::~ThreeDLookup() {};

double ThreeDLookup::Interp(double xq, double yq, double zq)
{
    /*
     * Assumes that all abscissa are monotonically increasing values
     */
    xq = std::max(minx, std::min(xq, maxx));
    yq = std::max(miny, std::min(yq, maxy));
    zq = std::max(minz, std::min(zq, maxz));

    auto xupper = std::upper_bound(xvec.cbegin(), xvec.cend(), xq);
    int x1 = (xupper == xvec.cend()) ? xupper - xvec.cbegin() - 1 : xupper - xvec.cbegin();
    int x0 = x1 - 1;

    auto yupper = std::upper_bound(yvec.cbegin(), yvec.cend(), yq);
    int y1 = (yupper == yvec.cend()) ? yupper - yvec.cbegin() - 1 : yupper - yvec.cbegin();
    auto y0 = y1 - 1;

    auto zupper = std::upper_bound(zvec.cbegin(), zvec.cend(), zq);
    int z1 = (zupper == zvec.cend()) ? zupper - zvec.cbegin() - 1 : zupper - zvec.cbegin();
    auto z0 = z1 - 1;

    double xd = (xq - xvec[x0])/(xvec[x1] - xvec[x0]);
    double yd = (yq - yvec[y0])/(yvec[y1] - yvec[y0]);
    double zd = (zq - zvec[z0])/(zvec[z1] - zvec[z0]);
        
    double c000 = dataTable[x0][y0][z0];
    double c010 = dataTable[x0][y1][z0];
    double c100 = dataTable[x1][y0][z0];
    double c110 = dataTable[x1][y1][z0];

    double c001 = dataTable[x0][y0][z1];
    double c011 = dataTable[x0][y1][z1];
    double c101 = dataTable[x1][y0][z1];
    double c111 = dataTable[x1][y1][z1];

    double c00 = c000*(1 - xd) + c100*xd;
    double c01 = c001*(1 - xd) + c101*xd;
    double c10 = c010*(1 - xd) + c110*xd;
    double c11 = c011*(1 - xd) + c111*xd;

    double c0 = c00*(1 - yd) + c10*yd;
    double c1 = c01*(1 - yd) + c11*yd;

    double c = c0*(1 - zd) + c1*zd;

    return c;
}