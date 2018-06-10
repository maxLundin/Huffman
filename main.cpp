#include "lib/ReadWriter.h"

int main(int argc, char *argv[]) {
    try {
        if (argc != 6) {
            throw std::runtime_error("Bad arguments format");
        }
        std::cout << "Output file is: " << argv[1] << std::endl;
        if (strcmp(argv[2], "code") == 0) {
            Tree tree = Tree(argv[3]);
            if (strcmp(argv[4], "false") != 0) {
                tree.buildTree(true, strcmp(argv[5], "false") != 0, argv[1]);
            } else {
                tree.buildTree(false, false, argv[1]);
            }
            ReadWriter::write(argv[3], argv[1], tree);
            std::cout << "Compressing is done" << std::endl;
        } else if (strcmp(argv[2], "decode") == 0) {
            Tree tree = Tree();
            std::ifstream in(argv[3], std::ios::binary);
            tree.buildTree(in, argv[1]);
            ReadWriter::read(in, tree, argv[1]);
            in.close();
            std::cout << "Decompressing is done" << std::endl;
        } else {
            std::cout << "Wrong code/decode arguments" << std::endl;
        }
    } catch (std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}