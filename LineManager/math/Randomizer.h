/** Static Class which provides random number generator. **/
class Randomizer
{

public:
	static void initSeed();
	static void renewSeed();
	static void resetRandomization();
	static int getRandomInt(int min, int max);

private:
	static inline bool seedSet=false;
	static inline int seed=0;
};
