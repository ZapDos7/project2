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
    //std::cout << "diasteaseis " << diastaseis << "\n";
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

    std::unordered_map<std::string, std::pair<int, double>> owned; //to flag poy lene oi diafaneies gia to an kaparw8hke ena shmeio kai apo poio index kai me poia aktina
    int n =0;
    //for (auto x :(*vectors_array)) //hasharw ta vectors k arxikopoiw flags
    for (auto x :(*vectors_array)) //hasharw ta vectors k arxikopoiw flags
    {
      for (int j = 0; j < number_of_vector_hash_tables; j++)
        {our_hash_tables[j].hash_vector(&((*vectors_array)[x.first]));}

      std::pair<int, double> index_and_radius;
      index_and_radius.first = -1;
      index_and_radius.second = 0.0;
      owned[x.first] = index_and_radius;
      n++;
    }

    int num_unassigned = n; //posa exoun meinei xwris anathesh
    double radius = initialize_radius(clusters);
    std::vector<std::string> this_center_neighbs;
    std::vector<std::string> this_HT_neighbs;
    bool repetition = false;
    int kill_countdown = 15; //an den exoun ginei nees anatheseis meta apo tosous diplasiasmous aktinas, stop
    int num_unassigned_prev = num_unassigned; //arithmos unassigned shmeiwn prin th loypa gia na sugkrinoyme proodo kathe fora kai na stamatame

    while((num_unassigned > n/10) && (kill_countdown >0) ){ //h anazhthsh range search lsh tha ginetai mexri to 90% twn shmeiwn ginei assign se kapoio kentro. Epeita klassikh methodos opws prota8hke

      num_unassigned_prev = num_unassigned;
      for(unsigned int i=0; i< clusters->size(); i++){ //gia kathe kentro twn clusters

        this_center_neighbs.clear();
        for (int j = 0; j < number_of_vector_hash_tables; j++){ //LSH se L hashtables
          this_HT_neighbs.clear();
          this_HT_neighbs = our_hash_tables[j].hash_query((*clusters)[i].get_center_ptr(), radius, repetition);
          //std::cout << this_HT_neighbs.size() << "-";
          this_center_neighbs.insert(this_center_neighbs.end(), this_HT_neighbs.begin(), this_HT_neighbs.end());
          //std::cout << this_center_neighbs.size() << " ";
        }
        //pros8hkh shmeiwn se cluster kai flag gia na mhn to paroyn kai ta ypoloipa clusters
        //std::cout << "eimai to cl " << (*clusters)[i].get_center_id() << "kai "<<this_center_neighbs.size() << "\n";
        for(unsigned int z=0; z< this_center_neighbs.size(); z++){
          //std::cout << owned[this_center_neighbs[z]].first ;
          //std::cout << this_center_neighbs[z] ;
          if( owned[this_center_neighbs[z]].first == -1 ){ //den exei kaparw8ei
              //std::cout << "kapakap ";
              (*clusters)[i].incorporate_point(&((*vectors_array)[this_center_neighbs[z]]));
              owned[this_center_neighbs[z]].first = i; //to kaparwse
              owned[this_center_neighbs[z]].second = radius; //to kaparwse entos aktinas toshs
              num_unassigned--;
          }
          else{ //sugkrinoume me auton poy to exei kaparwsei
            if(owned[this_center_neighbs[z]].second >= radius){ //an kapoios allos to exei kaparwsei me mikroterh aktina, apofeugoume th sugkrish kai proxwrame
              if(manhattan_distance((*vectors_array)[this_center_neighbs[z]].get_v() , (*clusters)[i].get_center_coords()) < manhattan_distance((*vectors_array)[this_center_neighbs[z]].get_v() , (*clusters)[owned[this_center_neighbs[z]].first].get_center_coords()) ){
                (*clusters)[owned[this_center_neighbs[z]].first].discorporate_point(&((*vectors_array)[this_center_neighbs[z]])); //to bgazei ap to palio
                (*clusters)[i].incorporate_point(&((*vectors_array)[this_center_neighbs[z]])); //to vazei sto neo
                owned[this_center_neighbs[z]].first = i; //to kaparwse
                owned[this_center_neighbs[z]].second = radius; //to kaparwse entos aktinas toshs
              } //telos if gia sugkrish apostasewn
            } //telos if poy afora an ena kaparwmeno shmeio exei kaparw8ei apo mikroterh aktina ara den exei nohma na koitaksoume pali
          } //telos else poy afora to an ena shmeio einai kaparwmeno h oxi
        } //telos gor gia auta poy brhke auto to cluster gia authn thn aktina
      } //telos for gia ta clusters

      if(num_unassigned_prev - num_unassigned <=0 ) //den kaname proodo, arxise antistrofh metrhsh
        kill_countdown--;
      else
        kill_countdown = 15; //eixame proodo, mhdenise thn antistrofh metrhsh

      if(kill_countdown<=0) //den yphrkse veltiwsh gia sunexomenes loypes, telos
        break;

      radius = radius*2; //diplasiazoume aktina kai sunexizoume
      repetition = true;
      //std::cout << num_unassigned << "\n";
    } //telos ths while poy diplasiazoume thn aktina


    //twra an kapoio exei meinei akaparwto, prepei na paei sto kontinotero tou kentro
    for(auto x: owned){
      if(x.second.first == -1){ //akaparwto
        double min1 = std::numeric_limits<double>::max(); //apeiro
        int min_clust_index = -1; //to index tou cluster opou anoikei kathe x
        //(*vectors_array)[x.first].get_id()
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
