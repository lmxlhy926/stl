//
// Created by lhy on 2020/11/10.
//

#include <iostream>
#include "stlString.h"
#include "basic_class.h"
#include "inherit.h"
#include "poly.h"
#include "callableObj.h"
#include "fileaccess.h"
#include "opoverload.h"
#include "mthread.h"
#include "mmutex.h"
#include "condition.h"
#include "matomic.h"
#include "stream.h"
#include "overload.h"

#include <thread>
#include <future>

using namespace std;
using namespace fundamental;
using namespace mthread;
using namespace mstream;


int main(int argc, char *argv[]){

    fundamental::overloadtest();

    return 0;
}