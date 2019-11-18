#include "utils.h"

#include <unordered_map>
#include "cluster_object.h"
#include "my_vector.h"

//LLOYD - each input vector to it nearest!
template <typename T>
void lloyd_ass(std::vector<cluster<T>>* clusters, std::unordered_map<std::string, my_vector<T> > *vectors_array)
{
    for (auto x :(*vectors_array))
    {
        double min1 = std::numeric_limits<double>::max(); //apeiro
        int min_clust_index = -1; //to index tou cluster opou anoikei kathe x
        for (unsigned int j = 0; j < (*clusters).size(); j++)
        {
            double tmp = 0.0;
            tmp = manhattan_distance(x.second.get_v(), (*clusters)[j].get_center_coords()); //dist(x.second.get_v(), (*clusters)[j].get_center_coords()) 
            if (tmp < min1)
            {
                min1 = tmp;
                min_clust_index = j;
            }
            //std::cout << "Eimai " << x.first << " & sugkrinw " << (*clusters)[j].get_center_id() << " dist " << tmp << '\n';
        }
        (*clusters)[min_clust_index].incorporate_point(&(x.second));
        //std::cout << "Eimai " << x.first << " & kentro " << (*clusters)[min_clust_index].get_center_id() << " dist " << min1 << '\n';
        //std::cout << '\n';
    }
    //std::cout << "\n\n\n";
}