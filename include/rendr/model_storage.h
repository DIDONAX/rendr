#pragma once

#include "rendr/constants.h"
#include "rendr/gl/mmbuffer.h"

namespace rendr {

// TODO: seems application specific, how to make storage take arbitrary
//

struct model_storage {
    mvector<offset_t> offsets_{kGlobalInstanceCapacity};
    mvector<color_t> colors_{kGlobalInstanceCapacity};
    mvector<quaternion_t> quaternions_{kGlobalInstanceCapacity};
    mvector<scale_t> scales_{kGlobalInstanceCapacity};
};

} // namespace rendr
