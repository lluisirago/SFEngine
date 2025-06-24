int main() {
    int a       = 1;
    int b       = 8;
    int hola[3] = {8, 3, 4};
    int c       = 0;
    return 0;
}

// engine/src/main.cpp
int unused_function() {
    int x = 42;  // variable no usada -> debería salir advertencia
    return 0;
}
