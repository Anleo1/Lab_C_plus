#include "TestFunctions.hpp"

template<typename T>
bool checkResult(const CommandResult& result, T expected) {
    if (std::holds_alternative<VoidResult>(result)) {
        return false;
    }

    const auto& value = std::get<ArgValue>(result);
    try {
        return std::get<T>(value) == expected;
    }
    catch (...) {
        return false;
    }
}

bool isVoidResult(const CommandResult& result) {
    return std::holds_alternative<VoidResult>(result);
}

void runAssignmentTest() {

    TestClass obj;

    Wrapper<TestClass, int, int, int> wrapper(&obj, &TestClass::f3, { {"arg1", 0}, {"arg2", 0} });

    Engine engine;

    engine.register_command(&wrapper, "command1");

    std::cout << "Результат сумма: " << engine.execute("command1", { {"arg1", "4"}, {"arg2", "5"} }) << std::endl;

    auto result = engine.execute("command1", { {"arg1", "4"}, {"arg2", "5"} });
    assert(checkResult(result, 9) && "Тест не пройден");

}

void runAllTypeTests() {

    TestClass obj;
    Engine engine;

    Wrapper<TestClass, int, int, int> intWrapper(&obj, &TestClass::f3, { {"a", 0}, {"b", 0} });
    engine.register_command(&intWrapper, "add");

    Wrapper<TestClass, std::string, std::string, int> stringWrapper(&obj, &TestClass::repeat, { {"s", std::string("")}, {"n", 0} });
    engine.register_command(&stringWrapper, "repeat");

    Wrapper<TestClass, double, double, double> avgWrapper(&obj, &TestClass::average,
        { {"a", 0.0}, {"b", 0.0} });
    engine.register_command(&avgWrapper, "average");

    Wrapper<TestClass, double, double, double> multiplyWrapper(&obj, &TestClass::multiply,
        { {"a", 0.0}, {"b", 0.0} });
    engine.register_command(&multiplyWrapper, "multiply");

    Wrapper<TestClass, bool, bool> boolWrapper(&obj, &TestClass::yes_no, { {"flag", false} });
    engine.register_command(&boolWrapper, "yes_no");

    Wrapper<TestClass, void, std::string, int> voidWrapper(&obj, &TestClass::print_greeting,
        { {"name", std::string("")}, {"times", 1} });
    engine.register_command(&voidWrapper, "greet");

    Wrapper<TestClass, int, int> squareWrapper(&obj, &TestClass::getSquare, { {"x", 0} });
    engine.register_command(&squareWrapper, "square");

    Wrapper<TestClass, double, double, double, double, double, double> multiWrapper(&obj, &TestClass::multiply5,
        { {"a", 1.0}, {"b", 1.0}, {"c", 1.0}, {"d", 1.0}, {"e", 1.0} });
    engine.register_command(&multiWrapper, "multiply5");


    {
        auto result = engine.execute("add", { {"a", "10"}, {"b", "20"} });
        assert(checkResult(result, 30) && "Int метод: add(10, 20) не вернул 30");
        std::cout << "add(10, 20) = " << std::get<int>(std::get<ArgValue>(result)) << "\n";
    }

    {
        auto result = engine.execute("repeat", { {"s", "AB"}, {"n", "3"} });
        assert(checkResult(result, std::string("ABABAB")) && "String метод: repeat(\"AB\", 3) не вернул \"ABABAB\"");
        std::cout << "repeat(\"AB\", 3) = \""
            << std::get<std::string>(std::get<ArgValue>(result)) << "\"\n";
    }

    {
        auto result = engine.execute("average", { {"a", "10.0"}, {"b", "20.0"} });
        assert(checkResult(result, 15.0) && "Double метод: average(10, 20) не вернул 15.0");
        std::cout << "average(10, 20) = "
            << std::get<double>(std::get<ArgValue>(result)) << "\n";
    }

    {
        auto result = engine.execute("multiply", { {"a", "3.5"}, {"b", "2.0"} });
        assert(checkResult(result, 7.0) && "Double метод: multiply(3.5, 2.0) не вернул 7.0");
        std::cout << "multiply(3.5, 2.0) = "
            << std::get<double>(std::get<ArgValue>(result)) << "\n";
    }

    {
        auto result = engine.execute("yes_no", { {"flag", "true"} });
        assert(checkResult(result, true) && "Bool метод: yes_no(true) не вернул true");
        std::cout << "yes_no(true) = "
            << std::boolalpha << std::get<bool>(std::get<ArgValue>(result)) << "\n";
    }

    {
        auto result = engine.execute("greet", { {"name", "World"}, {"times", "2"} });
        assert(isVoidResult(result) && "Void метод: greet(\"World\", 2) не вернул void результат");
        std::cout << "greet(\"World\", 2) выполнено\n";
    }

    {
        auto result = engine.execute("square", { {"x", "6"} });
        assert(checkResult(result, 36) && "Метод: square(6) не вернул 36");
        std::cout << "square(6) = "
            << std::get<int>(std::get<ArgValue>(result)) << "\n";
    }

    {
        auto result = engine.execute("multiply5",
            { {"a", "1.0"}, {"b", "2.0"}, {"c", "3.0"},
             {"d", "4.0"}, {"e", "5.0"} });
        assert(checkResult(result, 120.0) && "Метод с 5 аргументами: multiply5(1,2,3,4,5) не вернул 120.0");
        std::cout << "multiply5(1, 2, 3, 4, 5) = "
            << std::get<double>(std::get<ArgValue>(result)) << "\n";
    }
}

void testDefaultArguments() {

    TestClass obj;
    Engine engine;

    Wrapper<TestClass, int, int, int> wrapper(&obj, &TestClass::f3, { {"arg1", 100}, {"arg2", 200} });
    engine.register_command(&wrapper, "default_test");

    auto result1 = engine.execute("default_test");
    assert(checkResult(result1, 300) && "Значения по умолчанию: default_test() не вернул 300");
    std::cout << "default_test() = 300\n";

    auto result2 = engine.execute("default_test", { {"arg1", "50"} });
    assert(checkResult(result2, 250) && "Переопределение аргумента: default_test(arg1=50) не вернул 250");
    std::cout << "default_test(arg1=50) = 250\n";
}