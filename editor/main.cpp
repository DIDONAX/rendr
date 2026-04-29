#include "rendr/context.h"
#include "rendr/load.h"
#include "editor.h"


using namespace rendr;
int main() {
    editor e;
    while (e.running()) {
        // do stuff with opengl context / scene

        e.display();
    }
}
