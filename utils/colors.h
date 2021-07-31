#ifndef _COLORS_
#define _COLORS_

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

std::string FGRN(std::string x) { return KGRN + x + RST; }
std::string FRED(std::string x) { return KRED + x + RST; }
std::string FBLU(std::string x) { return KBLU + x + RST; }
std::string FYEL(std::string x) { return KYEL + x + RST; }
std::string FMAG(std::string x) { return KMAG + x + RST; }
std::string FCYN(std::string x) { return KCYN + x + RST; }
std::string FWHT(std::string x) { return KWHT + x + RST; }

std::string UNDL(std::string x) { return "\x1B[4m" + x + RST; }
std::string BOLD(std::string x) { return "\x1B[1m" + x + RST; }

#endif	/* _COLORS_ */