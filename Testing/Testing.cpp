#include "src/TestManager.h"

int main(int argc, char* argv[]) {
    Quasi::Debug::Info("Execution Directory: {}", argv[0]);
    Quasi::Debug::Info("Project Directory: {}", Test::PROJECT_DIRECTORY);

    Test::TestManager tManager;
    tManager.OnInit();
    while (tManager.WindowIsOpen())
        tManager.OnRun();
    tManager.OnDestroy();
}
