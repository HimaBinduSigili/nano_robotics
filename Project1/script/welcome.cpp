#include <gazebo/gazebo.hh>

namespace gazebo
{
  class WorldPluginMyNewRobot : public WorldPlugin
  {
    public: WorldPluginMyNewRobot() : WorldPlugin()
            {
              printf("Welcome to Bindu's World!\n");
            }

    public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
            {
            }
  };
  GZ_REGISTER_WORLD_PLUGIN(WorldPluginMyNewRobot)
}
