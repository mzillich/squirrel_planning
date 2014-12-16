#include <ros/ros.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "squirrel_planning_knowledge_msgs/KnowledgeItem.h"
#include "mongodb_store/message_store.h"
#include "geometry_msgs/Pose.h"
#include "nav_msgs/GetMap.h"
#include "std_srvs/Empty.h"

#ifndef KCL_roadmap
#define KCL_roadmap

/**
 * This file defines the RPRoadmapServer class.
 * RPRoadmapServer is used to generate waypoints from a given map.
 * Currently this works throught the nav_msgs/GetMap service.
 * Waypoints are stored symbolically in the Knoweldge Base.
 * Waypoint coordinates are stored in the SceneDB (implemented by mongoDB).
 */
namespace KCL_rosplan {

	struct Waypoint 
	{
		Waypoint(const std::string &id, unsigned int xCoord, unsigned int yCoord)
			: wpID(id), x(xCoord), y(yCoord) {}

		Waypoint()
			: wpID("wp_err"), x(0), y(0) {}

		std::string wpID;
		unsigned int x;
		unsigned int y;
		std::vector<std::string> neighbours;
	};

	struct Edge 
	{
		Edge(const std::string &s, const std::string &e)
			: start(s), end(e) {}

		std::string start;
		std::string end;
	};

	class RPRoadmapServer
	{

	private:

		std::string dataPath;

		// Scene database
		mongodb_store::MessageStoreProxy message_store;

		// Knowledge base
		ros::ServiceClient map_client;
		ros::Publisher add_knowledge_pub;
		ros::Publisher remove_knowledge_pub;

		// Roadmap
		std::map<std::string, Waypoint> waypoints;
		std::map<std::string, std::string> db_name_map;
		std::vector<Edge> edges;

	public:

		/* constructor */
		RPRoadmapServer(ros::NodeHandle &nh, std::string &dp);

		/* service to (re)generate waypoints */
		bool generateRoadmap(std_srvs::Empty::Request &req, std_srvs::Empty::Response &res);
		void createPRM(nav_msgs::OccupancyGrid map, unsigned int K, unsigned int D, unsigned int R);
		bool allConnected();
		void connectRecurse(std::map<std::string,bool> &connected, Waypoint &waypoint);
		bool makeConnections(unsigned int R);
	};
}
#endif
