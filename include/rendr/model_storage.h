#pragma once

#include "rendr/constants.h"
#include "rendr/gl/mmbuffer.h"

namespace rendr {

// TODO: seems application specific, how to make storage take arbitrary
//

struct model_storage {
    mvector<offset_t> offsets_{10};
    mvector<color_t> colors_{10};
    mvector<quaternion_t> quaternions_{10};
    mvector<scale_t> scales_{10};
};

} // namespace rendr
