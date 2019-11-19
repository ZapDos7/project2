#include "utils.h"

#include <unordered_map>
#include "cluster_object.h"
#include "my_vector.h"
#include "NNpair.h"
#include "ht.h"

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



//INVERSE ASSIGNMENT WITH RANGE SEARCH LSH BLA BLA
template <typename T> //briskei thn arxikh aktina
double initialize_radius(std::vector<cluster<T>>* clusters)
{
  double init_radius = std::numeric_limits<double>::max();
  for(unsigned int i=0; i< clusters->size(); i++){
    for(unsigned int j=0; j< clusters->size(); j++){

      if((*clusters)[i].get_center_id() == (*clusters)[j].get_center_id()) //einai to idio kentro
        continue;

      if( manhattan_distance((*clusters)[i].get_center_coords(), (*clusters)[j].get_center_coords()) < init_radius)
          init_radius = manhattan_distance((*clusters)[i].get_center_coords(), (*clusters)[j].get_center_coords()) ;

    }
  }
  return init_radius / 2;
}



template <typename T> //APO ERGASIA 1
void LSH_range_ass(std::vector<cluster<T>>* clusters, std::unordered_map<std::string, my_vector<T> > *vectors_array,int diastaseis ,int number_of_vector_hash_tables, int number_of_vector_hash_functions)
{
    //auto start_of_w_calc = std::chrono::high_resolution_clock::now();
    std::vector<NNpair> input_actual_NNs; //pinakas apo zeugaria actual NNs me prwto stoixeio to p
    for (auto x :(*vectors_array))
    { //prepei na brw ta zeugaria ap to input gia ypologismo w

      std::string min_id1;
      double min1 = std::numeric_limits<double>::max(); //min pairnei timh apeiro
      for (auto y :(*vectors_array))
      {
        if (manhattan_distance(x.second.get_v(), y.second.get_v()) == 0) //einai to idio shmeio
          continue;

        if (manhattan_distance(x.second.get_v(), y.second.get_v()) < min1)
        {
          min1 = manhattan_distance(x.second.get_v(), y.second.get_v());
          min_id1 =  y.second.get_id();
        }
      }
      NNpair single_pair1(x.second.get_id(), min_id1);
      single_pair1.set_distance(min1);
      input_actual_NNs.push_back(single_pair1);
    }

    double tmp = 0.0;
    double mean_distance = 0;
    for (unsigned int i = 0; i < input_actual_NNs.size(); i++)
    {
      tmp += input_actual_NNs.at(i).get_distance();
    }
    //int diastaseis = vectors_array->begin().second.get_v().size(); //gia hash tables
    std::cout << "diasteaseis " << diastaseis << "\n";
    int Table_Size = (*vectors_array).size() / 8;
    mean_distance = tmp / input_actual_NNs.size(); //fp division
    //auto end_of_w_calc = std::chrono::high_resolution_clock::now() - start_of_w_calc;
    //long long microseconds_w = std::chrono::duration_cast<std::chrono::microseconds>(end_of_w_calc).count();
    //fprintf(stderr, "Time needed for w calculation is %lld microseconds.\n\n", microseconds_w);
    fprintf(stderr, "Value of w = %f\n", mean_distance);
    //also test gia w = 10 * mean_distance
    /*const*/ double w = 4 * mean_distance; //to w pou vazw sta ai, STH XEIROTERH HARD CODED

    /////////////////////////////LSH TIME////////////////////////////////
    std::vector<ht<T>> our_hash_tables;
    for (int i = 0; i < number_of_vector_hash_tables; i++)
    {
      ht<T> temp_hash_table(Table_Size, number_of_vector_hash_functions, diastaseis, w);
      our_hash_tables.push_back(temp_hash_table);
    }

    std::unordered_map<std::string, int> owned; //to flag poy lene oi diafaneies gia to an kaparw8hke ena shmeio apo ena cluster
    int n =0;
    for (auto x :(*vectors_array)) //hasharw ta vectors k arxikopoiw flags
    {
      for (int j = 0; j < number_of_vector_hash_tables; j++)
        our_hash_tables[j].hash_vector(&(x.second));

      owned[x.first] = -1;
      n++;
    }

    int num_remaining = n; //posa exoun meinei xwris anathesh
    double radius = initialize_radius(clusters);
    std::vector<std::string> this_center_neighbs;
    std::vector<std::string> this_HT_neighbs;
    for(unsigned int i=0; i< clusters->size(); i++){

      this_center_neighbs.clear();
      for (int j = 0; j < number_of_vector_hash_tables; j++){
        this_HT_neighbs.clear();
        this_HT_neighbs = our_hash_tables[j].hash_query((*clusters)[i].get_center_ptr(), radius, false);
        this_center_neighbs.insert(this_center_neighbs.end(), this_HT_neighbs.begin(), this_HT_neighbs.end());
      }
      //pros8hkh shmeiwn se cluster kai flag gia na mhn to paroyn kai ta ypoloipa clusters
      for(unsigned int z=0; z< this_center_neighbs.size(); z++){
        if( owned[this_center_neighbs[z]] == -1 ){ //den exei kaparw8ei
            (*clusters)[i].incorporate_point(&((*vectors_array)[this_center_neighbs[z]]));
            owned[this_center_neighbs[z]] = i; //to kaparwse
        }
        else{ //sugkrinoume me auton poy to exei kaparwsei
          if(manhattan_distance((*vectors_array)[this_center_neighbs[z]].get_v() , (*clusters)[i].get_center_coords()) < manhattan_distance((*vectors_array)[this_center_neighbs[z]].get_v() , (*clusters)[owned[this_center_neighbs[z]]].get_center_coords()) ){
            (*clusters)[owned[this_center_neighbs[z]]].discorporate_point(&((*vectors_array)[this_center_neighbs[z]])); //to bgazei ap to palio
            (*clusters)[i].incorporate_point(&((*vectors_array)[this_center_neighbs[z]])); //to vazei sto neo
            owned[this_center_neighbs[z]] = i; //to kaparwse
          }
        }
      }


    }
    //twra an kapoio exei meinei akaparwto, prepei na paei sto kontinotero tou kentro
    for(auto x: owned){
      if(x.second == -1){ //akaparwto
        double min1 = std::numeric_limits<double>::max(); //apeiro
        int min_clust_index = -1; //to index tou cluster opou anoikei kathe x
        for (unsigned int j = 0; j < (*clusters).size(); j++)
        {
            double tmp = 0.0;
            tmp = manhattan_distance( (*vectors_array)[x.first].get_v(), (*clusters)[j].get_center_coords()); //dist(x.second.get_v(), (*clusters)[j].get_center_coords())
            if (tmp < min1)
            {
                min1 = tmp;
                min_clust_index = j;
            }
            //std::cout << "Eimai " << x.first << " & sugkrinw " << (*clusters)[j].get_center_id() << " dist " << tmp << '\n';
        }
        (*clusters)[min_clust_index].incorporate_point(&((*vectors_array)[x.first]));
      }
    }


}
