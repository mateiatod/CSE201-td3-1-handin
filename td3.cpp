#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double *newarray;
  newarray = new double[new_size];
  for (int i = 0; i < length; i++){
      newarray[i] = array[i];
  }
  for (int i = length; i < new_size; i++){
      newarray[i] = 0;
  }
//  for (double *p = array; p < array + length; p++){
//      delete p;
//  }
   delete[] array;

  return newarray;
}

double* shrink_array(double* array, int length, int new_size) {
  double *newarray;
  newarray = new double[new_size];
  for (int i = 0; i < new_size; i++){
      newarray[i] = array[i];
  }
  delete[] array;

  return newarray;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  if (current_size == max_size){
//      array = extend_array(array,max_size,max_size + 5);
//      array[current_size] = element;
//      return array;
      double* newarray;
      newarray = extend_array(array,max_size,max_size + 5);
      newarray[current_size] = element;
      current_size++;
      max_size += 5;
      return newarray;
  }
  else {
      array[current_size] = element;
      current_size++;
      return array;
  }
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
//  array[current_size - 1] = 0;
  current_size--;
  if (max_size - current_size > 4){
      double *newarray;
      newarray = shrink_array(array,max_size,max_size - 5);
      max_size -= 5;
      return newarray;
  }

  return array;
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;
//  telemetry = append_to_array(t,telemetry,telemetry_current_size,telemetry_max_size);
//  telemetry = append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
//  telemetry = append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);
  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
      telemetry = append_to_array(t,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry = append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry = append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
      telemetry = append_to_array(t,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry = append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry = append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);
    } else {
      telemetry = append_to_array(t,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry = append_to_array(x,telemetry,telemetry_current_size,telemetry_max_size);
      telemetry = append_to_array(y,telemetry,telemetry_current_size,telemetry_max_size);
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
  }

  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  if (tot_telemetries == 0){
      return;
  }
  int pointers[tot_telemetries],sum = 0;
  for (int i = 0; i < tot_telemetries; i++){
      pointers[i] = 0;
      sum += telemetries_sizes[i];
  }
  sum /= 3;
//  std::cout<<telemetries;
  for (int _ = 0; _ < sum ; _++){
      double min_t;
      for (int i = 0; i < tot_telemetries;i++){
          if (telemetries_sizes[i] > pointers[i]){
              min_t = telemetries[i][pointers[i]];
              break;
          }

      }
/*      double min_t = telemetries[][]*/;
      int position;
      for (int i = 0; i < tot_telemetries; i++){
          if (telemetries_sizes[i] > pointers[i] && telemetries[i][pointers[i]] <= min_t){
              min_t = telemetries[i][pointers[i]];
              position = i;
          }
      }
      global_telemetry = append_to_array(telemetries[position][pointers[position]],global_telemetry,global_telemetry_current_size,global_telemetry_max_size);
      global_telemetry = append_to_array(telemetries[position][pointers[position]+1],global_telemetry,global_telemetry_current_size,global_telemetry_max_size);
      global_telemetry = append_to_array(telemetries[position][pointers[position]+2],global_telemetry,global_telemetry_current_size,global_telemetry_max_size);
      pointers[position] += 3;
  }
}
