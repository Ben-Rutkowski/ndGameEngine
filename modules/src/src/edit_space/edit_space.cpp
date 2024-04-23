#include "edit_space.hpp"

ndEditSapceModule::ndEditSapceModule()
:ndModuleImplement<ndEditSpaceStateLen>(Module::EDIT_SPACE)
{

}

ndEditSapceModule::~ndEditSapceModule() {
}

void ndEditSapceModule::linkRenderer(ndRoutine routine_in) {
    printf("Linking Routine");
    routine = routine_in;
}
