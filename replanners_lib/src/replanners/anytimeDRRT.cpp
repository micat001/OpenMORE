﻿#include "anytimeDRRT.h"

namespace pathplan
{

AnytimeDynamicRRT::AnytimeDynamicRRT(Eigen::VectorXd& current_configuration,
                                     PathPtr& current_path,
                                     const double& max_time,
                                     const TreeSolverPtr &solver): DynamicRRT(current_configuration,current_path,max_time,solver)
{
  goal_conf_ = current_path->getConnections().back()->getChild()->getConfiguration();

  const std::type_info& ti1 = typeid(AnytimeRRT);
  const std::type_info& ti2 = typeid(*solver);

  if(std::type_index(ti1) != std::type_index(ti2))
  {
    solver_ = std::make_shared<pathplan::AnytimeRRT>(solver->getMetrics(), solver->getChecker(), solver->getSampler());
    solver_->importFromSolver(solver); //copy the required fields

//    solver_->import

//    if(!solver->solved())
//      assert(0);

    if(goal_conf_ != solver_->getGoal()->getConfiguration())
      assert(0);
  }
}
void AnytimeDynamicRRT::updatePath(NodePtr& node)
{
  TreePtr tree = solver_->getStartTree();
  NodePtr goal = solver_->getSolution()->getConnections().back()->getChild();
  tree->changeRoot(node);
  PathPtr updated_path = std::make_shared<Path>(tree->getConnectionToNode(goal),metrics_,checker_);
  solver_->setSolution(updated_path,true);
}
bool AnytimeDynamicRRT::improvePath(NodePtr &node, const double& max_time)
{
  ros::WallTime tic = ros::WallTime::now();

  PathPtr solution;
  double time = (ros::WallTime::now()-tic).toSec();
  while(time<max_time)
  {
    NodePtr start_node = std::make_shared<Node>(node->getConfiguration());
    NodePtr goal_node  = std::make_shared<Node>(goal_conf_);

    bool success = solver_->improve(start_node,goal_node,solution,1000,(max_time-time));

    if(success)
    {
      replanned_path_ = solution;
      success_ = true;
    }

    time = (ros::WallTime::now()-tic).toSec();
  }
}

bool AnytimeDynamicRRT::replan()
{
  ros::WallTime tic = ros::WallTime::now();

  if(current_path_->getCostFromConf(current_configuration_) == std::numeric_limits<double>::infinity())
  {
    ConnectionPtr conn = current_path_->findConnection(current_configuration_);
    NodePtr node_replan = current_path_->addNodeAtCurrentConfig(current_configuration_,conn,true);

    ROS_INFO_STREAM("Starting node for replanning: \n"<< *node_replan);

    regrowRRT(node_replan);

    updatePath(node_replan);

    double max_time_impr = 0.98*max_time_-(ros::WallTime::now()-tic).toSec();
    improvePath(node_replan,max_time_impr);
  }
  else //replan not needed
  {
    ConnectionPtr conn = current_path_->findConnection(current_configuration_);
    NodePtr node_replan = current_path_->addNodeAtCurrentConfig(current_configuration_,conn,true);

    success_ = false;

    ROS_INFO_STREAM("Starting node for replanning: \n"<< *node_replan);

    updatePath(node_replan);

    double max_time_impr = 0.98*max_time_-(ros::WallTime::now()-tic).toSec();
    improvePath(node_replan,max_time_impr);
  }

  return success_;
}

}
