#include "vector_utils.h"

vector<double> to_vector(VectorXd &v1){
    vector<double> v2(v1.size());
    VectorXd::Map(&v2[0], v1.size()) = v1;
    return v2;
}