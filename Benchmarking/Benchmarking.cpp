// Benchmarking.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <random>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <stdint.h>

//  Windows
#ifdef _WIN32
#include <intrin.h>
uint64_t rdtsc() {
	return __rdtsc();
}
//  Linux/GCC
#else

uint64_t rdtsc() {
	unsigned int lo, hi;
	__asm__ __volatile__( "rdtsc" : "=a" ( lo ), "=d" ( hi ) );
	return ( (uint64_t)hi << 32 ) | lo;
}

#endif

#pragma warning(disable : 4244 4305) // double <-> float conversions

using namespace std;

struct Vector4 {
	Vector4() {};
	Vector4( float x, float y, float z, float h ) : x( x ), y( y ), z( z ), h( h ) {}
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float h = 0.0;
};

std::random_device random_device;
std::mt19937 generator( ::random_device() ); // init a standard mersenne_twister_engine
std::vector<Vector4> vector_a;
std::vector<Vector4> vector_b;
//std::vector<Vector4> vector_c;
//clock_t clicks;
//unsigned time_stamp_counter;
std::vector<unsigned> samples_GetDistanceBetween3DCoords;
std::vector<unsigned> samples_GetIsDistanceBetween3DCoordsLessThan;


inline int GetFromUniformIntDistribution( int first, int second ) {
	std::uniform_int_distribution<> uniform_int_distribution( first, second );
	int result = uniform_int_distribution( generator );
	return result;
}

inline double GetFromUniformRealDistribution( double first, double second ) {
	std::uniform_real_distribution<> uniform_real_distribution( first, second );
	double result = uniform_real_distribution( generator );
	return result;
}

inline double GetDistanceBetween3DCoords( Vector4 vec_a, Vector4 vec_b ) {
	return std::sqrt( std::pow( vec_a.x - vec_b.x, 2 ) + std::pow( vec_a.y - vec_b.y, 2 ) + std::pow( vec_a.z - vec_b.z, 2 ) );
}

inline bool GetIsDistanceBetween3DCoordsLessThan( Vector4 vec_a, Vector4 vec_b, double distance ) {
	if ( !( std::abs( vec_a.x - vec_b.x ) < distance ) ) return false;
	if ( !( std::abs( vec_a.y - vec_b.y ) < distance ) ) return false;
	if ( !( std::abs( vec_a.z - vec_b.z ) < distance ) ) return false;
	if ( !std::sqrt( std::pow( vec_a.x - vec_b.x, 2 )
		+ std::pow( vec_a.y - vec_b.y, 2 )
		+ std::pow( vec_a.z - vec_b.z, 2 ) < distance ) ) return false;
	return true;
}

int myrandom( int i ) { return GetFromUniformIntDistribution( 0, RAND_MAX ) % i; }

std::vector<Vector4> PopulateVectors( std::vector<Vector4> vector, unsigned count ) {
	for ( unsigned i = 0; i < count; i++ ) {
		Vector4 v;
		v.x = GetFromUniformRealDistribution( -3000, 3000 );
		v.y = GetFromUniformRealDistribution( -3000, 3000 );
		v.z = GetFromUniformRealDistribution( -200, 200 );
		vector.push_back( v );
	}
	return vector;
}

unsigned BenchmarkGetDistanceBetween3DCoords( std::vector<Vector4> vector_a, std::vector<Vector4> vector_b ) {

	clock_t clicks = clock();
	unsigned time_stamp_counter = rdtsc();
	std::vector<Vector4> vector_c;
	for ( Vector4 a : vector_a ) {
		for ( Vector4 b : vector_b ) {
			if ( GetDistanceBetween3DCoords( a, b ) < 1.0f ) {
				vector_c.push_back( a );
			}
		}
	}

	time_stamp_counter = rdtsc() - time_stamp_counter;
	return time_stamp_counter;
}

unsigned BenchmarkGetIsDistanceBetween3DCoordsLessThan( std::vector<Vector4> vector_a, std::vector<Vector4> vector_b ) {

	clock_t clicks = clock();
	unsigned time_stamp_counter = rdtsc();
	std::vector<Vector4> vector_c;
	for ( Vector4 a : vector_a ) {
		for ( Vector4 b : vector_b ) {
			if ( GetIsDistanceBetween3DCoordsLessThan( a, b, 1.0f ) ) {
				vector_c.push_back( a );
			}
		}
	}

	time_stamp_counter = rdtsc() - time_stamp_counter;
	return time_stamp_counter;
}

int main() {

	unsigned vectors_size = 400;
	unsigned test_count = 1000;
	unsigned average = 0;

	clock_t clicks = clock(); unsigned time_stamp_counter = rdtsc();
	vector_a = PopulateVectors( vector_a, vectors_size );
	vector_b = PopulateVectors( vector_b, vectors_size * 10 );
	for ( Vector4 v : vector_a ) {
		if ( GetFromUniformRealDistribution( 0, 1 ) < 0.5f ) {
			vector_b.push_back( v );
		}
	}
	std::random_shuffle( vector_b.begin(), vector_b.end(), myrandom );
	clicks = clock() - clicks; time_stamp_counter = rdtsc() - time_stamp_counter;
	cout << "vector_a.size(): " << vector_a.size() << endl << "vector_b.size(): " << vector_b.size() << endl;
	cout << "time to generate vectors: " << clicks << " clicks ( " << time_stamp_counter << " CPU cycles)" << endl;
	cout << endl;

	cout << "GetDistanceBetween3DCoords(): " << test_count << " iterations" << endl;
	for ( unsigned i = 0; i < test_count; i++ ) {
		samples_GetDistanceBetween3DCoords.push_back( BenchmarkGetDistanceBetween3DCoords( vector_a, vector_b ) );
		cout << ".";
	}
	cout << endl;
	unsigned average_slow = std::accumulate( samples_GetDistanceBetween3DCoords.begin(), samples_GetDistanceBetween3DCoords.end(), 0.0 ) / samples_GetDistanceBetween3DCoords.size();
	cout << "average CPU cycles: " << average_slow << endl;
	cout << endl;

	cout << "GetIsDistanceBetween3DCoordsLessThan(): " << test_count << " iterations" << endl;
	for ( unsigned i = 0; i < test_count; i++ ) {
		samples_GetIsDistanceBetween3DCoordsLessThan.push_back( BenchmarkGetIsDistanceBetween3DCoordsLessThan( vector_a, vector_b ) );
		cout << ".";
	}
	cout << endl;
	unsigned average_fast = std::accumulate( samples_GetIsDistanceBetween3DCoordsLessThan.begin(), samples_GetIsDistanceBetween3DCoordsLessThan.end(), 0.0 ) / samples_GetIsDistanceBetween3DCoordsLessThan.size();
	cout << "average CPU cycles: " << average_fast << endl;
	cout << endl;

	double ratio = (double)average_slow / (double)average_fast;
	cout << "GetIsDistanceBetween3DCoordsLessThan() is approvximately " << ratio << " times faster than GetDistanceBetween3DCoords()" << endl;
	//system("pause");
}