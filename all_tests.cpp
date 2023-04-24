#include "./all_tests.hpp"
int main(){
    //By default, the image will be output to the screen
    //You can also output to a file by specifying the file name in the tests file
    //By specifying RiDisplay(filename, "file", "rgb"); in the tests file

    // auto trans_test = make_decorator(TransparencyTest);
    // trans_test();

    // auto earth_test = make_decorator(Earth);
    // earth_test();

    // auto snowman_test = make_decorator(snowman);
    // snowman_test();

    // auto snowman_Ri_test = make_decorator(snowman_ri);
    // snowman_Ri_test();

    // auto shader_test = make_decorator(ShaderTest);
    // shader_test();

    // auto tornado_test = make_decorator(Tornado);
    // tornado_test();

    auto tunnel_test = make_decorator(Tunnel);
    tunnel_test();
}