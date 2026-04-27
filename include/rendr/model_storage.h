#pragma once

#include "rendr/constants.h"
#include "rendr/gl/mmbuffer.h"

namespace rendr {

struct model_storage {
    mmbuffer<offset_t, WriteO> offsets_{10};
    mmbuffer<color_t, WriteO> colors_{10};
    mmbuffer<rotation_t, WriteO> rotations_{10};
    mmbuffer<scale_t, WriteO> scales_{10};
};

} // namespace rendr
