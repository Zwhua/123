#ifndef GIRL_H_
#define GIRL_H_

#include "People.h"

class Girl : public People {
public:
	Girl();
	virtual ~Girl();
	bool init();
	CREATE_FUNC(Girl);
};

#endif /* GIRL_H_ */
