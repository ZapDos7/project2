#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <stdlib.h> //labs, llabs
#include <sstream>
#include <cmath> //abs with overload for float
#include <limits>
#include "cluster_object.h"
#include "my_vector.h"
#include "curve_point.h"
#include "curve.h"

/*Manhattan Distance*/
template <typename T>
double manhattan_distance(std::vector<T> v1, std::vector<T> v2);

template <typename T> /*template <class//typename T>*/
double manhattan_distance(std::vector<T> v1, std::vector<T> v2)
{ //sum twn (apoluti timi twn diaforwn twn  (suntetagmeni i tou v1, syntetagmeni i tou v2) )
	if (v1.size() != v2.size())
	{
		std::cerr << "Can't calculate distance, need same dimensions! Aborting...\n";
		exit(-1);
	}
	T temp;
	if ((typeid(temp) == typeid(float)) || (typeid(temp) == typeid(double)))
	{
		temp = 0.0;
	}
	else if (typeid(temp) == typeid(int))
	{
		temp = 0;
	}

	std::vector<T> diffs;
	for (unsigned int i = 0; i < v1.size(); ++i)
	{
		temp = abs(v1[i] - v2[i]);
		diffs.push_back(temp);
		//ws edw leitourgei ok
	}
	double result = 0.0;

	for (typename std::vector<T>::iterator it = diffs.begin(); it != diffs.end(); ++it)
	{
		//std::cerr << *it << "\n";
		result = result + (double)*it; //to athroisma twn apol. diaforwn
									   //std::cerr << result << "\n";
	}
	//std::cerr << "Ara telika exw " << result << "\n";
	return result;
}


/*Functions for DTW*/

template <typename T>
double true_euclidean(curve_point<T> cp1, curve_point<T> cp2)
{
  T x1 = cp1.get_x();
  T y1 = cp1.get_y();

  T x2 = cp2.get_x();
  T y2 = cp2.get_y();

  T x = x1 - x2; //calculating number to square in next step
  T y = y1 - y2;
  double dist;

  if (typeid(x) != typeid(y))
  {
    std::cerr << "Incompatible curve point types (What are you doing?!)\n";
    exit(-1);
  }
  if (typeid(x) == typeid(int))
  {
    int x_faux = x;
    int y_faux = y;
    dist = pow((double)x_faux, 2) + pow((double)y_faux, 2); //calculating Euclidean distance
    dist = sqrt(dist);
  }
  else if (typeid(x) == typeid(double))
  {
    double x_faux = x;
    double y_faux = y;
    dist = pow(x_faux, 2) + pow(y_faux, 2); //calculating Euclidean distance
    dist = sqrt(dist);
  }

  return dist;
}

//dtw me dynamic programming
template <typename T>
double dtw(curve<T> *c1, curve<T> *c2){

  double dist_mat[c1->get_size()+1][c2->get_size()+1];
  for(unsigned int i=1; i<= c1->get_size(); i++)
    dist_mat[i][0] = std::numeric_limits<double>::max();
  for(unsigned int i=1; i<= c2->get_size(); i++)
    dist_mat[0][i] = std::numeric_limits<double>::max();
  dist_mat[0][0] = 0.0;
  for(unsigned int i=1; i<= c1->get_size(); i++){
    for(unsigned int j=1; j<= c2->get_size(); j++){
      double mintrash = std::min(dist_mat[i-1][j-1], dist_mat[i][j-1]);
      mintrash = std::min(mintrash, dist_mat[i-1][j]);
      dist_mat[i][j] = true_euclidean(c1->get_points()[i-1], c2->get_points()[j-1] ) + mintrash;
    }
  }
  return dist_mat[c1->get_size()][c2->get_size()];

}


//format clusters based on kentra pou brethikan parapanw
template <typename T>
void format_clusters (std::vector<my_vector<T>> *cluster_centers, std::vector<cluster<T>> *clusters) {
  (*clusters).clear();
  for(unsigned int i=0; i<(*cluster_centers).size(); i++){
      cluster<T> onecatatime(&((*cluster_centers)[i]));
      (*clusters).push_back(onecatatime);
    }
}

template <typename T>
void format_curve_clusters (std::vector<curve<T>> *cluster_centers, std::vector<curve_cluster<T>> *clusters) {
  (*clusters).clear();
  for(unsigned int i=0; i<(*cluster_centers).size(); i++){
      curve_cluster<T> onecatatime(&((*cluster_centers)[i]));
      (*clusters).push_back(onecatatime);
    }
}


//shmantiko gia lsh kampylwn
template <typename T>
std::pair<double, T> calculate_delta(std::unordered_map<std::string, curve<T> > *kurv_array){
	std::vector<curve<T>> curves_array;
	for(auto x:*kurv_array){
		curves_array.push_back(x.second);
	}

	double delta = 0.0;                                        //mesi apostasi shmeiwn kampulws
    double inf = std::numeric_limits<double>::max();           //apeiro kai kala
    double max_coord = -1 * inf;                               //arxika -apeiro
    ;                                                          //h megisti metabliti pou uparxei sto dataset
    for (unsigned int i = 0; i < curves_array.size(); i++) //an theloume ola & query, sbhnoume to "-q" kai ola popa
    {
      double tmp = 0.0;
      double plithos_athroismatwn = 0.0;
      std::vector<curve_point<T>> shmeia = curves_array[i].get_points(); //pairnw to vector apo shmeia kathe kampulhs
      if (shmeia.size() < 2)
      {
        if (shmeia[i].get_x() > max_coord)
        {
          max_coord = shmeia[i].get_x();
        }
        if (shmeia[i].get_y() > max_coord)
        {
          max_coord = shmeia[i].get_y();
        }
        continue;
      }
      else
      {
        if (shmeia[i].get_x() > max_coord)
        {
          max_coord = shmeia[i].get_x();
        }
        if (shmeia[i].get_y() > max_coord)
        {
          max_coord = shmeia[i].get_y();
        }
        for (unsigned int j = 0; j < shmeia.size() - 1; j++) //pairnw kathe shmeio tou vector ^
        {
          tmp += true_euclidean<double>(shmeia[j], shmeia[j + 1]);
          plithos_athroismatwn += 1.0;
        }
        tmp = tmp / plithos_athroismatwn;
        delta += tmp;
        //std::cerr << "Tmp delta is " << delta << '\n';
      }
    }
    delta = delta / (double)(curves_array.size()); //ki edw sbhnw to "-q" an eimai stin periptosi pou thelw kai ta query
		std::pair<double, T> twoelems;
		twoelems.first = delta;
		twoelems.second = max_coord;
		return twoelems;
}


#endif
