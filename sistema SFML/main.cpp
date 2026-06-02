#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// ======================= FUNCIONES AUXILIARES =======================

// --- Fibonacci ---
std::vector<long long> calcularFibonacci(int n)
{
    std::vector<long long> seq;
    if (n <= 0) return seq;
    seq.push_back(0);
    if (n == 1) return seq;
    seq.push_back(1);
    for (int i = 2; i < n; i++)
        seq.push_back(seq[i - 1] + seq[i - 2]);
    return seq;
}

// --- Ordenamiento Burbuja ---
void ordenarBurbuja(int arr[], int tam)
{
    bool cambio;
    for (int i = 0; i < tam - 1; i++)
    {
        cambio = false;
        for (int j = 0; j < tam - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int aux = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = aux;
                cambio = true;
            }
        }
        if (!cambio) break;
    }
}

// --- Arbol BST ---
struct NodoBST
{
    int dato;
    NodoBST* izquierdo;
    NodoBST* derecho;
    NodoBST(int v) : dato(v), izquierdo(nullptr), derecho(nullptr) {}
};

NodoBST* insertarBST(NodoBST* raiz, int valor)
{
    if (!raiz) return new NodoBST(valor);
    if (valor < raiz->dato)
        raiz->izquierdo = insertarBST(raiz->izquierdo, valor);
    else if (valor > raiz->dato)
        raiz->derecho = insertarBST(raiz->derecho, valor);
    return raiz;
}

void inOrder(NodoBST* raiz, std::string& out)
{
    if (!raiz) return;
    inOrder(raiz->izquierdo, out);
    out += std::to_string(raiz->dato) + " ";
    inOrder(raiz->derecho, out);
}

void preOrder(NodoBST* raiz, std::string& out)
{
    if (!raiz) return;
    out += std::to_string(raiz->dato) + " ";
    preOrder(raiz->izquierdo, out);
    preOrder(raiz->derecho, out);
}

void postOrder(NodoBST* raiz, std::string& out)
{
    if (!raiz) return;
    postOrder(raiz->izquierdo, out);
    postOrder(raiz->derecho, out);
    out += std::to_string(raiz->dato) + " ";
}

void liberarBST(NodoBST* raiz)
{
    if (!raiz) return;
    liberarBST(raiz->izquierdo);
    liberarBST(raiz->derecho);
    delete raiz;
}

// --- Arbol AVL ---
struct NodoAVL
{
    int dato, altura;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    NodoAVL(int v) : dato(v), altura(1), izquierdo(nullptr), derecho(nullptr) {}
};

int obtenerAlturaAVL(NodoAVL* n) { return n ? n->altura : 0; }

void actualizarAlturaAVL(NodoAVL* n)
{
    if (n) n->altura = 1 + std::max(obtenerAlturaAVL(n->izquierdo), obtenerAlturaAVL(n->derecho));
}

int obtenerBalanceAVL(NodoAVL* n)
{
    return n ? obtenerAlturaAVL(n->izquierdo) - obtenerAlturaAVL(n->derecho) : 0;
}

NodoAVL* rotarDerechaAVL(NodoAVL* z)
{
    NodoAVL* y = z->izquierdo;
    NodoAVL* T3 = y->derecho;
    y->derecho = z;
    z->izquierdo = T3;
    actualizarAlturaAVL(z);
    actualizarAlturaAVL(y);
    return y;
}

NodoAVL* rotarIzquierdaAVL(NodoAVL* z)
{
    NodoAVL* y = z->derecho;
    NodoAVL* T2 = y->izquierdo;
    y->izquierdo = z;
    z->derecho = T2;
    actualizarAlturaAVL(z);
    actualizarAlturaAVL(y);
    return y;
}

