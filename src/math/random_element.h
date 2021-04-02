#ifndef RANDOMELEMENT_SIMPLENETWORK_H
#define RANDOMELEMENT_SIMPLENETWORK_H

#include <random>

//#define SAME_RANDOM

using namespace std;

template<typename T = void>
class RandomElement
{
private:
	static bool is_created;
	static default_random_engine random_engine;
	
	static uniform_real_distribution<> u_normalized_distribution;
	static uniform_real_distribution<> normalized_distribution;
	static uniform_int_distribution<unsigned long long int> int_distribution;

protected:
	unsigned long long int MAX_RND_INT_VAL = ULLONG_MAX;

	RandomElement()
	{
		if(!is_created)
		{
			is_created = true;
			new (&u_normalized_distribution) uniform_real_distribution<>(0, 1);
			new (&normalized_distribution) uniform_real_distribution<>(-1, 1);
			new (&int_distribution) uniform_int_distribution<unsigned long long int>();
#ifndef SAME_RANDOM
			new (&random_engine) default_random_engine(
				random_device{}());
#endif
		}
	}

	static double r_norm_udbl()
	{ return u_normalized_distribution(random_engine); }
	static double r_norm_dbl()
	{ return normalized_distribution(random_engine); }

	static unsigned long long rnd()
	{ return int_distribution(random_engine); }
	static unsigned long long rnd(unsigned long long max)
	{ return int_distribution(random_engine) % max; }
	static unsigned long long rnd(unsigned long long min, unsigned long long max)
	{ return int_distribution(random_engine) % (max - min) + min; }
};

template<typename T>
bool RandomElement<T>::is_created;
template<typename T>
default_random_engine RandomElement<T>::random_engine;

template<typename T>
uniform_real_distribution<> RandomElement<T>::u_normalized_distribution;
template<typename T>
uniform_real_distribution<> RandomElement<T>::normalized_distribution;
template<typename T>
uniform_int_distribution<unsigned long long int> RandomElement<T>::int_distribution;

#endif //RANDOMELEMENT_SIMPLENETWORK_H
