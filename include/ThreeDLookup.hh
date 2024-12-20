#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

//3D interpolation https://gist.github.com/sputnick1124/b40ca05d9cc7b5a5575804bc914e26bb

class ThreeDLookup {
    public:
        ThreeDLookup(std::vector<double> &x,
                     std::vector<double> &y,
                     std::vector<double> &z,
                     std::vector< std::vector< std::vector<double> > > &a_dataTable);
        ~ThreeDLookup();
        double Interp(double xq, double yq, double zq);

    private:
        std::vector<double> xvec;
        std::vector<double> yvec;
        std::vector<double> zvec;
        std::vector< std::vector< std::vector<double> > > dataTable;

        double minx, miny, minz;
        double maxx, maxy, maxz;
};