NodoAVL* balancearAVL(NodoAVL* nodo, std::string& log)
{
    if (!nodo) return nullptr;
    nodo->izquierdo = balancearAVL(nodo->izquierdo, log);
    nodo->derecho = balancearAVL(nodo->derecho, log);
    actualizarAlturaAVL(nodo);

    int balance = obtenerBalanceAVL(nodo);

    if (balance > 1 && obtenerBalanceAVL(nodo->izquierdo) >= 0)
    {
        log += "Rot. DERECHA en " + std::to_string(nodo->dato) + "\n";
        return rotarDerechaAVL(nodo);
    }
    if (balance > 1 && obtenerBalanceAVL(nodo->izquierdo) < 0)
    {
        log += "Rot. DOBLE IzqDer en " + std::to_string(nodo->dato) + "\n";
        nodo->izquierdo = rotarIzquierdaAVL(nodo->izquierdo);
        return rotarDerechaAVL(nodo);
    }
    if (balance < -1 && obtenerBalanceAVL(nodo->derecho) <= 0)
    {
        log += "Rot. IZQUIERDA en " + std::to_string(nodo->dato) + "\n";
        return rotarIzquierdaAVL(nodo);
    }
    if (balance < -1 && obtenerBalanceAVL(nodo->derecho) > 0)
    {
        log += "Rot. DOBLE DerIzq en " + std::to_string(nodo->dato) + "\n";
        nodo->derecho = rotarDerechaAVL(nodo->derecho);
        return rotarIzquierdaAVL(nodo);
    }
    return nodo;
}

int calcularAlturaAVL(NodoAVL* raiz) { return raiz ? 1 + std::max(calcularAlturaAVL(raiz->izquierdo), calcularAlturaAVL(raiz->derecho)) : 0; }
int contarNodosAVL(NodoAVL* raiz) { return raiz ? 1 + contarNodosAVL(raiz->izquierdo) + contarNodosAVL(raiz->derecho) : 0; }
int contarHojasAVL(NodoAVL* raiz)
{
    if (!raiz) return 0;
    if (!raiz->izquierdo && !raiz->derecho) return 1;
    return contarHojasAVL(raiz->izquierdo) + contarHojasAVL(raiz->derecho);
}

std::string buscarAVL(NodoAVL* raiz, int objetivo, std::string camino)
{
    if (!raiz) return "";
    camino += std::to_string(raiz->dato);
    if (raiz->dato == objetivo)
        return "ENCONTRADO!\nCamino: " + camino;
    camino += " -> ";
    std::string res = buscarAVL(raiz->izquierdo, objetivo, camino);
    if (!res.empty()) return res;
    return buscarAVL(raiz->derecho, objetivo, camino);
}

void liberarAVL(NodoAVL* raiz)
{
    if (!raiz) return;
    liberarAVL(raiz->izquierdo);
    liberarAVL(raiz->derecho);
    delete raiz;
}

// ======================= FUNCIÓN AUXILIAR: crear texto centrado =======================
sf::Text makeText(const sf::Font& font, const std::string& str, unsigned int size,
    sf::Color color, float x, float y)
{
    sf::Text t(font, str, size);
    t.setFillColor(color);
    t.setPosition({ x, y });
    return t;
}

