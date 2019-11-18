#include "my_vector.h"
#include "curve.h"
#include <set>
#include <unordered_map>
#include "cluster_object.h"
#include <random>
#include <cmath>

//random init
template <typename T>
std::vector<my_vector<T>> initialise_centers(int clusters, std::unordered_map<std::string, my_vector<T>> *vectors_array) {
  srand(time(NULL));
  std::set<int> ids;
  //logw anwmalias do8entwn input files, prepei na kanoyme workaround auto to problhma...
  std::vector<std::string> keys;
  for(auto kv : *vectors_array)
    keys.push_back(kv.first);


  for (int i = 0; i < clusters; i++)
  { //epilegw tuxaio int anamesa sto 0 kai to n
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double>  distr(0, keys.size());
    int rand_id = floor(distr(generator));
    //int rand_id = rand()%keys.size();
    while (ids.find(rand_id) != ids.end()) //uparxei hdh auto pou brhke h rand
    {
      rand_id = floor(distr(generator));
      //rand_id = rand()%keys.size();
    }
    ids.insert(rand_id); //en telei bazw to unique value sto set m
  }
  std::vector<int> v_ids(ids.begin(), ids.end()); //epistrefw to set alla se vector apo id
  std::vector<my_vector<T>> ta_kentra;
  ta_kentra.clear();
  for (int i = 0; i < clusters; i++)
  {
    my_vector<T> one_v_atime;
    one_v_atime.set_id((*vectors_array)[keys[v_ids[i]]].get_id());
    one_v_atime.set_v((*vectors_array)[keys[v_ids[i]]].get_v());
    ta_kentra.push_back(one_v_atime);
  }
  if (ta_kentra.size() != (unsigned int)clusters)
  {
    std::cerr << "Error in initialise_centers.\n";
    exit(-2);
  }
  return ta_kentra;
}

//kmeans++
/*std::vector<my_vector<int>> initialise_centers_plus(int clusters){

}
*/
template <typename T>
void initialize_clusters (std::vector<my_vector<T>> *cluster_centers, std::vector<cluster<T>> *clusters) {
  for(unsigned int i=0; i<(*cluster_centers).size(); i++){
      cluster<T> onecatatime(&((*cluster_centers)[i]));
      (*clusters).push_back(onecatatime);
    }
}
