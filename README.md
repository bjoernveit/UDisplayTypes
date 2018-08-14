# UDisplayTypes
This Plugin is split into two Parts. UVisPackage which is essentially a library plugin, for spawning simple marker and trajectories. The second part is UROSVisPackage, which provides the needed files to publish the functionalities to ROS. The UVisPackage can be used without UROSVisPackage if that is desired, but not the other way around.

## Services
The ROS-Services provided are:
  * *DisplayBasicMarker* : This service spawns an Actor displaying a simple Marker. The Marker can be one of the following types: Point, Cube, Sphere, Arrow, Cone or Cylinder (Picture below). The Markers also support transparency if needed.
  * *DisplayTrajectory* : This service spawns an Actor displaying a trajectory (given number of points) in the world. The trajectory either be of one color, or have a start and end color, where the Color of each point depends on its order in the list.
  * *RemoveMarker* : This can be used to remove Actors spawned by the other services.

## Installation

For the this Plugin to work completely you need to install two more plugins, [UROSBridge](https://github.com/robcog-iai/UROSBridge) and [UE4_GPUPointCloudRenderer](https://github.com/hausfrau87/UE4_GPUPointCloudRenderer). To install plugins simply clone them into the plugin folder of your unreal project, make sure to activate the plugins in the editor if needed.

For the publishing to ROS to work you need to set some stuff up on the Ros side as well.
You need to install [rosbridge](http://wiki.ros.org/rosbridge_suite) and the needed srv files, which you can get [here](https://github.com/robcog-iai/unreal_ros_pkgs).

## Connection to ROS
If you have everything setup on both sides, to connect and publish to Ros all you need to do is switch to the ROSBridge editor tool, enter the RosbridgeServer adress, add the UVisManager to the publisher list and hit the ConnectToRosBridge-Button. Check the Output Log to see if the services successfully got published.

## Marker types Preview
![BasicMarkerTypes](https://raw.githubusercontent.com/bjoernveit/UDisplayTypes/gsoc18/Resources/BasicMarkerPreview.PNG "BasicMarkerTypes")


## Unreal Version
This Plugin only works for 4.18, if the third party plugins start supporting higher versions it this plugin should at least support 4.19 as well.
