/* Define the BatwarnFlags enumeration, used for passing command line options
 * throughout the program.  */
#ifndef BATWARN_BATWARNFLAGS_H
#define BATWARN_BATWARNFLAGS_H
enum BatwarnFlags {
	BATWARN_ENABLE_DEBUG = 1,
	BATWARN_ENABLE_HIBERNATE = 2,
	BATWARN_ENABLE_SUSPEND = 4
};
#endif // ! BATWARN_BATWARNFLAGS_H
