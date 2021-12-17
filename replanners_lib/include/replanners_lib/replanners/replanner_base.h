#ifndef REPLANNERBASE_H__
#define REPLANNERBASE_H__
#include <eigen3/Eigen/Core>
#include <ros/ros.h>
#include <graph_core/util.h>
#include <graph_core/graph/graph_display.h>
#include <graph_core/graph/tree.h>
#include <graph_core/graph/path.h>
#include <graph_core/graph/connection.h>
#include <graph_core/graph/node.h>
#include <graph_core/metrics.h>
#include <graph_core/solvers/tree_solver.h>
#include <graph_core/solvers/path_solver.h>
#include <graph_core/collision_checker.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <rviz_visual_tools/rviz_visual_tools.h>
#include <moveit/robot_model/robot_model.h>
#include <mutex>

namespace pathplan
{
class ReplannerBase;
typedef std::shared_ptr<ReplannerBase> ReplannerBasePtr;

class ReplannerBase: public std::enable_shared_from_this<ReplannerBase>
{
protected:
  Eigen::VectorXd current_configuration_;
  PathPtr current_path_;
  PathPtr replanned_path_;
  TreeSolverPtr solver_;
  MetricsPtr metrics_;
  CollisionCheckerPtr checker_;
  Eigen::VectorXd lb_;
  Eigen::VectorXd ub_;
  DisplayPtr disp_;

  bool success_;
  bool verbose_;
  double max_time_;

  virtual bool computeConnectingPath(const NodePtr &path1_node_fake, const NodePtr &path2_node_fake, const double &diff_subpath_cost, PathPtr &connecting_path, bool &directly_connected);
  virtual bool computeConnectingPath(const NodePtr &path1_node_fake, const NodePtr &path2_node_fake, const double &diff_subpath_cost, PathPtr &connecting_path, bool &directly_connected, TreeSolverPtr &solver);
  virtual PathPtr concatConnectingPathAndSubpath2(const std::vector<ConnectionPtr>& connecting_path_conn, const std::vector<ConnectionPtr>& subpath2, const NodePtr& path1_node, const NodePtr& path2_node);

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  ReplannerBase(const Eigen::VectorXd &current_configuration,
                const PathPtr &current_path,
                const double &max_time,
                const TreeSolverPtr& solver);

  PathPtr getReplannedPath()
  {
    return replanned_path_;
  }

  PathPtr getCurrentPath()
  {
    return current_path_;
  }

  virtual void setCurrentPath(const PathPtr& path)
  {
    current_path_ = path;
    success_ = false;
  }

  void setReplannedPath(const PathPtr& path)
  {
    replanned_path_ = path;
  }

  void setMaxTime(const double& max_time)
  {
    max_time_ = max_time;
  }

  virtual void setVerbosity(const bool& verbose)
  {
    verbose_ = verbose;
  }

  virtual void setCurrentConf(const Eigen::VectorXd& q)
  {
    current_configuration_ = q;
    success_ = false;
  }

  Eigen::VectorXd getCurrentConf()
  {
    return current_configuration_;
  }

  TreeSolverPtr getSolver()
  {
    return solver_;
  }

  void setChecker(const CollisionCheckerPtr &checker)
  {
    checker_ = checker;
  }

  void setDisp(const DisplayPtr &disp)
  {
    ROS_INFO("QUA");
    disp_ = disp;
    ROS_INFO("QUA1");

  }

  DisplayPtr getDisp()
  {
    return disp_;
  }

  bool getSuccess()
  {
    return success_;
  }

  virtual std::vector<ConnectionPtr> startReplannedTreeFromNewCurrentConf(const Eigen::VectorXd &configuration, const PathPtr &current_path_copy);
  virtual void startReplannedPathFromNewCurrentConf(const Eigen::VectorXd &configuration);

  virtual bool replan() = 0;
};
}

#endif // REPLANNERBASE_H
