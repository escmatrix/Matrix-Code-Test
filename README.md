# Matrix Code Test
My micro-mouse matrix code test solution.

## Goals
My goals for my solution included - 

* finding the end in as few iterations as possible
* solving both islanded and unislanded mazes 
* guaranteeing to find the end if there is one
* understanding when there may be no end to the maze

## Concept
My precise goals demanded a searching algorithm that could - 

* map out the entire maze in the form of a graph
* find the most shortest path between any two given locations
* optimally search through the maze and find new locations to visit

## Solution
In the end, I made a searching algorithm that essentially works like this - 

1. Find open squares around the character
2. Add these specific locations to the "togo" list
3. Iterate through the "togo" list
   1. BFS to find the path from the current location to the "togo" location
4. Go by the path with the shortest length - Greedy
5. Repeat from Step 1 until end found

## Improvements
A few ways I could improve my current solution include -

* possibly further enhancing the performance of my code in terms time and space
* changing my solution from a Greedy one that tries to pick the shortest path in every case 
  into a solution that determines the shortest distance in total while visiting every place togo
  * This solution may not be an improvement because the map is consantly updating
