#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "ThetaStar4Grid.h"
#define ROS_ERROR printf
#define GRID_SIZE 4  //Step of the grid is 5, the higher value of this value, the faster processing time is


namespace thetastar_planner{
#define POT_HIGH 1.0e10        // unassigned cell potential
#define MAX_DIST 1.0e4
class Index {
    public:
        Index(int a, float b) {
            i = a;
            cost = b;
        }
        int i;
        float cost;
};

struct greater1 {
        bool operator()(const Index& a, const Index& b) const {
            return a.cost > b.cost;
        }
};


  class ThetaStarPlanner {
    public:
	ThetaStar4Grid grid_graph_;
    ThetaStarPlanner();
    void outlineMap(unsigned char* costarr, int nx, int ny, unsigned char value);
    //void initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros);
	void initialize(int nx_,int ny_,unsigned char *cost_map);

	std::vector<std::pair<float, float>> makePlan(int start_x,int start_y, int goal_x,int goal_y);

	std::vector<std::pair<float, float>> makeThetaStarPlan(int start_x, int start_y, int goal_x, int goal_y);
     
	bool calculatePotentials(unsigned char* costs, double start_x, double start_y, double end_x, double end_y,
                                        int cycles, float* potential);
     
    void add(unsigned char* costs, float* potential, float prev_potential, int next_i, int end_x,
                         int end_y);

    void mapToWorld(double mx, double my, double& wx, double& wy);

    bool worldToMap(double wx, double wy, double& mx, double& my);

    bool getPath(float* potential, double start_x, double start_y, double end_x, double end_y, std::vector<std::pair<float, float> >& path);
  
    float calculatePotential(float* potential, unsigned char cost, int n, float prev_potential=-1){
            if(prev_potential < 0){
                // get min of neighbors
                float min_h = std::min( potential[n - 1], potential[n + 1] ),
                      min_v = std::min( potential[n - nx], potential[n + nx]);
                prev_potential = std::min(min_h, min_v);
            }

            return prev_potential + cost;
    }

    std::string frame_id_;
    
	inline void FillLinePotential(int x1, int y1, int x2, int y2, float *ptr);
    
private:
      //costmap_2d::Costmap2DROS* costmap_ros_;
      double step_size_, min_dist_from_robot_;
      //costmap_2d::Costmap2D* costmap_;
      unsigned char* cost_array_;
      float* potential_array_;
      int nx,ny,ns;
      //base_local_planner::WorldModel* world_model_; 
      double footprintCost(double x_i, double y_i, double theta_i);
      bool initialized_;
      std::vector<Index> queue_;
      int counter;
      unsigned char lethal_cost_, neutral_cost_;
      bool unknown_;
      float convert_offset_;
  };
};  

