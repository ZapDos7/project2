#include "utils.h"
#include <unordered_map>
#include "cluster_object.h"
#include "my_vector.h"


template <typename T>
void update_mean(std::vector<cluster<T>> * clusts, int diastaseis){

  std::vector<my_vector<T>> nea_kentra;
  for(unsigned int i=0; i< (*clusts).size(); i++){ //gia ka8e cluster
    T mean_coords[diastaseis] = {0.0};
    unsigned int n = (*clusts)[i].get_set_of_points()->size();
    for(auto x:(*(*clusts)[i].get_set_of_points()) ){

      for(unsigned int j=0; j< x.second->get_v().size(); j++ ){
        mean_coords[j] += x.second->get_v()[j] ;
      }//telos for gia tis suntetagmenes enos shmeiou tou cluster
      //n++;
    } //telos for gia kathe shmeiou tou cluster
    for(unsigned int z=0; z< diastaseis; z++)
      mean_coords[z] /= n;
    std::vector<T> mean_coordz(mean_coords, mean_coords+diastaseis); //metatroph se vector
    my_vector<T> mean_center;
    mean_center.set_id("\t");
    mean_center.set_v(mean_coordz);
    nea_kentra.push_back(mean_center);
  }//telos for gia ka8e cluster

  format_clusters(&nea_kentra, clusts);
  //return nea_kentra;
}//telos sunarthshs
