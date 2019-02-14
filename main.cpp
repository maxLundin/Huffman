#include "ReadWriter.h"
#include "haffman.h"

int main(int argc, char *argv[]) {
    try {
        if (argc <= 4) {
            throw std::runtime_error("Bad arguments format");
        }
        std::cout << "Output file is: " << argv[1] << std::endl;
        Haffman haff{argv[3], argv[1]};
        if (strcmp(argv[2], "code") == 0) {
            haff.code(argc >= 5 && strcmp(argv[4], "false") != 0, argc >= 6 && strcmp(argv[5], "false") != 0);
        } else if (strcmp(argv[2], "decode") == 0) {
            haff.decode();
        } else {
            std::cout << "Wrong code/decode arguments" << std::endl;
        }
    } catch (std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}