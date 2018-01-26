
#include "lir.h"

NS_LIR_BEGIN
namespace test{

class LirTest
{
public:
	LirTest() {};
	virtual std::string test()=0;
protected:
	~LirTest() {};
};

}
NS_LIR_END