// ======================= INICIO DEL PROGRAMA PRINCIPAL =======================
int main()
{
    srand((unsigned)time(nullptr));

    // ======================= CREACIÓN DE VENTANA =======================
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Menu SFML");

    // ======================= CARGA DE FUENTE =======================
    sf::Font font;
    if (!font.openFromFile("ARLRDBD.TTF"))
        return -1;

    // ======================= FONDO PRINCIPAL =======================
    sf::RectangleShape fondo({ 800.f, 600.f });
    fondo.setFillColor(sf::Color(0, 150, 150));

    // ======================= BARRA INFERIOR =======================
    sf::RectangleShape barra({ 800.f, 50.f });
    barra.setPosition({ 0.f, 550.f });
    barra.setFillColor(sf::Color(100, 100, 100));

    // ======================= BOTÓN INICIO =======================
    sf::RectangleShape boton({ 120.f, 40.f });
    boton.setPosition({ 10.f, 555.f });
    boton.setFillColor(sf::Color(50, 50, 50));

    sf::Text txtInicio(font, "Inicio", 20);
    txtInicio.setPosition({ 30.f, 560.f });
    txtInicio.setFillColor(sf::Color::White);

    // ======================= NOMBRE EN LA BARRA INFERIOR =======================
    sf::Text nombre(font, "Garcia Salazar Leonardo Guadalupe 25040066", 16);
    nombre.setFillColor(sf::Color::White);

    // ======================= MENÚ LATERAL (9 opciones) =======================
    // El menú se extiende desde y=245 hasta y=540 para caber las 9 opciones
    sf::RectangleShape menu({ 250.f, 300.f });
    menu.setPosition({ 10.f, 245.f });
    menu.setFillColor(sf::Color(70, 70, 70));

    const int NUM_OPS = 9;
    sf::RectangleShape ops[NUM_OPS];
    sf::Text tOps[NUM_OPS] = {
        sf::Text(font, "Fibonacci",              17),
        sf::Text(font, "Factorial",              17),
        sf::Text(font, "Cuadro Magico",          17),
        sf::Text(font, "Pino",                   17),
        sf::Text(font, "Suma de numeros",         17),
        sf::Text(font, "Metodo Burbuja",          17),
        sf::Text(font, "Vectores (Carrito)",      17),
        sf::Text(font, "Arbol Binario BST",       17),
        sf::Text(font, "Arbol AVL",               17)
    };

    for (int i = 0; i < NUM_OPS; i++)
    {
        ops[i].setSize({ 230.f, 28.f });
        ops[i].setPosition({ 20.f, 250.f + i * 31.f });
        ops[i].setFillColor(sf::Color(120, 120, 120));
        tOps[i].setPosition({ 28.f, 252.f + i * 31.f });
        tOps[i].setFillColor(sf::Color::White);
    }

    // ======================= VARIABLES DE CONTROL =======================
    bool menuActivo = false;

    // Flags por módulo
    bool modFibonacci = false, modFactorial = false, modCuadro = false;
    bool modPino = false, modSuma = false, modBurbuja = false;
    bool modVectores = false, modBST = false, modAVL = false;

    // --- Suma ---
    std::string n1 = "", n2 = "", res = "";
    bool escribiendo1 = true;

    // --- Factorial ---
    std::string factIn = "", factRes = "";

    // --- Pino ---
    bool mostrarTextoPino = true;
    std::string altura = "";
    std::vector<sf::Text> pinoDibujo;

    // --- Cuadro Mágico ---
    std::string cuadroInput = "";
    bool cuadroListo = false;
    std::vector<sf::RectangleShape> celdas;
    std::vector<sf::Text> numeros;

    // --- Fibonacci ---
    std::string fibInput = "";
    bool fibListo = false;
    std::vector<sf::Text> fibTextos;

    // --- Burbuja ---
    const int TAM_BURBUJA = 7;
    int datosOriginales[TAM_BURBUJA] = {};
    int datosOrdenados[TAM_BURBUJA] = {};
    bool burbujaListo = false;

    // --- Vectores ---
    // Se construye el texto de resultado al activarse el módulo
    std::string vectoresTexto = "";

    // --- BST ---
    std::string bstInput = "";
    bool bstListo = false;
    std::string bstInOrd = "", bstPreOrd = "", bstPostOrd = "";
    NodoBST* raizBST = nullptr;

    // --- AVL ---
    bool avlListo = false;
    std::string avlStats = "", avlLog = "", avlBusqueda = "";

    // Botón reset
    sf::RectangleShape reset({ 150.f, 40.f });
    reset.setPosition({ 300.f, 490.f });
    reset.setFillColor(sf::Color(80, 80, 80));

    sf::Text txtReset(font, "Reiniciar", 18);
    txtReset.setPosition({ 310.f, 495.f });
    txtReset.setFillColor(sf::Color::White);

    // Texto dinámico general
    sf::Text texto(font, "", 18);
    texto.setPosition({ 200.f, 80.f });
    texto.setFillColor(sf::Color::White);

    // ======================= LOOP PRINCIPAL =======================
    while (window.isOpen())
    {
        // ======================= EVENTOS =======================
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // ---- Click del mouse ----
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                auto m = event->getIf<sf::Event::MouseButtonPressed>();
                if (m && m->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f pos((float)m->position.x, (float)m->position.y);

                    // Abrir/cerrar menú
                    if (boton.getGlobalBounds().contains(pos))
                    {
                        menuActivo = !menuActivo;
                        modFibonacci = modFactorial = modCuadro = modPino = modSuma = false;
                        modBurbuja = modVectores = modBST = modAVL = false;
                    }

                    // Selección de opciones del menú
                    auto activar = [&](bool& mod)
                        {
                            mod = true; menuActivo = false;
                            modFibonacci = modFactorial = modCuadro = modPino = modSuma = false;
                            modBurbuja = modVectores = modBST = modAVL = false;
                            mod = true;
                            // Reset estados
                            n1 = n2 = res = ""; factIn = factRes = ""; altura = "";
                            cuadroInput = ""; cuadroListo = false;
                            fibInput = ""; fibListo = false;
                            burbujaListo = false;
                            bstInput = ""; bstListo = false;
                            bstInOrd = bstPreOrd = bstPostOrd = "";
                            if (raizBST) { liberarBST(raizBST); raizBST = nullptr; }
                            avlListo = false; avlStats = avlLog = avlBusqueda = "";
                            vectoresTexto = "";
                            escribiendo1 = true;
                            mostrarTextoPino = true;
                            pinoDibujo.clear(); celdas.clear(); numeros.clear(); fibTextos.clear();
                        };

                    if (menuActivo && ops[0].getGlobalBounds().contains(pos)) activar(modFibonacci);
                    if (menuActivo && ops[1].getGlobalBounds().contains(pos)) activar(modFactorial);
                    if (menuActivo && ops[2].getGlobalBounds().contains(pos)) activar(modCuadro);
                    if (menuActivo && ops[3].getGlobalBounds().contains(pos)) activar(modPino);
                    if (menuActivo && ops[4].getGlobalBounds().contains(pos)) activar(modSuma);
                    if (menuActivo && ops[5].getGlobalBounds().contains(pos))
                    {
                        activar(modBurbuja);
                        // Generar números aleatorios al entrar
                        for (int i = 0; i < TAM_BURBUJA; i++)
                            datosOriginales[i] = rand() % 100;
                        for (int i = 0; i < TAM_BURBUJA; i++)
                            datosOrdenados[i] = datosOriginales[i];
                    }
                    if (menuActivo && ops[6].getGlobalBounds().contains(pos))
                    {
                        activar(modVectores);
                        // Construir resultado del carrito
                        std::vector<double> precios = { 10.50, 5.25, 20.00 };
                        vectoresTexto = "=== Vectores: Carrito de Compras ===\n\n";
                        vectoresTexto += "Productos agregados: " + std::to_string((int)precios.size()) + "\n";
                        vectoresTexto += "Primer precio: $" + std::to_string(precios[0]).substr(0, 5) + "\n";
                        vectoresTexto += "Lista de precios: ";
                        for (double p : precios) vectoresTexto += "[$" + std::to_string(p).substr(0, 5) + "] ";
                        precios.pop_back();
                        vectoresTexto += "\n\nDespues de eliminar el ultimo:\n";
                        vectoresTexto += "Productos restantes: " + std::to_string((int)precios.size()) + "\n";
                        vectoresTexto += "Precios: ";
                        for (double p : precios) vectoresTexto += "[$" + std::to_string(p).substr(0, 5) + "] ";
                    }
                    if (menuActivo && ops[7].getGlobalBounds().contains(pos)) activar(modBST);
                    if (menuActivo && ops[8].getGlobalBounds().contains(pos))
                    {
                        activar(modAVL);
                        // Construir árbol AVL fijo y calcular stats
                        NodoAVL* raizAVL = new NodoAVL(27);
                        raizAVL->izquierdo = new NodoAVL(11);
                        raizAVL->derecho = new NodoAVL(63);
                        raizAVL->izquierdo->izquierdo = new NodoAVL(5);
                        raizAVL->izquierdo->derecho = new NodoAVL(18);
                        raizAVL->derecho->izquierdo = new NodoAVL(45);
                        raizAVL->derecho->derecho = new NodoAVL(80);
                        raizAVL->izquierdo->izquierdo->izquierdo = new NodoAVL(3);
                        raizAVL->izquierdo->izquierdo->derecho = new NodoAVL(6);
                        raizAVL->izquierdo->derecho->izquierdo = new NodoAVL(12);
                        raizAVL->izquierdo->derecho->derecho = new NodoAVL(19);
                        raizAVL->derecho->izquierdo->izquierdo = new NodoAVL(31);
                        raizAVL->derecho->izquierdo->derecho = new NodoAVL(52);
                        raizAVL->derecho->derecho->izquierdo = new NodoAVL(74);
                        raizAVL->derecho->derecho->derecho = new NodoAVL(88);
                        raizAVL->izquierdo->izquierdo->izquierdo->izquierdo = new NodoAVL(7);
                        raizAVL->derecho->derecho->derecho->derecho = new NodoAVL(99);

                        // Buscar nodo 74
                        avlBusqueda = "Busqueda DFS del nodo 74:\n" + buscarAVL(raizAVL, 74, "");

                        // Stats antes
                        int hAntes = calcularAlturaAVL(raizAVL);
                        int tAntes = contarNodosAVL(raizAVL);
                        int ljAntes = contarHojasAVL(raizAVL);
                        avlStats = "ANTES del balanceo:\n";
                        avlStats += "  Altura: " + std::to_string(hAntes) + " niveles\n";
                        avlStats += "  Total nodos: " + std::to_string(tAntes) + "\n";
                        avlStats += "  Hojas: " + std::to_string(ljAntes) + "  Internos: " + std::to_string(tAntes - ljAntes) + "\n\n";

                        // Balanceo
                        avlLog = "Rotaciones aplicadas:\n";
                        raizAVL = balancearAVL(raizAVL, avlLog);
                        if (avlLog == "Rotaciones aplicadas:\n") avlLog += "  (ninguna necesaria)\n";

                        // Stats después
                        int hDes = calcularAlturaAVL(raizAVL);
                        int tDes = contarNodosAVL(raizAVL);
                        int ljDes = contarHojasAVL(raizAVL);
                        avlStats += "DESPUES del balanceo:\n";
                        avlStats += "  Altura: " + std::to_string(hDes) + " niveles\n";
                        avlStats += "  Total nodos: " + std::to_string(tDes) + "\n";
                        avlStats += "  Hojas: " + std::to_string(ljDes) + "  Internos: " + std::to_string(tDes - ljDes);

                        liberarAVL(raizAVL);
                        avlListo = true;
                    }

                    // Reset general
                    if (reset.getGlobalBounds().contains(pos))
                    {
                        n1 = n2 = res = ""; factIn = factRes = ""; altura = "";
                        cuadroInput = ""; cuadroListo = false;
                        fibInput = ""; fibListo = false;
                        burbujaListo = false;
                        bstInput = ""; bstListo = false;
                        bstInOrd = bstPreOrd = bstPostOrd = "";
                        if (raizBST) { liberarBST(raizBST); raizBST = nullptr; }
                        avlListo = false; avlStats = avlLog = avlBusqueda = "";
                        vectoresTexto = "";
                        escribiendo1 = true; mostrarTextoPino = true;
                        pinoDibujo.clear(); celdas.clear(); numeros.clear(); fibTextos.clear();
                        // Si estaba en burbuja, regenerar números
                        if (modBurbuja)
                        {
                            for (int i = 0; i < TAM_BURBUJA; i++)
                                datosOriginales[i] = rand() % 100;
                            for (int i = 0; i < TAM_BURBUJA; i++)
                                datosOrdenados[i] = datosOriginales[i];
                        }
                    }
                }
            }

            // ---- Entrada de teclado ----
            if (event->is<sf::Event::TextEntered>())
            {
                auto t = event->getIf<sf::Event::TextEntered>();
                if (!t) continue;
                char c = (char)t->unicode;

                // ================= SUMA =================
                if (modSuma)
                {
                    if (isdigit(c))
                        (escribiendo1 ? n1 : n2) += c;
                    else if (c == 8) // Backspace
                    {
                        auto& s = (escribiendo1 ? n1 : n2); if (!s.empty()) s.pop_back();
                    }
                    else if (c == 13)
                    {
                        if (escribiendo1 && !n1.empty()) escribiendo1 = false;
                        else if (!n1.empty() && !n2.empty())
                            res = std::to_string(std::stoi(n1) + std::stoi(n2));
                    }
                }

                // ================= FACTORIAL =================
                if (modFactorial)
                {
                    if (isdigit(c)) factIn += c;
                    else if (c == 8 && !factIn.empty()) factIn.pop_back();
                    else if (c == 13 && !factIn.empty())
                    {
                        long long f = 1;
                        int n = std::stoi(factIn);
                        for (int i = 1; i <= n; i++) f *= i;
                        factRes = std::to_string(f);
                    }
                }

                // ================= FIBONACCI =================
                if (modFibonacci)
                {
                    if (isdigit(c)) fibInput += c;
                    else if (c == 8 && !fibInput.empty()) fibInput.pop_back();
                    else if (c == 13 && !fibInput.empty())
                    {
                        fibListo = true;
                        fibTextos.clear();
                        int n = std::stoi(fibInput);
                        auto seq = calcularFibonacci(n);

                        // Mostrar en filas de 8 números
                        std::string linea = "";
                        int col = 0;
                        for (int i = 0; i < (int)seq.size(); i++)
                        {
                            linea += std::to_string(seq[i]);
                            if (i < (int)seq.size() - 1) linea += ", ";
                            col++;
                            if (col == 8 || i == (int)seq.size() - 1)
                            {
                                sf::Text tl(font, linea, 17);
                                tl.setFillColor(sf::Color::White);
                                tl.setPosition({ 160.f, 130.f + ((float)(fibTextos.size()) * 26.f) });
                                fibTextos.push_back(tl);
                                linea = ""; col = 0;
                            }
                        }
                    }
                }

                // ================= PINO =================
                if (modPino)
                {
                    if (isdigit(c)) altura += c;
                    else if (c == 8 && !altura.empty()) altura.pop_back();
                    else if (c == 13 && !altura.empty())
                    {
                        mostrarTextoPino = false;
                        int h = std::stoi(altura);
                        pinoDibujo.clear();
                        float centroX = 400.f;
                        for (int i = 1; i <= h; i++)
                        {
                            std::string estrellas(i * 2 - 1, '*');
                            sf::Text t(font, estrellas, 22);
                            t.setFillColor(sf::Color::White);
                            float ancho = t.getLocalBounds().size.x;
                            t.setPosition({ centroX - ancho / 2.f, 70.f + i * 28.f });
                            pinoDibujo.push_back(t);
                        }
                    }
                }

                // ================= CUADRO MÁGICO =================
                if (modCuadro)
                {
                    if (isdigit(c)) cuadroInput += c;
                    else if (c == 8 && !cuadroInput.empty()) cuadroInput.pop_back();
                    else if (c == 13 && !cuadroInput.empty())
                    {
                        cuadroListo = true;
                        int inicio = std::stoi(cuadroInput);
                        int base[3][3] = { {8,1,6},{3,5,7},{4,9,2} };
                        celdas.clear(); numeros.clear();
                        float startX = 250.f, startY = 120.f, cellW = 60.f, cellH = 60.f;
                        for (int i = 0; i < 3; i++)
                            for (int j = 0; j < 3; j++)
                            {
                                sf::RectangleShape celda({ cellW, cellH });
                                celda.setPosition({ startX + j * cellW, startY + i * cellH });
                                celda.setFillColor(sf::Color(90, 90, 90));
                                celda.setOutlineColor(sf::Color::White);
                                celda.setOutlineThickness(2);
                                celdas.push_back(celda);
                                int valor = base[i][j] + (inicio - 1);
                                sf::Text num(font, std::to_string(valor), 20);
                                num.setFillColor(sf::Color::White);
                                float tx = celda.getPosition().x + cellW / 2.f - num.getLocalBounds().size.x / 2.f;
                                float ty = celda.getPosition().y + cellH / 2.f - num.getLocalBounds().size.y;
                                num.setPosition({ tx, ty });
                                numeros.push_back(num);
                            }
                    }
                }

                // ================= BURBUJA =================
                if (modBurbuja && c == 13 && !burbujaListo)
                {
                    burbujaListo = true;
                    ordenarBurbuja(datosOrdenados, TAM_BURBUJA);
                }

                // ================= BST =================
                if (modBST)
                {
                    if (isdigit(c)) bstInput += c;
                    else if (c == 8 && !bstInput.empty()) bstInput.pop_back();
                    else if (c == ',' || (c == 13 && !bstInput.empty()))
                    {
                        // Insertar el número actual
                        int val = std::stoi(bstInput);
                        raizBST = insertarBST(raizBST, val);
                        bstInput = "";
                        // Actualizar recorridos
                        bstInOrd = ""; bstPreOrd = ""; bstPostOrd = "";
                        inOrder(raizBST, bstInOrd);
                        preOrder(raizBST, bstPreOrd);
                        postOrder(raizBST, bstPostOrd);
                        bstListo = true;
                    }
                }
            }
        }

        // ======================= RENDER =======================
        window.clear();
        window.draw(fondo);
        window.draw(barra);
        window.draw(boton);
        window.draw(txtInicio);

        float textWidth = nombre.getLocalBounds().size.x;
        nombre.setPosition({ 800.f - textWidth - 10.f, 565.f });
        window.draw(nombre);

        bool alguienActivo = modFibonacci || modFactorial || modCuadro || modPino ||
            modSuma || modBurbuja || modVectores || modBST || modAVL;

        if (!alguienActivo)
        {
            if (menuActivo)
            {
                window.draw(menu);
                for (int i = 0; i < NUM_OPS; i++) { window.draw(ops[i]); window.draw(tOps[i]); }
            }
        }
        else
        {
            // -------- SUMA --------
            if (modSuma)
            {
                texto.setString(
                    "=== Suma de Numeros ===\n\n"
                    "Num1: " + n1 + "\n"
                    "Num2: " + n2 + "\n"
                    "Resultado: " + res + "\n\n"
                    "[Digita numero, ENTER para pasar]\n"
                    "[ENTER de nuevo para sumar]"
                );
                window.draw(texto);
            }

            // -------- FACTORIAL --------
            if (modFactorial)
            {
                texto.setString(
                    "=== Factorial ===\n\n"
                    "N: " + factIn + "\n"
                    "Factorial: " + factRes + "\n\n"
                    "[Escribe N y presiona ENTER]"
                );
                window.draw(texto);
            }

            // -------- FIBONACCI --------
            if (modFibonacci)
            {
                if (!fibListo)
                {
                    texto.setString(
                        "=== Fibonacci ===\n\n"
                        "Cuantos terminos? " + fibInput + "\n\n"
                        "[Escribe N y presiona ENTER]"
                    );
                    window.draw(texto);
                }
                else
                {
                    texto.setString("=== Fibonacci (primeros " + fibInput + " terminos) ===");
                    window.draw(texto);
                    for (auto& ft : fibTextos) window.draw(ft);
                }
            }

            // -------- PINO --------
            if (modPino)
            {
                if (mostrarTextoPino)
                {
                    texto.setString(
                        "=== Pino de Asteriscos ===\n\n"
                        "Altura: " + altura + "\n\n"
                        "[Escribe la altura y presiona ENTER]"
                    );
                    window.draw(texto);
                }
                else
                {
                    texto.setString("=== Pino (altura " + altura + ") ===");
                    window.draw(texto);
                    for (auto& l : pinoDibujo) window.draw(l);
                }
            }

            // -------- CUADRO MÁGICO --------
            if (modCuadro)
            {
                if (!cuadroListo)
                {
                    texto.setString(
                        "=== Cuadro Magico ===\n\n"
                        "Numero inicial: " + cuadroInput + "\n\n"
                        "[Escribe el numero inicial y presiona ENTER]"
                    );
                    window.draw(texto);
                }
                else
                {
                    texto.setString("=== Cuadro Magico (inicio " + cuadroInput + ") ===");
                    window.draw(texto);
                    for (auto& c : celdas) window.draw(c);
                    for (auto& n : numeros) window.draw(n);
                }
            }

            // -------- BURBUJA --------
            if (modBurbuja)
            {
                std::string orig = "Original:  ";
                std::string ord = "Ordenado:  ";
                for (int i = 0; i < TAM_BURBUJA; i++)
                {
                    orig += std::to_string(datosOriginales[i]) + " ";
                    ord += std::to_string(datosOrdenados[i]) + " ";
                }
                if (!burbujaListo)
                    texto.setString(
                        "=== Metodo Burbuja ===\n\n" + orig +
                        "\n\n[Presiona ENTER para ordenar]"
                    );
                else
                    texto.setString(
                        "=== Metodo Burbuja ===\n\n" + orig + "\n\n" + ord
                    );
                window.draw(texto);
            }

            // -------- VECTORES --------
            if (modVectores)
            {
                texto.setString(vectoresTexto);
                window.draw(texto);
            }

            // -------- BST --------
            if (modBST)
            {
                std::string bstStr =
                    "=== Arbol Binario BST ===\n\n"
                    "Escribe numeros separados por coma\n"
                    "o presiona ENTER despues de cada uno.\n\n"
                    "Entrada actual: " + bstInput + "\n\n";
                if (bstListo)
                    bstStr +=
                    "In-Order   : " + bstInOrd + "\n"
                    "Pre-Order  : " + bstPreOrd + "\n"
                    "Post-Order : " + bstPostOrd;
                texto.setString(bstStr);
                window.draw(texto);
            }

            // -------- AVL --------
            if (modAVL && avlListo)
            {
                texto.setString(
                    "=== " + avlBusqueda + "\n\n" +
                    avlLog + "\n" + avlStats
                );
                window.draw(texto);
            }

            window.draw(reset);
            window.draw(txtReset);
        }

        window.display();
    }

    if (raizBST) liberarBST(raizBST);
    return 0;
}
// ======================= FIN DEL PROGRAMA PRINCIPAL =======================