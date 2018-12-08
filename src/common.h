#pragma once
#ifndef COMMON_H
#define COMMON_H

#define RETURN_ACCELERATION 4.0f
#define NUM_AGENTS 50
#define CONTAIN_RADIUS 5.5f
#define NEARBY_RADIUS .5f
#define DEBUG_ON 1
#define CROWD_RADIUS .3f 
#define MIN_SPEED 2.0f
#define MAX_SPEED 3.5f
#define DEFAULT_COHESION 1.0f
#define DEFAULT_ALIGNMENT 1.0f
#define DEFAULT_NOISE 0.5f
#define DEFAULT_SEPARATION 1.0f


class Common
{
public:
	static float cohesion_weight;
	static float separation_weight;
	static float alignment_weight;
	static float noise_weight;
};


#endif // !CONSTANTS_H
