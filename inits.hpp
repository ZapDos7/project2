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
  //srand(time(NULL));
  std::set<int> ids; //indexes ston pinaka cluster
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
  std::vector<int> v_ids(ids.begin(), ids.end()); //epistrefw to set alla se vector apo indexes
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
template <typename T>
std::vector<my_vector<T>> initialise_centers_plus(int clusters, std::unordered_map<std::string, my_vector<T>> *vectors_array){
  //logw anwmalias do8entwn input files, prepei na kanoyme workaround auto to problhma...
  std::vector<std::string> keys; //ta ids
  for(auto kv : *vectors_array)
    keys.push_back(kv.first);

  //distance matrix - bazw edw distance otan auti ipologistei
  std::unordered_map <string ,std::unordered_map<string, double>> Distance_Map;
  //double Distance_Matrix[keys.size()][keys.size()]; //all points with all points distances
  for (unsigned int i = 0; i < keys.size(); i++)
  {
    for (unsigned int j = 0; j < keys.size(); j++)
    {
      Distance_Map[keys[i]][keys[j]] =-1.0; //arxika -1 gt den gnt negative distance - 8a upologizw & 8a vazw ligo ligo o,ti m xreiazetai => less upologismoi
    }
  }

  std::vector<std::string> ids_kentrwn; //ta Ids twn kentrwn
  ids_kentrwn.clear();

  //to 1o kentro random
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<double>  distr(0, keys.size());
  int rand_id_1 = floor(distr(generator));
  
  std::vector<my_vector<T>> ta_kentra; //pou 8a epistrepsoume
  ta_kentra.clear();

  my_vector<T> one_v_atime;
  one_v_atime.set_id((*vectors_array)[keys[rand_id_1]].get_id());
  one_v_atime.set_v((*vectors_array)[keys[rand_id_1]].get_v());
  ta_kentra.push_back(one_v_atime);

  ids_kentrwn.push_back(one_v_atime.get_id()); //balame to 1o kentro

  //array P
  std::vector<std::pair<std::string, double>> P_array; //auto pou mporei na ginei BST later on
  P_array.clear();

  std::vector<double> apostaseis; //edw 8a valw tis apostaseis gia na vrw thn max kai na diairesw me auti tis distances pou 8a balw sto P_array
  apostaseis.clear();

  //twra ta upoloipa kentra
  for (unsigned int j = 0; j < keys.size(); ++) //kathe my_vector
  {
    //elegxw an einai kentro idi
    if (ids_kentrwn.find((*vectors_array)[keys[j]].get_id()) != ids_kentrwn.end()) //uparxei hdh auto to my_v ws kentro
      continue;

    //an den einai kentro:
    double min1 = std::numeric_limits<double>::max(); //apeiro
    for (unsigned int i = 0; i < ids_kentrwn.size(); i++) //vres thn apostasi tou j apo kathe kentro i
    {
      //elegxw an exw upologisei to dist panw
      //if ((Distance_Map[ids_kentrwn[i]][(*vectors_array)[keys[j]].get_id()] > 0) && (Distance_Map[(*vectors_array)[keys[j]].get_id()][ids_kentrwn[i]] > 0)) //exei upologistei
      if (Distance_Map[ids_kentrwn[i]][(*vectors_array)[keys[j]].get_id()] < 0) //den tin exw upologisei
      {
        Distance_Map[ids_kentrwn[i]][(*vectors_array)[keys[j]].get_id()] = manhattan_distance((*vectors_array)[ids_kentrwn[i]].get_v(), (*vectors_array)[keys[j]].get_v()); //upologizw to distance auto
      }   

      if (Distance_Map[ids_kentrwn[i]][(*vectors_array)[keys[j]].get_id()] < min1)
      {
        min1 = Distance_Map[ids_kentrwn[i]][(*vectors_array)[keys[j]].get_id()]; //krata tin elaxisti apostasi
      }
    }
    //else einai ypologismeno hdh, pame na kanoume push back:
    std::pair<std::string, double> temp_pair;
    temp_pair.first = (*vectors_array)[keys[j]].get_id(); //se poio shmeio anaferomai?
    temp_pair.second = min1*min1;
    apostaseis.push_back(min1); //apothikeuw
    P_array.push_back(temp_pair);
  }
  //find max in apostaseis
  double max1 = 0.0;
  for (unsigned int i = 0; i < apostaseis.size(); i++)
  {
    if (apostaseis[i] > max1)
    {
      max1 = apostaseis[i];
    }
  }
    
  //divide all pair.second me autin tin timi
  for (unsigned int i = 0; i < P_array.size(); i++)
  {
    P_array[i].second = P_array[i].second/max1; //normalized
  }

  for (unsigned int i = 0; i < P_array.size(); i++)
  {
    double sum = 0.0;
    for (unsigned int j = 0; j < i; j++)
    {
      sum += P_array[j];
    }
    P_array[i].second += sum;
  }
  //bres random x (float) anamesa sto 0 kai to P_array[last].second
  double megisto = P_array[P_array.size()-1].second; //last value is tops
  std::uniform_real_distribution<double>  distr2(0, megisto);
  double x = distr2(generator);
  //auto to x einai anamesa se alles 2 times P_array[i].second kai P_array[i+1].second
  //binary search - P_array is already sorted! :D
  int z = P_array.size()/2;
  while (z > 0)
  {
    if (P_array[z].second > x)
    {
      /* code */
    }
    else if (P_array[z].second < x)
    {

    }
    else // iso
    {

    }
    z /= 2;
  }
  
  //ara to neo kentro einai P_array[i+1].first //afou ta slides lene <, <=
  //push back to x sta kentra kai ksanamana
}


//init custers based on kentra pou brethikan parapanw 
template <typename T>
void initialize_clusters (std::vector<my_vector<T>> *cluster_centers, std::vector<cluster<T>> *clusters) {
  for(unsigned int i=0; i<(*cluster_centers).size(); i++){
      cluster<T> onecatatime(&((*cluster_centers)[i]));
      (*clusters).push_back(onecatatime);
    }
}
